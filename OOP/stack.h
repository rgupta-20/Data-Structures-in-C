#ifndef _STACK_H
#define _STACK_H

#include "object.h"

typedef struct elt *Stack;

Stack *stackCreate(void);
void stackDestroy(Stack *);

void stackPush(Stack *s, Object *);

Object *stackPop(Stack *s);

int stackNotEmpty(const Stack *s);

void stackPrint(const Stack *s);

#endif