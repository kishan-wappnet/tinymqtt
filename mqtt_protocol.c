#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "headers/mqtt_protocol.h"
#include "headers/log_manager.h"

void mqtt_handle_message(int client_fd, char *buffer, int length)
{
    // For now, just echo the message back (placeholder for MQTT protocol handling)
    log_with_timestamp(LOG_INFO, "Received %d bytes from client", length);

    // Simple echo for testing - replace with proper MQTT message parsing
    mqtt_send_response(client_fd, buffer, length);
}

void mqtt_send_response(int client_fd, char *response, int length)
{
    int bytes_sent = write(client_fd, response, length);
    if (bytes_sent < 0)
    {
        log_with_timestamp(LOG_ERR, "Failed to send response to client");
    }
}