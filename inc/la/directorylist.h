/*
 * =====================================================================================
 *
 *       Filename:  directorylist.h
 *    Description:  lists directory structure
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#ifndef LA_DIRECTORYLIST_H
#define LA_DIRECTORYLIST_H

#include <la/boolean.h>
#include <la/stringlist.h>

stringlist_t *directory_list(const char *directoryname, boolean_t recursive);

#endif
