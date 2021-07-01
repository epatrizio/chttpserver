#ifndef TESTS_H
#define TESTS_H

#include "lib/ctestfmk.h"
#include "../src/utils.h"
#include "../src/http.h"

void test_str_concat(const void*);
void test_is_image_file(const void*);
void test_get_file_size(const void*);
void test_get_query_string(const void*);

void test_parse_client_request(const void*);
void test_get_trace_http_header(const void*);

#endif