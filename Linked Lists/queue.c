#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct elt {
    int value;
    struct elt *next;
};

struct queue {
    struct elt *head;
    struct elt *tail;
};

struct queue * queueCreate(void) {
    struct queue *q;
    q = malloc(sizeof(struct queue));
    q->head = 0;
    q->tail = 0;

    return q;
}

void enq(struct queue *q, int value) {
    struct elt * add;
    add = malloc(sizeof(struct elt));
    assert(add);

    add->value = value;
    add->next = 0;

    if (q->head == 0) {
        q->head = add;
    }
    else {
        q->tail->next = add;
    }

    q->tail = add;
}

int queueEmpty(const struct queue *q) {
    return (q->head == 0);
}

int deq(struct queue *q) {

    struct elt *e;
    int ret;

    assert(!queueEmpty(q));

    ret = q->head->value;

    e = q->head;
    q->head = e->next;

    free(e);

    return ret;

}

void queuePrint(struct queue *q) {
    struct elt *e;

    e = q->head;

    while(e != 0) {
        printf("%d ", e->value);
        e = e->next;
    }

    putchar('\n');
}

void queueDestroy(struct queue *q) {
    while(!queueEmpty(q)) {
        deq(q);
    }

    free(q);
}

void queueDestroy2(struct queue *q) {

    struct elt *current;
    struct elt *after;

    current = q->head;
    after = current->next;

    while(after != 0) {
        free(current);
        current = after;
        after = after->next;
    }

    free(current);
    free(q);
}

int main(int argc, char **argv) {
    int i;
    struct queue *q;

    q = queueCreate();

    for(i = 0; i < 5; i++) {
        printf("enq %d\n", i);
        enq(q, i);
        queuePrint(q);
    }

    while(!queueEmpty(q)) {
        printf("deq gets %d\n", deq(q));
        queuePrint(q);
    }

    queueDestroy(q);

    return 0;
}