#include <stdio.h>
#include <stdlib.h>

#include "tests.h"

void test_str_concat(const void *function_node)
{
    char *s1 = "str_1";
    char *s2 = "str_2";
    char *result = str_concat(s1, s2);

    assert_eaquals_charp("str_1str_2", result, function_node);

    free(result);
}

void test_is_image_file(const void *function_node)
{
    assert_true(is_image_file("/path/name.jpeg"), function_node);
    assert_true(is_image_file("/path/name.jpg"), function_node);
    assert_true(is_image_file("/path/name.png"), function_node);
    assert_true(is_image_file("/path/name.gif"), function_node);
    assert_true(is_image_file("/path/name.ico"), function_node);
    assert_true(is_image_file("/path/name.bmp"), function_node);
    assert_true(is_image_file("/path/name.webp"), function_node);

    assert_false(is_image_file("/path/name.htm"), function_node);
    assert_false(is_image_file("/path/name.js"), function_node);
    assert_false(is_image_file("/path/name.css"), function_node);
    assert_false(is_image_file("/path/name.txt"), function_node);
    assert_false(is_image_file("/path/name"), function_node);
}

void test_get_file_size(const void *function_node)
{
    FILE *resource = fopen("tests/data/test.txt", "r");
    if (resource == NULL) {
        perror("[test_get_file_size tests/data/test.txt]");
        return;
    }

    assert_eaquals_int(get_file_size(resource), 28, function_node);

    fclose(resource);
    resource = NULL;
}

void test_parse_client_request(const void *function_node)
{
    char client_request_buffer_1[] = "GET /index.htm HTTP/1.1\n\rother_headers...";
    Request *client_request = parse_client_request(client_request_buffer_1);
    assert_eaquals_charp("GET", client_request->method, function_node);
    assert_eaquals_charp("/index.htm", client_request->content_requested, function_node);
    assert_eaquals_charp("HTTP/1.1", client_request->http_version, function_node);
    free(client_request);

    char client_request_buffer_2[] = "GET / HTTP/1.1\n\rother_headers...";
    client_request = parse_client_request(client_request_buffer_2);
    assert_eaquals_charp("GET", client_request->method, function_node);
    assert_eaquals_charp("/", client_request->content_requested, function_node);
    assert_eaquals_charp("HTTP/1.1", client_request->http_version, function_node);
    free(client_request);

    char client_request_buffer_3[] = "POST /action.cgi HTTP/1.0\n\rother_headers...";
    client_request = parse_client_request(client_request_buffer_3);
    assert_eaquals_charp("POST", client_request->method, function_node);
    assert_eaquals_charp("/action.cgi", client_request->content_requested, function_node);
    assert_eaquals_charp("HTTP/1.0", client_request->http_version, function_node);
    free(client_request);
}