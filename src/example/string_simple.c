#include <stdio.h>
#include <la/str.h>

int main(void) {
    char *plain = "This ist a test!";
    char *lower = strlower(plain);
    char *upper = strupper(plain);
    char *camel = strcamel(plain);

    printf("PLAIN: %s\n", plain);
    printf("LOWER: %s\n", lower);
    printf("UPPER: %s\n", upper);
    printf("CAMEL: %s\n", camel);

    return 0;
}
