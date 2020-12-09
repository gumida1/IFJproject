#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include "scanner.h"
#include "expression.h"

typedef enum {
    T_UNDEF,
    T_INT,
    T_FLOAT,
    T_STRING,
    T_BOOL
} Types;

void gen_start();

void gen_head();

void gen_inbuilt_funcs();

void gen_free();

void generation();

void gen_main();

void gen_func_start(char* id_func);

void gen_func_end();

void gen_func_call(char* id_func);

void gen_func_ret_type(Types type);

void gen_func_arg(int position,char* id_arg);

void gen_func_newframe();

void gen_func_ret(char* id_func);

void gen_token_val(Token token_val);

void gen_func_arg_pass(Token token_arg, int position);

void gen_var_init(Types type);

void gen_var_declaration(char* id_var);

void gen_var_default (char* id_var, Types type_var);

void gen_label(char* id_func, int counter_label, int counter_label_depth);

void gen_if(char* id_func, int counter_label, int counter_label_depth);

void gen_else(char* id_func, int counter_label, int counter_label_depth);

void gen_push_to_stack(Token token_stack);

void gen_math_operations(Prec_rules rule);

void gen_concat_strs();

void gen_retype_to_float_1();

void gen_retype_to_float_2();

#endif