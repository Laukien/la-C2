/*
 * =====================================================================================
 *
 *       Filename:  tread.c
 *    Description:  common thread-handling
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <pthread.h>
#endif
#include <la/thread.h>

struct la_thread {
#ifdef __WIN32
	HANDLE thread;
	volatile DWORD id;
#else
	pthread_t *thread;
	volatile long id;
#endif
	THREAD_CALLBACK function;
	void *argument;
	int exit;
	volatile la_thread_status_t status;
};

thread_t *thread_init() {
	thread_t *self = (thread_t *)malloc(sizeof(thread_t));

	self->thread = NULL;
	self->function = NULL;
	self->argument = NULL;
	self->exit = -1;
	self->status = THREAD_STATUS_NONE;
	self->id = -1;

	return self;
}

void thread_free(thread_t *self) {
#ifdef __WIN32
#else
	int count;
#endif

	assert(self);

	if (self->thread) {
#ifdef __WIN32
		if (self->status == THREAD_STATUS_RUNNING) {
			thread_cancel(self);
			WaitForSingleObject(self->thread, 1000);
			if (self->status == THREAD_STATUS_RUNNING) {
				thread_kill(self);
			}
		}

		WaitForSingleObject(self->thread, INFINITE);
		CloseHandle(self->thread);
#else
		count = 0;
		if (self->status == THREAD_STATUS_RUNNING) {
			thread_cancel(self);
			while (self->status == THREAD_STATUS_RUNNING && count++ < 100) {
				usleep(1000);
			}
			if (self->status == THREAD_STATUS_RUNNING) {
				thread_kill(self);
			}
		}

		pthread_join(*(self->thread), NULL);
#endif
	}
	
	free(self->thread);
	self->thread = NULL;
	free(self);
	self = NULL;
}

void thread_setFunction(thread_t *self, THREAD_CALLBACK function, void *argument) {
	assert(self);
	assert(!self->function);
	self->function = function;
	self->argument = argument;
}

void _thread_clean(void *arg) {
	thread_t *self; /* object */

	assert(arg);

	self = (thread_t *)arg;

	self->status = THREAD_STATUS_EXIT;
}

#ifdef __WIN32
DWORD WINAPI _thread_run(LPVOID arg) {
#else
void *_thread_run(void *arg) {
#endif
	thread_t *self; /* object */
	THREAD_CALLBACK f; /* callback function */

	assert(arg);
	self = (thread_t *)arg;

#ifndef __WIN32
	self->id = pthread_self();
#endif

	f = self->function;
	self->status = THREAD_STATUS_RUNNING;
	self->exit = -1;

	self->exit = f(self->argument);

	if (self->status == THREAD_STATUS_RUNNING) {
		self->status = THREAD_STATUS_EXIT;
	}

	return 0;
}

void thread_run(thread_t *self) {
#ifdef __WIN32
	DWORD id;
#endif

	assert(self);
	assert(!self->thread);
	assert(self->status == THREAD_STATUS_NONE);

#ifdef __WIN32
	self->thread = CreateThread(NULL, 0, _thread_run, self, 0, &id);
	self->id = id;
	if (self->thread == NULL) {
		self->status = THREAD_STATUS_ERROR;
	} else {
		self->status = THREAD_STATUS_RUNNING;
	}
#else
	self->thread = (pthread_t *)malloc(sizeof(pthread_t));

	if (pthread_create(self->thread, NULL, _thread_run, self)) {
		self->status = THREAD_STATUS_ERROR;
	} else {
		self->status = THREAD_STATUS_RUNNING;
	}
#endif
}

void thread_cancel(thread_t *self) {
	assert(self);
	assert(self->thread);

	if (self->status == THREAD_STATUS_RUNNING) {
#ifdef __WIN32
		PostThreadMessage(self->id, WM_QUIT, (WPARAM) NULL, (LPARAM) NULL);
#else
		pthread_cancel(*(self->thread));
#endif
		self->status = THREAD_STATUS_CANCELED;
	}
}

void thread_kill(thread_t *self) {
	assert(self);
	assert(self->thread);

	if (self->status == THREAD_STATUS_RUNNING) {
#ifdef __WIN32
		TerminateThread(self->thread, 0);
#else
		int old_state;
		int old_type;
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old_state);
		pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old_type);

		pthread_cancel(*(self->thread));

		pthread_setcanceltype(old_type, NULL);
		pthread_setcancelstate(old_state, NULL);
#endif
		self->status = THREAD_STATUS_KILLED;
	}
}

void thread_wait(thread_t *self) {
	assert(self);
	assert(self->thread);

#ifdef __WIN32
	WaitForSingleObject(self->thread, INFINITE);
#else
	pthread_join(*(self->thread), NULL);
#endif
}

int thread_getStatus(thread_t *self) {
	assert(self);
	assert(self->thread);

	return self->status;
}

boolean_t thread_isRunning(thread_t *self) {
	assert(self);
	assert(self->thread);

	return self->status == THREAD_STATUS_RUNNING;
}

int thread_getExit(thread_t *self) {
	assert(self);
	assert(self->thread);

	if (self->status == THREAD_STATUS_RUNNING) {
#ifdef __WIN32
		WaitForSingleObject(self->thread, INFINITE);
#else
		pthread_join(*(self->thread), NULL);
#endif
	}

	return self->exit;
}
