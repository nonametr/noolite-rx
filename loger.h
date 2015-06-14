/*
 *  Copyright 2015 Igor K. aka nonametr
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef LOGER
#define LOGER

#include <stdint.h>
#include <syslog.h>
#include <stdarg.h>

/**
 * @brief traceError - prints error message
 * @param msg - C string that contains the text to be printed.
It can optionally contain embedded format specifiers that are replaced by the values specified in subsequent additional arguments and formatted as requested.
 */
static void traceError(const char * msg, ...)
{
    va_list args;
    va_start(args, msg);
    vsyslog(LOG_ERR, msg, args);
    va_end(args);
}

/**
 * @brief traceNotice - prints notice message
 * @param msg - C string that contains the text to be printed.
It can optionally contain embedded format specifiers that are replaced by the values specified in subsequent additional arguments and formatted as requested.
 */
static void traceNotice(const char * msg, ...)
{
    va_list args;
    va_start(args, msg);
    vsyslog(LOG_NOTICE, msg, args);
    va_end(args);
}

#endif // LOGER

