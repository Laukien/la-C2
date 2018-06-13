/*
 * =====================================================================================
 *
 *       Filename:  system.h
 *    Description:  system specific functions
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#ifndef LA_SYSTEM_H
#define LA_SYSTEM_H

#include <la/boolean.h>

/* OS-Type & OS-Vendor*/
#if defined WIN32 || defined _WIN32 || defined _WIN32_ || defined __WIN32 || defined __WIN32__ || defined _MSC_VER || defined __NT__ || defined _Windows
	#define SYSTEM_OS_VENDOR_MICROSOFT
	#define SYSTEM_OS_TYPE_WINDOWS
#elif defined MSDOS || defined _MSDOS || defined _MSDOS_ || defined __MSDOS || defined __MSDOS__ || defined __BORLANDC__ || defined __TURBOC__
	#define SYSTEM_OS_VENDOR_MICROSOFT
	#define SYSTEM_OS_TYPE_DOS
#elif defined __UNIX || defined __UNIX__
	#define SYSTEM_OS_TYPE_UNIX
#elif defined __linux || defined __linux__
	#define SYSTEM_OS_TYPE_UNIX
	#define SYSTEM_OS_TYPE_LINUX
#elif defined _SGI_SOURCE || defined _MIPS_ISA
	#define SYSTEM_OS_VENDOR_SGI
	#define SYSTEM_OS_TYPE_UNIX
	#define SYSTEM_OS_TYPE_IRIX
#elif defined__hpux 
	#define SYSTEM_OS_VENDOR_HP
	#define SYSTEM_OS_TYPE_UNIX
	#define SYSTEM_OS_TYPE_HPUX
#elif defined __APPLE__ || defined __MACH__
	#define SYSTEM_OS_VENDOR_APPLE
	#define SYSTEM_OS_TYPE_UNIX
	#define SYSTEM_OS_TYPE_OSX
#elif defined __OS2__ 
	#define SYSTEM_OS_VENDOR_IBM
	#define SYSTEM_OS_TYPE_OS2
#else
	#define SYSTEM_OS_TYPE_UNKNOWN
#endif

#if defined __LP64__ || defined __x86_64__
	#define SYSTEM_ARCH_64
#elif defined __i386__
	#define SYSTEM_ARCH_32
#endif

/* OS-Type */
boolean_t system_isWindows();
boolean_t system_isDOS();
boolean_t system_isOS2();
boolean_t system_isUnix();
boolean_t system_isLinux();
boolean_t system_isIRIX();
boolean_t system_isHPUX();
boolean_t system_isOSX();

/* OS-Vendor */
boolean_t system_isMicrosoft();
boolean_t system_isSGI();
boolean_t system_isHP();
boolean_t system_isIBM();
boolean_t system_isApple();

/* Arch */
boolean_t system_is32Bit();
boolean_t system_is64Bit();
int system_getCPUArch();

void system_sleep(unsigned long milliseconds);

#endif
