#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <la/string.h>

int main(int argc, char *argv[]) {
	char *filename;
	char *str; /* content of loaded file */

	if (argc == 2) {
		filename = (char *) malloc(strlen(argv[1] + 1));
		strcpy(filename, argv[1]);
	} else {
		filename = (char *) malloc(strlen("README") + 1);
		strcpy(filename, "README");
	}

	printf ( "Filename: %s\n", filename );
	str = string_loadFromFile(filename);

	if (str != NULL) {
		printf ( "String:\n%s\n", str );
		free(str);
	} else
		fprintf(stderr, "Error: Unable to read file.\n");

	free(filename);

	return (EXIT_SUCCESS);
}
