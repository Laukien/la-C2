/*
 * =====================================================================================
 *
 *       Filename:  str.h
 *    Description:  simplified string
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#ifndef LA_STR_H
#define LA_STR_H

#define strlower(str) string_toLower(str)
#define strupper(str) string_toUpper(str)
#define strcamel(str) string_toCamel(str)
#define strtrim(str) string_trim(str)
#define strreplace(str, from, to) string_replace(str, from, to)
#define strload(filename) string_loadFromFile(filename);
#define strsave(filename, str) string_saveToFile(filename, str);

#include <la/string.h>

#endif

