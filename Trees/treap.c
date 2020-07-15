#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <limits.h>

#define NUM_CHILDREN (2)

#define LEFT (0)
#define RIGHT (1)

struct treap {
    int key;
    int heapKey;
    struct treap *child[NUM_CHILDREN];
};

void treapDestroy(struct treap **t) {
    
    if (*t) {
        for (int dir = LEFT; dir <= RIGHT; dir++) {
            treapDestroy(&(*t)->child[dir]);
        }
    }

    free(*t);
    *t = 0;
}

void treapPrintHelper(const struct treap *t, int depth) {
    if (t == 0) {
        return;
    }

    treapPrintHelper(t->child[LEFT], depth + 1);

    for(int i = 0; i < depth; i++) {
        putchar(' ');
    }

    printf("%d [%d]\n", t->key, t->heapKey);

    treapPrintHelper(t->child[RIGHT], depth + 1);
}

void treapPrint(const struct treap *t) {
    treapPrintHelper(t, 0);
}

int treapSearch(const struct treap *t, int key) {
    if(t == 0) {
        return 0;
    } else if(key == t->key) {
        // found it
        return 1;
    } else if(key < t->key) {
        // look in left
        return treapSearch(t->child[LEFT], key);
    } else {
        // look in right
        return treapSearch(t->child[RIGHT], key);
    }
}

int treapSearchMaxLE(const struct treap *t, int key) {

    if(t == 0) {
        // no key!
        return INT_MIN;
    } 
    else if(key == t->key) {
        // found it
        return key;
    } 
    else if(key < t->key) {
        // look in left
        return treapSearchMaxLE(t->child[LEFT], key);
    } 
    else {
        // look in right
        int result = treapSearchMaxLE(t->child[RIGHT], key);

        if(result == INT_MIN) {
            // didn't find it
            return t->key;
        } else {
            return result;
        }
    }
}

void treapRotateUp(struct treap **parent, int dir) {
    assert(parent);
    struct treap *child = *parent;
    assert(child);
    struct treap *grandchild = child->child[dir];
    assert(grandchild);
    struct treap *middleSubTreap = grandchild->child[!dir];

    *parent = grandchild;
    grandchild->dir[!dir] = child;
    child->child[dir] = middleSubTreap;
}

void treapInsert(struct treap **parent, int key)
{
    if(*parent == 0) {
        // no key!
        *parent = malloc(sizeof(struct treap));
        (*parent)->key = key;
        (*parent)->heapKey = rand();
        (*parent)->child[LEFT] = (*parent)->child[RIGHT] = 0;
    } else if(key == (*parent)->key) {
        // found it
        return;
    } else if(key < (*parent)->key) {
        // look in left
        treapInsert(&(*parent)->child[LEFT], key);
    } else {
        // look in right
        treapInsert(&(*parent)->child[RIGHT], key);
    }

    // check heap property
    for(int dir = LEFT; dir <= RIGHT; dir++) {
        if((*parent)->child[dir] != 0 && (*parent)->child[dir]->heapKey > (*parent)->heapKey) {
            treapRotateUp(parent, dir);
        }
    }
}

void
treapDelete(struct treap **parent, int key) {
    // first we look for it
    if(*parent == 0) {
        // not there
        return;
    } else if(key == (*parent)->key) {
        // got it; rotate down until we have a missing kid
        for(;;) {
            // do we have a missing child?
            for(int dir = LEFT; dir <= RIGHT; dir++) {
                if((*parent)->child[dir] == 0) {
                    // yes; free this node and promote other kid
                    struct treap *toDelete = *parent;
                    *parent = toDelete->child[!dir];
                    free(toDelete);
                    return;
                }
            }

            // no missing child, have to rotate down
            int biggerKid;
            if((*parent)->child[LEFT]->heapKey > (*parent)->child[RIGHT]->heapKey) {
                biggerKid = LEFT;
            } else {
                biggerKid = RIGHT;
            }

            // rotate up the bigger kid
            treapRotateUp(parent, biggerKid);

            // node to delete is now on opposite side
            parent = &(*parent)->child[!biggerKid];
        }
    } else {
        treapDelete(&(*parent)->child[key < (*parent)->key ? LEFT : RIGHT], key);
    }
}