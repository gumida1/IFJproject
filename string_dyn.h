#ifndef STRING_DYN_H
#define STRING_DYN_H

#include <stdlib.h>
#include <string.h>

typedef struct {
    char* string;
    size_t size;
    size_t quantity;
} string_dyn;

void string_dyn_clear(string_dyn *str);

void string_dyn_init(string_dyn *str, size_t init_quantity);

void string_dyn_free(string_dyn *str);

void string_dyn_add(string_dyn *str, const char *c_string);

void string_add(char *str, const char *c_string);

void string_dyn_add_char(string_dyn *str, char x);

#endif