#include "headers/log_manager.h"

void log_with_timestamp(int priority, const char *format, ...)
{
    time_t now;
    struct tm *timeinfo;
    char timestamp[64];
    char message[512];
    va_list args;

    time(&now);
    timeinfo = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    // Log to syslog and console
    syslog(priority, "%s", message);
    printf("[%s] %s\n", timestamp, message);
}
