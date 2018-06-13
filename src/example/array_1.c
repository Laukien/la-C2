#include <stdlib.h>
#include <stdio.h>
#include <la/stringarray.h>

int main() {
	char *arg;
	stringarray_t *v;
	v = stringarray_init();

	/* create array from string */
	stringarray_loadFromString(v, "arg1 arg3", ' ');

	/* show size of array */
	printf ( "SIZE: %ld\n", (long int)stringarray_getSize(v) );

	/* show second argument */
	arg = stringarray_getString(v, 1);
	printf("ARG 2: %s\n", arg);
	free(arg);

	/* change second argument */
	stringarray_setString(v, 1, "arg2");

	/* show array */
	stringarray_show(v);

	/* free resources */
	stringarray_free(v);

	return 0;
}
