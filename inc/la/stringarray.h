/*
 * =====================================================================================
 *
 *       Filename:  stringarray.h
 *    Description:  simple string-vector
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#ifndef LA_STRINGARRAY_H
#define LA_STRINGARRAY_H

#include <stddef.h>
	typedef struct la_stringarray stringarray_t;

	stringarray_t *stringarray_init();
	void stringarray_free(stringarray_t *self);
	void stringarray_reset(stringarray_t *self);
	void stringarray_setSize(stringarray_t *self, size_t size);
	size_t stringarray_getSize(stringarray_t *self);
	void stringarray_show(stringarray_t *self);
	void stringarray_setString(stringarray_t *self, unsigned int idx, const char *str);
	char *stringarray_getString(stringarray_t *self, unsigned int idx);
	char **stringarray_getArray(stringarray_t *self);
	void stringarray_showArray(char **array);
	void stringarray_freeArray(char **array);
	void stringarray_loadFromString(stringarray_t *self, const char *str, const char sep);
	
#endif

