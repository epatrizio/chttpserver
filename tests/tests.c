#include <stdio.h>
#include <stdlib.h>

#include "tests.h"

void test_str_concat(const void *function_node)
{
    char *s1 = "str_1";
    char *s2 = "str_2";
    char *result = str_concat(s1, s2);

    assert_equals_charp("str_1str_2", result, function_node);

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

    assert_equals_int(get_file_size(resource), 28, function_node);

    fclose(resource);
    resource = NULL;
}

void test_get_query_string(const void *function_node)
{
    char **result = get_query_string("/folder/file.ext?par1=val1&par2=val2&par3=val3");
    assert_equals_charp("/folder/file.ext", result[0], function_node);
    assert_equals_charp("par1=val1&par2=val2&par3=val3", result[1], function_node);

    free(result[0]);
    free(result[1]);
    free(result);

    result = get_query_string("/folder/file2.ext");
    assert_equals_charp("/folder/file2.ext", result[0], function_node);
    assert_null(result[1], function_node);

    free(result[0]);
    free(result[1]);
    free(result);

    result = get_query_string("/folder/file3.ext?");
    assert_equals_charp("/folder/file3.ext", result[0], function_node);
    assert_null(result[1], function_node);

    free(result[0]);
    free(result[1]);
    free(result);
}

void test_parse_client_request(const void *function_node)
{
    char client_request_buffer_1[] = "GET /index.htm HTTP/1.1\n\rother_headers...";
    Request *client_request = parse_client_request(client_request_buffer_1);
    assert_equals_charp("GET", client_request->method, function_node);
    assert_equals_charp("/index.htm", client_request->content_requested, function_node);
    assert_equals_charp("HTTP/1.1", client_request->http_version, function_node);
    free(client_request);

    char client_request_buffer_2[] = "GET / HTTP/2\n\rother_headers...";
    client_request = parse_client_request(client_request_buffer_2);
    assert_equals_charp("GET", client_request->method, function_node);
    assert_equals_charp("/", client_request->content_requested, function_node);
    assert_equals_charp("HTTP/2", client_request->http_version, function_node);
    free(client_request);

    char client_request_buffer_3[] = "GET /cgi/action.cgi?par1=val1&par2=val2&par3=val3 HTTP/1.0\n\rother_headers...";
    client_request = parse_client_request(client_request_buffer_3);
    assert_equals_charp("GET", client_request->method, function_node);
    assert_equals_charp("/cgi/action.cgi", client_request->content_requested, function_node);
    assert_equals_charp("par1=val1&par2=val2&par3=val3", client_request->query_string, function_node);
    assert_equals_charp("HTTP/1.0", client_request->http_version, function_node);
    free(client_request);

    char client_request_buffer_4[] = "POST /action.cgi HTTP/1.1\n\rother_headers...";
    client_request = parse_client_request(client_request_buffer_4);
    assert_equals_charp("POST", client_request->method, function_node);
    assert_equals_charp("/action.cgi", client_request->content_requested, function_node);
    assert_equals_charp("HTTP/1.1", client_request->http_version, function_node);
    free(client_request);

    char client_request_buffer_5[] = "POST /action.cgi?par=val HTTP/1.1\n\rContent-Length: 20\n\rContent-Type: my_type\n\rcontent-content---20";
    client_request = parse_client_request(client_request_buffer_5);
    assert_equals_charp("POST", client_request->method, function_node);
    assert_equals_charp("/action.cgi", client_request->content_requested, function_node);
    assert_equals_charp("HTTP/1.1", client_request->http_version, function_node);
    assert_equals_charp("par=val", client_request->query_string, function_node);
    assert_equals_charp("20", client_request->post_content_length, function_node);
    assert_equals_charp("my_type", client_request->post_content_type, function_node);
    assert_equals_charp("content-content---20", client_request->post_content_data, function_node);
    free(client_request);
}