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

char *get_text_file_http_header(const char* path_file)
{
    if (strstr(path_file, ".htm") != NULL)
        return "HTTP/1.1 200 OK\n"SERVER_STRING"Content-Type: text/html\n\n";
    else if (strstr(path_file, ".js") != NULL)
        return "HTTP/1.1 200 OK\n"SERVER_STRING"Content-Type: text/javascript\n\n";
    else if (strstr(path_file, ".css") != NULL)
        return "HTTP/1.1 200 OK\n"SERVER_STRING"Content-Type: text/css\n\n";
    else
        return "HTTP/1.1 200 OK\n"SERVER_STRING"Content-Type: text/css\n\n";
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
