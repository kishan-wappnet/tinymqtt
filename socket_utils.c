#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "headers/socket_utils.h"
#include "headers/log_manager.h"

int socket_create_server(int port)
{
    int server_fd;
    int opt = 1;
    struct sockaddr_in address;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        log_with_timestamp(LOG_ERR, "Socket creation failed");
        return -1;
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        log_with_timestamp(LOG_ERR, "Setsockopt failed");
        close(server_fd);
        return -1;
    }

    // Configure address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        log_with_timestamp(LOG_ERR, "Binding failed");
        close(server_fd);
        return -1;
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0)
    {
        log_with_timestamp(LOG_ERR, "Listen failed");
        close(server_fd);
        return -1;
    }

    return server_fd;
}

int socket_accept_connection(int server_fd, struct sockaddr_in *address)
{
    socklen_t addrlen = sizeof(*address);
    int new_socket = accept(server_fd, (struct sockaddr *)address, &addrlen);

    if (new_socket < 0)
    {
        log_with_timestamp(LOG_ERR, "Accept failed");
        return -1;
    }

    log_with_timestamp(LOG_INFO, "New connection, socket fd is %d, ip is: %s, port: %d",
                       new_socket, inet_ntoa(address->sin_addr), ntohs(address->sin_port));

    return new_socket;
}

void socket_close(int fd)
{
    if (fd > 0)
    {
        close(fd);
    }
}