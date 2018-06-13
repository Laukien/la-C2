/*
 * =====================================================================================
 *
 *       Filename:  file.c
 *    Description:  copies files
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

/* large files */
#if defined __LP64__ || defined __x86_64__
#define FILE_x64
#ifndef __USE_LARGEFILE64
#define __USE_LARGEFILE64
#endif
#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE
#endif
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif
#endif

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <la/boolean.h>
#include <la/filecopy.h>

#define FILE_BUFFER_SIZE 1024


boolean_t file_copy(const char *from, const char *to, FILE_PROCESS proc) {
	int fd_r; /* file handle */
	int fd_w; /* file handle */
	size_t size; /* size of file */
	size_t done; /* counter of copied bytes */
	char *buf[FILE_BUFFER_SIZE]; /* copy-buffer */
	int rc; /* read code - how many bytes are copied */

	assert(from);
	assert(to);

	/* open file to read */
#ifdef __WIN32
	fd_r = open(from, O_RDONLY|O_BINARY);
#else
	fd_r = open(from, O_RDONLY);
#endif
	if (fd_r == -1) {
		return boolean_false;
	}

	/* open file to write */
#ifdef __WIN32
	fd_w = open(to, O_WRONLY|O_CREAT|O_BINARY, 0644);
#else
	fd_w = open(to, O_WRONLY|O_CREAT, 0644);
#endif
	if (fd_w == -1) {
		close(fd_r);
		return boolean_false;
	}

	/* get size */
	size = lseek(fd_r, 0L, SEEK_END);
	lseek(fd_r, 0L, SEEK_SET);
	done = 0;

	/* call process */
	if (proc) {
		proc(from, to, size, done);
	}

	/* copy */
	while ((rc = read(fd_r, buf, FILE_BUFFER_SIZE))) {
		if (rc < 0) {
			close(fd_w);
			close(fd_r);
			return boolean_false;
		}
		write(fd_w, buf, rc);

		done += rc;

		/* call process */
		if (proc) {
			proc(from, to, size, size);
		}

	}

	/* close file descriptors */
	close(fd_w);
	close(fd_r);

	return boolean_true;
}

void FILE_PROCESS_SIMPLE(const char *from, const char *to, size_t size, size_t done) {
	if (done == 0) {
		printf ( "%s -> %s (%u)\n", from, to, (unsigned int)size );
	}
}

void FILE_PROCESS_DOT(const char *from, const char *to, size_t size, size_t done) {
	if (done == 0) {
		printf ( "%s -> %s ", from, to );
	}

	printf ( "." );
	fflush(stdout);

	if (done == size) {
		printf ( "\n" );
	}
}

void FILE_PROCESS_PERCENT(const char *from, const char *to, size_t size, size_t done) {
	printf ( "\r%s -> %s: %3d%%", from, to, (int)(done * 100 / size) );
	fflush(stdout);
	if (done == size) {
		printf ( "\n" );
	}
}
