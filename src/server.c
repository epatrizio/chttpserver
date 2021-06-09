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
#include "http.h"
#include "utils.h"

#define BACKLOG 10
#define BUFFER_SIZE 1024
#define HEADER_MAX_SIZE 200

int socket_init(int);
void socket_init_trace(struct sockaddr_in*);
void handle_request(Request*, int);
void send_text_content(int, const char*);
void send_image_content(int, const char*);
void send_ping(int);

void server_start(int port)
{
    int client_socket;
    long client_request;
    Request *client_request_struct;
    struct sockaddr_in client_name;
    int client_name_len = sizeof(client_name);

    int server_socket = socket_init(port);

    while (true) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_name, (socklen_t*)&client_name_len);
        if (client_socket < 0) {
            perror_exit("[client socket accept]");
        }

        char client_request_buffer[BUFFER_SIZE] = {0};
        client_request = read(client_socket, client_request_buffer, BUFFER_SIZE);
        if (client_request < 0) {
            printf("Client request error!\n");
        }
        printf("%s", client_request_buffer);

        client_request_struct = parse_client_request(client_request_buffer);
        handle_request(client_request_struct, client_socket);

        free(client_request_struct);
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

    if (listen(server_socket, BACKLOG) < 0) {
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

void handle_request(Request* request, int client_socket)
{
    if (strcmp(request->method,"GET") == 0) {
        if (strcmp(request->content_requested,"/ping") == 0) {
            send_ping(client_socket);
        } else {
            if (is_image_file(request->content_requested))
                send_image_content(client_socket, request->content_requested);
            else
                send_text_content(client_socket, request->content_requested);
        }
    } else {
        send_not_implemented(client_socket);
    }
}

void send_text_content(int client_socket, const char *content_local_path)
{
    char header[HEADER_MAX_SIZE];
    char content[BUFFER_SIZE];
    size_t file_size;
    char *path = str_concat(WWWROOT, content_local_path);
    FILE *resource = fopen(path, "r");
    free(path);
    if (resource == NULL) {
        send_not_found(client_socket);
        perror("[send text content]");
        return;
    }

    file_size = get_file_size(resource);
    get_text_file_http_header(content_local_path, file_size, header);
    write(client_socket, header, strlen(header));

    while (fgets(content, BUFFER_SIZE, resource) != NULL) {
        write(client_socket, content, strlen(content));
        bzero(content, strlen(content));
    }

    fclose(resource);
    resource = NULL;
}

void send_image_content(int client_socket, const char *content_local_path)
{
    char header[HEADER_MAX_SIZE];
    char content[BUFFER_SIZE];
    size_t file_size, buffer_size;
    ssize_t written_size;
    char *path = str_concat(WWWROOT, content_local_path);
    FILE *resource = fopen(path, "rb");
    free(path);
    if (resource == NULL) {
        send_not_found(client_socket);
        perror("[send image content]");
        return;
    }

    file_size = get_file_size(resource);
    get_image_file_http_header(content_local_path, file_size, header);
    write(client_socket, header, strlen(header));

    while (!feof(resource)) {
        buffer_size = fread(content, 1, sizeof(content)-1, resource);
        do { 
            written_size = write(client_socket, content, buffer_size);
        } while (written_size < 0);
        bzero(content, sizeof(content));
    }

    fclose(resource);
    resource = NULL;
}

void send_ping(int client_socket)
{
    char ping[] = "HTTP/1.1 200 OK\n"SERVER_STRING"Content-Type: text/plain\nContent-Length: 5\n\nping!";
    write(client_socket, ping, strlen(ping));
}
