#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#include "server.h"

int socket_init(int);
void socket_init_trace(struct sockaddr_in*);
void send_ping(int);
void perror_exit(const char*);

void server_start(int port)
{
    int client_socket;
    long client_request;
    struct sockaddr_in client_name;
    int client_name_len = sizeof(client_name);

    int server_socket = socket_init(port);

    while (true) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_name, (socklen_t*)&client_name_len);
        if (client_socket < 0) {
            perror_exit("[client socket accept]");
        }

        char client_request_buffer[1024] = {0};
        client_request = read(client_socket, client_request_buffer, 1024);
        if (client_request < 0) {
            printf("Client request error!\n");
        }
        printf("%s", client_request_buffer);

        // For the moment, send always a ping return to client
        send_ping(client_socket);

        shutdown(client_socket, SHUT_RDWR); 
        close(client_socket);
    }
}

int socket_init(int port)
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror_exit("[server socket create]");
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //127.0.0.1 - INADDR_ANY
    memset(server_address.sin_zero, '\0', sizeof(server_address.sin_zero));

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror_exit("[server socket bind]");
    }

    if (listen(server_socket, 10) < 0) {
        perror_exit("[server socket listen]");
    }

    socket_init_trace(&server_address);

    return server_socket;
}

void socket_init_trace(struct sockaddr_in *server_address)
{
    char hostBuffer[INET6_ADDRSTRLEN];
    char serviceBuffer[NI_MAXSERV];
    socklen_t addr_len = sizeof(*server_address);

    int ret = getnameinfo(
        (struct sockaddr *) server_address,
        addr_len,
        hostBuffer,
        sizeof(hostBuffer),
        serviceBuffer,
        sizeof(serviceBuffer),
        NI_NUMERICHOST | NI_NUMERICSERV
    );

    if (ret != 0) {
        printf("getnameinfo error: %s\n", gai_strerror(ret));
        perror_exit("[server socket init trace]");
    }

    printf("Server listening on http://%s:%s\n", hostBuffer, serviceBuffer);
}

void send_ping(int client_socket)
{
    char ping[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nping return!";
    write(client_socket, ping, strlen(ping));
}

void perror_exit(const char *prefix_message)
{
    perror(prefix_message);
    exit(EXIT_FAILURE);
}
