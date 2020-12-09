/*
 * Vyhodnocovani vyrazu
 * Jan Kleisl xkleis00
 */

#include "expression.h"
#include "symstack.c"

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
        {R, S, S,          R,          R,          S,          R},
        {R, R, S,          R,          R,          S,          R},
        {S, S, S,          E,          S,          S,          ERROR_SIGN},
        {R, R, ERROR_SIGN, R,          R,          ERROR_SIGN, R},
        {S, S, S,          R,          ERROR_SIGN, S,          R},
        {R, R, ERROR_SIGN, R,          R,          ERROR_SIGN, R},
        {S, S, S,          ERROR_SIGN, S,          S,          ERROR_SIGN}
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

static Prec_symbols token_to_symbol(Token *token) {
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
        case IDENTIFIER_VAR:
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

static Types get_t_data(Token *token) {
    switch (token->type) {
        case IDENTIFIER_VAR:
            if (token->attribute.keyword == INT) {
                return T_INT;
            } else if (token->attribute.keyword == FLOAT64) {
                return T_FLOAT;
            } else if (token->attribute.keyword == STRING) {
                return T_STRING;
            } else {
                return T_UNDEF;
            }
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

static int symbols_after_stop(bool* stop) {
    Symstack_item* tmp = symstack_top(&stack);
    int counter =0;
    while (tmp != NULL) {
        if (tmp->symbol) {
            *stop = false;
            counter++;
        } else {
            *stop = true;
            break;
        }
        tmp = tmp->nextptr;
    }
    return counter;
}

static Prec_rules test_prec_rules(int number, Symstack_item* item1, Symstack_item* item2, Symstack_item* item3) {
    switch (number) {
    case 1:
        if (item1->symbol == SYMBOL_IDENTIFIER || item1->symbol == SYMBOL_INT || item1->symbol == SYMBOL_FLOAT || item1->symbol == SYMBOL_STRING) {
            return RULE_OPERAND;
        }
        return N_RULE;
    case 3:
        if (item1->symbol == SYMBOL_L_BRACKET && item2->symbol == SYMBOL_N_TERM && item3->type == SYMBOL_R_BRACKET) {
            return RULE_BRACKETS;
        }
        if (item1->symbol == SYMBOL_N_TERM && item3->type == SYMBOL_N_TERM) {
            switch (item2->symbol) {
            case SYMBOL_PLUS:
                return RULE_PLUS;
            case SYMBOL_MINUS:
                return RULE_MINUS;
            case SYMBOL_MUL:
                return RULE_MUL;
            case SYMBOL_DIV:
                return RULE_DIV;
            case SYMBOL_EQUAL:
                return RULE_EQUAL;
            case SYMBOL_N_EQUAL:
                return RULE_N_EQUAL;
            case SYMBOL_L_EQUAL:
                return RULE_L_EQUAL;
            case SYMBOL_M_EQUAL:
                return RULE_M_EQUAL;
            case SYMBOL_LESS:
                return RULE_LESS;
            case SYMBOL_MORE:
                return RULE_MORE;
            default:
                return N_RULE;
            }
        }
    default:
        return N_RULE;
    }
    return N_RULE;
}

static int exp_sem(Prec_rules rule, Symstack_item* item1, Symstack_item* item2, Symstack_item* item3, Types* finish_type) {
    bool rtp_item1_tfloat = false;
    bool rtp_item3_tfloat = false;

    if (rule == RULE_OPERAND) {
        if (item1->type == T_UNDEF) {
            return SEM_UNDEF_REDEF_ERROR;
        }
        if (item1->type == T_BOOL) {
            return SEM_DATA_TYPE_ERROR;
        }
    }

    if (rule != RULE_OPERAND && rule != RULE_BRACKETS) {
        if (item1->type == T_UNDEF || item3->type == T_UNDEF) {
            return SEM_UNDEF_REDEF_ERROR;
        }
        if (item1->type == T_BOOL || item3->type == T_BOOL) {
            return SEM_DATA_TYPE_ERROR;
        }
    }

    if (rule == RULE_BRACKETS) {
        if (item2->type == T_UNDEF) {
            return SEM_UNDEF_REDEF_ERROR;
        }
    }

    switch (rule) {
        case RULE_OPERAND:
            *finish_type = item1->type;
            break;
        case RULE_BRACKETS:
            *finish_type = item2->type;
            break;
        case RULE_PLUS:
        case RULE_MINUS:
        case RULE_MUL:
            if (item1->type == T_STRING && item3->type == T_STRING && rule == RULE_PLUS) {
                *finish_type = T_STRING;
                break;
            }
            if (item1->type == T_INT && item3->type == T_INT) {
                *finish_type = T_INT;
                break;
            }
            if (item1->type == T_INT) {
                rtp_item1_tfloat = true;
            }
            if (item3->type == T_INT) {
                rtp_item3_tfloat = true;
            }
            if (item1->type == T_STRING || item3->type == T_STRING) {
                return SEM_DATA_TYPE_ERROR;
            }
            *finish_type = T_FLOAT;
            break;
        case RULE_DIV:
            *finish_type = T_FLOAT;
            if (item1->type == T_INT) {
                rtp_item1_tfloat = true;
            }
            if (item3->type == T_INT) {
                rtp_item3_tfloat = true;
            }
            if (item1->type == T_STRING || item3->type == T_STRING) {
                return SEM_DATA_TYPE_ERROR;
            }
        case RULE_EQUAL:
        case RULE_N_EQUAL:
        case RULE_L_EQUAL:
        case RULE_M_EQUAL:
        case RULE_LESS:
        case RULE_MORE:
            *finish_type = T_BOOL;
            if (item1->type == T_INT && item3->type == T_FLOAT) {
                rtp_item1_tfloat = true;
            } else if (item1->type == T_FLOAT && item3->type == T_INT) {
                rtp_item3_tfloat = true;
            } else if (item1->type != item3->type) {
                return SEM_DATA_TYPE_ERROR;
            }
            break;
        default:
            break;
    }
    if (rtp_item1_tfloat == true) {
        gen_retype_to_float_2();
    }
    if (rtp_item3_tfloat == true) {
        gen_retype_to_float_1();
    } 
    return SYNTAX_OK;
}

static int reduce(Token *token) {
    (void) token;
    int finish;

    Symstack_item* item1 = NULL;
    Symstack_item* item2 = NULL;
    Symstack_item* item3 = NULL;
    Prec_rules rule;
    Types finish_type;
    bool stop = false;
    int counter = symbols_after_stop(&stop);

    if (counter == 1 && stop == true) {
        item1 = stack.top;
        rule = test_prec_rules(counter, item1, NULL, NULL);
    } else if (counter == 3 && stop == true) {
        item1 = stack.top->nextptr->nextptr;
        item2 = stack.top->nextptr;
        item3 = stack.top;
        rule = test_prec_rules(counter, item1, item2, item3);
    } else {
        return SYNTAX_ERROR;
    }
    if (rule == N_RULE) {
        return SYNTAX_ERROR;
    } else {
        if ((finish = exp_sem(rule, item1, item2, item3, &finish_type))) {
            return finish;
        }
        if (rule == RULE_PLUS && finish_type == T_STRING) {
            gen_concat_strs();
        } else {
            gen_math_operations(rule);
        }
        symstack_pop_counter(&stack, counter + 1);
        symstack_push(&stack, SYMBOL_N_TERM, finish_type);
    }

    return SYNTAX_OK;
}

int expression(Token *token) {
    int finish = SYNTAX_ERROR;
    symstack_init(&stack);
    symstack_push(&stack, SYMBOL_DOLLAR, T_UNDEF);
    bool succ = false;
    Symstack_item *top_terminal;
    Prec_symbols symb;
    do {
        symb = token_to_symbol(token);
        top_terminal = symstack_top_term(&stack);
        if (top_terminal == NULL) {
            EXTRA_RET(INTERN_ERROR);
        }
        switch (precedence_table[symbol_to_index(top_terminal->symbol)][symbol_to_index(symb)]) {
            case S:
                if ((!symstack_after_top_term(&stack, SYMBOL_STOP, T_UNDEF))) {
                    EXTRA_RET(INTERN_ERROR);
                }
                symstack_push(&stack, symb, get_t_data(token));
                if (symb == SYMBOL_IDENTIFIER || symb == SYMBOL_INT || symb == SYMBOL_FLOAT ||
                    symb == SYMBOL_STRING) {
                    gen_push_to_stack(*token);
                }
                if ((finish = getToken(token)) == SCANNER_ERROR) {
                    EXTRA_RET(finish);
                }
                break;
            case E:
                symstack_push(&stack, symb, get_t_data(token));
                if ((finish = getToken(token))) {
                    EXTRA_RET(finish);
                }
                break;
            case R:
                if ((finish = reduce(token)) != SYNTAX_ERROR) {
                    EXTRA_RET(finish);
                }
                break;
            case ERROR_SIGN:
                if (symb == SYMBOL_DOLLAR && top_terminal->symbol == SYMBOL_DOLLAR) {
                    succ = true;
                } else {
                    EXTRA_RET(SYNTAX_ERROR);
                }
                break;
        }
    } while (!succ);
    Symstack_item *final_n_term = symstack_top(&stack);
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

