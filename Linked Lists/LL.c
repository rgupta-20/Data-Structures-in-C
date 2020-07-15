#include <stdio.h>
#include <stdlib.h>

struct elt {
    struct elt *next;
    int contents;
};