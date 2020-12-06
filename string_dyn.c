#include <stdlib.h>
#include <string.h>
#include "string_dyn.h"

//clears content 
void string_dyn_clear(string_dyn *str) {
    str->size = 0;
    str->quantity = 0;
    str->string[str->size] = '\0';
}

//initialization
void string_dyn_init(string_dyn *str, size_t init_quantity) {
    str->string = (char *) malloc(init_quantity);
    string_dyn_clear(str);
    str->quantity = init_quantity;
}

//frees string pointer
void string_dyn_free(string_dyn *str) {
    free(str->string);
}

//adds string to existing string
void string_dyn_add(string_dyn *str, const char *c_string) {
    size_t c_string_size = (size_t) strlen(c_string);
    if (str->size + c_string_size + 1 >= str->quantity) {
        size_t realloc_size = str->size + c_string_size + 1;
        str->string = (char *) realloc(str->string, realloc_size);
        str->quantity = realloc_size;
    }
    str->size = str->size + c_string_size;
    strcat(str->string, c_string);
    str->string[str->size] = '\0';
}

//adds char to existing string
void string_dyn_add_char(string_dyn *str, char x) {
    if (str->size + 1 >= str->quantity) {
        size_t realloc_size = str->size + 8;
        str->string = (char *) realloc(str->string, realloc_size);
        str->quantity = realloc_size;
    }
    str->string[str->size++] = x;
    str->string[str->size] = '\0';
}

