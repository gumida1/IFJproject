#include "parser.h"

int prog(Token *token) {
    GET_TOKEN;
    if (token->type == END_OF_LINE) {
        return prog(token);
    } else if (token->type == KEYWORD && token->attribute.keyword == PACKAGE) {
        GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
        if (!strcmp(token->attribute.string, "main")) {
            GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(func);
        }
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}

int func(Token *token) {
    GET_TOKEN;
    if (token->type == END_OF_LINE) {
        return func(token);
    } else if (token->type == KEYWORD && token->attribute.keyword == FUNC) {
        GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
        GET_TOKEN_AND_CHECK_TYPE(LEFT_PAREN);
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(params_in);
        CHECK_TOKEN_TYPE(RIGHT_PAREN);
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(params_out);
        GET_TOKEN_AND_CHECK_TYPE(LEFT_BRACKET);
        GET_TOKEN_AND_CHECK_TYPE(END_OF_LINE);
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(statement);
        GET_TOKEN_AND_CHECK_TYPE(END_OF_LINE);
        GET_TOKEN_AND_CHECK_TYPE(RIGHT_BRACKET);
        return func(token);
    }
    return SYNTAX_ERROR;
}

int type(Token *token) {}

int params_in(Token *token) {
    if (token->type == IDENTIFIER) {
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(var);
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(params_in_n);
        return SYNTAX_OK;
    } else if (token->type == RIGHT_PAREN) {
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}

int params_in_n(Token *token) {
    if (token->type == COMMA) {
        GET_TOKEN_AND_CHECK_TYPE(IDENTIFIER);
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(var);
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(params_in_n);
        return SYNTAX_OK;
    } else if (token->type == RIGHT_PAREN) {
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}

int params_out(Token *token) {
    CHECK_TOKEN_TYPE(LEFT_PAREN);
    GET_TOKEN;
    if (token->type == RIGHT_PAREN) {
        return SYNTAX_OK;
    } else {
        JUMP_INTO_AND_CHECK_RES(var);
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(params_out_n);
        CHECK_TOKEN_TYPE(RIGHT_PAREN);
    }
    return SYNTAX_OK;
}

int params_out_n(Token *token) {
    if (token->type == COMMA) {
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(var);
        GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(params_out_n);
        return SYNTAX_OK;
    } else if (token->type == RIGHT_PAREN) {
        return SYNTAX_OK;
    }
    return SYNTAX_ERROR;
}

int statement(Token *token) {

}

int id_statement_n(Token *token) {}

int arg(Token *token) {}

int arg_n(Token *token) {}

int var(Token *token) {
    if (token->type == KEYWORD) {
        if (token->attribute.keyword == INT ||
            token->attribute.keyword == FLOAT64 ||
            token->attribute.keyword == STRING) {
            return SYNTAX_OK;
        }
        return SYNTAX_ERROR;
    } else
        return SYNTAX_ERROR;
}

int def_var(Token *token) {}

int for_3(Token *token) {}

int diff_var(Token *token) {}

int ret(Token *token) {}

int ret_n(Token *token) {}