/*
 * =====================================================================================
 *
 *       Filename:  string.c
 *    Description:  common string operations
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
#include <assert.h>
#include <time.h>
#include <la/character.h>
#include <la/string.h>

char *string_toLower(const char *str) {
	size_t len; /* length of string */
	char *result; /* result of convert */
	size_t i; /* for loop */

	if (!str) {
		errno = EINVAL;
		return NULL;
	}

	len = strlen(str);

	result = (char *)malloc (len + 1);
	for (i = 0; i < len; ++i) {
		result[i] = tolower(str[i]);
	}
	result[len] = '\0';

	return result;
}

char *string_toUpper(const char *str) {
	size_t len; /* length of string */
	char *result; /* result of convert */
	size_t i; /* for loop */

	if (!str) {
		errno = EINVAL;
		return NULL;
	}

	len = strlen(str);

	result = (char *)malloc (len + 1);
	for (i = 0; i <= len; ++i) {
		result[i] = toupper(str[i]);
	}
	result[len] = '\0';

	return result;
}

char *string_toCamel(const char *str) {
	char *result; /* trimmed result-string */
	size_t len; /* length of string */
	char *tmp; /* temporary result-string */
	size_t i, j; /* loops */
	int up; /* marks upper case */

	if (!str) {
		errno = EINVAL;
		return NULL;
	}

	len = strlen(str);
	tmp = (char *)malloc (len + 1);
	j = 0;
	up = 1;
	for (i = 0; i <= len; ++i) {
		if (!isalnum(str[i])) {
			up = 1;
			continue;
		}
		if (up) {
			tmp[j] = toupper(str[i]);
			up = 0;
		} else {
			tmp[j] = str[i];
		}
		++j;
	}
	tmp[j] = '\0';

	result = strdup(tmp);
	free(tmp);

	return result;
}

char *string_trim(const char *str) {
	char *trim;
	int idx_start, idx_stop;

	if (!str) {
		errno = EINVAL;
		return NULL;
	}

	idx_start = 0;
	idx_stop = strlen(str);
		
	while (str[idx_start] != '\0' && (iscntrl(str[idx_start]) || str[idx_start] == ' ')) idx_start++;
	while (idx_stop > idx_start && (iscntrl(str[idx_stop - 1]) || str[idx_stop -1] == ' ')) idx_stop--;
	if (idx_start == idx_stop) return NULL;


	trim = (char *)malloc (idx_stop - idx_start + 1);
	if (!trim) {
		errno = ENOMEM;
		return NULL;
	}

	memcpy(trim, str + idx_start, idx_stop - idx_start);
	trim[idx_stop - idx_start] = '\0';

	return trim;
}

char *string_replaceFirst(const char *str, const char *from, const char *to) {
	int string_size;
	int from_size;
	int to_size;
	int result_size;
	char *result;
	const char *begin; /* get the first position */

	if (!str || !from || !to) {
		errno = EINVAL;
		return NULL;
	}

	string_size = strlen(str);
	from_size = strlen(from);
	to_size = strlen(to);
	result_size = string_size - from_size + to_size;

	begin = strstr(str, from);   /* get the first position */
	if (begin == NULL) {                        /* 'from' doesn't exists */
		result = (char *)malloc(string_size + 1);
		if (result == NULL) {
			return NULL;
		}
		strcpy(result, str);                 /* copy  */
	} else {
		int idx = begin - str;
		result = (char *)malloc(result_size + 1);
		if (result == NULL) {
			return NULL;
		}
		memcpy(result, str, idx);
		memcpy(result + idx, to, to_size);
		memcpy(result + idx + to_size, begin + from_size, string_size - from_size - idx);
		result[result_size] = '\0';
	}

	return result;
}

char *string_replace(const char *str, const char *from, const char *to) {
	char *result;
	char *ins;
	char *tmp;
	int len_from;
	int len_to;
	int len_front;
	int count;

	if (!str || !from || !to) {
		errno = EINVAL;
		return NULL;
	}

	len_from = strlen(from);
	len_to = strlen(to);
	ins = strstr((char*)str, from);
	if (!ins) {
		return strdup(str);
	}

	/* count hits */
	for (count = 0; (tmp = strstr(ins, from)) != NULL; ++count) {
		ins = tmp + len_from;
	}

	/* get memory */
	tmp = result = (char *) malloc(strlen(str) + (len_to - len_from) * count + 1);
	if (result == NULL) {
		return NULL;
	}

	while (count--) {
		ins = strstr((char *)str, from);
		len_front = ins - str;
		tmp = strncpy(tmp, str, len_front) + len_front;
		tmp = strcpy(tmp, to) + len_to;
		str += len_front + len_from;
	}
	strcpy(tmp, str);

	return result;
}



boolean_t string_isEmpty(const char *str) {
	char *t;
	size_t len;

	if (!str) {
		errno = EINVAL;
		return boolean_false;
	}

	t = string_trim(str);
	if (t == NULL) return boolean_true;
	len = strlen(t);
	free(t);

	return len == 0 ? boolean_true : boolean_false;
}

boolean_t string_isAlnum(const char *str) {
	size_t i;

	if (!str) {
		errno = EINVAL;
		return boolean_false;
	}

	for (i = 0; i < strlen(str); ++i) {
		if (!isalnum(str[i])) return boolean_false;
	}

	return boolean_true;
}

boolean_t string_isAlpha(const char *str) {
	size_t i;

	if (!str) {
		errno = EINVAL;
		return boolean_false;
	}

	for (i = 0; i < strlen(str); ++i) {
		if (!isalpha(str[i])) return boolean_false;
	}

	return boolean_true;
}

boolean_t string_isCntrl(const char *str) {
	size_t i;

	if (!str) {
		errno = EINVAL;
		return boolean_false;
	}

	for (i = 0; i < strlen(str); ++i) {
		if (!iscntrl(str[i])) return boolean_false;
	}

	return boolean_true;
}

boolean_t string_isDigit(const char *str) {
	size_t i;

	if (!str) {
		errno = EINVAL;
		return boolean_false;
	}

	for (i = 0; i < strlen(str); ++i) {
		if (!isdigit(str[i])) return boolean_false;
	}

	return boolean_true;
}

boolean_t string_isGraph(const char *str) {
	size_t i;

	if (!str) {
		errno = EINVAL;
		return boolean_false;
	}

	for (i = 0; i < strlen(str); ++i) {
		if (!isgraph(str[i])) return boolean_false;
	}

	return boolean_true;
}

boolean_t string_isLower(const char *str) {
	size_t i;

	if (!str) {
		errno = EINVAL;
		return boolean_false;
	}

	for (i = 0; i < strlen(str); ++i) {
		if (!islower(str[i])) return boolean_false;
	}

	return boolean_true;
}

boolean_t string_isPrint(const char *str) {
	size_t i;

	if (!str) {
		errno = EINVAL;
		return boolean_false;
	}

	for (i = 0; i < strlen(str); ++i) {
		if (!isprint(str[i])) return boolean_false;
	}

	return boolean_true;
}

boolean_t string_isPunct(const char *str) {
	size_t i;

	if (!str) {
		errno = EINVAL;
		return boolean_false;
	}

	for (i = 0; i < strlen(str); ++i) {
		if (!ispunct(str[i])) return boolean_false;
	}

	return boolean_true;
}

boolean_t string_isSpace(const char *str) {
	size_t i;

	if (!str) {
		errno = EINVAL;
		return boolean_false;
	}

	for (i = 0; i < strlen(str); ++i) {
		if (!isspace(str[i])) return boolean_false;
	}

	return boolean_true;
}

boolean_t string_isUpper(const char *str) {
	size_t i;

	if (!str) {
		errno = EINVAL;
		return boolean_false;
	}

	for (i = 0; i < strlen(str); ++i) {
		if (!isupper(str[i])) return boolean_false;
	}

	return boolean_true;
}

boolean_t string_isXdigit(const char *str) {
	size_t i;

	if (!str) {
		errno = EINVAL;
		return boolean_false;
	}

	for (i = 0; i < strlen(str); ++i) {
		if (!isxdigit(str[i])) return boolean_false;
	}

	return boolean_true;
}

char *string_loadFromFile(const char *filename) {
	FILE *file;
	char line[512 + 1];
	char *str;

	if (!filename) {
		errno = EINVAL;
		return NULL;
	}

	file = fopen(filename, "r");
	if (file == NULL) return NULL;

	str = (char *) malloc(1024 * 10);           /* get 10KB */
	str[0] = '\0';                              /* empty string */
	while (fgets(line, 512, file) != NULL) {
		str = (char *) realloc(str, strlen(str) + strlen(line) + 1);
		strcat(str, line);
	}

	fclose(file);

	return str;
}

boolean_t string_saveToFile(const char *filename, const char *str) {
	FILE *file;

	if (!filename || !str) {
		errno = EINVAL;
		return boolean_false;
	}

	file = fopen(filename, "w");
	if (file == NULL) return boolean_false;

	fputs(str, file);

	fclose(file);

	return boolean_true;
}

char *string_fromHex(const char *str) {
	unsigned int len; /* length of the result-string */
	char *result;
	unsigned int i;

	if (!str) {
		errno = EINVAL;
		return NULL;
	}
	
	len = strlen(str) / 2;	

	result = (char *)malloc (len + 1);
	if (!result) {
		errno = ENOMEM;
		return NULL;
	}

	for (i = 0; i < len; ++i) {
		result[i] = character_fromHex(str[i * 2]) << 4 | character_fromHex(str[i * 2 + 1]);
	}

	result[len] = '\0';

	return result;
}

char *string_toHex(const char *str) {
	unsigned int len; /* length of string */
	char *result; /* result as string */
	unsigned int i; /* for-loop */

	if (!str) {
		errno = EINVAL;
		return NULL;
	}
	
	len = strlen(str);
	result = (char *)malloc ((len * 2) + 1);
	if (!result) {
		errno = ENOMEM;
		return NULL;
	}

	for (i = 0; i < len; ++i) {
		result[i * 2] = character_toHex(str[i] >> 4);
		result[i * 2 + 1] = character_toHex(str[i] & 15);
	}
	result [len * 2] = '\0';

	return result;
}

char *string_getRandom(size_t size, boolean_t upper, boolean_t lower, boolean_t number, boolean_t special) {
	int start;
	int stop;
	char *res;
	char c;
	size_t i;

	if (!size) {
		errno = EINVAL;
		return NULL;
	}
	
	srand(time(NULL));

	if (special) {
		start = 32;
		stop = 126;
	} else {
		start = 48;
		stop = 122;
	}

	res = (char *)malloc(size + 1);
	if (!res) {
		errno = ENOMEM;
		return NULL;
	}
	
	for (i = 0; i < size; ++i) {
		while (1) {
			c = (rand() % (stop - start)) + start;
			if (special) break;
			if (c >= '0' && c <= '9' && number) break;
			if (c >= 'A' && c <= 'Z' && upper) break;
			if (c >= 'a' && c <= 'z' && lower) break;
		}
		res[i] = c;
	}
	res[size] = '\0';

	return res;
}

boolean_t string_startsWith(const char *str, const char *prefix) {
    int len_str;
    int len_prefix;

	if (!str) {
		errno = EINVAL;
		return boolean_false;
	}

	if (!prefix) {
		errno = EINVAL;
		return boolean_false;
	}

    len_str = strlen(str);
    len_prefix = strlen(prefix);
    if (len_prefix > len_str) return boolean_false;

    if (strncmp(str, prefix, len_prefix) == 0) {
        return boolean_true;
    } else {
        return boolean_false;
    }

}

boolean_t string_endsWith(const char *str, const char *suffix) {
    char *idx;
    size_t len_str;
    size_t len_suffix;

	if (!str) {
		errno = EINVAL;
		return boolean_false;
	}

	if (!suffix) {
		errno = EINVAL;
		return boolean_false;
	}

    idx = strstr(str, suffix);
    if (!idx) return boolean_false;

    len_str = strlen(str);
    len_suffix = strlen(suffix);
    if (len_suffix > len_str) return boolean_false;

    if (strncmp(idx, suffix, len_suffix) == 0) {
        return boolean_true;
    } else {
        return boolean_false;
    }
}

