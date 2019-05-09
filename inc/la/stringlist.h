/*
 * =====================================================================================
 *
 *       Filename:  stringlist.h
 *    Description:  handle (string) objects in a linked list
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#ifndef LA_STRINGLIST_T_H
#define LA_STRINGLIST_T_H

#define STRINGLIST_VALUE_SIZE 2048

#include <la/boolean.h>

typedef struct la_stringlist stringlist_t;

stringlist_t *stringlist_init(void);
void stringlist_add(stringlist_t *self, const char *value);
void stringlist_addUnique(stringlist_t *self, const char *value);
boolean_t stringlist_exists(stringlist_t *self, const char *value);
void stringlist_remove(stringlist_t *self, unsigned int index);
void stringlist_swap(stringlist_t *self, unsigned int index1, unsigned int index2);
void stringlist_sort(stringlist_t *self);
void stringlist_free(stringlist_t *self);
void stringlist_reset(stringlist_t *self);
char *stringlist_get(stringlist_t *self, unsigned int index);
unsigned int stringlist_size(stringlist_t *self);
void stringlist_show(stringlist_t *self);
unsigned int stringlist_loadFromFile(stringlist_t *self, const char *filename);
unsigned int stringlist_saveToFile(stringlist_t *self, const char *filename);

#endif
