#ifndef SYMSTACK_H
#define SYMSTACK_H

#include <stdbool.h>
//#include "symtable.h"
#include "expression.h"
#include "scanner.h"
#include "codegenerator.h"

typedef struct item_in_stack {
    Prec_symbols symbol;
    struct item_in_stack *nextptr;
    Types type;
} Symstack_item;

typedef struct {
    Symstack_item *top;
} Symstack;

void symstack_init(Symstack* stack);

void symstack_free(Symstack* stack);

void symstack_push(Symstack* symstack, Prec_symbols symbol, Types type);

bool symstack_pop(Symstack* stack);

Symstack_item* symstack_top(Symstack* stack);

Symstack_item* symstack_top_term(Symstack* stack);

bool symstack_after_top_term(Symstack* stack, Prec_symbols symbol, Types type);

#endif

