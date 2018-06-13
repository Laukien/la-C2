/*
 * =====================================================================================
 *
 *       Filename:  filelist.h
 *
 *    Description:  lists files
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

#ifndef LA_FILELIST_H
#define LA_FILELIST_H

#include <la/boolean.h>
#include <la/stringlist.h>

stringlist_t *file_list(const char *directoryname, boolean_t recursive);

#endif
