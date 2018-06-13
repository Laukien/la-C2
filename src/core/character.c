/*
 * =====================================================================================
 *
 *       Filename:  la_character.c
 *    Description:  character functions
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <la/character.h>

char character_fromHex(const char chr) {
	assert(chr >= '0' && chr <= 'f'); 
	assert(chr <= '9' || chr >= 'A'); 
	assert(chr <= 'F' || chr >= 'a'); 

	return isdigit(chr) ? (chr - '0') : (tolower(chr) - 'a' + 10);
}

char character_toHex(const char chr) {
	static char hex[] = "0123456789abcdef";

	return hex[chr & 15];
}

char character_fromBase64(const char chr) {
	if(chr >= 'A' && chr <= 'Z') return(chr - 'A');
	if(chr >= 'a' && chr <= 'z') return(chr - 'a' + 26);
	if(chr >= '0' && chr <= '9') return(chr - '0' + 52);
	if(chr == '+') return 62;

	return 63;
}

char character_toBase64(const char chr) {
	if(chr < 26)  return 'A'+ chr;
	if(chr < 52)  return 'a'+ (chr - 26);
	if(chr < 62)  return '0'+ (chr - 52);
	if(chr == 62) return '+';

	return '/';
}

boolean_t character_isBase64(const char chr) {
	if (
		(chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z') ||
		(chr >= '0' && chr <= '9') || (chr == '+') ||
		(chr == '/') || (chr == '=')
		) {
		return boolean_true;
	}

	return boolean_false;
}

char *character_replace(const char *str, const char from, const char to) {
	size_t len; /* length of string */
	char *res; /* result as string */
	size_t i; /* for-loop */

	assert(str != NULL);

	len = strlen(str);
	res = (char *) malloc(len + 1);
	if (res == NULL) return NULL;
	memset(res, '\0', len + 1);

	for (i = 0; i < len; ++i) {
		if (str[i] == from)
			res[i] = to;
		else
			res[i] = str[i];
	}

	return res;
}

unsigned int character_count(const char *str, const char chr) {
	unsigned int i; /* for-loop */
	unsigned int count; /* counter */

	count = 0;
	for (i = 0; i < strlen(str); ++i) {
		if (str[i] == chr)
			++count;
	}

	return count;
}
