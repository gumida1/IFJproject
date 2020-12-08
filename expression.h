#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "scanner.h"
#include "error.h"

typedef enum {
    RULE_OPERAND,
    RULE_PLUS,
    RULE_MINUS,
    RULE_MUL,
    RULE_DIV,
    RULE_EQUAL,
    RULE_N_EQUAL,
    RULE_L_EQUAL,
    RULE_M_EQUAL,
    RULE_LESS,
    RULE_MORE,
    RULE_BRACKETS
} Prec_rules;

typedef enum {
    SYMBOL_IDENTIFIER,
    SYMBOL_PLUS,
    SYMBOL_MINUS,
    SYMBOL_MUL,
    SYMBOL_DIV,
    SYMBOL_EQUAL,
    SYMBOL_N_EQUAL,
    SYMBOL_L_EQUAL,
    SYMBOL_M_EQUAL,
    SYMBOL_LESS,
    SYMBOL_MORE,
    SYMBOL_L_BRACKET,
    SYMBOL_R_BRACKET,
    SYMBOL_INT,
    SYMBOL_FLOAT,
    SYMBOL_STRING,
    SYMBOL_DOLLAR,
    SYMBOL_STOP,
    SYMBOL_N_TERM
} Prec_symbols;

static Prec_symbols token_to_symbol(Token* token);

//static Prec_index symbol_to_index(Prec_symbols symbol);

int expression();

#endif