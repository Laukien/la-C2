/*
 * =====================================================================================
 *
 *       Filename:  boolean.h
 *    Description:  boolean values
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#ifndef LA_BOOLEAN_H
#define LA_BOOLEAN_H

typedef enum {
	boolean_false = 0,
	boolean_true = 1
} boolean_t;


boolean_t boolean_isBoolean(const char *value);
boolean_t boolean_toBoolean(const char *value);
boolean_t boolean_isTrue(const char *value);
boolean_t boolean_isFalse(const char *value);
char *boolean_toString(boolean_t value);

#endif
