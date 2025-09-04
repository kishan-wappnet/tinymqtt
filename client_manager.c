#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "headers/server.h"
#include "headers/client_manager.h"
#include "headers/socket_utils.h"
#include "headers/mqtt_protocol.h"
#include "headers/log_manager.h"

void client_manager_init(int *client_sockets)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        client_sockets[i] = 0;
    }
}

int client_manager_add(int *client_sockets, int new_socket)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (client_sockets[i] == 0)
        {
            client_sockets[i] = new_socket;
            return 0;
        }
    }
    return -1; // No space available
}

void client_manager_remove(int *client_sockets, int index)
{
    if (index >= 0 && index < MAX_CLIENTS)
    {
        socket_close(client_sockets[index]);
        client_sockets[index] = 0;
    }
}

void client_manager_handle_data(int *client_sockets, int index, struct sockaddr_in *address)
{
    int sd = client_sockets[index];
    char buffer[BUFFER_SIZE];
    int bytes_read = read(sd, buffer, BUFFER_SIZE - 1);

    if (bytes_read <= 0)
    {
        if (bytes_read == 0)
        {
            log_with_timestamp(LOG_INFO, "Client disconnected, ip %s, port %d",
                               inet_ntoa(address->sin_addr), ntohs(address->sin_port));
        }
        else
        {
            log_with_timestamp(LOG_ERR, "Read error");
        }
        client_manager_remove(client_sockets, index);
    }
    else
    {
        buffer[bytes_read] = '\0';
        mqtt_handle_message(sd, buffer, bytes_read);
    }
}

int client_manager_get_max_fd(int *client_sockets, int server_fd)
{
    int max_sd = server_fd;

    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        int sd = client_sockets[i];
        if (sd > max_sd)
        {
            max_sd = sd;
        }
    }

    return max_sd;
}

void client_manager_add_to_fdset(int *client_sockets, fd_set *readfds)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        int sd = client_sockets[i];
        if (sd > 0)
        {
            FD_SET(sd, readfds);
        }
    }
}