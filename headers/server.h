#ifndef SERVER_H
#define SERVER_H

#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 1883
#define MAX_CLIENTS 30

typedef struct {
    int server_fd;
    int client_sockets[MAX_CLIENTS];
    struct sockaddr_in address;
    fd_set readfds;
} server_t;

int server_init(server_t *server);
void server_run(server_t *server);
void server_cleanup(server_t *server);

#endif