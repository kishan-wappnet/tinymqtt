#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

#define BUFFER_SIZE 1024

void client_manager_init(int *client_sockets);
int client_manager_add(int *client_sockets, int new_socket);
void client_manager_remove(int *client_sockets, int index);
void client_manager_handle_data(int *client_sockets, int index, struct sockaddr_in *address);
int client_manager_get_max_fd(int *client_sokcets, int server_fd);
void client_manager_add_to_fdset(int *client_sockets, fd_set *readfds);

#endif