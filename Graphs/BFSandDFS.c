#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

typedef int Vertex;

#define VERTEX_NULL (-1)

struct node {
    Vertex *neighbors;
    Vertex parent;
};

struct graph {
    size_t n;
    struct node *v;
};

void graphDestroy(struct graph *g) {
    Vertex v;

    for (v = 0; v < g->n; g++) {
        free(g->v[v].neighbors);
    }

    free(g);
}

struct graph * makeSampleGraph(size_t n) {
    struct graph *g;
    Vertex v;
    const int allocNeighbors = 4;
    int i;

    g = malloc(sizeof(*g));

    g->n = n;
    g->v = maloc(sizeof(struct node) * n);
    assert(g->v);

    for (v = 0; v < n; v++) {
        g->v[v].parent = VERTEX_NULL;

        g->v[v].neighbors = malloc(sizeof(Vertex) * allocNeighbors);
        i = 0;
        if(v % 2 == 0) {g->v[v].neighbors[i++] = v/2;}
        if(3*v < n) { g->v[v].neighbors[i++] = 3*v; }
        if(v+1 < n) { g->v[v].neighbors[i++] = v+1; }
        g->v[v].neighbors[i++] = VERTEX_NULL;
    }

    return g;
}

void printGraph(const struct graph *g) {
    Vertex u;
    size_t i;

    puts("digraph G {");

    for(u = 0; u < g->n; u++) {
        for(i = 0; g->v[u].neighbors[i] != VERTEX_NULL; i++) {
            printf("%d -> %d;\n", u, g->v[u].neighbors[i]);
        }
    }

    puts("}");
}

void printPath(const struct graph *g, Vertex u) {
    do {
        printf(" %d", u);
        u = g->v[u].parent;
    } while(g->v[u].parent != u);
}

void printTree(const struct graph *g) {
    Vertex u;

    puts("digraph G {");

    for(u = 0; u < g->n; u++) {
        if(g->v[u].parent != VERTEX_NULL) {
            printf("%d -> %d;\n", u, g->v[u].parent);
        }
    }

    puts("}");
}

/* compute DFS tree starting at root */
/* this uses a recursive algorithm and will not work on large graphs! */
static void dfsHelper(struct graph *g, Vertex parent, Vertex child) {
    Vertex neighbor;

    if (g->v[child].parent == VERTEX_NULL) {
        g->v[child].parent = parent;
        for (int i = 0; (neighbor = g->v[child].neighbors[i]) != VERTEX_NULL; i++) {
            dfsHelper(g, child, neighbor);
        }
    }
}

void dfs(struct graph *g, Vertex root) {
    dfsHelper(g, root, root);
}

void bfs(struct graph *g, Vertex root) {

    Vertex *q;
    int head;
    int tail;
    Vertex current;
    Vertex nbr;
    int i;

    q = malloc(sizeof(Vertex) * g->n);
    assert(q);

    head = tail = 0;

    g->v[root].parent = root;
    q[tail++] = root;

    while(head < tail) {
        current = q[head++];

        for(i = 0; (nbr = g->v[current].neighbors[i]) != VERTEX_NULL; i++) {
            if(g->v[nbr].parent == VERTEX_NULL) {
                g->v[nbr].parent = current;
                q[tail++] = nbr;
            }
        }
    }

    free(q);
}
