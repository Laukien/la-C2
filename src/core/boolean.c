/*
 * =====================================================================================
 *
 *       Filename:  boolean.c
 *    Description:  boolean values
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <la/boolean.h>
#include <la/string.h>

boolean_t boolean_isBoolean(const char *value) {
	int len; /* length of string */
	char clow; /* temporary lowwered character */
	char *slow; /* temporary lowered string */
	boolean_t res; /* result as boolean */

	if (value == NULL) {
		return boolean_false;
	}

	len = strlen(value);
	if (len == 0) {
		return boolean_false;
	}

	/* one character */
	if (len == 1) {
		clow = tolower(value[0]);
		return
			clow == 't' ||
			clow == 'f' ||
			clow == 'y' ||
			clow == 'n' ||
			clow == '1' ||
			clow == '0';
	}

	/* one word */
	slow = string_toLower(value);
	res =
		strcmp(slow, "true") == 0 ||
		strcmp(slow, "false") == 0||
		strcmp(slow, "yes") == 0 ||
		strcmp(slow, "no") == 0 ||
		strcmp(slow, "on") == 0 ||
		strcmp(slow, "off") == 0;
	free(slow);

	return res;
}

boolean_t  boolean_isTrue(const char *value) {
	int len; /* length of string */
	char clow; /* temporary lowwered character */
	char *slow; /* temporary lowered string */
	boolean_t res; /* result as boolean */

	if (value == NULL) {
		return boolean_false;
	}

	len = strlen(value);
	if (len == 0) {
		return boolean_false;
	}

	/* one character */
	if (len == 1) {
		clow = tolower(value[0]);
		return
			clow == 't' ||
			clow == 'y' ||
			clow == '1';
	}

	/* one word */
	slow = string_toLower(value);
	res =
		strcmp(slow, "true") == 0 ||
		strcmp(slow, "yes") == 0 ||
		strcmp(slow, "on") == 0;
	free(slow);

	return res;
}

boolean_t boolean_isFalse(const char *value) {
	int len; /* length of string */
	char clow; /* temporary lowwered character */
	char *slow; /* temporary lowered string */
	boolean_t res; /* result as boolean */

	if (value == NULL) {
		return boolean_false;
	}

	len = strlen(value);
	if (len == 0) {
		return boolean_false;
	}

	/* one character */
	if (len == 1) {
		clow = tolower(value[0]);
		return
			clow == 'f' ||
			clow == 'n' ||
			clow == '0';
	}

	/* one word */
	slow = string_toLower(value);
	res =
		strcmp(slow, "false") == 0 ||
		strcmp(slow, "no") == 0 ||
		strcmp(slow, "off") == 0;
	free(slow);

	return res;
}

boolean_t boolean_toBoolean(const char *value) {
	char *slow; /* length of string */
	boolean_t res; /* result as boolean */

	if (value == NULL) {
		return boolean_false;
	}

	/* one character */
	if (strlen(value) == 1) {
		return
			value[0] == 't' ||
			value[0] == 'y' ||
			value[0] == '1';
	}

	/* one word */
	slow = string_toLower(value);
	res =
		strcmp(slow, "true") == 0 ||
		strcmp(slow, "yes") == 0 ||
		strcmp(slow, "on") == 0;
	free (slow);

	return res;
}

char *boolean_toString(boolean_t value) {
	char *res; /* result as string */

	if (value) {
		res = strdup("true");
	} else {
		res = strdup("false");
	}

	return res;
}
