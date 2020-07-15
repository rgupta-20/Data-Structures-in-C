#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "stack.h"

struct elt {
    struct elt *next;
    Object *value;
};

Stack * stackCreate(void) {
    Stack *s;

    s = malloc(sizeof(Stack));
    assert(s);
    *s = 0;  // empty stack
    return s;
}

void stackDestroy(Stack *s) {
    Object *o;
    while(stackNotEmpty(s)) {
        o = stackPop(s);
        o->methods->destroy(o);
    }
    free(s);
}

void stackPush(Stack *s, Object *value) {
    struct elt *e = malloc(sizeof(struct elt));
    e->next = *s;
    e->value = value->methods->clone(value);
    *s = e;
}

Object * stackPop(Stack *s) {
    assert(stackNotEmpty(s));
    struct elt *e = *s;
    Object *ret = e->value;
    *s = e->next;
    free(e);

    return ret;
}

int stackNotEmpty(const Stack *s) {
    return *s != 0;
}

void stackPrint(const Stack *s) {
    for(struct elt *e = *s; e; e = e->next) {
        e->value->methods->print(e->value);
        putchar(' ');
    }
    putchar('\n');
}

