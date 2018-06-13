/**
 * =====================================================================================
 *
 *       Filename:  config.c
 *    Description:  parses INI files
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2018
 *
 * =====================================================================================
 */

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <la/config.h>
#include <la/file.h>
#include <la/string.h>
#include <la/stringlist.h>
#include <la/stringmap.h>

struct la_config {
	stringlist_t *sec;
	stringmap_t *map;
};

/*
 * Generates an internal ID to identify section-key-combination in the map.
 *
 * @param section       parent category
 * @param key           identifier inside the category
 * @return unique       key in the form "[section].key"; if error NULL
 * @errno EINVAL        key is empty
 * @errno EOVERFLOW     key-size to huge
 */
static char *getKeyId(const char *section, const char *key) {
    char *sec_lo; /* section lowercased */
    char *key_lo; /* key lowercased */
    char *key_id; /* complete key in the map */

    assert(section);
    assert(key);

    /* check values */
    if (strlen(key) == 0) {
        errno = EINVAL;
        return NULL;
    }
    if (strlen(section) + 3 + strlen(key) >= STRINGMAP_KEY_SIZE) {
        errno = EOVERFLOW;
        return NULL;
    }

    /* lowercase section and key */
    sec_lo = string_toLower(section);
    key_lo = string_toLower(key);

    /* create id -> [sec_lo].key_lo*/
    key_id = (char *)malloc(1 + strlen(section) + 1 + 1 + strlen(key) + 1);
    sprintf(key_id, "[%s].%s", sec_lo, key_lo);

    free(key_lo);
    free(sec_lo);

    return key_id;
}

/*
 * Generates an internal ID to identify section in the map.
 *
 * @param section       parent category
 * @return              section in the format "[section]."
 */
static char *getSecId(const char *section) {
    char *res;

    assert(section);

    res = (char *)malloc(1 + strlen(section) + 2 + 1);
    sprintf(res, "[%s].", section);

    return res;
}

config_t *config_init() {
	config_t *self;

	self = (config_t *)malloc(sizeof(config_t));
	if (!self) {
		errno = ENOMEM;
		return NULL;
	}
	
	self->sec = stringlist_init();
	self->map = stringmap_init();

	return self;
}

void config_free(config_t *self) {
    assert(self);

    stringmap_free(self->map);
    stringlist_free(self->sec);
    free(self);
    self = NULL;
}

void config_set(config_t *self, const char *section, const char *key, const char *value) {
    char *sec_lo; /* section lowercased */
    char *key_id; /* complete key in the map */

    assert(self);
    assert(section);
    assert(key);
    assert(value);

    /* check values */
    if (strlen(value) == 0 || strlen(value) >= STRINGMAP_VALUE_SIZE) {
        errno = EINVAL;
        return;
    }


    /* check section and add new section (if not exists) */
    sec_lo = string_toLower(section);
    if (strlen(sec_lo) > 0 && !stringlist_exists(self->sec, sec_lo)) {
        stringlist_add(self->sec, sec_lo);

    }
    free(sec_lo);

    /* add value to map */
    key_id = getKeyId(section, key);
    if (!errno) {
        stringmap_addReplace(self->map, key_id, value);
    }

    free(key_id);
}

char *config_get(config_t *self, const char *section, const char *key) {
    char *key_id; /* key in the map */
    char *res; /* result */

    assert(self);
    assert(section);
    assert(key);

    res = NULL;
    key_id = getKeyId(section, key);
    if (!errno) {
        res = stringmap_getValueByKey(self->map, key_id);
    }
    free(key_id);

    return res;
}

void config_remove(config_t *self, const char *section, const char *key) {
    char *key_id;

    assert(self);
    assert(section);
    assert(key);

    key_id = getKeyId(section, key);
    if (stringmap_exists(self->map, key_id)) stringmap_remove(self->map, key_id);
    free(key_id);
}

void config_load(config_t *self, const char *filename) {
    FILE *file;
	char line[STRINGMAP_KEY_SIZE + 1 + STRINGMAP_VALUE_SIZE + 2 + 1];
	int len; /* length of line */
	char *sep; /* position of the separator */
    char *tmp;
    char sec[STRINGMAP_KEY_SIZE + 1] = "";
    char *sec_lo;
    char *key;
    char *val;
    char *key_id;

    assert(self);
    assert(filename);

    /* check value */
    if (strlen(filename) == 0) {
        errno = EINVAL;
        return;
    }
    if (!file_exists(filename)) {
        errno = ENOENT;
        return;
    }

    /* open file */
    file = fopen(filename, "r");
    if (!file) {
        return;
    }

	len = sizeof(line) / sizeof(line[0]);
    key = NULL;
    val = NULL;
    while (fgets(line, len, file)) {
        /* check and trim line */
        tmp = string_trim(line);
        if (!tmp) continue;
        if (tmp[0] == '#' || tmp[0] == ';') {
            free(tmp);
            continue;
        }
        strcpy(line, tmp);
        free(tmp);

        /* check for section */
        if (line[0] == '[') {
            if (strlen(line) >= (STRINGMAP_KEY_SIZE - 2) || line[strlen(line) - 1] != ']') {
                errno = EINVAL;
                goto quit;
            }
            memcpy(sec, line + 1, strlen(line) - 2);
            sec[strlen(line) - 2] = '\0';

            /* lower case section */
            sec_lo = string_toLower(sec);
            if (strlen(sec_lo) > 0 && !stringlist_exists(self->sec, sec_lo)) {
                stringlist_add(self->sec, sec_lo);

            }
            free(sec_lo);

            continue;
        }

        /* split line to key and value */
        sep = strchr(line, '=');
        if (!sep) {
            sep = strchr(line, ':');
            if (!sep) {
                errno = EINVAL;
                goto quit;
            }
        }

		if ((sep - line) > STRINGMAP_KEY_SIZE) {
            errno = EINVAL;
            goto quit;
        }
		if ((strlen(line) - (sep - line) - 1) > STRINGMAP_VALUE_SIZE) {
            errno = EINVAL;
            goto quit;
        }
		sep[0] = '\0';                          /* make the separator th the key-end */
		key = string_trim(line);
        if (!key) {
            errno = EINVAL;
            goto quit;
        }
		++sep;                                  /* move one byte right */
		val = string_trim(sep);
		if (!val) goto free;

        /* check length of pair */
        if (strlen(key) > STRINGMAP_KEY_SIZE) {
            errno = EINVAL;
            goto quit;
        }
        if (strlen(val) > STRINGMAP_VALUE_SIZE) {
            errno = EINVAL;
            goto quit;
        }


        key_id = getKeyId(sec, key);
        stringmap_addReplace(self->map, key_id, val);
        free(key_id);
free:
        if (key) {
            free(key);
            key = NULL;
        }
        if (val) {
            free(val);
            val = NULL;
        }
    }

quit:
    fclose(file);
}

void config_save(config_t *self, const char *filename) {
    FILE *file; /* file-object */
    size_t i, j; /* foor-loop */
    size_t map_size; /* count of map-entries */
    size_t sec_size; /* count of section-entries */
    char *key;
    char *key_id;
    char *val;
    char *sec;
    char *sec_id;
    boolean_t sec_set;

    assert(self);
    assert(filename);

    sec_set = boolean_false;
    sec_size = stringlist_size(self->sec);
    map_size = stringmap_size(self->map);

    file = fopen(filename, "w");
    if (!file) goto quit;

    /* all entries without section */
    for (i = 0; i < map_size; ++i) {
        key_id = stringmap_getKeyByIndex(self->map, i);
        if (strstr(key_id, "[].") == key_id) {
            sec_set = boolean_true;
            val = stringmap_getValueByKey(self->map, key_id);
            fprintf(file, "%s=%s\n", key_id + 3, val);
            free(val);
        }
        free(key_id);
    }

    /* write all entries found in all sections */
    for (i = 0; i < sec_size; i++) {
        sec = stringlist_get(self->sec, i);
        sec_id = getSecId(sec);
        if (sec_set) {
            fprintf(file,"\n");
        }
        sec_set= boolean_false; /* reet section */
        for (j = 0; j < map_size; ++j) {
            key_id = stringmap_getKeyByIndex(self->map, j); /* [sec].key */
            if (strstr(key_id, sec_id) == key_id) {
                /* write section only if there is data in it */
                if (!sec_set) {
                    fprintf(file,"[%s]\n", sec);
                    sec_set = boolean_true;
                }
                key = strdup(strstr(key_id, "].") + 2);
                val = stringmap_getValueByKey(self->map, key_id);
                fprintf(file, "%s=%s\n", key, val);
                free(key);
                free(val);
            }
            free(key_id);
        }

        free(sec_id);
        free(sec);
    }


quit:
    fclose(file);
}
