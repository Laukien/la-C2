/*
 * =====================================================================================
 *
 *       Filename:  stringbuffer.h
 *    Description:  variable-length sequence of characters (strings)
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#ifndef LA_STRINGBUFFER_H
#define LA_STRINGBUFFER_H

typedef struct la_stringbuffer stringbuffer_t;

stringbuffer_t *stringbuffer_init();
void stringbuffer_free(stringbuffer_t *self);
size_t stringbuffer_getLength(stringbuffer_t *self);
void stringbuffer_append(stringbuffer_t *self, const char *str);
void stringbuffer_insert(stringbuffer_t *self, const char *str, unsigned int pos);
char *stringbuffer_getText(stringbuffer_t *self);
char *stringbuffer_getTextPointer(stringbuffer_t *self);
stringbuffer_t *stringbuffer_clone(stringbuffer_t *self);
void stringbuffer_reset(stringbuffer_t *self);

#endif
