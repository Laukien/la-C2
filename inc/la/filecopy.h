/*
 * =====================================================================================
 *
 *       Filename:  file.h
 *
 *    Description:  copies files
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

#ifndef LA_FILECOPY_H
#define LA_FILECOPY_H


typedef void (*FILE_PROCESS)(const char *from, const char *to, size_t size, size_t done);

void FILE_PROCESS_SIMPLE(const char *from, const char *to, size_t size, size_t done);
void FILE_PROCESS_DOT(const char *from, const char *to, size_t size, size_t done);
void FILE_PROCESS_PERCENT(const char *from, const char *to, size_t size, size_t done);
boolean_t file_copy(const char *from, const char *to, FILE_PROCESS proc);

#endif
