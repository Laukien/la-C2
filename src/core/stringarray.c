/*
 * =====================================================================================
 *
 *       Filename:  array.c
 *    Description:  simple string-array
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <la/message.h>
#include <la/stringarray.h>

struct la_stringarray {
	size_t size;
	char **array;
};

stringarray_t *stringarray_init() {
	stringarray_t *self; /* object */

	self = (stringarray_t*) malloc(sizeof(stringarray_t));
	if (!self) {
		return NULL;
	}
	
	self->size = 0;
	self->array = NULL;

	return self;
}

void stringarray_free(stringarray_t *self) {
	assert(self);

	stringarray_reset(self);

	if (self) {
        free(self);
        self = NULL;
    }
}

void stringarray_reset(stringarray_t *self) {
	size_t i; /* for loop */

	assert(self);

	for (i = 0; i < self->size; ++i) {
		if (self->array[i]) {
			free(self->array[i]);
			self->array[i] = NULL;
		}
	}

	if (self->array) {
		free(self->array);
		self->array = NULL;
	}

	self->size = 0;
}

void stringarray_setSize(stringarray_t *self, size_t size) {
	size_t i; /* for loop */

	assert(self);

	if (self->size) {
		stringarray_reset(self);
	} else {
		errno = EINVAL;
		return;
	}

	/* get memory */
	self->size = size;
	self->array = (char **)malloc((self->size + 1) * sizeof(char *));
	if (!self->array) {
		errno = ENOMEM;
		return;
	}
	
	/* clear memory */
	for (i = 0; i <= self->size; ++i) {
		self->array[i] = NULL;
	}
}

size_t stringarray_getSize(stringarray_t *self) {
	assert(self);

	return self->size;
}

void stringarray_show(stringarray_t *self) {
	size_t i; /*for loop */

	assert(self);

	for (i = 0; i < self->size; ++i) {
		puts(self->array[i]);
	}
}

void stringarray_setString(stringarray_t *self, unsigned int idx, const char *str) {
	assert(self);

	if (!str) {
		errno = EINVAL;
		return;
	}

	if (idx >= self->size) {
		errno = EINVAL;
		return;
	}

	if (self->array[idx]) {
		free(self->array[idx]);
		self->array[idx] = strdup(str);
	}
}

char *stringarray_getString(stringarray_t *self, unsigned int idx) {
	char *res; /* result */
	assert(self);

	if (idx >= self->size) {
		errno = EINVAL;
		return NULL;
	}

	res = strdup(self->array[idx]);

	return res;
}

char **stringarray_getArray(stringarray_t *self) {
	size_t i; /* for loop */
	char **array; /* buffer of pointers */

	assert(self);

	array = (char **)malloc((self->size + 1) * sizeof(char *));
	if (!self->array) {
		return NULL;
	}
	
	/* clear memory */
	for (i = 0; i <= self->size; ++i) {
		if (self->array[i]) {
			array[i] = strdup(self->array[i]);
		} else {
			array[i] = (char *)NULL;
		}
	}
	array[self->size] = (char *)NULL;

	return array;
}

void stringarray_showArray(char **array) {
	size_t count; /* increments index */

	assert(array);

	count = 0;
	while (array[count]) {
		puts(array[count++]);
	}
}

void stringarray_freeArray(char **array) {
	size_t count; /* increments index */

	assert(array);

	count = 0;
	while(array[count]) {
		if (array[count]) {
			free(array[count]);
			array[count] = NULL;
		}
		++count;
	}

	if (array) {
		free(array);
		array = NULL;
	}
}

void stringarray_loadFromString(stringarray_t *self, const char *str, const char sep) {
	size_t i;
	size_t count;
	int start;
	int stop;
	char *tok;

	assert(self);

	stringarray_reset(self);

	/* count tokens */
	count = 0;
	for (i = 0; i < strlen(str); ++i) {
		if (str[i] == sep) ++count;
	}
	++count;

	/* init memory */
	stringarray_setSize(self, count);

	/* separate & copy tokens */
	start = 0;
	count = 0;
	for (i = 0; i < strlen(str) + 1; ++i) {
		if (str[i] == sep || str[i] == '\0') {
			stop = i;
			tok = (char *)malloc(stop - start + 1);
			memcpy(tok, str + start, stop - start);
			tok[stop - start] = '\0';
			self->array[count++] = strdup(tok);
			free(tok);
			start = stop + 1;
		}
	}
	self->array[count] = NULL;
}
