#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "tests.h"

int main(int argc, char *argv[])
{
    TestSuite *ts_server = createTestSuite("SERVER");
    addTestFunction(ts_server, "fake", test_fake);
    runTestSuite(ts_server, argc, argv);
    displayTestSuite(ts_server);
    deleteTestSuite(ts_server);

    return EXIT_SUCCESS;
}