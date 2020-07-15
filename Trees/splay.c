#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include "splay.h"

/* free all elements of a tree, replacing it with TREE_EMPTY */
void 
treeDestroy(struct tree **root)
{
    /* we want to avoid doing this recursively, because the tree might be deep */
    /* so we will repeatedly delete the root until the tree is empty */
    while(*root) {
        treeDelete(root, (*root)->key);
    }
}

/* rotate child in given direction to root */
void
treeRotate(struct tree **root, int direction)
{
    struct tree *x;
    struct tree *y;
    struct tree *b;

    /*
     *      y           x 
     *     / \         / \
     *    x   C  <=>  A   y
     *   / \             / \
     *  A   B           B   C
     */

    y = *root;                  assert(y);
    x = y->child[direction];    assert(x);
    b = x->child[!direction];

    /* do the rotation */
    *root = x;
    x->child[!direction] = y;
    y->child[direction] = b;
}

/* link operations for top-down splay */
/* this pastes a node in as !d-most node in subtree on side d */
static inline void
treeLink(struct tree ***hook, int d, struct tree *node)
{
    *hook[d] = node;
    /* strictly speaking we don't need to do this, but it allows printing the partial trees */
    node->child[!d] = 0;
    hook[d] = &node->child[!d];
}

/* splay last element on path to target to root */
static void
treeSplay(struct tree **root, int target)
{
    struct tree *t;
    struct tree *child;
    struct tree *grandchild;
    struct tree *top[TREE_NUM_CHILDREN];   /* accumulator trees that will become subtrees of new root */
    struct tree **hook[TREE_NUM_CHILDREN]; /* where to link new elements into accumulator trees */
    int d;
    int dChild;        /* direction of child */                   
    int dGrandchild;   /* direction of grandchild */

    /* we don't need to keep following this pointer, we'll just fix it at the end */
    t = *root;

    /* don't do anything to an empty tree */
    if(t == 0) { return; }

    /* ok, tree is not empty, start chopping it up */
    for(d = 0; d < TREE_NUM_CHILDREN; d++) {
        top[d] = 0;
        hook[d] = &top[d];
    }

    /* keep going until we hit the key or we would hit a null pointer in the child */
    while(t->key != target && (child = t->child[dChild = t->key < target]) != 0) {
        /* child is not null */
        grandchild = child->child[dGrandchild = child->key < target];

#ifdef DEBUG_SPLAY
        treePrint(top[0]);
        puts("---");
        treePrint(t);
        puts("---");
        treePrint(top[1]);
        puts("===");
#endif

        if(grandchild == 0 || child->key == target) {
            /* zig case; paste root into opposite-side hook */
            treeLink(hook, !dChild, t);
            t = child;
            /* we can break because we know we will hit child == 0 next */
            break;
        } else if(dChild == dGrandchild) {
            /* zig-zig case */
            /* rotate and then hook up child */
            /* grandChild becomes new root */
            treeRotate(&t, dChild);
            treeLink(hook, !dChild, child);
            t = grandchild;
        } else {
            /* zig-zag case */
            /* root goes to !dChild, child goes to dChild, grandchild goes to root */
            treeLink(hook, !dChild, t);
            treeLink(hook, dChild, child);
            t = grandchild;
        }
    }

    /* now reassemble the tree */
    /* t's children go in hooks, top nodes become t's new children */
    for(d = 0; d < TREE_NUM_CHILDREN; d++) {
        *hook[d] = t->child[d];
        t->child[d] = top[d];
    }

    /* and put t back in *root */
    *root = t;
}

/* return 1 if target is in tree, 0 otherwise */
int
treeContains(struct tree **root, int target)
{
    treeSplay(root, target);
    return *root != 0 && (*root)->key == target;
}


/* insert an element into a tree pointed to by root */
void
treeInsert(struct tree **root, int newElement)
{
    struct tree *e;
    struct tree *t;
    int d;             /* which side of e to put old root on */

    treeSplay(root, newElement);

    t = *root;

    /* skip if already present */
    if(t && t->key == newElement) { return; }

    /* otherwise split the tree */
    e = malloc(sizeof(*e));
    assert(e);

    e->key = newElement;

    if(t == 0) {
        e->child[LEFT] = e->child[RIGHT] = 0;
    } else {
        /* split tree and put e on top */
        /* we know t is closest to e, so we don't have to move anything else */
        d = (*root)->key > newElement;
        e->child[d] = t;
        e->child[!d] = t->child[!d];
        t->child[!d] = 0;
    }

    /* either way we stuff e in *root */
    *root = e;
}

/* delete target from the tree */
/* has no effect if target is not in tree */
void 
treeDelete(struct tree **root, int target)
{
    struct tree *left;
    struct tree *right;

    treeSplay(root, target);

    if(*root && (*root)->key == target) {
        /* save pointers to kids */
        left = (*root)->child[LEFT];
        right = (*root)->child[RIGHT];

        /* free the old root */
        free(*root);

        /* if left is empty, just return right */
        if(left == 0) {
            *root = right;
        } else {
            /* first splay max element in left to top */
            treeSplay(&left, INT_MAX);

            /* now paste in right subtree */
            left->child[RIGHT] = right;

            /* return left */
            *root = left;
        }
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
        printf("%d (%p)\n", root->key, (void *) root);

        treePrintIndented(root->child[RIGHT], depth+1);
    }
}

/* print the contents of a tree */
void 
treePrint(const struct tree *root)
{
    treePrintIndented(root, 0);
}


#ifdef TEST_MAIN
int
main(int argc, char **argv)
{
    int i;
    const int n = 10;
    struct tree *root = TREE_EMPTY;

    if(argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }

    for(i = 0; i < n; i++) {
        assert(!treeContains(&root, i));
        treeInsert(&root, i);
        assert(treeContains(&root, i));
        treePrint(root);
        puts("===");
    }

    /* now delete everything */
    for(i = 0; i < n; i++) {
        assert(treeContains(&root, i));
        treeDelete(&root, i);
        assert(!treeContains(&root, i));
        treePrint(root);
        puts("===");
    }

    treeDestroy(&root);

    return 0;
}
#endif