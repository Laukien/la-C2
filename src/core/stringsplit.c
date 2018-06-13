/*
 * =====================================================================================
 *
 *       Filename:  stringsplit.c
 *    Description:  splits a string
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <la/stringsplit.h>


stringlist_t *string_split(const char *str, const char *delimiters) {
	stringlist_t *list; /* list of tokens */
	const char *s; /* constant pointer to string */
	const char *e; /* copy of the pointer */
	size_t len; /* length of the token */
	char *token; /* token as pointer to the string */

	if (!str || !delimiters) {
		errno = EINVAL;
		return NULL;
	}

	list = stringlist_init();
	s = str;
	e = s;

	while (*e != 0) {
		e = s;
		while (*e != 0 && strchr(delimiters, *e) == 0) ++e;
		if (e - s > 0) {
			len = e - s;
			token = (char *) malloc(len + 1);

			memcpy(token, s, len);
			token[len] = '\0';                  /* finish string */

			stringlist_add(list, token);        /* add token to list */

			free(token);
		}
		s = e + 1;
	}

	return list;
}

