#include "symstack.h"


void symstack_init(Symstack* stack) {
    stack->top = NULL;
}

void symstack_free(Symstack* stack) {
    while (symstack_pop(stack));
}

void symstack_push(Symstack* symstack, Prec_symbols symbol, Types type) {
    Symstack_item* new_item = (Symstack_item*) malloc(sizeof(Symstack_item));
    new_item->symbol = symbol;
    new_item->type = type;
    new_item->nextptr = symstack->top;
    symstack->top = new_item;
}

bool symstack_pop(Symstack* stack) {
    if (stack->top != NULL) {
        Symstack_item* tmp = stack->top;
        stack->top = tmp->nextptr;
        free(tmp);
        return true;
    }
    return false;
}

Symstack_item* symstack_top(Symstack* stack) {
    return stack->top;
}

Symstack_item* symstack_top_term(Symstack* stack) {
    Symstack_item* tmp = stack->top;
    while(tmp != NULL) {
        if (tmp->symbol < SYMBOL_STOP) {
            return tmp;
        }
        tmp = tmp->nextptr;
    }
    return NULL;
}

bool symstack_after_top_term(Symstack* stack, Prec_symbols symbol, Types type) {
    Symstack_item* prev = NULL;
    Symstack_item* tmp = stack->top;

    while(tmp != NULL) {
        if (tmp->symbol < SYMBOL_STOP) {
            Symstack_item* new_item = (Symstack_item*) malloc(sizeof(Symstack_item));
            new_item->symbol = symbol;
            new_item->type = type;
            if (prev == NULL) {
                new_item->nextptr = stack->top;
                stack->top = new_item;
            } else {
                new_item->nextptr = prev->nextptr;
                prev->nextptr = new_item;
            }
            return true;
        }
        prev = tmp;
        tmp = tmp->nextptr;
    }
}



