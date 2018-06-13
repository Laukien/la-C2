/*
 * =====================================================================================
 *
 *       Filename:  atomic.c
 *    Description:  synchronize code
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef __WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif
#include <la/atomic.h>

struct la_atomic {
#ifdef __WIN32
	HANDLE mutex;
#else
	pthread_mutex_t mutex;
#endif
};

atomic_t *atomic_init() {
	atomic_t *self; /* object */

	self = (atomic_t *)malloc(sizeof(atomic_t));
	if (!self) {
		return NULL;
	}

#ifdef __WIN32
	self->mutex = CreateMutex(NULL, FALSE, NULL);
#else
	pthread_mutex_init(&(self->mutex), NULL);
#endif

	return self;
}

void atomic_free(atomic_t *self) {
	assert(self);

	atomic_unlock(self);
#ifdef __WIN32
	CloseHandle(self->mutex);
#else
	pthread_mutex_destroy(&(self->mutex));
#endif
	free(self);
	self = NULL;
}

void atomic_lock(atomic_t *self) {
	assert(self);

#ifdef __WIN32
	WaitForSingleObject(self->mutex, INFINITE);
#else
	pthread_mutex_lock(&(self->mutex));
#endif
}

void atomic_unlock(atomic_t *self) {
	assert(self);

#ifdef __WIN32
	ReleaseMutex(self->mutex);
#else
	pthread_mutex_unlock(&(self->mutex));
#endif
}
