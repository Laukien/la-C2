/*
 * =====================================================================================
 *
 *       Filename:  number.h
 *    Description:  number converting
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#ifndef LA_NUMBER_H
#define LA_NUMBER_H

#include <string.h>
#include <la/boolean.h>

boolean_t number_isNumber(const char *str);
boolean_t number_isInteger(const char *str);
boolean_t number_isUnsignedInteger(const char *str);
boolean_t number_isSigned(const char *str);
int number_toInteger(const char *str);
unsigned int number_toUnsignedInteger(const char *str);
size_t number_getIntegerLength(int num);
size_t number_getUnsignedIntegerLength(unsigned int num);
char *number_integerToString(int num);
char *number_unsignedIntegerToString(unsigned int num);

#endif
