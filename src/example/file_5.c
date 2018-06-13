#include <stdio.h>
#include <stdlib.h>
#include <la/file.h>

void touch(const char *filename) {
    FILE *file;
    
    file = fopen(filename, "w");
    fclose(file);
}

int main(void) {
    char *filename = __FILE__;
    char *next;
    int i;

    printf("NEXT0: %s\n", filename);

    for (i = 1; i < 6; ++i) {
        next = file_getNext(filename);
        printf("NEXT%d: %s\n", i, next);
        touch(next);
        free(next);
    }

    puts("REMOVE: rm src/example/file_5\\ \\(*");

    return EXIT_SUCCESS;
}
