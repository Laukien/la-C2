/*
 * gcc -I . -I /usr/local/include -o array_1 array_1.c la_array.c /usr/local/lib/libla.a
 */
#include <stdlib.h>
#include <stdio.h>
#include <la/stringarray.h>

int main() {
	char **args;
	stringarray_t *v;
	v = stringarray_init();

	/* create array from string */
	stringarray_loadFromString(v, "arg1 arg2", ' ');

	/* show size of array */
	printf ( "SIZE: %ld\n", (long int)stringarray_getSize(v) );

	/* show second argument */
	args = stringarray_getArray(v);

	/* show array */
	stringarray_showArray(args);

	/* free resources */
	stringarray_freeArray(args);
	stringarray_free(v);

	return 0;
}
