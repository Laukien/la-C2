/*
 * =====================================================================================
 *
 *       Filename:  la_list.c
 *    Description:  handle (string) objects in a linked list
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <la/string.h>
#include <la/stringlist.h>

struct la_stringlist {
	char *value;
	struct la_stringlist *next;
};

stringlist_t *stringlist_getNode(stringlist_t *self, unsigned int index) {
	stringlist_t *node;
	unsigned int count;

	assert(self);

	node = self;
	if (self->next == NULL) return NULL;       /* check if list-set is empty */

	count = 0;
	do {
		node = node->next;

		if (index == count) {
			return node;
		} else ++count;
	} while (node->next != NULL);

	return NULL;
}

stringlist_t *stringlist_init () {
	stringlist_t *self;
	
	self = (stringlist_t*)malloc(sizeof(stringlist_t));
	if (self == NULL) {
		return NULL;
	}
	
	self->value = NULL;
	self->next = NULL;

	return self;
}

void stringlist_add(stringlist_t *self, const char *value) {
	unsigned int size; /* size of list */
	stringlist_t *node; /* new node */

	assert(self);

	node = (stringlist_t *)malloc(sizeof(stringlist_t));
	if (node == NULL) {
		return;
	}

	/* init next */
	node->next = NULL;

	/* create value */
	node->value = (char *)malloc(strlen(value) + 1);
	if ( node->value==NULL) {
		free(node);
		return;
	}
	strcpy(node->value, value);

	/* set pointer */
	size = stringlist_size(self);
	if (size == 0) {
		node->next = self->next;
		self->next = node;
	} else {
		stringlist_t *last = stringlist_getNode(self, size - 1);
		last->next = node;
	}
}

void stringlist_addUnique(stringlist_t *self, const char *value) {
	assert(self);

	if (!stringlist_exists(self, value))
		stringlist_add(self, value);
}

boolean_t stringlist_exists(stringlist_t *self, const char *value) {
	stringlist_t *node;

	assert(self);

	node = self;
	if (self->next == NULL)
		return boolean_false;

	do {
		node = node->next;

		if (strcmp(value, node->value) == 0)
			return boolean_true;
	} while (node->next != NULL);

	return boolean_false;
}

void stringlist_remove(stringlist_t *self, unsigned int index) {
	unsigned int size; /* size of list */
	stringlist_t *nodeSelf, *nodePrev, *nodeNext;

	assert(self);

	nodeSelf = stringlist_getNode(self, index);
	if (nodeSelf == NULL) return;
	
	size = stringlist_size(self);
	if (size <= 1) {
		stringlist_reset(self);
		return;
	}

	if (index == 0) {                           /* first element */
		nodeNext = nodeSelf->next;
		self->next = nodeNext;
	} else if (index == (size - 1)) {           /* last element */
		nodePrev = stringlist_getNode(self, index - 1);
		nodePrev->next = NULL;
	} else {
		nodePrev = stringlist_getNode(self, index - 1);
		nodeNext = nodeSelf->next;
		nodePrev->next = nodeNext;
	}
	free(nodeSelf->value);
	nodeSelf->value = NULL;
	free(nodeSelf);
	nodeSelf = NULL;
}

void stringlist_swap(stringlist_t *self, unsigned int index1, unsigned int index2) {
	stringlist_t *node1, *node2;
	char *tmp;

	assert(self);

	if (index1 == index2) return;

	node1 = stringlist_getNode(self, index1);
	node2 = stringlist_getNode(self, index2);

	tmp = node1->value;
	node1->value = node2->value;
	node2->value = tmp;
}

void stringlist_sort(stringlist_t *self) {
	boolean_t swap;
	unsigned int len;
	unsigned int i;

	assert(self);

	/* only two or more items */
	len = stringlist_size(self);
	if (len <= 1) {
		return;
	}

	do {
		swap = boolean_false;
		for (i = 0; i < len - 1; ++i) {
			if (strcmp(stringlist_getNode(self, i)->value, stringlist_getNode(self, i + 1)->value) > 0) {
				swap = boolean_true;
				stringlist_swap(self, i, i + 1);
			}
		}
	} while (swap);
}

char *stringlist_get(stringlist_t *self, unsigned int index) {
	stringlist_t *node;
	unsigned int count; /* counter */

	assert(self);

	node = self;
	if (self->next == NULL) return NULL;       /* check if list-set is empty */

	count = 0;
	do {
		node = node->next;

		if (index == count) {
			/* copy value */
			char *value;
			value = (char *) malloc (strlen(node->value)+1);
			strcpy(value, node->value);
			return value;
		} else ++count;
	} while (node->next != NULL);

	return NULL;
}

unsigned int stringlist_size(stringlist_t *self) {
	stringlist_t *node;
	unsigned int count; /* counter */

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


void stringlist_free(stringlist_t *self) {
	stringlist_t *node;
	stringlist_t *next;

	assert(self);

	node = self;                               /* get the first list */
	while (node->next != NULL) {
		next = node->next;
		if (node->value != NULL) free(node->value);
		free(node);
		node = next;
	}
	
	if (node->value != NULL) free(node->value);
    if (node != NULL) free(node);               /* free only if there are items */
	node = NULL;
}

void stringlist_reset(stringlist_t *self) {
	stringlist_t *node;
	stringlist_t *next;

	assert(self);

	node = self->next;                         /* get the next list */
	if (node == NULL) return;

	while (node->next != NULL) {
		next = node->next;
		if ( node->value != NULL) free(node->value);
		free(node);
		node = next;
	}
	
	if ( node->value != NULL) free(node->value);
	free(node);
	node = NULL;

	self->next = NULL;
}

void stringlist_show(stringlist_t *self) {
	unsigned int i;
	char *str;

	assert(self);

	for (i = 0; i < stringlist_size(self); ++i) {
		str = stringlist_get(self, i);
		puts(str);
		free(str);
	}
}

unsigned int stringlist_loadFromFile(stringlist_t *self, const char *filename) {
	unsigned int count; /* counter */
	FILE *file;
	char line[STRINGLIST_VALUE_SIZE + 2 + 1]; /* buffer for one line */
	int len; /* length of line */

	assert(self);

	stringlist_reset(self);

	file = fopen(filename, "r");
	if (file == NULL) return -1;

	count = 0;
	len = sizeof(line)/sizeof(line[0]);
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

		stringlist_add(self, line);
		++count;

	}
	fclose(file);

	return count;
}

unsigned int stringlist_saveToFile(stringlist_t *self, const char *filename) {
	stringlist_t *node;
	unsigned int count;
	FILE *file;

	assert(self);

	if (self->next == NULL) return 0;          /* no lists to save */

	node = self;

	file = fopen(filename, "w");
	if (file == NULL) return -1;

	count = 0;
	do {
		node = node->next;
		if (!string_isEmpty(node->value)) {
			fprintf(file, "%s\n", node->value);
			++count;
		}
	 } while (node->next != NULL);

	fclose(file);

	return count;
}
