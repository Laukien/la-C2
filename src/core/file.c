/*
 * =====================================================================================
 *
 *       Filename:  file.c
 *    Description:  I/O file operations
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

/* large files */
#if defined __LP64__ || defined __x86_64__
#define FILE_x64
#ifndef __USE_LARGEFILE64
#define __USE_LARGEFILE64
#endif
#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE
#endif
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif
#endif

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <la/file.h>
#include <la/directory.h>
#include <la/number.h>
#include <la/string.h>

boolean_t file_exists(const char *filename) {
/* 
	FILE *pFile = fopen(filename, "r");
	if ( pFile == NULL) return boolean_false;

	fclose(pFile);
	return boolean_true;
*/

	struct stat st;
	if (!stat(filename, &st) == 0) {
        errno = 0;
		return boolean_false;
	}
	return (st.st_mode & S_IFREG) ? boolean_true : boolean_false;
}

boolean_t file_remove(const char *filename) {
	int ret = unlink(filename);
	return ret ? boolean_false : boolean_true;
}

char *file_getName(const char *file) {
	int len; /* length of filename */
	int i; /* for loop */
	char *name; /* name of directory */

	for (i = strlen(file)-1; i >= -1; i--) {
		if (i == -1) break;                     /* not found */
		if (file[i] == DIRECTORY_SEPARATOR_CHAR) break;
	}

	len = strlen(file) - i;                    /* len + 1 */
	name = (char *) malloc(len);
	if(name == NULL) {
		return NULL;
	}

	memcpy(name, file + i + 1, len);            /* name + '\0' */

	return name;
}

char *file_getTemp(void) {
	time_t t; /* time structure */
	int pid; /* id of current process */
	char *pid_s; /* process-id as string */
	int rnd; /* random number */
	char *rnd_s; /* random number as string */
	char *dir; /* temporary director */
	size_t size; /* size of result string */

	time(&t);
	srand((unsigned int) t);

	pid = getpid();
	pid_s = number_integerToString(pid);
	rnd = rand();
	rnd_s = number_integerToString(rnd);

	dir = directory_getTemp();
	size = strlen(dir) + 1 + strlen(pid_s) + 1 + strlen(rnd_s);
	dir = (char *) realloc(dir, size + 1);
	strcat(dir, DIRECTORY_SEPARATOR_STRING);
	strcat(dir, pid_s);
	strcat(dir, "_");
	strcat(dir, rnd_s);

	free(rnd_s);
	free(pid_s);

	return dir;
}

size_t file_getSize(const char *filename) {
	size_t size; /* size of file */
#ifdef FILE_64
	struct stat64 st;
	if (stat64(filename, &st) != 0) return -1;
#else
	struct stat st;
	if (stat(filename, &st) != 0) return -1;
#endif
	size = st.st_size;

	return size;
}

char *file_getExtension(const char *filename) {
    char *sep; /*separator '.' */
    char *ext; /* extension - result */

    assert(filename);

    sep = strrchr((char *)filename, '.');
    if (!sep || sep == filename + strlen(filename) - 1) {
        return NULL;
    }
    
    if (sep[0] =='\0' || strlen(sep) <= 1) return NULL;
    ext = sep + 1;
    ext = string_trim(sep + 1);

    if (!string_isAlnum(ext)) {
        free(ext);
        return NULL;
    }

    return ext;
}

char *file_getNext(const char *filename) {
    char *current; /* current filename */
    char *clean; /* clean filename (no extension) */
    size_t len; /* length of filename */
    char *ext; /* extension */
    char *dot; /* extension-seperator */
    char *path; /* path-seperator */
    int i; /* for-loop */

    assert(filename);

    clean = string_trim(filename);

    /* check if filename exists */
    if (access(clean, F_OK) == -1) {
        return clean;
    }

    /* get & set extension */
    ext = NULL; /* default - file has no extension */
    len = 0; /* length of extension */
    dot = strrchr(clean, '.');
    if (dot > clean && dot && strlen(dot) > 1) {
        path = strrchr(clean, DIRECTORY_SEPARATOR_CHAR);
        if (dot - 1 > path) {
            ext = dot + 1;
            len = strlen(ext);
            dot[0] = '\0';
        }
    }

    for (i = 1; i < 1000; ++i) {
        /* filename (i).ext */
        if (ext) {
            current = (char *)malloc(strlen(clean) + 2 + number_getIntegerLength(i) + 2 + len + 1);
            sprintf(current, "%s (%d).%s", clean, i, ext);
        } else {
            current = (char *)malloc(strlen(clean) + 2 + number_getIntegerLength(i) + 1);
            sprintf(current, "%s (%d)", clean, i);
        }
        
        if (access(current, F_OK) == -1) {
            free(clean);
            return current;
        }
        free(current); 
    }

    free(clean);

    return NULL;
}
