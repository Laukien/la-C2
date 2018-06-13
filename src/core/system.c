/*
 * =====================================================================================
 *
 *       Filename:  system.c
 *    Description:  system specific functions
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#ifdef __WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <la/system.h>

boolean_t system_isWindows() {
#ifdef SYSTEM_OS_TYPE_WINDOWS
	return boolean_true;
#else
	return boolean_false;
#endif
}

boolean_t system_isDOS() {
#ifdef SYSTEM_OS_TYPE_DOS
	return boolean_true;
#else
	return boolean_false;
#endif
}

boolean_t system_isUnix() {
#ifdef SYSTEM_OS_TYPE_UNIX
	return boolean_true;
#else
	return boolean_false;
#endif
}

boolean_t system_isLinux() {
#ifdef SYSTEM_OS_TYPE_LINUX
	return boolean_true;
#else
	return boolean_false;
#endif
}

boolean_t system_isIRIX() {
#ifdef SYSTEM_OS_TYPE_IRIX
	return boolean_true;
#else
	return boolean_false;
#endif
}

boolean_t system_isHPUX() {
#ifdef SYSTEM_OS_TYPE_HPUX
	return boolean_true;
#else
	return boolean_false;
#endif
}

boolean_t system_isOSX() {
#ifdef SYSTEM_OS_TYPE_OSX
	return boolean_true;
#else
	return boolean_false;
#endif
}

boolean_t system_isOS2() {
#ifdef SYSTEM_OS_TYPE_OS2
	return boolean_true;
#else
	return boolean_false;
#endif
}

boolean_t system_isMicrosoft() {
#ifdef SYSTEM_OS_VENDOR_MICROSOFT
	return boolean_true;
#else
	return boolean_false;
#endif
}

boolean_t system_isSGI() {
#ifdef SYSTEM_OS_VENDOR_SGI
	return boolean_true;
#else
	return boolean_false;
#endif
}

boolean_t system_isHP() {
#ifdef SYSTEM_OS_VENDOR_HP
	return boolean_true;
#else
	return boolean_false;
#endif
}

boolean_t system_isIBM() {
#ifdef SYSTEM_OS_VENDOR_IBM
	return boolean_true;
#else
	return boolean_false;
#endif
}

boolean_t system_isApple() {
#ifdef SYSTEM_OS_VENDOR_APPLE
	return boolean_true;
#else
	return boolean_false;
#endif
}

boolean_t system_is32Bit() {
#ifdef SYSTEM_ARCH_32
	return boolean_true;
#else
	return boolean_false;
#endif
}

boolean_t system_is64Bit() {
#ifdef SYSTEM_ARCH_64
	return boolean_true;
#else
	return boolean_false;
#endif
}

int system_getCPUArch() {
#ifdef SYSTEM_OS_TYPE_WINDOWS
	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo);
	switch (siSysInfo.wProcessorArchitecture) {
		case PROCESSOR_ARCHITECTURE_AMD64:
		case PROCESSOR_ARCHITECTURE_IA64:
			return 64;
		case PROCESSOR_ARCHITECTURE_INTEL:
			return 32;
		default:
			return -1;
	}
#else
	FILE *fp = NULL;
	char line[2048];
	char *flags = NULL;

	fp = fopen("/proc/cpuinfo", "r");
	if (!fp)
		return -1;


	while (fgets(line, 2048, fp)) {
		flags = strstr(line, "flags");
		if (flags != NULL)
			break;
	}

	fclose(fp);

	if (strstr(flags, " lm ")) {
		return 64;
	} else if (strstr(flags, " tm ")) {
		return 32;
	} else if (strstr(flags, " rm ")) {
		return 16;
	} else return -1;
#endif
}

void system_sleep(unsigned long milliseconds) {
#ifdef __WIN32
	Sleep(milliseconds);
#else
	usleep(milliseconds * 1000);
#endif
}
