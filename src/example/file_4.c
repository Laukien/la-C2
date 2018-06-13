#include <stdio.h>
#include <stdlib.h>
#include <la/file.h>

int main(void) {
    char *filename = __FILE__;
    char *name = file_getName(filename);
    char *ext = file_getExtension(filename);

    printf("FILENAME: %s\n", filename);
    printf("NAME: %s\n", name);
    printf("EXTENSION: %s\n", ext);
    
    free(ext);
    free(name);

    return EXIT_SUCCESS;
}
