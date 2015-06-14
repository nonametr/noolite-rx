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

