/*
 * =====================================================================================
 *
 *       Filename:  string.h
 *    Description:  common string operations
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#ifndef LA_STRING_H
#define LA_STRING_H

#include <la/boolean.h>

char *string_toLower(const char *str);
char *string_toUpper(const char *str);
char *string_toCamel(const char *str);
char *string_trim(const char *str);
char *string_replaceFirst(const char *str, const char *from, const char *to);
char *string_replace(const char *str, const char *from, const char *to);
boolean_t string_isEmpty(const char *str);
boolean_t string_isAlnum(const char *str);
boolean_t string_isAlpha(const char *str);
boolean_t string_isCntrl(const char *str);
boolean_t string_isDigit(const char *str);
boolean_t string_isGraph(const char *str);
boolean_t string_isLower(const char *str);
boolean_t string_isPrint(const char *str);
boolean_t string_isPunct(const char *str);
boolean_t string_isSpace(const char *str);
boolean_t string_isUpper(const char *str);
boolean_t string_isXdigit(const char *str);
char *string_loadFromFile(const char *filename);
boolean_t string_saveToFile(const char *filename, const char *str);
char *string_fromHex(const char *str);
char *string_toHex(const char *str);
char *string_getRandom(size_t size, boolean_t upper, boolean_t lower, boolean_t number, boolean_t special);
boolean_t string_startsWith(const char *str, const char *prefix);
boolean_t string_endsWith(const char *str, const char *suffix);

#endif
