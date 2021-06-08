#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "tests.h"

int main(int argc, char *argv[])
{
    TestSuite *ts_server = createTestSuite("UTILS");
    addTestFunction(ts_server, "str_concat", test_str_concat);
    addTestFunction(ts_server, "is_image_file", test_is_image_file);
    addTestFunction(ts_server, "get_file_size", test_get_file_size);
    runTestSuite(ts_server, argc, argv);
    displayTestSuite(ts_server);
    deleteTestSuite(ts_server);

    return EXIT_SUCCESS;
}