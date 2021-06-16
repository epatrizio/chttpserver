#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

char *str_concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);

    strcpy(result, s1);
    strcat(result, s2);

    return result;
}

void perror_exit(const char *prefix_message)
{
    perror(prefix_message);
    exit(EXIT_FAILURE);
}

size_t get_file_size(FILE *resource)
{
    size_t size;

    fseek(resource, 0, SEEK_END);
    size = ftell(resource);
    fseek(resource, 0, SEEK_SET);

    return size;
}

bool is_image_file(const char* path_file)
{
    return (
        strstr(path_file, ".jpg") != NULL || strstr(path_file, ".jpeg") != NULL ||
        strstr(path_file, ".png") != NULL ||
        strstr(path_file, ".gif") != NULL ||
        strstr(path_file, ".bmp") != NULL ||
        strstr(path_file, ".webp") != NULL ||
        strstr(path_file, ".ico") != NULL
    );
}

char **get_query_string(const char* uri)
{
    char **result = (char**)malloc(2*sizeof(char*));
    unsigned short int i = 0;
    char *uri_cpy = (char *)malloc(strlen(uri)+1);
    strcpy(uri_cpy, uri);

    result[0] = NULL;
    result[1] = NULL;
    char* token = strtok(uri_cpy, "?");
    while (token != NULL && i <= 1) {
        result[i] = (char *)malloc(strlen(token)+1);
        strcpy(result[i], token);
        token = strtok(NULL, "?");
        i++;
    }

    free(uri_cpy);

    return result;
}