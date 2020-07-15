#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "intObject.h"

// wrap ints up as objects
// this extends Object with extra field
struct intObject {
    struct methods *methods;
    int value;
};

static void printInt(const Object *s);
static Object *cloneInt(const Object *s);
static void destroyInt(Object *s);

static struct methods intObjectMethods = {
    cloneInt,
    printInt,
    destroyInt
};

static void printInt(const Object *self) {
    printf("%d", ((struct intObject *) self)->value);
}

static Object * cloneInt(const Object *self) {
    return intObjectCreate(((struct intObject *) self)->value);
}

static void destroyInt(Object *self) {
    // we don't have any pointers, so we can just free the block
    free(self);
}

Object * intObjectCreate(int value) {
    struct intObject *self = malloc(sizeof(struct intObject));
    assert(self);

    self->methods = &intObjectMethods;
    self->value = value;

    return (Object *) self;
}