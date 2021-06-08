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
