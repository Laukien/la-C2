/*
 * =====================================================================================
 *
 *       Filename:  number.c
 *    Description:  number converting
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <la/number.h>

boolean_t number_isNumber(const char *str) {
	unsigned int idx; /* index for search loop */

	if (!str) {
		errno = EINVAL;
		return boolean_false;
	}
	
	idx = 0;
	if (str[0] == '-')
		++idx;

	for (; idx < strlen(str); ++idx) {
        if (!isdigit(str[idx]))                 /* valid */
			return boolean_false;
	}

	return boolean_true;
}

boolean_t number_isInteger(const char *str) {
	unsigned int idx; /* index for search loop */
	int res;
	int old;

	if (!str) {
		errno = EINVAL;
		return boolean_false;
	}
	
	idx = 0;
	res = 0;
	old = 0;
	if (str[0] == '-')
		++idx;

	for (; idx < strlen(str); ++idx) {
		if (!isdigit(str[idx])) {                 /* valid */
			return boolean_false;
		}
		res *= 10;
		res += str[idx] - '0';                  /* ASCII */
		if (res < old) {                          /* overflow */
			return boolean_false;
		}
		old = res;
	}

	return boolean_true;
}

boolean_t number_isUnsignedInteger(const char *str) {
	unsigned int idx; /* index for search loop */
	unsigned int res;
	unsigned int old;

	if (!str) {
		errno = EINVAL;
		return boolean_false;
	}
	
	res = 0;
	old = 0;
	for (idx = 0; idx < strlen(str); ++idx) {
		if (!isdigit(str[idx])) {                /* valid */
			return boolean_false;
		}
		res *= 10;
		res += str[idx] - '0';                  /* ASCII */
		if (res < old) {                         /* overflow */
			return boolean_false;
		}
		old = res;
	}

	return boolean_true;
	
}

boolean_t number_isSigned(const char *str) {
	if (!str) {
		errno = EINVAL;
		return boolean_false;
	}
	
	return (str[0] == '-');
}

int number_toInteger(const char *str) {
	boolean_t min;
	unsigned int idx;
	int res;
	int old;

	if (!str) {
		errno = EINVAL;
		return 0;
	}
	
	min = boolean_false;
	idx = 0;
	res = 0;
	old = 0;

	if (str[0] == '-') {
		min = boolean_true;
		++idx;
	}

	for (; idx < strlen(str); ++idx) {
		if (!isdigit(str[idx])) {                 /* valid */
			errno = EINVAL;
			return 0;
		}
		res *= 10;
		res += str[idx] - '0';                  /* ASCII */
		if (res < old) {                         /* overflow */
			errno = EOVERFLOW;
			return 0;
		}
		old = res;
	}

	if (min) {
		old = res;
		res *= -1;                              /* add sign */
		if (res > old) {
			errno = EOVERFLOW;
			return 0;
		}
	}

	return res;
}

unsigned int number_toUnsignedInteger(const char *str) {
	unsigned int idx;
	unsigned int res;
	unsigned int old;

	if (!str) {
		errno = EINVAL;
		return 0;
	}
	
	idx = 0;
	res = 0;
	old = 0;

	for (idx = 0; idx < strlen(str); ++idx) {
		if (!isdigit(str[idx])) {                /* valid */
			errno = EINVAL;
			return 0;
		}
		res *= 10;
		res += str[idx] - '0';                  /* ASCII */
		if (res < old) {                          /* overflow */
			errno = EOVERFLOW;
		}
		old = res;
	}

	return res;
}

size_t number_getIntegerLength(int num) {
	size_t size; /* size of string */
	int i; /*for loop */

	if (num == 0) return 1;

	/* get size (log10) */
	size = 0;
	for (i = num; i != 0; i /= 10) {
		++size;
	}

	if (num < 0) ++size;                        /* negative value */

	return size;
}

size_t number_getUnsignedIntegerLength(unsigned int num) {
	size_t size; /* size of string */
	int i; /*for loop */

	if (num == 0) return 1;

	/* get size (log10) */
	size = 0;
	for (i = num; i != 0; i /= 10) {
		++size;
	}

	return size;
}

char *number_integerToString(int num) {
	size_t size; /* length of an integer number */
	char *str; /* integer as string */

	size = number_getIntegerLength(num);

	str = (char *) malloc(size + 1);

	sprintf(str, "%d", num);

	return str;
}

char *number_unsignedIntegerToString(unsigned int num) {
	size_t size; /* length of an integer number */
	char *str; /* integer as string */

	size = number_getUnsignedIntegerLength(num);

	str = (char *) malloc(size + 1);

	sprintf(str, "%d", num);

	return str;
}
