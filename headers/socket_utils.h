#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <sys/socket.h>
#include <netinet/in.h>

int socket_create_server(int port);
int socket_accept_connection(int server_fd, struct sockaddr_in *address);
void socket_close(int fd);

#endif