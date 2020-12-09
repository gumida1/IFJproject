/*
 * Generator kodu
 * Jan Kleisl xkleis00
 */

#include "codegenerator.h"

#define ADD(addition) string_dyn_add(&code_dest, addition)

#define ADD_W_EOL(addition) string_dyn_add(&code_dest, (addition "\n"))

#define ADD_INT(addition_int)							\
	do {												\
		char int_to_str[42];							\
		sprintf(int_to_str, "%d", addition_int);		\
		ADD(int_to_str);								\
	} while (0)

#define FUNC_INPUTS \
"\nLABEL $inputs" \
"\nPUSHFRAME" \
"\nDEFVAR LF@?ret" \
"\nREAD LF@?ret string" \
"\nPOPFRAME" \
"\nRETURN"

#define FUNC_INPUTI \
"\nLABEL $inputi" \
"\nPUSHFRAME" \
"\nDEFVAR LF@?ret" \
"\nREAD LF@?ret int" \
"\nPOPFRAME" \
"\nRETURN"

#define FUNC_INPUTF \
"\nLABEL $inputf" \
"\nPUSHFRAME" \
"\nDEFVAR LF@?ret" \
"\nREAD LF@?ret float" \
"\nPOPFRAME" \
"\nRETURN"

#define FUNC_PRINT \
"\nLABEL $print" \
"\nPUSHFRAME" \
"\nWRITE LF@?i" \
"\nPOPFRAME" \
"\nRETURN"

#define FUNC_INT2FLOAT \
"\nLABEL $int2float" \
"\nPUSHFRAME" \
"\nDEFVAR LF@?ret" \
"\nINT2FLOAT LF@?ret LF@?0" \
"\nPOPFRAME" \
"\nRETURN"

#define FUNC_FLOAT2INT \
"\nLABEL $float2int" \
"\nPUSHFRAME" \
"\nDEFVAR LF@?ret" \
"\nFLOAT2INT LF@?ret LF@?0" \
"\nPOPFRAME" \
"\nRETURN"

#define FUNC_LEN \
"\nLABEL $len" \
"\nPUSHFRAME" \
"\nDEFVAR LF@?ret" \
"\nSTRLEN LF@?ret LF@?0" \
"\nPOPFRAME" \
"\nRETURN"

#define FUNC_ORD \
"\nLABEL $ord" \
"\nPUSHFRAME" \
"\nDEFVAR LF@?ret" \
"\nMOVE LF@?ret int@0" \
"\nDEFVAR LF@help1" \
"\nLT LF@help1 LF@?1 int@0" \
"\nJUMPIFEQ $ordret LF@help1 bool@true" \
"\nDEFVAR LF@help2" \
"\nCREATEFRAME" \
"\nDEFVAR TF@?0" \
"\nMOVE TF@?0 LF@?0" \
"\nCALL $len" \
"\nMOVE LF@help2 TF@?ret" \
"\nGT LF@help1 LF@?1 LF@help2" \
"\nJUMPIFEQ $ordret LF@help1 bool@true" \
"\nSUB LF@?1 LF@?1 int@1" \
"\nSTRI2INT LF@?ret LF@?0 LF@?1" \
"\nLABEL $ordret" \
"\nPOPFRAME" \
"\nRETURN"

#define FUNC_CHR \
"\nLABEL $chr" \
"\nPUSHFRAME" \
"\nDEFVAR LF@?ret" \
"\nMOVE LF@?ret string@" \
"\nDEFVAR LF@help1" \
"\nLT LF@help1 LF@?0 int@0" \
"\nJUMPIFEQ $chrret LF@help1 bool@true" \
"\nGT LF@help1 LF@?0 int@255" \
"\nJUMPIFEQ $chrret LF@help1 bool@true" \
"\nINT2CHAR LF@?ret LF@?0" \
"\nLABEL $chrret" \
"\nPOPFRAME" \
"\nRETURN"

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
    ADD_W_EOL("DEFVAR GF@?global_math_var1");
    ADD_W_EOL("DEFVAR GF@?global_math_var2");
    ADD_W_EOL("DEFVAR GF@?global_math_var3");
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
    gen_inbuilt_funcs();
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
    ADD_W_EOL("?ret");
}

void gen_token_val(Token token_val) {
    char tmp_str[42];
    unsigned char x;
    string_dyn tmp;
    string_dyn_init(&tmp, 8);
    switch (token_val.type) {
    case IDENTIFIER_VAR:
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

void gen_math_operations(Prec_rules rule) {
    switch(rule) {
    case RULE_PLUS:
        ADD_W_EOL("ADDS");
        break;
    case RULE_MINUS:
        ADD_W_EOL("SUBS");
        break;
    case RULE_MUL:
        ADD_W_EOL("MULS");
        break;
    case RULE_DIV:
        ADD_W_EOL("DIVS");
        break;
    case RULE_EQUAL:
        ADD_W_EOL("EQS");
        break;
    case RULE_N_EQUAL:
        ADD_W_EOL("EQS");
        ADD_W_EOL("NOTS");
        break;
    case RULE_L_EQUAL:
        ADD_W_EOL("POPS GF@?global_math_var1");
		ADD_W_EOL("POPS GF@?global_math_var2");
		ADD_W_EOL("PUSHS GF@?global_math_var2");
		ADD_W_EOL("PUSHS GF@?global_math_var1");
		ADD_W_EOL("LTS");
		ADD_W_EOL("PUSHS GF@?global_math_var2");
		ADD_W_EOL("PUSHS GF@?global_math_var1");
		ADD_W_EOL("EQS");
		ADD_W_EOL("ORS");
		break;
    case RULE_M_EQUAL:
        ADD_W_EOL("POPS GF@?global_math_var1");
		ADD_W_EOL("POPS GF@?global_math_var2");
		ADD_W_EOL("PUSHS GF@?global_math_var2");
		ADD_W_EOL("PUSHS GF@?global_math_var1");
		ADD_W_EOL("GTS");
		ADD_W_EOL("PUSHS GF@?global_math_var2");
		ADD_W_EOL("PUSHS GF@?global_math_var1");
		ADD_W_EOL("EQS");
		ADD_W_EOL("ORS");
		break;
    case RULE_LESS:
        ADD_W_EOL("LTS");
        break;
    case RULE_MORE:
        ADD_W_EOL("GTS");
        break;
    default:
        break;
    }
}

void gen_concat_strs() {
    ADD_W_EOL("POPS GF@?global_math_var3");
    ADD_W_EOL("POPS GF@?global_math_var2");
    ADD_W_EOL("CONCAT GF@?global_math_var1 GF@?global_math_var2 GF@?global_math_var3");
    ADD_W_EOL("PUSHS GF@?global_math_var1");
}

void gen_retype_to_float_1() {
    ADD_W_EOL("INT2FLOATS");
}

void gen_retype_to_float_2() {
    ADD_W_EOL("POPS GF@?global_math_var1");
    ADD_W_EOL("INT2FLOATS");
    ADD_W_EOL("PUSHS GF@?global_math_var1");
}

/*
*
*
*   MISSING FOR
*
*
*/