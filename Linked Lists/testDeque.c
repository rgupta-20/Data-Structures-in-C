#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "deque.h"

int
main(int argc, char **argv)
{
    int i;
    int n;
    Deque *d;

    if(argc != 2) {
        fprintf(stderr, "Usage: %s iterations\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);

    d = dequeCreate();

    assert(dequeIsEmpty(d));

    for(i = 0; i < n; i++) {
        dequePush(d, DEQUE_FRONT, i);
        assert(!dequeIsEmpty(d));
    }

    for(i = 0; i < n; i++) {
        assert(dequePop(d, DEQUE_BACK) == i);
    }

    assert(dequeIsEmpty(d));

    for(i = 0; i < n; i++) {
        dequePush(d, DEQUE_BACK, i);
        assert(!dequeIsEmpty(d));
    }

    for(i = 0; i < n; i++) {
        assert(dequePop(d, DEQUE_FRONT) == i);
    }

    assert(dequeIsEmpty(d));

    for(i = 0; i < n; i++) {
        dequePush(d, DEQUE_BACK, i);
        assert(!dequeIsEmpty(d));
    }

    for(i = n-1; i >= 0; i--) {
        assert(dequePop(d, DEQUE_BACK) == i);
    }

    assert(dequeIsEmpty(d));

    for(i = 0; i < n; i++) {
        dequePush(d, DEQUE_FRONT, i);
    }

    for(i = n-1; i >= 0; i--) {
        assert(dequePop(d, DEQUE_FRONT) == i);
    }

    assert(dequeIsEmpty(d));

    assert(dequePop(d, DEQUE_FRONT) == DEQUE_EMPTY);

    assert(dequePop(d, DEQUE_BACK) == DEQUE_EMPTY);

    dequePush(d, DEQUE_FRONT, 12);

    dequeDestroy(d);

    return 0;
}