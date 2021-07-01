#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utils.h"
#include "http.h"

Request *parse_client_request(char* client_request)
{
    Request *request = (Request *) malloc(sizeof(Request));
    unsigned short int i = 0;
    bool is_first_line = true;
    char *token, *first_line_token, *rest = client_request;

    while ((token = strtok_r(rest, "\n\r", &rest))) {
        if (is_first_line) {
            first_line_token = strtok(token, " ");
            while (first_line_token != NULL) {
                if (i == 0)
                    request->method = first_line_token;
                else if (i == 1) {
                    request->content_requested = first_line_token;
                    request->query_string = NULL;
                }
                else if (i == 2)
                    request->http_version = first_line_token;
                first_line_token = strtok(NULL, " ");
                i++;
            }
            is_first_line = false;
        } else if (strstr(token, "Content-Length: ") != NULL) {
            char *w_cl = (char*) malloc(strlen(token)-16+1);
            strncpy(w_cl, token+16, strlen(token)-16+1);
            request->post_content_length = w_cl;
        } else if (strstr(token, "Content-Type: ") != NULL) {
            char *w_ct = (char*) malloc(strlen(token)-14+1);
            strncpy(w_ct, token+14, strlen(token)-14+1);
            request->post_content_type = w_ct;
        } else if (strstr(token, ": ") == NULL) {
            request->post_content_data = token;
        }
    }

    if (strstr(request->content_requested, "?") != NULL) {
        char **content_parse = get_query_string(request->content_requested);
        request->content_requested = content_parse[0];
        request->query_string = content_parse[1];
    }

    return request;
}

void free_request(Request *request)
{
    free(request->query_string);
    free(request->post_content_length);
    free(request->post_content_type);

    free(request);
}

void get_trace_http_header(const size_t request_length, char *header)
{
    sprintf(header, "HTTP/1.1 200 OK\n"SERVER_STRING"Content-Type: message/http\nContent-Length: %li\n\n", request_length);
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

void send_ok_php_cgi(int client_socket)
{
    char content[] = "HTTP/1.1 200 OK\n"SERVER_STRING"X-Powered-By: php-cgi\n";
    write(client_socket, content, strlen(content));
}

void send_internal_server_error(int client_socket)
{
    char content[] = "HTTP/1.1 500 INTERNAL SERVER ERROR\n"SERVER_STRING"Content-Type: text/plain\nContent-Length: 22\n\ninternal server error!";
    write(client_socket, content, strlen(content));
}
