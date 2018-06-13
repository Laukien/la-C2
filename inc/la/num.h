/*
 * =====================================================================================
 *
 *       Filename:  num.h
 *    Description:  simplified number.h
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#ifndef LA_NUM_H
#define LA_NUM_H

#define numisi(str) number_isInteger(str)
#define numisu(str) number_isUnsignedInteger(str)
#define numiss(str) number_isSigned(str)
#define atou(str) number_toUnsignedInteger(str)
#define utoa(str) number_unsignedIntegerToString(str)

#include <la/number.h>

#endif
