#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "avl.h"

int treeHeight(const struct tree *root) {
    if (root == 0) {
        return TREE_EMPTY_HEIGHT;
    }
    else {
        return root->height;
    }
}

static int treeComputeHeight(const struct tree *root) {
    int childHeight;
    int maxChildHeight;
    int i;

    if (root == 0) {
        return TREE_EMPTY_HEIGHT;
    }
    else {
        maxChildHeight = TREE_EMPTY_HEIGHT;
        for (i = 0; i < TREE_NUM_CHILDREN; i++) {
            if (treeHeight(root->child[i]) > maxChildHeight) {
                maxChildHeight = treeHeight(root->child[i];
            }
        }
    }

    return maxChildHeight + 1;
}

size_t treeSize(const struct tree *root) {
    if(root == 0) {
        return 0;
    } else {
        return root->size;
    }
}

static int treeComputeSize(const struct tree *root) { 
    int size;
    int i;

    if (root == 0) {
        return 0;
    }
    else {
        size = 1;
        for (i = 0; i < TREE_NUM_CHILDREN; i++) {
            size += treeSize(root->child[i]);
        }
    }

    return size;
}

static void treeAggregateFix(struct tree *root)
{
    if(root) {
        root->height = treeComputeHeight(root);
        root->size = treeComputeSize(root);
    }
}

static void treeRotate(struct tree **root, int direction) {
    struct tree *x;
    struct tree *y;
    struct tree *b;

    y = *root;                  
    assert(y);
    x = y->child[direction];    
    assert(x);
    b = x->child[!direction];

    *root = x;
    x->child[!direction] = y;
    y->child[direction] = b;

    treeAggregateFix(y);
    treeAggregateFix(x);
}

static void treeRebalance(struct tree **root) {
    int tallerChild;

    if(*root) {
        for (tallerChild = 0; tallerChild < TREE_NUM_CHILDREN; tallerChild++) {
            if (treeHeight((*root)->child[tallerChild]) >= treeHeight((*root)->child[tallerChild]) + 2) {
                if (treeHeight((*root)->child[tallerChild]->child[!tallerChild]) > treeHeight((*root)->child[tallerChild]->child[tallerChild])) {
                    treeRotate(&(*root)->child[tallerChild], !tallerChild);
                }

                treeRotate(root, tallerChild);
                break;
            }
        }

        assert(abs(treeHeight((*root)->child[LEFT]) - treeHeight((*root)->child[RIGHT])) <= 1);

        #ifdef PARANOID_REBALANCE
        treeSanityCheck(*root);
        #endif
    }
}

void treeDestroy(struct tree **root) {
    int i;

    if(*root) {
        for(i = 0; i < TREE_NUM_CHILDREN; i++) {
            treeDestroy(&(*root)->child[i]);
        }
        free(*root);
        *root = TREE_EMPTY;
    }
}

void treeInsert(struct tree **root, int newElement) {
    struct tree *e;

    if(*root == 0) {

        e = malloc(sizeof(*e));
        assert(e);

        e->key = newElement;
        e->child[LEFT] = e->child[RIGHT] = 0;

        *root = e;
    } else if((*root)->key == newElement) {
        return;
    } else {
        /* do this recursively so we can fix data on the way back out */
        treeInsert(&(*root)->child[(*root)->key < newElement], newElement);
    }

    treeAggregateFix(*root);
    treeRebalance(root);
}

int treeContains(const struct tree *t, int target) {
    while(t && t->key != target) {
        t = t->child[t->key < target];
    }

    return t != 0;
}

int treeDeleteMin(struct tree **root) {
    struct tree *toFree;
    int retval;

    assert(*root);  

    if((*root)->child[LEFT]) {
        retval = treeDeleteMin(&(*root)->child[LEFT]);
    } else {
        toFree = *root;
        retval = toFree->key;
        *root = toFree->child[RIGHT];
        free(toFree);
    }

    treeAggregateFix(*root);
    treeRebalance(root);

    return retval;
}

void treeDelete(struct tree **root, int target) {
    struct tree *toFree;

    if(*root) {
        if((*root)->key == target) {
            if((*root)->child[RIGHT]) {
                (*root)->key = treeDeleteMin(&(*root)->child[RIGHT]);
            } else {
                toFree = *root;
                *root = toFree->child[LEFT];
                free(toFree);
            }
        } else {
            treeDelete(&(*root)->child[(*root)->key < target], target);
        }

        treeAggregateFix(*root);
        treeRebalance(root);
    }
}

/* how far to indent each level of the tree */
#define INDENTATION_LEVEL (2)

/* print contents of a tree, indented by depth */
static void
treePrintIndented(const struct tree *root, int depth)
{
    int i;

    if(root != 0) {
        treePrintIndented(root->child[LEFT], depth+1);

        for(i = 0; i < INDENTATION_LEVEL*depth; i++) {
            putchar(' ');
        }
        printf("%d Height: %d Size: %zu (%p)\n", root->key, root->height, root->size, (void *) root);

        treePrintIndented(root->child[RIGHT], depth+1);
    }
}

/* print the contents of a tree */
void 
treePrint(const struct tree *root)
{
    treePrintIndented(root, 0);
}

size_t treeRank(const struct tree *t, int target) {
    size_t rank = 0;

    while(t && t->key != target) {
        if(t->key < target) {
            /* go right */
            /* root and left subtree are all less than target */
            rank += (1 + treeSize(t->child[LEFT]));
            t = t->child[RIGHT];
        } else {
            /* go left */
            t = t->child[LEFT];
        }
    }

    /* we must also count left subtree */
    return rank + treeSize(t->child[LEFT]);
}

int treeUnrank(const struct tree *t, size_t rank) {
    size_t leftSize;

    /* basic idea: if rank < treeSize(child[LEFT]), recurse in left child */
    /* if it's equal, return the root */
    /* else recurse in right child with rank = rank - treeSize(child[LEFT]) - 1 */
    while(rank != (leftSize = treeSize(t->child[LEFT]))) {
        if(rank < leftSize) {
            t = t->child[LEFT];
        } else {
            t = t->child[RIGHT];
            rank -= (leftSize + 1);
        }
    }

    return t->key;
}

void treeSanityCheck(const struct tree *root) {
    int i;

    if(root) {
        assert(root->height == treeComputeHeight(root));
        assert(root->size == treeComputeSize(root));

        assert(abs(treeHeight(root->child[LEFT]) - treeHeight(root->child[RIGHT])) < 2);

        for(i = 0; i < TREE_NUM_CHILDREN; i++) {
            treeSanityCheck(root->child[i]);
        }
    }
}