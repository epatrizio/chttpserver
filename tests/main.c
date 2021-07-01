#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "tests.h"

int main(int argc, char *argv[])
{
    TestSuite *ts_utils = createTestSuite("UTILS");
    addTestFunction(ts_utils, "str_concat", test_str_concat);
    addTestFunction(ts_utils, "is_image_file", test_is_image_file);
    addTestFunction(ts_utils, "get_file_size", test_get_file_size);
    addTestFunction(ts_utils, "get_query_string", test_get_query_string);
    runTestSuite(ts_utils, argc, argv);
    displayTestSuite(ts_utils);
    deleteTestSuite(ts_utils);

    TestSuite *ts_http = createTestSuite("HTTP");
    addTestFunction(ts_http, "parse_client_request", test_parse_client_request);
    addTestFunction(ts_http, "get_trace_http_header", test_get_trace_http_header);
    runTestSuite(ts_http, argc, argv);
    displayTestSuite(ts_http);
    deleteTestSuite(ts_http);

    TestSuite *ts_server = createTestSuite("SERVER");
    runTestSuite(ts_server, argc, argv);
    displayTestSuite(ts_server);
    deleteTestSuite(ts_server);

    return EXIT_SUCCESS;
}