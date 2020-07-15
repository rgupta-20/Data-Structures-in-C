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