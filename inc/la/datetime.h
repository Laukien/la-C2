/*
 * =====================================================================================
 *
 *       Filename:  datetime.h
 *    Description:  date & time functions
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2017
 *
 * =====================================================================================
 */

#ifndef LA_DATETIME_H
#define LA_DATETIME_H

typedef struct {
        short year;
        short month;
        short monthday;
        short weekday;
        short hour;
        short minute;
        short second;
} datetime_t;

datetime_t datetime_now();
char *datetime_getTimestampAsString();
char *datetime_getDateAsString();
char *datetime_getTimeAsString();
char *datetime_getDateTimeAsString();

#endif
