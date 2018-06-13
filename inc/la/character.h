/*
 * =====================================================================================
 *
 *       Filename:  character.h
 *    Description:  character functions
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#ifndef LA_CHARACTER_H
#define LA_CHARACTER_H

#include <la/boolean.h>

char character_fromHex(const char chr);
char character_toHex(const char chr);
char character_fromBase64(const char chr);
char character_toBase64(const char chr);
boolean_t character_isBase64(const char chr);
char *character_replace(const char *str, const char from, const char to);
unsigned int character_count(const char *str, const char chr);

#endif
