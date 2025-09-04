#ifndef MQTT_PROTOCOL_H
#define MQTT_PROTOCOL_H

void mqtt_handle_message(int client_fd, char *buffer, int length);
void mqtt_send_response(int client_fd, char *response, int length);

#endif