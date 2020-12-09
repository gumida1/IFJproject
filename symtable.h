/*
 * Tabulka symbolu (binarni strom)
 * Prevzato z lonskeho projektu
 */

#include "error.h"

typedef enum {
    VARIABLE,
    FUNCTION
} tType;

typedef union {
    int integer_num;
    float float_num;
    char* string_a;
} T_Attributes;

typedef struct {
    tType type;
    T_Attributes attribute;
    int count_param;
} tData;

typedef struct tBSTNode {
    char *Key;
    tData BSTNodeCont;
    struct tBSTNode *LPtr;
    struct tBSTNode *RPtr;
} *tBSTNodePtr;


/* funkce pro tabulku*/

void SymbolTableInit(tBSTNodePtr *RootPtr);
int SymbolTableSearch(tBSTNodePtr RootPtr, char *K, tType var_func ,int number_params) ;
int SymbolTableInsert(tBSTNodePtr *RootPtr, char *K, tData Data);
void SymbolTableDispose(tBSTNodePtr *RootPtr);