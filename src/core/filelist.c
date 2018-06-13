/*
 * =====================================================================================
 *
 *       Filename:  file.c
 *    Description:  lists files
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <la/directory.h>
#include <la/filelist.h>


void _file_list(const char *directoryname, boolean_t recursive, stringlist_t *list) {
	DIR *dp;	
	struct dirent *ep;
	struct stat st;
	char *name;
	size_t len; /* size of directory-name */

	len = strlen(directoryname);
	dp = opendir(directoryname);
	if (dp != NULL) {
		while ((ep = readdir(dp)) != NULL) {
			/* check DOT-path */
			if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0) continue;

			/* build filename */
			name = (char *) malloc(len + 1 + strlen(ep->d_name) + 1);
			strcpy(name, directoryname);
			strcat(name, DIRECTORY_SEPARATOR_STRING);
			strcat(name, ep->d_name);
			stat(name, &st);

			/* call its self */
			if (recursive && (st.st_mode & S_IFDIR)) {
				_file_list(name, boolean_true, list);
				free(name);
				continue;
			}

			if (st.st_mode & S_IFREG) {
				stringlist_add(list, name);
			}

			free(name);
		}
		closedir(dp);
	}
}

stringlist_t *file_list(const char *directoryname, boolean_t recursive) {
	stringlist_t *list; /* list-object */

	list = stringlist_init();

	_file_list(directoryname, recursive, list);

	return list;
}

