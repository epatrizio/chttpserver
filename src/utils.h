#ifndef UTILS_H
#define UTILS_H

char *str_concat(const char*, const char*);
void perror_exit(const char*);
size_t get_file_size(FILE*);
bool is_image_file(const char*);
char **get_query_string(const char*);

#endif