#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "http.h"

Request *parse_client_request(char* client_request)
{
    Request *request = (Request *) malloc(sizeof(Request));
    unsigned short int i = 0;

    char* first_line_request = strtok(client_request, "\n\r");
    char* token = strtok(first_line_request, " ");
    while (token != NULL) {
        if (i == 0)
            request->method = token;
        else if (i == 1)
            request->content_requested = token;
        else if (i == 2)
            request->http_version = token;
        token = strtok(NULL, " ");
        i++;
    }

    return request;
}

void get_text_file_http_header(const char* path_file, const size_t file_size, char *header)
{
    if (strstr(path_file, ".htm") != NULL)
        sprintf(header, "HTTP/1.1 200 OK\n"SERVER_STRING"Content-Type: text/html\nContent-Length: %li\n\n", file_size);
    else if (strstr(path_file, ".js") != NULL)
        sprintf(header, "HTTP/1.1 200 OK\n"SERVER_STRING"Content-Type: text/javascript\nContent-Length: %li\n\n", file_size);
    else if (strstr(path_file, ".css") != NULL)
        sprintf(header, "HTTP/1.1 200 OK\n"SERVER_STRING"Content-Type: text/css\nContent-Length: %li\n\n", file_size);
    else
        sprintf(header, "HTTP/1.1 200 OK\n"SERVER_STRING"Content-Type: text/plain\nContent-Length: %li\n\n", file_size);
}

void get_image_file_http_header(const char* path_file, const size_t file_size, char *header)
{
    if (strstr(path_file, ".jpg") != NULL || strstr(path_file, ".jpeg") != NULL)
        sprintf(header, "HTTP/1.1 200 OK\n"SERVER_STRING"Content-Type: image/jpeg\nContent-Transfer-Encoding: binary\nContent-Length: %li\n\n", file_size);
    else if (strstr(path_file, ".gif") != NULL)
        sprintf(header, "HTTP/1.1 200 OK\n"SERVER_STRING"Content-Type: image/gif\nContent-Transfer-Encoding: binary\nContent-Length: %li\n\n", file_size);
    else if (strstr(path_file, ".png") != NULL)
        sprintf(header, "HTTP/1.1 200 OK\n"SERVER_STRING"Content-Type: image/png\nContent-Transfer-Encoding: binary\nContent-Length: %li\n\n", file_size);
    else if (strstr(path_file, ".bmp") != NULL)
        sprintf(header, "HTTP/1.1 200 OK\n"SERVER_STRING"Content-Type: image/bmp\nContent-Transfer-Encoding: binary\nContent-Length: %li\n\n", file_size);
    else if (strstr(path_file, ".webp") != NULL)
        sprintf(header, "HTTP/1.1 200 OK\n"SERVER_STRING"Content-Type: image/webp\nContent-Transfer-Encoding: binary\nContent-Length: %li\n\n", file_size);
    else if (strstr(path_file, ".ico") != NULL)
        sprintf(header, "HTTP/1.1 200 OK\n"SERVER_STRING"Content-Type: image/x-icon\nContent-Transfer-Encoding: binary\nContent-Length: %li\n\n", file_size);
    else
        sprintf(header, "HTTP/1.1 200 OK\n"SERVER_STRING"Content-Type: application/octet-stream\nContent-Transfer-Encoding: binary\nContent-Length: %li\n\n", file_size);
}

void send_not_implemented(int client_socket)
{
    char content[] = "HTTP/1.1 501 NOT IMPLEMENTED\n"SERVER_STRING"Content-Type: text/plain\nContent-Length: 16\n\nnot implemented!";
    write(client_socket, content, strlen(content));
}

void send_not_found(int client_socket)
{
    char content[] = "HTTP/1.1 404 NOT FOUND\n"SERVER_STRING"Content-Type: text/plain\nContent-Length: 10\n\nnot found!";
    write(client_socket, content, strlen(content));
}
