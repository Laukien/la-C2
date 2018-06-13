/*
 * =====================================================================================
 *
 *       Filename:  process.h
 *    Description:  handle operating system processes
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#ifndef LA_PROCESS_H
#define LA_PROCESS_H

#ifdef __WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <la/boolean.h>

typedef struct la_process process_t;

typedef enum la_process_status {
	process_status_error = -1,
	process_status_none = 0,
	process_status_running = 1,
	process_status_finished = 2,
	process_status_terminated = 3
} process_status_t;

#ifdef __WIN32
typedef DWORD process_id_t;
typedef DWORD process_exit_t;
#else
typedef pid_t process_id_t;
typedef int process_exit_t;
#endif

process_t *process_init();
void process_free(process_t *self);
void process_setWait(process_t *self, boolean_t wait);
boolean_t process_isWait(process_t *self);
void process_wait(process_t *self);
void process_setTempDirectory(process_t *self, const char *dir);
void process_setWorkDirectory(process_t *self, const char *dir);
void process_setInputFromString(process_t *self, const char *str);
void process_setInputFromFile(process_t *self, const char *file);
void process_execute(process_t *self, const char *command);
void process_kill(process_t *self);
process_id_t process_getId(process_t *self);
process_exit_t process_getExit(process_t *self);
process_status_t process_getStatus(process_t *self);
char *process_getInputAsString(process_t *self);
char *process_getInputAsFilename(process_t *self);
char *process_getOutputAsString(process_t *self);
char *process_getOutputAsFilename(process_t *self);
char *process_getErrorAsString(process_t *self);
char *process_getErrorAsFilename(process_t *self);

#endif
