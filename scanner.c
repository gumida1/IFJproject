#include "scanner.h"

#define SCANNER_BLOCK_COMMENT 101
#define SCANNER_EQUAL 110
#define SCANNER_IDENTIFIER_KEYWORD 103
#define SCANNER_INIT 100
#define SCANNER_LINE_COMMENT 104
#define SCANNER_NUMBER 105
#define SCANNER_OPERATION 106
#define SCANNER_OPERATOR 107
#define SCANNER_SLASH 111
#define SCANNER_STRING 108
#define SCANNER_SYMBOL 109

FILE *source_file;

void setSourceFile(FILE *file) { source_file = file; }

int getToken(Token *token) {
    token->type = END_OF_FILE;
    int scanner_state = SCANNER_INIT;
    char c = 0;
    char tmp = 0;
    char *string = initString();
    while (1) {
        c = (char) getc(source_file);
        switch (scanner_state) {
            case SCANNER_BLOCK_COMMENT: // DONE
                if (c == '*') {
                    c = getc(source_file);
                    if (c == '/') {
                        scanner_state = SCANNER_INIT;
                    }
                }
                break;

            case SCANNER_EQUAL: // DONE
                if (c == '=') {
                    token->type = EQUAL;
                } else {
                    ungetc(c, source_file);
                    token->type = ASSIGN;
                }
                return SCANNER_OK;

            case SCANNER_IDENTIFIER_KEYWORD: // DONE
                while (c == '_' || isalpha(c) || isdigit(c)) {
                    appendCharToString(c, string);
                    c = getc(source_file);
                }
                ungetc(c, source_file);
                Keyword keyword = isKeyword(string);
                if (keyword != 0) {
                    token->type = KEYWORD;
                    token->attribute.keyword = keyword;
                } else {
                    token->type = IDENTIFIER;
                    token->attribute.string = string;
                }
                return SCANNER_OK;

            case SCANNER_INIT:
                if (c == '=') {
                    scanner_state = SCANNER_EQUAL;
                } else if (c == '/') {
                    scanner_state = SCANNER_SLASH;
                } else if (c == '+' || c == '-' || c == '*') {
                    ungetc(c, source_file);
                    scanner_state = SCANNER_OPERATOR;
                } else if (c == '!' || c == '<' || c == '>' || c == ':') {
                    ungetc(c, source_file);
                    scanner_state = SCANNER_OPERATION;
                } else if (c == '(' || c == ')' || c == '{' || c == '}' ||
                           c == '[' || c == ']' || c == ',' || c == ';') {
                    ungetc(c, source_file);
                    scanner_state = SCANNER_SYMBOL;
                } else if (c == '_' || isalpha(c)) {
                    appendCharToString(c, string);
                    scanner_state = SCANNER_IDENTIFIER_KEYWORD;
                } else if (c >= 48 && c <= 57) {
                    ungetc(c, source_file);
                    scanner_state = SCANNER_NUMBER;
                } else if (c == '"') {
                    scanner_state = SCANNER_STRING;
                } else if (c == '\n') {
                    token->type = END_OF_LINE;
                    return SCANNER_OK;
                } else if (isspace(c)) {
                    break;
                } else {
                    return SCANNER_ERROR;
                }
                break;

            case SCANNER_LINE_COMMENT: // DONE
                while ((c != '\n') && (c != EOF)) {
                    c = getc(source_file);
                }
                ungetc(c, source_file);
                scanner_state = SCANNER_INIT;
                break;

            case SCANNER_NUMBER: // TODO
                if (c == 48) {
                    tmp = getc(source_file);
                    if (tmp == '.') {
                        appendCharToString(c, string);
                        ungetc(tmp, source_file);
                    } else {
                        ungetc(tmp, source_file);
                        appendCharToString(c, string);
                        token->type = DATA_TYPE_INT;
                        token->attribute.integer = stringToInteger(string);
                        return SCANNER_OK;
                    }
                } else {
                    // Begin of 1
                    while (isdigit(c)) {
                        appendCharToString(c, string);
                        c = getc(source_file);
                    }
                    bool dec = false;
                    if (c == '.') {
                        appendCharToString(c, string);
                        c = getc(source_file);
                        while (isdigit(c)) {
                            appendCharToString(c, string);
                            c = getc(source_file);
                        }
                        dec = true;
                    }
                    if (c == 'e' || c == 'E') {
                        // Begin of 2
                        appendCharToString(c, string);
                        c = getc(source_file);
                        bool op = false;
                        if (c == '+' || c == '-') {
                            appendCharToString(c, string);
                            c = getc(source_file);
                            op = true;
                        }
                        while (c == 48) {
                            c = getc(source_file);
                            if (c == 48) {
                                break;
                            } else if (c > 48 && c <= 57) {
                                break;
                            } else {
                                appendCharToString(0, string);
                                token->type = DATA_TYPE_FLOAT64;
                                token->attribute.float64 = exponentionalNumToFloat(string);
                                return SCANNER_OK;
                            }
                        }
                        if (c > 48 && c <= 57) {
                            appendCharToString(c, string);
                            c = getc(source_file);
                            while (isdigit(c)) {
                                appendCharToString(c, string);
                                c = getc(source_file);
                            }
                            ungetc(c, source_file);
                            //result
                            token->type = DATA_TYPE_FLOAT64;
                            token->attribute.float64 = exponentionalNumToFloat(string);
                            return SCANNER_OK;
                        } else {
                            ungetc(c, source_file);
                            if (op && isdigit(string[strlen(string)])) {
                                token->type = DATA_TYPE_FLOAT64;
                                token->attribute.float64 = exponentionalNumToFloat(string);
                                return SCANNER_OK;
                            } else {
                                return SCANNER_ERROR;
                            }
                        }
                        // End of 2
                    }
                    ungetc(c, source_file);
                    if (dec) {
                        token->type = DATA_TYPE_FLOAT64;
                        token->attribute.float64 = stringToFloat(string);
                    } else {
                        token->type = DATA_TYPE_INT;
                        token->attribute.integer = stringToInteger(string);
                    }
                    return SCANNER_OK;
                    // End of 1
                }
                break;

            case SCANNER_OPERATION: // DONE
                tmp = getc(source_file);
                if (tmp == '=') {
                    switch (c) {
                        case '!':
                            token->type = NOT_EQUAL;
                            break;
                        case '<':
                            token->type = LESS_EQUAL;
                            break;
                        case '>':
                            token->type = MORE_EQUAL;
                            break;
                        case ':':
                            token->type = DECLARE;
                            break;
                    }
                } else {
                    ungetc(tmp, source_file);
                    switch (c) {
                        case '!':
                            return SCANNER_ERROR;
                        case '<':
                            token->type = LESS;
                            break;
                        case '>':
                            token->type = MORE;
                            break;
                        case ':':
                            return SCANNER_ERROR;
                    }
                }
                return SCANNER_OK;

            case SCANNER_OPERATOR: // DONE
                switch (c) {
                    case '+':
                        token->type = ADDITION;
                        return SCANNER_OK;
                    case '-':
                        token->type = SUBSTRACTION;
                        return SCANNER_OK;
                    case '*':
                        token->type = MULTIPLICATION;
                        return SCANNER_OK;
                }

            case SCANNER_SLASH: // DONE
                if (c == '/') {
                    scanner_state = SCANNER_LINE_COMMENT;
                    break;
                } else if (c == '*') {
                    scanner_state = SCANNER_BLOCK_COMMENT;
                    break;
                } else {
                    token->type = DIVISION;
                    return SCANNER_OK;
                }
                return SCANNER_ERROR;

            case SCANNER_STRING:
                while (c != '"') {
                    if (c == 0x5c) {
                        c = getc(source_file);
                        if (c == 'x') {
                            c = getc(source_file);
                            if (isdigit(c) || (c >= 'A' && c <= 'F') ||
                                (c >= 'a' && c <= 'f')) {
                                char first = c;
                                if (isdigit(c) || (c >= 'A' && c <= 'F') ||
                                    (c >= 'a' && c <= 'f')) {
                                    appendCharToString(0x5c, string);
                                    appendCharToString('x', string);
                                    appendCharToString(first, string);
                                    appendCharToString(c, string);
                                } else {
                                    return SCANNER_ERROR;
                                }
                            } else {
                                return SCANNER_ERROR;
                            }
                        } else if (c == '"' || c == 'n' || c == 't' ||
                                   c == '\\') {
                            appendCharToString(0x5c, string);
                            appendCharToString(c, string);
                        } else {
                            return SCANNER_ERROR;
                        }
                    } else if (c > 31) {
                        appendCharToString(c, string);
                    } else {
                        return SCANNER_ERROR;
                    }
                    c = getc(source_file);
                }
                token->type = DATA_TYPE_STRING;
                token->attribute.string = string;
                return SCANNER_OK;

            case SCANNER_SYMBOL: // DONE
                switch (c) {
                    case '(':
                        token->type = LEFT_PAREN;
                        break;
                    case ')':
                        token->type = RIGHT_PAREN;
                        break;
                    case '{':
                        token->type = LEFT_BRACKET;
                        break;
                    case '}':
                        token->type = RIGHT_BRACKET;
                        break;
                    case '[':
                        token->type = LEFT_CORNER_BRACKET;
                        break;
                    case ']':
                        token->type = RIGHT_CORNER_BRACKET;
                        break;
                    case ',':
                        token->type = COMMA;
                        break;
                    case ';':
                        token->type = SEMICOLON;
                        break;
                }
                return SCANNER_OK;
        }
    }

    return 0;
}

int isKeyword(char *string) {
    if (strcmp(string, "else") == 0) {
        return ELSE;
    } else if (strcmp(string, "float64") == 0) {
        return FLOAT64;
    } else if (strcmp(string, "for") == 0) {
        return FOR;
    } else if (strcmp(string, "func") == 0) {
        return FUNC;
    } else if (strcmp(string, "if") == 0) {
        return IF;
    } else if (strcmp(string, "int") == 0) {
        return INT;
    } else if (strcmp(string, "package") == 0) {
        return PACKAGE;
    } else if (strcmp(string, "return") == 0) {
        return RETURN;
    } else if (strcmp(string, "string") == 0) {
        return STRING;
    } else {
        return 0;
    }
}

// TODO verify strtol
int stringToInteger(char *string) {
    char *ptr;
    int returnInt = (int) strtol(string, &ptr, 10);

    return returnInt;
}

// TODO verify strtof
float stringToFloat(char *string) {
    char *ptr;
    float returnFloat = strtof(string, &ptr);

    return returnFloat;
}

// Transform number in exponential form to float
float exponentionalNumToFloat(char *string) {
    // Find position e
    char *e = strchr(string, 'e');
    // If e was not found
    if (e == NULL)
        // String must have E
        e = strchr(string, 'E');

    // Position e/E in string
    int posE = e - string;

    // Declare var for base + '\0'
    char base[posE + 1];
    // Copy all chars from begin to posE(without e/E)
    strncpy(base, string, posE);
    // Append '\0' to end of base
    base[posE + 1] = '\0';
    // Convert base to float
    float a = stringToFloat(base);

    // Declare var for exponent(strlen(string) - posE == length of exponent)
    char exponent[strlen(string) - posE];
    // Copy all chars from pos after e/E
    strcpy(exponent, e + 1);
    // Convert exponent o float
    float n = stringToFloat(exponent);

    // a^n
    float result = pow(a, n);

    return result;
}
