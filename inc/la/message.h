/*
 * =====================================================================================
 *
 *       Filename:  message.h
 *    Description:  central messaging functions
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#ifndef LA_MESSAGE_H
#define LA_MESSAGE_H

#define MESSAGE_SIZE 1024 * 4

void message_error(const char *text, ...);
void message_debug(const char *text, ...);
void message_info(const char *text, ...);
void message_warn(const char *text, ...);

#endif
