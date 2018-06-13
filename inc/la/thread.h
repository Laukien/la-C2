/*
 * =====================================================================================
 *
 *       Filename:  tread.h
 *    Description:  common thread-handling
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#ifndef LA_THREAD_H
#define LA_THREAD_H

#include <la/boolean.h>

typedef enum {
	THREAD_STATUS_ERROR = -1,
	THREAD_STATUS_NONE = 0,
	THREAD_STATUS_RUNNING = 1,
	THREAD_STATUS_CANCELED = 2,
	THREAD_STATUS_KILLED = 3,
	THREAD_STATUS_EXIT = 4
} la_thread_status_t;

typedef struct la_thread thread_t;
typedef int (*THREAD_CALLBACK)(void *object);

thread_t *thread_init();
void thread_free(thread_t *self);
void thread_setFunction(thread_t *self, THREAD_CALLBACK function, void *argument);
void thread_run(thread_t *self);
void thread_cancel(thread_t *self);
void thread_kill(thread_t *self);
void thread_wait(thread_t *self);
int thread_getStatus(thread_t *self);
boolean_t thread_isRunning(thread_t *self);
int thread_getExit(thread_t *self);

#endif
