#ifndef HTTP_H
#define HTTP_H

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
    char *http_version;
} Request;

Request *parse_client_request(char*);
void send_not_implemented(int);
void send_not_found(int);

#endif