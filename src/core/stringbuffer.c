/*
 * =====================================================================================
 *
 *       Filename:  stringbuffer.c
 *    Description:  variable-length sequence of characters (strings)
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
#include <la/stringbuffer.h>

struct la_stringbuffer {
	char *text;
	size_t size;
};

stringbuffer_t *stringbuffer_init() {
	stringbuffer_t *self; /* object */

	self = (stringbuffer_t *)malloc(sizeof(stringbuffer_t));
	if (!self) {
		return NULL;
	}

	self->text = (char *)malloc(1);
	self->text[0] = '\0';
	self->size = 0;

	return self;
}

void stringbuffer_free(stringbuffer_t *self) {
	assert(self);

	self->size = 0;

	if (self->text) {
        free(self->text);
        self->text = NULL;
    }

    if (self) {
        free(self);
        self = NULL;
    }
}

size_t stringbuffer_getLength(stringbuffer_t *self) {
	assert(self);

	return self->size;
}

void stringbuffer_append(stringbuffer_t *self, const char *str) {
	assert(self);

	if (!str) {
		errno = EINVAL;
		return;
	}

	self->size = self->size + strlen(str);        /* set new length */
	self->text = (char *)realloc(self->text, self->size + 1);
	if (self->text == NULL) {
		return;
	}
	memcpy(self->text + strlen(self->text), str, strlen(str)); 
	self->text[self->size] = '\0';             /* end string */
}

void stringbuffer_insert(stringbuffer_t *self, const char *str, unsigned int pos) {
	char *tmp; /* temporary buffer */

	assert(self);

	if (!str) {
		errno = EINVAL;
		return;
	}

	self->size = self->size + strlen(str);        /* set new length */
	tmp = (char*) malloc(self->size + 1);
	if (!tmp) {
		return;
	}
	if (pos) {
		memcpy(tmp, self->text, pos); 
	}
	memcpy(tmp + pos, str, strlen(str));
	if(pos != strlen(self->text)) {
		memcpy(tmp + pos + strlen(str), self->text + pos, strlen(self->text) - pos);
	}

	free(self->text);
	self->text = tmp;
}

char *stringbuffer_getText(stringbuffer_t *self) {
	assert(self);

	return strdup(self->text);
}

char *stringbuffer_getTextPointer(stringbuffer_t *self) {
	assert(self);

	return self->text;
}

stringbuffer_t *stringbuffer_clone(stringbuffer_t *self) {
	stringbuffer_t *new_self; /* object */

	assert(self);

	new_self = stringbuffer_init();
	stringbuffer_append(new_self, self->text);

	return new_self;
}

void stringbuffer_reset(stringbuffer_t *self) {
	assert(self);

	free(self->text);
	self->text = (char *)malloc(1);
	if (self->text == NULL) {
		return;
	}
	self->size = 0;
}
