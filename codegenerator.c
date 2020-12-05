#include <stdio.h>
#include "codegenerator.h"
#include "scanner.h"


#define ADD(addition) string_dyn_add(&code_dest, addition)

#define ADD_W_EOL(addition) string_dyn_add(&code_dest, (addition "\n"))

#define ADD_INT(addition_int)							\
	do {												\
		char int_to_str[42];							\
		sprintf(int_to_str, "%d", addition_int);		\
		ADD(int_to_str);								\
	} while (0)


string_dyn code_dest;

//space for inbuilt fcs
/*
/
*/

void gen_head() {
    ADD_W_EOL("#Program IFJcode20 beginning");
    ADD_W_EOL(".IFJcode20");
    ADD_W_EOL("DEFVAR GF@?global_var");
    ADD_W_EOL("JUMP $$main");
}

void generation()
{
	fputs(code_dest.string, stdout);
}

void gen_start() {
    string_dyn_init(&code_dest, 8);
    gen_head();
}

void gen_main() {
    ADD_W_EOL("LABEL $$main");
    ADD_W_EOL("CREATEFRAME");
    ADD_W_EOL("PUSHFRAME");
}

void gen_main_end() {
    ADD_W_EOL("POPFRAME");
    ADD_W_EOL("CLEARS");
}

void gen_func_start(char* id_func) {
    ADD_W_EOL("#Function beginning");
    ADD("LABEL $");
    ADD(id_func);
    ADD("\n");
    ADD_W_EOL("PUSHFRAME");
}

void gen_func_end(char* id_func) {
    ADD_W_EOL("#Function ending");
    ADD("LABEL $");
    ADD(id_func);
    ADD_W_EOL("!return");
    ADD_W_EOL("POPFRAME");
    ADD_W_EOL("RETURN");
}

void gen_func_call(char* id_func) {
    ADD("CALL $");
    ADD(id_func);
    ADD_W_EOL();
}

void gen_func_ret_type(Types type) {
    ADD_W_EOL("LF@?ret");
    ADD("MOVE LF@?ret ");
    gen_var_init(type);
    ADD_W_EOL();
}

void gen_func_arg(int position, char* id_arg) {
    ADD("DEFVAR LF@");
    ADD(id_arg);
    ADD_W_EOL();
    ADD("MOVE LF@");
    ADD(id_arg);
    ADD_W_EOL();
    ADD(" LF@");
    ADD_INT(position);
    ADD_W_EOL();
}

void gen_func_newframe() {
    ADD_W_EOL("CREATEFRAME");
}

void gen_func_ret(char* id_func) {
    ADD_W_EOL("MOVE LF@?ret GF@?global_var");
    ADD("JUMP $");
    ADD(id_func);
    ADD("!return");
}

void gen_token_val(Token token_val) {
    char tmp_str[42];
    string_dyn tmp;
    string_dyn_init(&tmp, 8);
    switch (token_val.type) {
    case IDENTIFIER:
        ADD("LF@");
        ADD(token_val.attribute.string);
        break;
    case DATA_TYPE_INT:
        sprintf(tmp_str, "%d", token_val.attribute.integer);
        ADD("int@");
        ADD(tmp_str);
        break;
    case DATA_TYPE_FLOAT64:
        sprintf(tmp_str, "%a", token_val.attribute.float64);
        ADD("float@");
        ADD(tmp_str);
        break;
    case DATA_TYPE_STRING:
        unsigned char x;
        unsigned char y = (unsigned char) (token_val.attribute.string);
        for (int i = 0; x = y != '\0'; i++) {
            if (x <= 32 || x == 35 || x == '\\' || isprint(x) == 0) {
                string_dyn_add_char(&tmp, '\\');
                sprintf(tmp_str, "%03d", x);
                string_dyn_add(&tmp, tmp_str);
            } else {
                string_dyn_add_char(&tmp, x);
            }
        }
        ADD("string@");
        ADD(tmp.string);
        break;
    default:
        break; 
    }
    string_dyn_free(&tmp);
}

void gen_var_init(Types type) {
    switch (type) {
    case T_INT:
        ADD("int@0");
        break;
    case T_FLOAT:
        ADD("float@0.0");
        break;
    case T_STRING:
        ADD("string@");
        break;
    case T_BOOL:
        ADD("bool@false");
        break;
    default:
        break;
    }
}



