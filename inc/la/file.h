/*
 * =====================================================================================
 *
 *       Filename:  file.h
 *
 *    Description:  header for la_file.c
 *
 *        Version:  1.0
 *        Created:  11/11/2011 02:26:29 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien (slaukien), software@laukien.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef LA_FILE_H
#define LA_FILE_H

#include <string.h>
#include <la/boolean.h>

/*
 * checks if the given file exists
 */
boolean_t file_exists(const char *filename);

/*
 * removes the given file from the filesystem
 */
boolean_t file_remove(const char *filename);

/*
 * returns the name of the file without path
 */
char *file_getName(const char *file);

/*
 * returns a valid temp-file
 */
char *file_getTemp();
/*
 * returns the szie of the file
 */
size_t file_getSize(const char *filename);

/*
 * extracts the extension of the file
 */
char *file_getExtension(const char *filename);

/*
 * checks if the given file already exists
 * adds a counter until the file doesn't exists
 * the first free file will be returnd
 */
char *file_getNext(const char *filename);

#endif
