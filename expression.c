#include "scanner.h"
#include "expression.h"
#include "symstack.h"
#include "error.h"
#include "codegenerator.h"




#define EXTRA_RET(ret) \
    do { \
        symstack_free(&stack); \
        return ret; \
    } while(0)


Symstack stack;

typedef enum {
    S,
    E,
    R,
    ERROR_SIGN
} Prec_sign;

int precedence_table[7][7] = {
    {R, S, S, R, R, S, R},
    {R, R, S, R, R, S, R},
    {S, S, S, E, S, S, ERROR_SIGN},
    {R, R, ERROR_SIGN, R, R, ERROR_SIGN, R},
    {S, S, S, R, ERROR_SIGN, S, R},
    {R, R, ERROR_SIGN, R, R, ERROR_SIGN, R},
    {S, S, S, ERROR_SIGN, S, S, ERROR_SIGN}
};

typedef enum {
    INDEX_PLUS_MINUS,
    INDEX_MUL_DIV,
    INDEX_L_BRACKET,
    INDEX_R_BRACKET,
    INDEX_REL_OPERATOR,
    INDEX_DATA,
    INDEX_DOLLAR
} Prec_index;

static Prec_symbols token_to_symbol(Token* token) {
    switch (token->type) {
    case ADDITION:
        return SYMBOL_PLUS;
    case SUBSTRACTION:
        return SYMBOL_MINUS;
    case MULTIPLICATION:
        return SYMBOL_MUL;
    case DIVISION:
        return SYMBOL_DIV;
    case ASSIGN:
        return SYMBOL_EQUAL;
    case NOT_EQUAL:
        return SYMBOL_N_EQUAL;
    case LESS_EQUAL:
        return SYMBOL_L_EQUAL;
    case MORE_EQUAL:
        return SYMBOL_M_EQUAL;
    case LESS:
        return SYMBOL_LESS;
    case MORE:
        return SYMBOL_MORE;
    case LEFT_PAREN:
        return SYMBOL_L_BRACKET;
    case RIGHT_PAREN:
        return SYMBOL_R_BRACKET;
    case IDENTIFIER:
        return SYMBOL_IDENTIFIER;
    case DATA_TYPE_INT:
        return SYMBOL_INT;
    case DATA_TYPE_FLOAT64:
        return SYMBOL_FLOAT;
    case DATA_TYPE_STRING:
        return SYMBOL_STRING;
    default:
        return SYMBOL_DOLLAR;
    }
}

static Prec_index symbol_to_index(Prec_symbols symbol) {
    switch (symbol) {
    case SYMBOL_PLUS:
    case SYMBOL_MINUS:
        return INDEX_PLUS_MINUS;
    case SYMBOL_MUL:
    case SYMBOL_DIV:
        return INDEX_MUL_DIV;
    case SYMBOL_EQUAL:
    case SYMBOL_N_EQUAL:
    case SYMBOL_L_EQUAL:
    case SYMBOL_M_EQUAL:
    case SYMBOL_LESS:
    case SYMBOL_MORE:
        return INDEX_REL_OPERATOR;
    case SYMBOL_L_BRACKET:
        return INDEX_L_BRACKET;
    case SYMBOL_R_BRACKET:
        return INDEX_R_BRACKET;
    case SYMBOL_IDENTIFIER:
    case SYMBOL_INT:
    case SYMBOL_FLOAT:
    case SYMBOL_STRING:
        return INDEX_DATA;
    default:
        return INDEX_DOLLAR;
    }
}

static Types get_t_data(Token* token) {
    switch (token->type) { 
    case IDENTIFIER:
        // stuff missing - provisionally
        return T_UNDEF;
    case DATA_TYPE_INT:
        return T_INT;
    case DATA_TYPE_FLOAT64:
        return T_FLOAT;
    case DATA_TYPE_STRING:
        return T_STRING;
    default:
        return T_UNDEF;
    }
}

int expression(Token* token) {
    symstack_init(&stack);
    bool succ = false;
    Symstack_item* top_terminal;
    Prec_symbols symb;
    do {
        symb = token_to_symbol(token);
        top_terminal = symstack_top_term(&stack);
        if (top_terminal == NULL) {
            EXTRA_RET(INTERN_ERROR);
        }
        switch (precedence_table[symbol_to_index(top_terminal->symbol)][symbol_to_index(symb)]) {
            case S:
                symstack_after_top_term(&stack, SYMBOL_STOP, T_UNDEF);
                symstack_push(&stack, symb, get_t_data(token));
                if (symb == IDENTIFIER || symb == DATA_TYPE_INT || symb == DATA_TYPE_FLOAT64 || symb == DATA_TYPE_STRING) {
                    gen_push_to_stack(*token);
                }
                break;
            case E:
                symstack_push(&stack, symb, get_t_data(token));
                break;
            case R:
                //stuff missing - provisionally
                break;
            case ERROR_SIGN:
                if (symb == SYMBOL_DOLLAR && top_terminal->symbol == SYMBOL_DOLLAR) {
                    succ = true;
                } else {
                    EXTRA_RET(SYNTAX_ERROR);
                }
                break;
        }
    } while(!succ);
    Symstack_item* final_n_term = symstack_top(&stack);
    if (final_n_term == NULL) {
        EXTRA_RET(INTERN_ERROR);
    }
    if (final_n_term->symbol != SYMBOL_N_TERM) {
        EXTRA_RET(SYNTAX_ERROR);
    }
    /*
    *
    * missing some semant analysis 
    * 
    */
    EXTRA_RET(SYNTAX_OK);
}

