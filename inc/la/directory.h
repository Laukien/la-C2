/*
 * =====================================================================================
 *
 *       Filename:  directory.h
 *    Description:  I/O directory operations
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#ifndef LA_DIRECTORY_H
#define LA_DIRECTORY_H

#include <la/boolean.h>

#ifdef __WIN32
#define DIRECTORY_SEPARATOR_CHAR '\\'
#define DIRECTORY_SEPARATOR_STRING "\\"
#else	
#define DIRECTORY_SEPARATOR_CHAR '/'
#define DIRECTORY_SEPARATOR_STRING "/"
#endif

boolean_t directory_create(const char *directoryname);
boolean_t directory_exists(const char *directoryname);
char *directory_getName(const char *filename);
char *directory_getTemp();

#endif
