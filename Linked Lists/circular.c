#include <stdlib.h>

/* directions for doubly-linked list next pointers */
#define RIGHT (0)
#define LEFT (1)

struct elt {
    struct elt *next[2];
};

typedef struct elt *Elt;

/* create a new circular doubly-linked list with 1 element */
/* returns 0 on allocation error */
Elt listCreate(void) {
    Elt e;

    e = malloc(sizeof(*e));
    if(e) {
        e->next[LEFT] = e->next[RIGHT] = e;
    }

    return e;
}

void listRemove(Elt e) {
    e->next[RIGHT]->next[LEFT] = e->next[LEFT];
    e->next[LEFT]->next[RIGHT] = e->next[RIGHT];
}

void listInsert(Elt head, int dir, Elt e) {

    e->next[dir] = head->next[dir];
    e->next[!dir] = head;

    e->next[dir]->next[!dir] = e;

    e->next[!dir]->next[dir] = e;
}

void listSplit(Elt e1, Elt e2) {
/* splice out the new list */
    e2->next[RIGHT]->next[LEFT] = e1->next[LEFT];
    e1->next[LEFT]->next[RIGHT] = e2->next[RIGHT];
/* fix up the ends */
    e2->next[RIGHT] = e1;
    e1->next[LEFT] = e2;
}

void listSplice(Elt e1, Elt e2) {
    /* fix up tail end */
    e2->next[LEFT]->next[RIGHT] = e1->next[RIGHT];
    e1->next[RIGHT]->next[LEFT] = e2->next[LEFT];

    /* fix up e1 and e2 */
    e1->next[RIGHT] = e2;
    e2->next[LEFT] = e1;
}

void listDestroy(Elt e) {
    Elt target;
    Elt next;

    for (target = e->next[RIGHT]; target != e; target = next) {
        next = target->next[RIGHT];
        free(target);
    }

    free(e);
}