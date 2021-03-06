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
    OPTIONS,
    TRACE
} method;

typedef struct {
    char *method;
    char *content_requested;
    char *query_string;
    char *http_version;
    char *post_content_length;
    char *post_content_type;
    char *post_content_data;
} Request;

Request *parse_client_request(char*);
void free_request(Request*);
void get_trace_http_header(const size_t, char*);
void get_text_file_http_header(const char*, const size_t, char*);
void get_image_file_http_header(const char*, const size_t, char*);
void send_not_implemented(int);
void send_not_found(int);
void send_ok_php_cgi(int);
void send_internal_server_error(int);

#endif