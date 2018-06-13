/*
 * =====================================================================================
 *
 *       Filename:  memory.h
 *    Description:  additional memory management
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#ifndef LA_MEMORY_H
#define LA_MEMORY_H

#include <stddef.h>

void *memory_set(void *buffer, int character, size_t size);
void *memory_zero(void *buffer, size_t size);
void memory_print(void *buffer, size_t size);

#endif

