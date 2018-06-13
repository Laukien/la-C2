/*
 * =====================================================================================
 *
 *       Filename:  process.c
 *    Description:  handle operating system processes
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef __WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#endif
#include <la/datetime.h>
#include <la/directory.h>
#include <la/file.h>
#include <la/filecopy.h>
#include <la/process.h>
#include <la/string.h>
#include <la/stringarray.h>

struct la_process {
	process_id_t id;
	boolean_t wait;
	char *name;
	char *dir_temp;
	char *dir_work;
	process_status_t status;
	process_exit_t exit;
	char *file_input;
	char *file_output;
	char *file_error;
#ifdef __WIN32
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	HANDLE hi;
	HANDLE ho;
	HANDLE he;
#endif
};

static char *process_createName() {
	char *ts; /* timestamp */
	char *rs; /* random string */
	char *rnd; /*result buffer */

	ts = datetime_getTimestampAsString();
	rs = string_getRandom(10, boolean_true, boolean_true, boolean_true, boolean_false);

	rnd = (char *)malloc(strlen(ts) + 1 + strlen(rs) + 1);
	sprintf(rnd, "%s_%s", ts, rs);

	free(rs);
	free(ts);

	return rnd;
}

process_t *process_init() {
	process_t *self; /* object */
	size_t dir_len; /* length of directory name */
	size_t name_len; /* length of process name */

	self = (process_t *)malloc(sizeof(process_t));
	if (!self) {
		return NULL;
	}
	
	self->id = 0;
	self->wait = boolean_true;
	self->dir_work = NULL;
	self->dir_temp = directory_temp();
	self->name = process_createName();
	self->status = process_status_none;
	self->exit = -1;

	dir_len = strlen(self->dir_temp);
	name_len = strlen(self->name);

	self->file_input = (char *)malloc(dir_len + 1 + name_len + 1 + 2 + 1);
	sprintf(self->file_input, "%s%c%s.in", self->dir_temp, DIRECTORY_SEPARATOR_CHAR, self->name);
	unlink(self->file_input);

	self->file_output = (char *)malloc(dir_len + 1 + name_len + 1 + 3 + 1);
	sprintf(self->file_output, "%s%c%s.out", self->dir_temp, DIRECTORY_SEPARATOR_CHAR, self->name);
	unlink(self->file_output);

	self->file_error = (char *)malloc(dir_len + 1 + name_len + 1 + 3 + 1);
	sprintf(self->file_error, "%s%c%s.err", self->dir_temp, DIRECTORY_SEPARATOR_CHAR, self->name);
	unlink(self->file_error);

	return self;
}

void process_free(process_t *self) {
#ifdef __WIN32
#else
	int status;
#endif

	assert(self);

	/* wait if the process is running and the process should wait*/
	if (self->wait && self->status == process_status_running) {
#ifdef __WIN32
		WaitForSingleObject(self->pi.hProcess, INFINITE);
		CloseHandle(self->pi.hProcess);
		CloseHandle(self->pi.hThread);
		CloseHandle(self->he);
		CloseHandle(self->ho);
		CloseHandle(self->hi);
#else
		waitpid(self->id, &status, 0);
#endif
	}

#ifdef __WIN32
		CloseHandle(self->pi.hProcess);
		CloseHandle(self->pi.hThread);
		CloseHandle(self->he);
		CloseHandle(self->ho);
		CloseHandle(self->hi);
#endif

	unlink(self->file_input);
	unlink(self->file_output);
	unlink(self->file_error);

	free(self->name);
	free(self->dir_temp);
	free(self->file_input);
	free(self->file_output);
	free(self->file_error);

	free(self);
	self = NULL;
}

void process_setWait(process_t *self, boolean_t wait) {
	assert(self);

	self->wait = wait;
}

boolean_t process_isWait(process_t *self) {
	assert(self);

	return self->wait;
}

void process_wait(process_t *self) {
#ifdef __WIN32
#else
	int status;                             /* dummy */
#endif

	assert(self);

	if (self->status == process_status_running) {
#ifdef __WIN32
		WaitForSingleObject(self->pi.hProcess, INFINITE);
#else
		waitpid(self->id, &status, 0);
#endif
	}
}

void process_setWorkDirectory(process_t *self, const char *dir) {
	assert(self);

	if (!dir) {
		errno = EINVAL;
	}

	if (self->dir_work) {
		free(self->dir_work);
	}
	self->dir_work = strdup(dir);
}

void process_setTempDirectory(process_t *self, const char *dir) {
	assert(self);

	if (!dir) {
		errno = EINVAL;
	}

	if (self->dir_temp) {
		free(self->dir_temp);
	}
	self->dir_temp = strdup(dir);
}

void process_setInputFromString(process_t *self, const char *str) {
	assert(self);

	if (!str) {
		errno = EINVAL;
	}

	string_saveToFile(self->file_input, str);
}

void process_setInputFromFile(process_t *self, const char *file) {
	assert(self);

	if (!file) {
		errno = EINVAL;
	}

	if (!file_exists(file)) {
		errno = ENOENT;
		return;
	}

	file_copy(file, self->file_input, NULL);
}

void process_execute(process_t *self, const char *command) {
#ifdef __WIN32
	SECURITY_ATTRIBUTES sa;
	DWORD flags;
	boolean_t ok;
	char *cmdstr;
#else
	int status;
	char *cmdarg;
	char *cmdstr;
	const char *sep;
	int fd_o; /* file desciptior stdout */
	int fd_e; /* file descriptor stderr */
	stringarray_t *cmdary;
	char **args;
	int rc; /* return code */
#endif

	assert(self);

	if (!command) {
		errno = EINVAL;
	}

#ifdef __WIN32
	/* security settings */
	ZeroMemory( &sa, sizeof(sa) );
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = boolean_true;

	/* stdin */
	self->hi = GetStdHandle(STD_INPUT_HANDLE);
	if (file_exists(self->file_input)) {
		self->hi = CreateFile(self->file_input,
			GENERIC_READ,
			FILE_SHARE_WRITE | FILE_SHARE_READ,
			&sa,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (self->hi == INVALID_HANDLE_VALUE) {
			self->hi = GetStdHandle(STD_INPUT_HANDLE);
		}
	}

	/* stdout */
	self->ho = CreateFile(self->file_output,
		GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		&sa,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (self->ho == INVALID_HANDLE_VALUE) {
		self->ho = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	/* stderr */
	self->he = CreateFile(self->file_error,
		GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		&sa,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (self->he == INVALID_HANDLE_VALUE) {
		self->he = GetStdHandle(STD_ERROR_HANDLE);
	}

	/* set process information*/
	flags = CREATE_NO_WINDOW | CREATE_BREAKAWAY_FROM_JOB;

	ZeroMemory(&self->pi, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&self->si, sizeof(STARTUPINFO));
	self->si.cb = sizeof(STARTUPINFO); 

	self->si.dwFlags |= STARTF_USESTDHANDLES;
	self->si.hStdInput = self->hi;
	self->si.hStdOutput = self->ho;
	self->si.hStdError = self->he;

	/* call process */
	cmdstr = strdup(command);
	ok = CreateProcess(
		NULL,
		cmdstr,
		NULL,
		NULL,
		boolean_true,
		flags,
		NULL,
		self->dir_work,
		&self->si,
		&self->pi
	);
	free(cmdstr);
	if (ok) {
		self->status = process_status_running;
		self->id = self->pi.dwProcessId;
		if (self->wait) {
			WaitForSingleObject(self->pi.hProcess, INFINITE);
			GetExitCodeProcess(self->pi.hProcess, &self->exit);
			self->status = process_status_finished;
			CloseHandle(self->pi.hProcess);
			CloseHandle(self->pi.hThread);
			CloseHandle(self->he);
			CloseHandle(self->ho);
			CloseHandle(self->hi);
		}
	} else {
		self->exit = ok;
		self->status = process_status_error;
	}
#else
	self->id = fork();

    if (self->id < 0) {                         /* error */
		errno = ECHILD;
		return;
	}

    if (self->id > 0) {                         /* parent */
		self->status = process_status_running;
		if (self->wait) {
			waitpid(self->id, &status, 0);
			self->exit = status;
			self->status = process_status_finished;
		} else {                                /* wait for child */
			int dummy;
			waitpid(self->id, &dummy, WNOHANG);
		}
	} else {                                    /* child */
		if (file_exists(self->file_input)) {
			int fd_i = open(self->file_input, O_RDONLY);
			dup2(fd_i, 0);
			close(fd_i);
		}

		fd_o = open(self->file_output, O_WRONLY | O_CREAT | S_IRUSR | S_IWUSR, 0666);
		dup2(fd_o, 1);
		close(fd_o);

		fd_e = open(self->file_error, O_WRONLY | O_CREAT | S_IRUSR | S_IWUSR, 0666);
		dup2(fd_e, 2);
		close(fd_e);

		/* change working directory */
		if (self->dir_work) {
			chdir(self->dir_work);
		}

		/* format command */
		sep = strchr(command, ' ');
		if (sep) {
			int seplen = (sep - command) + 1;
			cmdstr = (char *)malloc(seplen - 1 + 1);
			memcpy(cmdstr, command, seplen - 1);
			cmdstr[seplen - 1] = '\0';

			cmdarg = strdup(command);
		} else {
			cmdstr = strdup(command);

			cmdarg = strdup(command);
		}
	
		cmdary = stringarray_init();
		stringarray_loadFromString(cmdary, cmdarg, ' ');
		args = stringarray_getArray(cmdary);
		stringarray_free(cmdary);

		rc = execv(cmdstr, args);

		/* in the case of an error */
		stringarray_freeArray(args);
		free(cmdarg);
		free(cmdstr);

		close(0);
		close(1);
		close(2);

		exit(rc);
	}
#endif
}

void process_kill(process_t *self) {
	unsigned int count;

	assert(self);
	
	if (process_getStatus(self) != process_status_running) return;

	count = 0;

#ifdef __WIN32
	PostThreadMessage(self->pi.dwThreadId, WM_CLOSE, 1, 0); /* terminate process */

	do {
		Sleep(100);
	} while(process_getStatus(self) == process_status_running && ++count < 100);

	if (process_getStatus(self) == process_status_running) {
		TerminateProcess(self->pi.hProcess, 0); /* kill process */
	}
#else
	kill(self->id, SIGTERM);                    /* terminate process */

	do {
		usleep(100);
	} while(process_getStatus(self) == process_status_running && ++count < 100);

	if (process_getStatus(self) == process_status_running) {
		kill(self->id, SIGKILL);                /* kill process */
	}
#endif

	self->status = process_status_terminated;
}

process_id_t process_getId(process_t *self) {
	assert(self);

	return self->id;
}

process_exit_t process_getExit(process_t *self) {
	assert(self);

	return self->exit;
}

process_status_t process_getStatus(process_t *self) {
	assert(self);

	if (self->status == process_status_running) {
#ifdef __WIN32
		DWORD rc = WaitForSingleObject(self->pi.hProcess, 1);
		if (rc == WAIT_OBJECT_0) {
			GetExitCodeProcess(self->pi.hProcess, &self->exit);
			self->status = process_status_finished;
		} else if (rc == WAIT_FAILED) {
			self->status = process_status_error;
		}
#else
		int stat = 0;                           /* set '0' to prevent a valgrind-error */
		int rc = waitpid(self->id, &stat, WNOHANG);
		if (rc) {
			if (WIFEXITED(stat)) {
				if (WEXITSTATUS(stat) == 255) {
					self->status = process_status_error;
				} else {
					self->exit = WEXITSTATUS(stat);
					self->status = process_status_finished;
				}
			} else if (WIFSTOPPED(stat)) {
				self->status = process_status_terminated;
			}
		}
#endif
		
	}
	return self->status;
}

char *process_getInputAsString(process_t *self) {
	assert(self);

	if (file_exists(self->file_input)) {
		return string_loadFromFile(self->file_input);
	} else {
		return NULL;
	}
}

char *process_getInputAsFilename(process_t *self) {
	assert(self);

	return strdup(self->file_input);
}

char *process_getOutputAsString(process_t *self) {
	assert(self);

	if (file_exists(self->file_output)) {
		return string_loadFromFile(self->file_output);
	} else {
		return NULL;
	}
}

char *process_getOutputAsFilename(process_t *self) {
	assert(self);

	return strdup(self->file_output);
}

char *process_getErrorAsString(process_t *self) {
	assert(self);

	if (file_exists(self->file_error)) {
		return string_loadFromFile(self->file_error);
	} else {
		errno = ENOENT;
		return NULL;
	}
}

char *process_getErrorAsFilename(process_t *self) {
	assert(self);

	return strdup(self->file_error);
}
