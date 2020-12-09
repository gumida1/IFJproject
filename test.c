#include <stdio.h>
#include "test.h"

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    FILE *f = fopen("/Users/list/Documents/frigi/ifj/examples/my.go", "r");
    setSourceFile(f);
    Token t;
    return prog(&t);
}

void printToken(Token token) {
    switch (token.type) {
        case DATA_TYPE_FLOAT64:
            printf("DATA_TYPE_FLOAT64: %f\n", token.attribute.float64);
            break;
        case DATA_TYPE_INT:
            printf("DATA_TYPE_INT: %d\n", token.attribute.integer);
            break;
        case DATA_TYPE_STRING:
            printf("DATA_TYPE_STRING: %s\n", token.attribute.string);
            break;
        case IDENTIFIER_FUNC:
            printf("IDENTIFIER_FUNC: %s\n", token.attribute.string);
            break;
        case IDENTIFIER_VAR:
            printf("IDENTIFIER_VAR: %s\n", token.attribute.string);
            break;
        case KEYWORD:
            switch (token.attribute.keyword) {
                case ELSE:
                    printf("KEYWORD: else\n");
                    break;
                case FLOAT64:
                    printf("KEYWORD: float64\n");
                    break;
                case FOR:
                    printf("KEYWORD: for\n");
                    break;
                case FUNC:
                    printf("KEYWORD: func\n");
                    break;
                case IF:
                    printf("KEYWORD: if\n");
                    break;
                case INT:
                    printf("KEYWORD: int\n");
                    break;
                case PACKAGE:
                    printf("KEYWORD: package\n");
                    break;
                case RETURN:
                    printf("KEYWORD: return\n");
                    break;
                case STRING:
                    printf("KEYWORD: string\n");
                    break;
                default:
                    printf("Print error\n");
            }
            break;
        case ADDITION:
            printf("ADDITION\n");
            break;
        case ASSIGN:
            printf("ASSIGN\n");
            break;
        case DECLARE:
            printf("DECLARE\n");
            break;
        case DIVISION:
            printf("DIVISION\n");
            break;
        case SUBSTRACTION:
            printf("SUBSTRACTION\n");
            break;
        case MULTIPLICATION:
            printf("MULTIPLICATION\n");
            break;
        case LESS:
            printf("LESS\n");
            break;
        case LESS_EQUAL:
            printf("LESS_EQUAL\n");
            break;
        case MORE:
            printf("MORE\n");
            break;
        case MORE_EQUAL:
            printf("MORE_EQUAL\n");
            break;
        case EQUAL:
            printf("EQUAL\n");
            break;
        case NOT_EQUAL:
            printf("NOT_EQUAL\n");
            break;
        case END_OF_FILE:
            printf("END_OF_FILE\n");
            break;
        case END_OF_LINE:
            printf("END_OF_LINE\n");
            break;
        case ERROR:
            printf("ERROR\n");
            break;
        case COMMA:
            printf("COMMA\n");
            break;
        case LEFT_BRACKET:
            printf("LEFT_BRACKET\n");
            break;
        case LEFT_CORNER_BRACKET:
            printf("LEFT_CORNER_BRACKET\n");
            break;
        case LEFT_PAREN:
            printf("LEFT_PAREN\n");
            break;
        case RIGHT_BRACKET:
            printf("RIGHT_BRACKET\n");
            break;
        case RIGHT_CORNER_BRACKET:
            printf("RIGHT_CORNER_BRACKET\n");
            break;
        case RIGHT_PAREN:
            printf("RIGHT_PAREN\n");
            break;
        case SEMICOLON:
            printf("SEMICOLON\n");
            break;
        case UNDERLINE:
            printf("UNDERLINE\n");
            break;
        case END_OF_FILE_TOKEN:
            printf("END_OF_FILE_TOKEN\n");
            break;
        default:
            printf("Print error\n");
    }
}