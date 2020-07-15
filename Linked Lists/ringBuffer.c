#include <stdlib.h>
#include <assert.h>

#include "deque.h"

/*
 * Alternative implementation of a deque using a ring buffer.
 *
 * Conceptually, this is an array whose indices wrap around at
 * the endpoints. 
 *
 * The region in use is specified by a base index pointing
 * to the first element, and a length count giving the number
 * of elements.  A size field specifies the number of slots
 * in the block.
 *
 * Picture:
 *
 *  ---------------------------------------------------
 *  |7|8|9| | | | | | | | | | | | | | | | |1|2|3|4|5|6|
 *  ---------------------------------------------------
 *       ^                                 ^
 *       |                                 |
 *       base + length - 1                 base
 *
 */

struct deque {
    size_t base;
    size_t length;
    size_t size;
    int *contents;
};

#define INTIAL_SIZE (8)

static Deque * dequeCreateInternal(size_t size) {
    struct deque *d;

    d = malloc(sizeof(struct deque));

    d->base = 0;
    d->length = 0;
    d->size = size;
    d->contents = malloc(d->size * sizeof(int));

    assert(d->contents);

    return d;
}

Deque * dequeCreate(void) {
    return dequeCreateInternal(INTIAL_SIZE);
}

void dequePush(Deque *d, int direction, int value) {
    struct deque *d2;
    int *oldcontents;

    if (d->length == d->size) {
        d2 = dequeCreateInternal(2 * d->size);

        while (!dequeIsEmpty(d)) {
            dequePush(d2, DEQUE_BACK, dequePop(d, DEQUE_FRONT));
        }
        oldcontents = d->contents;
        *d = *d2;

        free(oldcontents);
        free(d2);
    }

    /*
     * This requires completely different code 
     * depending on the direction, which is 
     * annoying.
     */
    if(direction == DEQUE_FRONT) {
        /* d->base is unsigned, so we have to check for zero first */
        if(d->base == 0) {
            d->base = d->size - 1;
        } else {
            d->base--;
        }

        d->length++;

        d->contents[d->base] = value;
    } else {
        d->contents[(d->base + d->length++) % d->size] = value;
    }
}

/* pop and return first value on direction side of deque d */
/* returns DEQUE_EMPTY if deque is empty */
int
dequePop(Deque *d, int direction)
{
    int retval;
    
    if(dequeIsEmpty(d)) {
        return DEQUE_EMPTY;
    }

    /* else */
    if(direction == DEQUE_FRONT) {
        /* base goes up by one, length goes down by one */
        retval = d->contents[d->base];

        d->base = (d->base+1) % d->size;
        d->length--;

        return retval;
    } else {
        /* length goes down by one */
        return d->contents[(d->base + --d->length) % d->size];
    }
}

int 
dequeIsEmpty(const Deque *d)
{
    return d->length == 0;
}

void 
dequeDestroy(Deque *d)
{
    free(d->contents);
    free(d);
}