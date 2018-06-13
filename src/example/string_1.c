#include <stdio.h>
#include <stdlib.h>
#include <la/string.h>

int main(void) {
	char *hex;
    char *txt;

	hex = string_toHex("abcABCxyzXYZ");
	printf ( "HEX: %s\n", hex );

	txt = string_fromHex(hex);
	printf ( "TXT: %s\n", txt );

	free(hex);
	free(txt);

	return EXIT_SUCCESS;
}
