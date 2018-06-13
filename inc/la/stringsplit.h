/*
 * =====================================================================================
 *
 *       Filename:  stringspilt.h
 *    Description:  splits a string
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#ifndef LA_STRINGSPLIT_H
#define LA_STRINGSPLIT_H

#include <la/stringlist.h>

stringlist_t *string_split(const char *str, const char *delimiters);

#endif
