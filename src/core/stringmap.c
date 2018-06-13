/*
 * =====================================================================================
 *
 *       Filename:  stringmap.c
 *    Description:  handle (string) objects in a linked map
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <la/file.h>
#include <la/string.h>
#include <la/stringmap.h>

struct la_stringmap {
	char *key;
	char *value;
	struct la_stringmap *next;
};

static stringmap_t *stringmap_getNode (stringmap_t *self, unsigned int index) {
	stringmap_t *node; /* object */
	unsigned int count; /* counts the nodes to hit the index */

	assert(self);

	node = self;
	if (self->next == NULL) {       /* check if parameter-set is empty */
		errno = EINVAL;
		return NULL;
	}

	count = 0;
	do {
		node = node->next;

		if (index == count) {
			return node;
		} else ++count;
	} while (node->next != NULL);

	return NULL;
}

stringmap_t *stringmap_init() {
	stringmap_t *param; /* empty object */

	param = (stringmap_t *)malloc(sizeof(stringmap_t));
	if (!param) {
		errno = ENOMEM;
		return NULL;
	}
	
	param->key = NULL;
	param->value = NULL;
	param->next = NULL;

	return param;
}

void stringmap_add (stringmap_t *self, const char *key, const char *value) {
	stringmap_t *node; /* memoray of one node */
	size_t size; /*i size of map */

	assert(self);

	if (!key || !value) {
		errno = EINVAL;
		return;
	}

	node = (stringmap_t *)malloc(sizeof(stringmap_t) );
	if (!node) {
		errno = ENOMEM;
		return;
	}

	/* init next */
	node->next = NULL;

	/* create key */
	node->key = (char *)malloc(strlen(key) + 1);
	if (node->key == NULL) {
		errno = EINVAL;
		return;
	}
	strcpy(node->key, key);

	/* create value */
	node->value = (char *)malloc(strlen(value) + 1);
	if (node->value == NULL) {
		errno = EINVAL;
		return;
	}
	strcpy(node->value, value);

	/* set pointer */
	size = stringmap_size(self);
	if (size == 0) {
		node->next = self->next;
		self->next = node;
	} else {
		stringmap_t *last = stringmap_getNode(self, size - 1);
		last->next = node;
	}
}

void stringmap_addUnique(stringmap_t *self, const char *key, const char *value) {
	assert(self);

	if (!key || !value) {
		errno = EINVAL;
		return;
	}

	if (!stringmap_exists(self, key)) {
		stringmap_add(self, key, value);
	}
}

void stringmap_addReplace(stringmap_t *self, const char *key, const char *value) {
	assert(self);

	if (!key || !value) {
		errno = EINVAL;
		return;
	}

	if (stringmap_exists(self, key)) {
		stringmap_remove(self, key);
	}
	stringmap_add(self, key, value);
}

boolean_t stringmap_exists(stringmap_t *self, const char *key) {
	stringmap_t *node; /* object */

	assert(self);

	if (!key) {
		errno = EINVAL;
		return boolean_false;
	}

	node = self;
	if (self->next == NULL)
		return boolean_false;

	do {
		node = node->next;

		if (strcmp(key, node->key) == 0) {
			return boolean_true;
		}
	} while (node->next != NULL);

	return boolean_false;
}

void stringmap_remove (stringmap_t *self, const char *key) {
	stringmap_t *nodeSelf, *nodePrev, *nodeNext;
	unsigned int index; /* found index-number */
	size_t size; /* size of map */

	assert(self);

	if (!key) {
		errno = EINVAL;
		return;
	}

	index = stringmap_getIndexByKey(self, key);
    if (index == 0 && !stringmap_exists(self, key)) return;

	nodeSelf = stringmap_getNode(self, index);
	if (nodeSelf == NULL) return;
	
	size = stringmap_size(self);
	if (size <= 1) {
		stringmap_reset(self);
		return;
	}

	if (index == 0) {                           /* first element */
		nodeNext = nodeSelf->next;
		self->next = nodeNext;
	} else if (index == (size - 1)) {           /* last element */
		nodePrev = stringmap_getNode(self, index - 1);
		nodePrev->next = NULL;
	} else {
		nodePrev = stringmap_getNode(self, index - 1);
		nodeNext = nodeSelf->next;
		nodePrev->next = nodeNext;
	}

	/* free key */
	free(nodeSelf->key);
	nodeSelf->key = NULL;

	/* free value */
	free(nodeSelf->value);
	nodeSelf->value = NULL;

	/* free node */
	free(nodeSelf);
	nodeSelf = NULL;
}

stringmap_t *stringmap_getByIndex (stringmap_t *self, unsigned int index) {
	stringmap_t *node; /* object */
	unsigned int count; /* counter to find index */

	assert(self);

	node = self;
	if (self->next == NULL) return NULL;       /* check if list-set is empty */

	count = 0;
	do {
		node = node->next;

		if (index == count) {
			/* init parameter */
			stringmap_t *param;
			param = (stringmap_t *) malloc(sizeof(stringmap_t));
			param->next = NULL;

			/* copy key */
			param->key = (char *) malloc (strlen(node->key) + 1);
			strcpy(param->key, node->key);

			/* copy value */
			param->value = (char *) malloc (strlen(node->value) + 1);
			strcpy(param->value, node->value);

			return param;
		} else ++count;
	} while (node->next != NULL);

	return NULL;
}

char *stringmap_getKeyByIndex (stringmap_t *self, unsigned int index) {
	stringmap_t *node; /* object */
	unsigned int count; /* counter to find index */

	assert(self);

	node = self;
	if (self->next == NULL) return NULL;       /* check if list-set is empty */

	count = 0;
	do {
		node = node->next;

		if (index == count) {
			/* copy key */
			char *key;
			key = (char *) malloc (strlen(node->key) + 1);
			strcpy(key, node->key);
			return key;
		} else ++count;
	} while (node->next != NULL);

	return NULL;
}

char *stringmap_getValueByIndex (stringmap_t *self, unsigned int index) {
	stringmap_t *node; /* object */
	unsigned int count; /* counter to find index */

	assert(self);

	node = self;
	if (self->next == NULL) return NULL;       /* check if list-set is empty */

	count = 0;
	do {
		node = node->next;

		if (index == count) {
			/* copy value */
			char *value;
			value = (char *) malloc (strlen(node->value) + 1);
			strcpy(value, node->value);
			return value;
		} else ++count;
	} while (node->next != NULL);

	return NULL;
}

unsigned int stringmap_getIndexByKey (stringmap_t *self, const char *key) {
	stringmap_t *node; /* object */
	unsigned int count; /* counter to find index */

	assert(self);

	if (!key) {
		errno = EINVAL;
		return 0;
	}

	node = self;
	if (self->next == NULL) return 0;           /* check if list-set is empty */

	count = 0;
	do {
		node = node->next;

		if (strcmp(node->key, key) == 0) return count;

		++count;
	} while (node->next != NULL);

	return 0;
}

char *stringmap_getValueByKey (stringmap_t *self, const char *key) {
	stringmap_t *node; /* object */

	assert(self);

	if (!key) {
		errno = EINVAL;
		return 0;
	}

	node = self;
	if (self->next == NULL) return NULL;       /* check if parameter-set is empty */

	do {
		node = node->next;

		if ( strcmp(node->key, key) == 0 ) {
			/* copy value */
			char *value;
			value = (char *) malloc (strlen(node->value) + 1);
			strcpy(value, node->value);
			return value;
		}
	} while (node->next != NULL);

	return NULL;
}

char *stringmap_get (stringmap_t *self, const char *key) {
	assert(self);

	if (!key) {
		errno = EINVAL;
		return 0;
	}

	return stringmap_getValueByKey(self, key);
}

unsigned int stringmap_size (stringmap_t *self) {
	stringmap_t *node; /* object */
	unsigned int count; /* counts nodes */

	assert(self);

	if ( self->next == NULL) return 0;

	node = self;
	count = 0;
	do {
		node = node->next;
		++count;
	} while (node->next != NULL);

	return count;
}


void stringmap_free (stringmap_t *self) {
	stringmap_t *node;
	stringmap_t *next;

	assert(self);

	node = self;                               /* get the first parameter */
	while (node->next != NULL) {
		next = node->next;
		if (node->key != NULL ) free(node->key);
		if (node->value != NULL) free(node->value);
		free(node);
		node = next;
	}
	
	if (node->key != NULL ) free(node->key);
	if (node->value != NULL) free(node->value);
	if (node != NULL) free(node);               /* free only if there are items */
	node = NULL;
}

void stringmap_reset (stringmap_t *self) {
	stringmap_t *node;
	stringmap_t *next;

	assert(self);

	node = self->next;                         /* get the next parameter */
	if (node == NULL) return;

	while (node->next != NULL) {
		next = node->next;
		if ( node->key != NULL ) free(node->key);
		if ( node->value != NULL) free(node->value);
		free(node);
		node = next;
	}
	
	if ( node->key != NULL ) free(node->key);
	if ( node->value != NULL) free(node->value);
	free(node);
	node = NULL;

	self->next = NULL;
}

void stringmap_show(stringmap_t *self) {
	unsigned int i;
	stringmap_t *param;

	assert(self);

	for (i = 0; i < stringmap_size(self); ++i) {
		param = stringmap_getByIndex(self, i);
		printf ( "%s = %s\n", param->key, param->value );
		stringmap_free(param);
	}
}

unsigned int stringmap_loadFromFile(stringmap_t *self, const char *filename) {
	FILE *file; /* file handle */
	unsigned int count; /* counts read lines */
	char *key;
	char *value;
	char line[STRINGMAP_KEY_SIZE + 1 + STRINGMAP_VALUE_SIZE + 2 + 1];
	int len; /* maximal length of one line */
	char *sep; /* position of the separator between key and value */

	assert(self);

	stringmap_reset(self);

	file = fopen(filename, "r");
	if (!file) {
		/* errno has been set by fopen */
		return 0;
	}

	count = 0;
	len = sizeof(line) / sizeof(line[0]);
	while (fgets(line, len, file) != NULL) {
		/* correct string */
		char *t = string_trim(line);
		if (t == NULL) continue;
		if (t[0] == '#') {
			free(t);
			continue;
		}

		strcpy(line, t);
		free(t);

		/* split key-value */
		sep = strchr(line, '=');
		if (sep == NULL) continue;              /* no separator */
		if ((sep - line) > STRINGMAP_KEY_SIZE) continue;
		if ((strlen(line) - (sep - line) - 1) > STRINGMAP_VALUE_SIZE) continue;
		sep[0] = '\0';                          /* make the separator th the key-end */
		key = string_trim(line);
        if (!key) continue;
		++sep;                                  /* move one byte right */
		value = string_trim(sep);
		if (value == NULL) {
			free(key);
			continue;
		}

		stringmap_addReplace(self, key, value);
		++count;

		free(value);
		free(key);
	}
	fclose(file);

	return count;
}

unsigned int stringmap_saveToFile(stringmap_t *self, const char *filename) {
	stringmap_t *node; /* object */
	FILE *file; /* file handle */
	unsigned int count; /* counts written lines */ 

	assert(self);

	if (!filename) {
		errno = EINVAL;
		return 0;
	}

	if (self->next == NULL) return 0;          /* no parameters to save */

	node = self;

	file = fopen(filename, "w");
	if (!file) {
		/* errno has been set by fopen */
		return 0;
	}

	count = 0;
	do {
		node = node->next;
		if (!string_isEmpty(node->key) && !string_isEmpty(node->value)) {
			fprintf(file, "%s=%s\n", node->key, node->value);
			++count;
		}
	 } while (node->next != NULL);

	fclose(file);

	return count;
}

unsigned int stringmap_loadFromArguments(stringmap_t *self, int argc, char *argv[]) {
	int i;
	const char *sep;
	unsigned int count; /* counts read lines */
	char *key, *value; /* key and value pair of a line */

	assert(self);

	if (argc <= 1) {
		return 0;
	}

	count = 0;
	for (i = 1; i < argc; ++i) {
		sep = strchr(argv[i], '=');
		if (sep) {
			key = (char *)malloc (sep - argv[i] + 1);
			if (!key) return 0;
			memcpy(key, argv[i], sep-argv[i]);
			key[sep - argv[i]] = '\0';
			value = strdup(sep + 1);
			if (!value)  {
				free(key);
				return 0;
			}
			stringmap_addReplace(self, key, value);
			if (errno) return count;
			count++;
			free(value);
			free(key);
		} else {
			if (!file_exists(argv[i])) {
				errno = EACCES;
				return 0;
			}
			count += stringmap_loadFromFile(self, argv[i]);
			if (errno) return 0;
		}
	}

	return count;
}
