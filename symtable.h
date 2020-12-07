#include <string.h>
#include "scanner.h"


/* funkce pro tabulku*/

void SymbolTableInit   (tBSTNodePtr *RootPtr);
int SymbolTableSearch  (tBSTNodePtr RootPtr, char *K, tType var_func ,int number_params) ;
int SymbolTableInsert (tBSTNodePtr *RootPtr, char *K, tData Data);
void SymbolTableDispose(tBSTNodePtr *RootPtr);