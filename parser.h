#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "string_dyn.h"
#include "error.h"
#include "symtable.h"
//#include "codegenerator.h"
#include "expression.h"

#define GET_TOKEN \
if (getToken(token) != SCANNER_OK) return SCANNER_ERROR;

#define CHECK_TOKEN_TYPE(_type) \
if (token->type != _type) return SYNTAX_ERROR;

#define GET_TOKEN_AND_CHECK_TYPE(_type) \
GET_TOKEN; \
CHECK_TOKEN_TYPE(_type);

#define JUMP_INTO_AND_CHECK_RES(_function) \
if (_function(token) != SYNTAX_OK) return SYNTAX_ERROR;

#define GET_TOKEN_AND_JUMP_INTO_AND_CHECK_RES(_function) \
GET_TOKEN; \
JUMP_INTO_AND_CHECK_RES(_function);

#define WHILE_END_OF_LINE \
while(token->type == END_OF_LINE) { \
    GET_TOKEN; \
}

int analyse_signatures();
int prog(Token *token);
int scope(Token *token);
int func_main(Token *token);
int func(Token *token);
int type(Token *token);
int params_in(Token *token);
int params_in_n(Token *token);
int params_out(Token *token);
int params_out_n(Token *token);
int statement(Token *token);
int id_statement_n(Token *token);
int arg(Token *token);
int arg_n(Token *token);
int var(Token *token);
int val(Token *token);
int def_var(Token *token);
int for_3(Token *token);
int diff_var(Token *token);
int ret(Token *token);
int ret_n(Token *token);
int end(Token *token);
int types_multi(Token *token);

#endif
