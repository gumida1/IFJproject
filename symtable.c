/*
 * Tabulka symbolu (binarni strom)
 * Prevzato z lonskeho projektu
 */


#include "symtable.h"

#define NO_PARAM_COMPARISON 0

void SymbolTableInit(tBSTNodePtr *RootPtr) {
    *RootPtr = NULL;
}

int SymbolTableSearch(tBSTNodePtr RootPtr, char *K, tType var_func ,int number_params) {
    if (RootPtr == NULL) {
        return SYNTAX_ERROR;
    }

    if (strcmp(RootPtr->Key, K) == 0) {  //Nasli sme polozku
       
        if (var_func == FUNCTION)   // pracujeme s funkci
        {
            if (RootPtr->BSTNodeCont.type == VARIABLE)
            {return SEM_UNDEF_REDEF_ERROR;}
            
            if (number_params == NO_PARAM_COMPARISON)
            {return SYNTAX_OK;}

            else if (number_params == RootPtr->BSTNodeCont.count_param)
            {return SYNTAX_OK;}     
            
            else
            {return SYNTAX_ERROR;}
        } 
        else    // pracujeme s proměnnou
        {
            if (RootPtr->BSTNodeCont.type == FUNCTION)
            {
                fprintf(stderr, "Kolize při definice proměnné %s, již existuje funcke s takovým názvem\n", K);
                return SEM_UNDEF_REDEF_ERROR;
            }
            else
            {
                return SYNTAX_OK;
            }
        } 
    }

    if (strcmp(RootPtr->Key, K) > 0) {  //Polozka je mensia ideme do lava
        return SymbolTableSearch(RootPtr->LPtr,K, var_func , number_params);
    } else {  //Polozka je vacsia ideme do prava
        return SymbolTableSearch(RootPtr->RPtr, K, var_func, number_params);
    }
}


//    INSERT
int SymbolTableInsert(tBSTNodePtr *RootPtr, char *K, tData Content) {

    if (*RootPtr == NULL) {  //Prvok neexistuje

        tBSTNodePtr tmp_node = (tBSTNodePtr) malloc (sizeof(struct tBSTNode));   // deklarace a alokovování paměti pro pomocný uzel
	    if (tmp_node == NULL) {return INTERN_ERROR; /*selhal malloc*/}
	   

        int key_length = strlen(K);
        tmp_node->Key = malloc(key_length * sizeof(char));
        if (tmp_node->Key == NULL) {return INTERN_ERROR; /*selhal malloc*/}
        strcpy(tmp_node->Key, K);
        
      
        tmp_node->BSTNodeCont = Content;
        tmp_node->RPtr = NULL;
        tmp_node->LPtr = NULL;

        (*RootPtr) = tmp_node;    // vložení uzlu
        return SYNTAX_OK;

    } else {
        if (strcmp((*RootPtr)->Key, K) > 0) {  //Polozka je mensia ideme do lava
            SymbolTableInsert(&(*RootPtr)->LPtr, K, Content);
            return SYNTAX_OK;
        } else if ((strcmp((*RootPtr)->Key, K) < 0)) {  //Polozka je vacsia ideme do prava
            SymbolTableInsert(&(*RootPtr)->RPtr, K, Content);
            return SYNTAX_OK;
        }
        else{ return INTERN_ERROR;}
        
    }
}

void SymbolTableDispose(tBSTNodePtr *RootPtr) {
    /*   ----------
** Zruší celý binární vyhledávací strom a korektně uvolní paměť.
**
** Po zrušení se bude BVS nacházet ve stejném stavu, jako se nacházel po
** inicializaci. Tuto funkci implementujte rekurzivně bez deklarování pomocné
** funkce.
**/

    if ((*RootPtr) == NULL) {
        return;
    }

    SymbolTableDispose(&((*RootPtr)->LPtr));
    SymbolTableDispose(&((*RootPtr)->RPtr));

    free((*RootPtr)->Key);
    (*RootPtr)->Key = NULL;

    free(*RootPtr);
    *RootPtr = NULL;
}


