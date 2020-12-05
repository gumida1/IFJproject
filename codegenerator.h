#include <stdlib.h>
#include "scanner.h"
#include "string_dyn.h"

typedef enum {
    T_INT,
    T_FLOAT,
    T_STRING,
    T_BOOL
} Types;

void gen_start();

void gen_head();

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




