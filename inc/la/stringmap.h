/*
 * =====================================================================================
 *
 *       Filename:  stringmap.h
 *    Description:  handle (string) objects in a linked map
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#ifndef LA_STRINGMAP_H
#define LA_STRINGMAP_H

#define STRINGMAP_KEY_SIZE 128
#define STRINGMAP_VALUE_SIZE 2048

#include <la/boolean.h>

typedef struct la_stringmap stringmap_t;

stringmap_t *stringmap_init();
void stringmap_add(stringmap_t *self, const char *key, const char *value);
void stringmap_addUnique(stringmap_t *self, const char *key, const char *value);
void stringmap_addReplace(stringmap_t *self, const char *key, const char *value);
boolean_t stringmap_exists(stringmap_t *self, const char *key);
void stringmap_remove(stringmap_t *self, const char *key);
void stringmap_free(stringmap_t *self);
void stringmap_reset(stringmap_t *self);
stringmap_t *stringmap_getByIndex(stringmap_t *self, unsigned int index);
char *stringmap_getKeyByIndex(stringmap_t *self, unsigned int index);
char *stringmap_getValueByIndex(stringmap_t *self, unsigned int index);
unsigned int stringmap_getIndexByKey (stringmap_t *self, const char *key);
char *stringmap_getValueByKey(stringmap_t *param, const char *key);
char *stringmap_get(stringmap_t *self, const char *key);
unsigned int stringmap_size(stringmap_t *self);
void stringmap_show(stringmap_t *self);
unsigned int stringmap_loadFromFile(stringmap_t *param, const char *filename);
unsigned int stringmap_saveToFile(stringmap_t *param, const char *filename);
unsigned int stringmap_loadFromArguments(stringmap_t *self, int argc, char *argv[]);

#endif

