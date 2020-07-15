#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct elt {
    struct elt *next;
    int value;
};

typedef struct elt *Stack;

#define STACK_EMPTY (0)

void stackPush(Stack *s, int value) {

    struct elt *e;

    e = malloc(sizeof(struct elt));
    assert(e);

    e->value = value;
    e->next = *s;

    *s = e;
}

int stackEmpty(Stack *s) {
    return (*s == 0);
}

int stackPop(Stack *s) {
    int value;
    struct elt *e;

    value = (*s)->value;
    e = *s;
    *s = e->next;
    free(e);

    return value;
}

void stackPrint(Stack *s) {
    struct elt *e;
    e = *s;

    while (e != 0) {
        printf("%d ", e->value);
        e = e->next;
    }

    putchar('\n');
}

void stackPrintReversed(struct stack *first) {
    struct stack *elt;
    Stack s2;

    s2 = stackCreate();

    for(elt = first; elt != 0; elt = elt->next) {
        s2 = stackPush(s2, elt->book);
    }

    stackPrint(s2);
    stackDestroy(s2);
}

void stackPrintReversedRecursive(struct stack *first) {

    if (first != 0) {
        stackPrintReversedRecursive(first->next);

        puts(first->value);
    }

int main(int argc, char **argv) {
    int i;
    Stack s;

    for (i = 0; i < 5; i++) {
        printf("pushing %d\n", i);
        stackPush(&s, i);
        stackPrint(&s);
    }

    while(!stackEmpty(&s)) {
        printf("pop gets %d\n", stackPop(&s));
        stackPrint(&s);
    }

    return 0;
}