#include <stdio.h>
#include "codegenerator.h"
//#include "scanner.h"


#define ADD(addition) string_dyn_add(&code_dest, addition)

#define ADD_W_EOL(addition) string_dyn_add(&code_dest, (addition "\n"))

#define ADD_INT(addition_int)							\
	do {												\
		char int_to_str[42];							\
		sprintf(int_to_str, "%d", addition_int);		\
		ADD(int_to_str);								\
	} while (0)

#define FUNC_INPUTS \
"\n LABEL $inputs" \
"\n PUSHFRAME" \
"\n DEFVAR LF@?ret" \
"\n READ LF@?ret string" \
"\n POPFRAME" \
"\n RETURN"

#define FUNC_INPUTI \
"\n LABEL $inputi" \
"\n PUSHFRAME" \
"\n DEFVAR LF@?ret" \
"\n READ LF@?ret int" \
"\n POPFRAME" \
"\n RETURN"

#define FUNC_INPUTF \
"\n LABEL $inputf" \
"\n PUSHFRAME" \
"\n DEFVAR LF@?ret" \
"\n READ LF@?ret float" \
"\n POPFRAME" \
"\n RETURN"

#define FUNC_PRINT \
"\n LABEL $print" \
"\n PUSHFRAME" \
"\n WRITE LF@?i" \
"\n POPFRAME" \
"\n RETURN"

#define FUNC_INT2FLOAT \
"\n LABEL $int2float" \
"\n PUSHFRAME" \
"\n DEFVAR LF@?ret" \
"\n INT2FLOAT LF@?ret LF@?0" \
"\n POPFRAME" \
"\n RETURN"

#define FUNC_FLOAT2INT \
"\n LABEL $float2int" \
"\n PUSHFRAME" \
"\n DEFVAR LF@?ret" \
"\n FLOAT2INT LF@?ret LF@?0" \
"\n POPFRAME" \
"\n RETURN"

#define FUNC_LEN \
"\n LABEL $len" \
"\n PUSHFRAME" \
"\n DEFVAR LF@?ret" \
"\n STRLEN LF@?ret LF@?0" \
"\n POPFRAME" \
"\n RETURN"

#define FUNC_ORD \
"\n LABEL $ord" \
"\n PUSHFRAME" \
"\n DEFVAR LF@?ret" \
"\n MOVE LF@?ret int@0" \
"\n DEFVAR LF@help1" \
"\n LT LF@help1 LF@?1 int@0" \
"\n JUMPIFEQ $ordret LF@help1 bool@true" \
"\n DEFVAR LF@help2" \
"\n CREATEFRAME" \
"\n DEFVAR TF@?0" \
"\n MOVE TF@?0 LF@?0" \
"\n CALL $len" \
"\n MOVE LF@help2 TF@?ret" \
"\n GT LF@help1 LF@?1 LF@help2" \
"\n JUMPIFEQ $ordret LF@help1 bool@true" \
"\n SUB LF@?1 LF@?1 int@1" \
"\n STRI2INT LF@?ret LF@?0 LF@?1" \
"\n LABEL $ordret" \
"\n POPFRAME" \
"\n RETURN"

#define FUNC_CHR \
"\n LABEL $chr" \
"\n PUSHFRAME" \
"\n DEFVAR LF@?ret" \
"\n MOVE LF@?ret string@" \
"\n DEFVAR LF@help1" \
"\n LT LF@help1 LF@?0 int@0" \
"\n JUMPIFEQ $chrret LF@help1 bool@true" \
"\n GT LF@help1 LF@?0 int@255" \
"\n JUMPIFEQ $chrret LF@help1 bool@true" \
"\n INT2CHAR LF@?ret LF@?0" \
"\n LABEL $chrret" \
"\n POPFRAME" \
"\n RETURN"

/* 
*
*
*   MISSING DEFINE OF FUNC_SUBSTR
*
*
*/

string_dyn code_dest;

void gen_head() {
    ADD_W_EOL("#Program IFJcode20 beginning");
    ADD_W_EOL(".IFJcode20");
    ADD_W_EOL("DEFVAR GF@?global_var");
    ADD_W_EOL("JUMP $$main");
}

void gen_inbuilt_funcs() {
    ADD_W_EOL(FUNC_INPUTS);
    ADD_W_EOL(FUNC_INPUTI);
    ADD_W_EOL(FUNC_INPUTF);
    ADD_W_EOL(FUNC_PRINT);
    ADD_W_EOL(FUNC_INT2FLOAT);
    ADD_W_EOL(FUNC_FLOAT2INT);
    ADD_W_EOL(FUNC_LEN);
    ADD_W_EOL(FUNC_ORD);
    ADD_W_EOL(FUNC_CHR);
}

void gen_free() {
    string_dyn_free(&code_dest);
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
    ADD("LABEL $");
    ADD(id_func);
    ADD("\n");
    ADD_W_EOL("PUSHFRAME");
}

void gen_func_end(char* id_func) {
    ADD_W_EOL("#testujeme");
    ADD("LABEL $");
    ADD(id_func);
    ADD_W_EOL("?ret");
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
    ADD(" LF@?");
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
    ADD("?ret");
}

void gen_token_val(Token token_val) {
    char tmp_str[42];
    unsigned char x;
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
        for (int i = 0; (x = (unsigned char) (token_val.attribute.string[i])) != '\0'; i++) {
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

void gen_func_arg_pass(Token token_arg, int position) {
    ADD("DEFVAR TF@?");
    ADD_INT(position);
    ADD_W_EOL();
    ADD("MOVE TF@?");
    ADD_INT(position);
    ADD(" ");
    gen_token_val(token_arg);
    ADD_W_EOL();
}

void gen_var_init(Types type_var) {
    switch (type_var) {
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

void gen_var_declaration(char* id_var) {
    ADD("DEFVAR LF@");
    ADD(id_var);
    ADD_W_EOL();
}

void gen_var_default (char* id_var, Types type_var) {
    ADD("MOVE LF@");
    ADD(id_var);
    ADD(" ");
    gen_var_init(type_var);
    ADD_W_EOL();
}

void gen_label(char* id_func, int counter_label, int counter_label_depth) {
    ADD("LABEL $");
    ADD(id_func);
    ADD("?");
    ADD_INT(counter_label);
    ADD("?");
    ADD_INT(counter_label_depth);
    ADD_W_EOL();
}

void gen_if(char* id_func, int counter_label, int counter_label_depth) {
    ADD("JUMPIFEQ $");
    ADD(id_func);
    ADD("?");
    ADD_INT(counter_label);
    ADD("?");
    ADD_INT(counter_label_depth);
    ADD_W_EOL(" GF@?global_var bool@false");
}

void gen_else(char* id_func, int counter_label, int counter_label_depth) {
    ADD("JUMP $");
    ADD(id_func);
    ADD("?");
    ADD_INT(counter_label + 1);
    ADD("?");
    ADD_INT(counter_label_depth);
    ADD_W_EOL();
    gen_label(id_func, counter_label, counter_label_depth);
}

void gen_push_to_stack(Token token_stack) {
    ADD("PUSHS ");
    gen_token_val(token_stack);
    ADD_W_EOL();
}

/*
*
*
*   MISSING FOR AND MATH OPERATIONS
*
*
*/