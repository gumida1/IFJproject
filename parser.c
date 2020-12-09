#include "parser.h"

// OK
int prog(Token *token) {
    GET_TOKEN;
    if (token->type == END_OF_LINE) {
        return prog(token);
    } else if (token->type == KEYWORD && token->attribute.keyword == PACKAGE) {
        GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER_VAR);
        if (!strcmp(token->attribute.string, "main")) {
            return func(token);
        }
        return SYNTAX_ERROR;
    }
    return SYNTAX_ERROR;
}

// OK
int func(Token *token) {
    GET_TOKEN;
    if (token->type == END_OF_LINE) {
        return func(token);
    } else if (token->type == KEYWORD && token->attribute.keyword == FUNC) {
        GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER_FUNC);
        GET_TOKEN_AND_CHECK_TYPE(LEFT_PAREN);
        WHILE_END_OF_LINE;
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(params_in);
        CHECK_TOKEN_TYPE(RIGHT_PAREN);
        GET_TOKEN;
        if (token->type != LEFT_BRACKET) {
            if (token->type == LEFT_PAREN) {
                GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(params_out);
                CHECK_TOKEN_TYPE(RIGHT_PAREN);
                GET_TOKEN;
            } else {
                GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(params_out);
            }
        }
        CHECK_TOKEN_TYPE(LEFT_BRACKET);
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(statement);
        CHECK_TOKEN_TYPE(RIGHT_BRACKET);
        return func(token);
    } else if (token->type == END_OF_FILE_TOKEN) {
        return end(token);
    }
    return SYNTAX_ERROR;
}

// OK
int params_in(Token *token) {
    if (token->type == IDENTIFIER_VAR) {
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(var);
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(params_in_n);
        return SYNTAX_OK;
    }
    return SYNTAX_OK;
}

// OK
int params_in_n(Token *token) {
    if (token->type == COMMA) {
        WHILE_END_OF_LINE;
        GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER_VAR);
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(var);
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(params_in_n);
        return SYNTAX_OK;
    }
    return SYNTAX_OK;
}

// OK
int var(Token *token) {
    if (token->type == KEYWORD) {
        if (token->attribute.keyword == INT ||
            token->attribute.keyword == FLOAT64 ||
            token->attribute.keyword == STRING) {
            return SYNTAX_OK;
        }
    }
    return SYNTAX_ERROR;
}

// OK
int params_out(Token *token) {
    if (token->type == KEYWORD) {
        JUMP_INTO_AND_CHECK_RES(var);
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(params_out_n);
        return SYNTAX_OK;
    }
    return SYNTAX_OK;
}

// OK
int params_out_n(Token *token) {
    if (token->type == COMMA) {
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(var);
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(params_out_n);
        return SYNTAX_OK;
    }
    return SYNTAX_OK;
}

int statement(Token *token) {
    if (token->type == END_OF_LINE) {
        GET_TOKEN;
        return statement(token);
    } else if (token->type == IDENTIFIER_VAR) {
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(diff_var);
        CHECK_TOKEN_TYPE(END_OF_LINE);
        GET_TOKEN;
        return statement(token);
    } else if (token->type == KEYWORD && token->attribute.keyword == RETURN) {
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(ret);
        CHECK_TOKEN_TYPE(END_OF_LINE);
        GET_TOKEN;
        return statement(token);
    }
    return SYNTAX_OK;
}

// OK
int id_statement_n(Token *token) {
    if (token->type == COMMA) {
        GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER_VAR);
        GET_TOKEN;
        return id_statement_n(token);
    } else {
        return SYNTAX_OK;
    }
}

int arg(Token *token) {
    if (token->type == DATA_TYPE_INT ||
        token->type == DATA_TYPE_FLOAT64 ||
        token->type == DATA_TYPE_STRING ||
        token->type == IDENTIFIER_VAR) {
        JUMP_INTO_AND_CHECK_RES(val);
        GET_TOKEN;
        return arg_n(token);
    }
    return SYNTAX_OK;
}

int arg_n(Token *token) {
    if (token->type == COMMA) {
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(val);
        GET_TOKEN;
        return arg_n(token);
    }
    return SYNTAX_OK;
}

int val(Token *token) {
    if (token->type == DATA_TYPE_INT ||
        token->type == DATA_TYPE_FLOAT64 ||
        token->type == DATA_TYPE_STRING ||
        token->type == IDENTIFIER_VAR) {
        JUMP_INTO_AND_CHECK_RES(expression);
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}

int def_var(Token *token) {}

int for_3(Token *token) {}

// OK
int type(Token *token) {
    if (token->type == DATA_TYPE_STRING ||
        token->type == DATA_TYPE_FLOAT64 ||
        token->type == DATA_TYPE_INT ||
        token->type == IDENTIFIER_VAR) {
        JUMP_INTO_AND_CHECK_RES(expression);
        return SYNTAX_OK;
    } else if (token->type == IDENTIFIER_FUNC) {
        GET_TOKEN_AND_CHECK_TYPE(LEFT_PAREN);
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(arg);
        CHECK_TOKEN_TYPE(RIGHT_PAREN);
        GET_TOKEN;
        return SYNTAX_OK;
    } else if (token->type == LEFT_PAREN) {
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(expression);
        CHECK_TOKEN_TYPE(RIGHT_PAREN);
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}

// OK
int diff_var(Token *token) {
    if (token->type == DECLARE) {
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(type);
        return SYNTAX_OK;
    } else {
        JUMP_INTO_AND_CHECK_RES(id_statement_n);
        CHECK_TOKEN_TYPE(ASSIGN);
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(type);
        return SYNTAX_OK;
    }
}

int ret(Token *token) {
    if (token->type == DATA_TYPE_INT ||
        token->type == DATA_TYPE_FLOAT64 ||
        token->type == DATA_TYPE_STRING ||
        token->type == IDENTIFIER_VAR) {
        JUMP_INTO_AND_CHECK_RES(expression);
        JUMP_INTO_AND_CHECK_RES(ret_n);
        return SYNTAX_OK;
    }
    return SYNTAX_OK;
}

int ret_n(Token *token) {
    if (token->type == COMMA) {
        GET_TOKEN;
        return ret(token);
    }
    return SYNTAX_OK;
}

int end(Token *token) {
    CHECK_TOKEN_TYPE(END_OF_FILE_TOKEN);
    return SYNTAX_OK;
}

int expression(Token *token) {
    GET_TOKEN;
    return SYNTAX_OK;
}