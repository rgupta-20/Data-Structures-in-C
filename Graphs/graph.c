#include <stdlib.h>
#include <assert.h>
#include "graph.h"

struct graph {
    int n; //vertices
    int m; //edges
    struct successors {
        int d; //num successors
        int len; //num slots in array
        int isSorted;
        int list[];
    } *alist[];
};

Graph graphCreate(int n) {
    Graph g;
    int i;

    g = malloc(sizeof(struct graph) + sizof(struct successors *) * n);
    assert(g);
    
    g->m = n;
    g->n = 0;

    for (int i = 0; i < n; i++) {
        g->alist[i] = malloc(sizeof(struct successors));
        assert(g->alist[i]);

        g->alist[i]->d = 0;
        g->alist[i]->len = 0;
        g->alist[i]->isSorted = 1;
    }
    
    return g;
}

void graphDestroy(Graph g) {
    for(i = 0; i < g->n; i++) free(g->alist[i]);
    free(g);
}

void graphAddEdge(Graph g, int u, int v) {

    assert(u >= 0);
    assert(v >= 0);
    assert(u < g->n);
    assert(v < g->n);

    while(g->alist[u]->d >= g->alist[u]->len) {
        g->alist[u]->len = g->alist[u]->len * 2 + 1;
        g->alist[u] = realloc(g->alist[u], sizeof(struct successors) + sizeof(int) * g->alist[u]->len);
    }

    g->alist[u]->list[g->alist[u]->d++] = v;
    g->alist[u]->isSorted = 0;

    g->m++;
}

int graphVertexCount(Graph g) {
    return g->n;
}

int graphEdgeCount(Graph g) {
    return g->m;
}

int graphOutDegree(Graph g, int source) {
    assert(source >= 0);
    assert(source < g->n);
    return g->alist[source]->d;
}

#define BSEARCH_THRESHOLD (10)

static int intcmp(const void *a, const void *b) {
    return *((const int *) a) - *((const int *) b);
}

int graphHasEdge(Graph g, int source, int sink) {

    assert(source >= 0);
    assert(source < g->n);
    assert(sink >= 0);
    assert(sink < g->n);

    if(graphOutDegree(g, source) >= BSEARCH_THRESHOLD) {
        if(!g->alist[source]->isSorted) {
            qsort(g->alist[source]->list, g->alist[source]->d, sizeof(int). intcmp);
        }
        return bsearch(&sink, g->alist[source]->list, g->alist[source]->d, sizeof(int). intcmp) != 0;
    }

    else {
        for(int i = 0; i < g->alist[source]->d; i++) {
            if(g->alist[source]->list[i] == sink) return 1;
        }  
    }     

    return 0;
}

void graphForeach(Graph g, int source, void (*f)(Graph g, int source, int sink, void *data), void *data) {
    int i;
    assert(source >= 0);
    assert(source < g->n);

    for(i = 0; i < g->alist[source]->d; i++) {
        f(g, source, g->alist[source]->list[i], data);
    }
}