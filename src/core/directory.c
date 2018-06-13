/*
 * =====================================================================================
 *
 *       Filename:  directory.c
 *    Description:  I/O directory operations
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#ifdef __WIN32
#include <windows.h>
#endif
#include <la/directory.h>


boolean_t directory_create(const char *directoryname) {
	char *dir; /* copy of directory-name */
	size_t i; /* for-loop */

	if (directory_exists(directoryname)) return boolean_true;

	dir = strdup(directoryname);

	/* make directory tokens */
	for (i = 1; i < strlen(directoryname); ++i) {
		if (dir[i] == DIRECTORY_SEPARATOR_CHAR) dir[i] = '\0';
	}

	/* rebuild directory from the root */
	for (i = 1; i < strlen(directoryname); ++i) {
		if (dir[i] != '\0') continue;
		if (!directory_exists(directoryname)) {
#ifdef __WIN32
			mkdir(dir);
#else
			mkdir(dir, 0755);
#endif

		}
		dir[i] = DIRECTORY_SEPARATOR_CHAR;
	}

#ifdef __WIN32
			mkdir(directoryname);
#else
			mkdir(directoryname, 0755);
#endif

	free(dir);

	return (directory_exists(directoryname));
}

boolean_t directory_exists(const char *name) {
#ifdef __WIN32
	DWORD dwAttrib = GetFileAttributes(name);
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
#else
	struct stat st;
	if (!stat(name, &st) == 0) {
        errno = 0;
		return boolean_false;
	}
	return (st.st_mode & S_IFDIR);
#endif
}

char *directory_getName(const char *filename) {
	int len; /* length of filename */
	int i; /*for loop */
	char *name; /* directory name */

	for (i = strlen(filename)-1; i >= -1; i--) {
		if (i == -1) break;                     /* not found */
		if (filename[i] == DIRECTORY_SEPARATOR_CHAR) break;
	}

	len = i;
	name = (char *) malloc(len + 1);
	if(name == NULL) {
		return NULL;
	}

	memcpy(name, filename, len);
	name[len] = '\0';

	return name;
}

char *directory_getTemp() {
	char *dir; /* directory */

#ifdef __WIN32
	char *tmp = getenv("TEMP");
	if (tmp != NULL) {
		dir = (char *) malloc(strlen(tmp) + 1);
		strcpy(dir, tmp);
	} else {
		char *win = getenv("WINDIR");
		if (win == NULL) {
			return NULL;
		}
		dir = (char *) malloc(strlen(win) + 6);
		strcpy(dir, win);
		strcat(dir, DIRECTORY_SEPARATOR_STRING);
		strcat(dir, "Temp");
	}
#else
	dir = (char *) malloc(5);
	strcpy(dir, "/tmp");
#endif

	return dir;
}

