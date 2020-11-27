#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef enum {
    // Data types
    DATA_TYPE_FLOAT64,
    DATA_TYPE_INT,
    DATA_TYPE_STRING,

    // Words
    IDENTIFIER,
    KEYWORD,

    // Operators
    ADDITION,       // +
    ASSIGN,         // =
    DECLARE,        // :=
    DIVISION,       // /
    SUBSTRACTION,   // -
    MULTIPLICATION, // *

    // Operations
    LESS,       // <
    LESS_EQUAL, // <=
    MORE,       // >
    MORE_EQUAL, // >=
    EQUAL,      // ==
    NOT_EQUAL,  // !=

    // States
    END_OF_FILE,
    END_OF_LINE,
    ERROR,

    // Symbols
    COMMA,                // ,
    LEFT_BRACKET,         // {
    LEFT_CORNER_BRACKET,  // [
    LEFT_PAREN,           // (
    RIGHT_BRACKET,        // }
    RIGHT_CORNER_BRACKET, // ]
    RIGHT_PAREN,          // )
    SEMICOLON,            // ;
    UNDERLINE,            // _
} Token_types;

typedef enum {
    ELSE = 1,
    FLOAT64 = 2,
    FOR = 3,
    FUNC = 4,
    IF = 5,
    INT = 6,
    PACKAGE = 7,
    RETURN = 8,
    STRING = 9,
} Keyword;

typedef union {
    double float64;
    int integer;
    Keyword keyword;
    char *string;
} Token_attribute;

typedef struct {
    Token_types type;
    Token_attribute attribute;
} Token;

void setSourceFile(FILE *file);

int isKeyword(char *string);

int getToken(Token *token);

char *appendCharToString(char c, char *string);

int stringToInteger(char *string);

float stringToFloat(char *string);

float exponentionalNumToFloat(char *string);
