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
