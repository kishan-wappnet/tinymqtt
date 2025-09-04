#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include "headers/server.h"
#include "headers/log_manager.h"

int main()
{
    openlog("tinymqtt", LOG_PID | LOG_CONS, LOG_DAEMON);

    server_t server;

    if (server_init(&server) != 0)
    {
        log_with_timestamp(LOG_ERR, "Failed to initialize server");
        exit(EXIT_FAILURE);
    }

    log_with_timestamp(LOG_INFO, "TinyMQTT server starting...");

    server_run(&server);

    server_cleanup(&server);
    closelog();

    return 0;
}