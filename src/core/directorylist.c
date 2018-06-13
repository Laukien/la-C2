/*
 * =====================================================================================
 *
 *       Filename:  directorylist.c
 *    Description:  lists directory structure
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <la/directory.h>
#include <la/directorylist.h>

void _directory_list(const char *directoryname, boolean_t recursive, stringlist_t *list) {
	DIR *dp;	
	struct dirent *ep;
	struct stat st;
	char *name;
	size_t len; /* length of directory */

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
			if (st.st_mode & S_IFDIR) {
				stringlist_add(list, name);
				if (recursive) {
					_directory_list(name, boolean_true, list);
				}
			}

			free(name);
		}
		closedir(dp);
	}
}

stringlist_t *directory_list(const char *directoryname, boolean_t recursive) {
	stringlist_t *list; /* lis of directories */

	list = stringlist_init();

	_directory_list(directoryname, recursive, list);

	return list;
}

