#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <errno.h>
#include "headers/server.h"
#include "headers/client_manager.h"
#include "headers/socket_utils.h"
#include "headers/log_manager.h"

int server_init(server_t *server)
{
    // Initialize client socket array
    client_manager_init(server->client_sockets);

    // Create server socket
    server->server_fd = socket_create_server(PORT);
    if (server->server_fd < 0)
    {
        return -1;
    }

    return 0;
}

void server_run(server_t *server)
{
    int activity, max_sd;
    // socklen_t addrlen = sizeof(server->address);

    log_with_timestamp(LOG_INFO, "Server listening on port %d...", PORT);

    while (1)
    {
        FD_ZERO(&server->readfds);

        // Add server socket to set
        FD_SET(server->server_fd, &server->readfds);

        // Get max file descriptor and add client sockets to set
        max_sd = client_manager_get_max_fd(server->client_sockets, server->server_fd);
        client_manager_add_to_fdset(server->client_sockets, &server->readfds);

        // Wait for activity on one of the sockets
        activity = select(max_sd + 1, &server->readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
        {
            log_with_timestamp(LOG_ERR, "Error in select");
            continue;
        }

        // Handle new incoming connections
        if (FD_ISSET(server->server_fd, &server->readfds))
        {
            int new_socket = socket_accept_connection(server->server_fd, &server->address);
            if (new_socket >= 0)
            {
                if (client_manager_add(server->client_sockets, new_socket) != 0)
                {
                    log_with_timestamp(LOG_ERR, "Maximum clients reached. Rejecting connection");
                    socket_close(new_socket);
                }
            }
        }

        // Handle client data
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            int sd = server->client_sockets[i];
            if (sd > 0 && FD_ISSET(sd, &server->readfds))
            {
                client_manager_handle_data(server->client_sockets, i, &server->address);
            }
        }
    }
}

void server_cleanup(server_t *server)
{
    socket_close(server->server_fd);

    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (server->client_sockets[i] > 0)
        {
            socket_close(server->client_sockets[i]);
        }
    }
}