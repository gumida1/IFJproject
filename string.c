#include "string.h"

#define STRING_INIT_LEN 100

char* initString() {
    char* pointer = (char*)malloc(STRING_INIT_LEN * sizeof(char));
	return pointer;
}

void appendCharToString(char c, char* string) {
    char append[2] = {c, '\0'};
    int used_size = strlen(string);
    if (((used_size % STRING_INIT_LEN) == 0) && (used_size != 0)) {
        string = (char*)realloc(string, (used_size + STRING_INIT_LEN) * sizeof(char));
    }
    strcat(string, append);
}