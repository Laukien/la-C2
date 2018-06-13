/*
 * =====================================================================================
 *
 *       Filename:  array.c
 *    Description:  additional memory management
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <la/memory.h>

void *memory_set(void *buffer, int character, size_t size) {
	char *p; /* pointer to buffer */

	if (!size) {
		errno = EINVAL;
		return NULL;
	}

	p = (char *)buffer;

	while (size) {
		*p++ = character;
		--size;
	}

	return buffer;
}

void *memory_zero(void *buffer, size_t size) { 
	char *p; /* pointer to buffer */

	if (!size) {
		errno = EINVAL;
		return NULL;
	}

	/* write buffer */
	memset(buffer, '\0', size);

	/* suppress GCC- & LLVM-optimize */
	asm volatile("" : : "r"(&buffer) : "memory");

	/* convert type */
	p = (char *)buffer;

	/* make read-operation */
	if (size > 1) {
		if (p[0] > p[size - 1]) p[0] = p[size - 1];
		else p[size - 1] = p[0];
	} else {
		if (p[0] != '\0') p[0] = '\0';
	}

	return buffer;
}

void memory_print(void *buffer, size_t size) {
	char *p; /* pointer to buffer */

	if (!size) {
		errno = EINVAL;
		return;
	}

	p = (char *)buffer;

	while (size) {
		putc(*p++, stdout);
		--size;
	}
}
