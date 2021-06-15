#ifndef HTTP_H
#define HTTP_H

#define SERVER_STRING "Server: chttpserver/1.0\n"

typedef enum {
    GET,
    HEAD,
    POST,
    PUT,
    PATCH,
    DELETE,
    CONNECT,
    OPTIONS
} method;

typedef struct {
    char *method;
    char *content_requested;
    char *query_string;
    char *http_version;
} Request;

Request *parse_client_request(char*);
void get_text_file_http_header(const char*, const size_t, char*);
void get_image_file_http_header(const char*, const size_t, char*);
void send_not_implemented(int);
void send_not_found(int);
void send_ok_php_cgi(int);
void send_internal_server_error(int);

#endif