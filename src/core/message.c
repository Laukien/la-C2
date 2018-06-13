/*
 * =====================================================================================
 *
 *       Filename:  message.c
 *    Description:  central messaging functions
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <la/message.h>

void message_error(const char *text, ...) {
	char buf[MESSAGE_SIZE];
	va_list args;

	va_start(args, text);
	vsnprintf(buf, MESSAGE_SIZE, text, args);

	fprintf(stderr, "ERROR:\t\t%s\n", buf);

	va_end(args);
}

void message_debug(const char *text, ...) {
	char *env; /*pointer to an environment entry */
	char buf[MESSAGE_SIZE]; /* buffer of the message */
	va_list args;

	env = getenv("DEBUG");
	if (env != NULL && strlen(env) > 0) {
		char c = tolower(env[0]);
		if (c == 'y' || c == 't' || c == '1') {
			va_start(args, text);
			vsnprintf(buf, MESSAGE_SIZE, text, args);

			printf("DEBUG:\t\t%s\n", buf);

			va_end(args);
		}
	}
}

void message_info(const char *text, ...) {
	char buf[MESSAGE_SIZE];
	va_list args;

	va_start(args, text);
	vsnprintf(buf, MESSAGE_SIZE, text, args);

	printf("INFO:\t\t%s\n", buf);

	va_end(args);
}

void message_warn(const char *text, ...) {
	char buf[MESSAGE_SIZE];
	va_list args;

	va_start(args, text);
	vsnprintf(buf, MESSAGE_SIZE, text, args);

	printf("WARNING:\t%s\n", buf);

	va_end(args);
}
