#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <syslog.h>
#include <stdio.h>
// Function to log messages with a timestamp
void log_with_timestamp(int priority, const char *format, ...);

#endif // LOG_MANAGER_H