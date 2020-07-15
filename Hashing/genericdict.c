#include <stdlib.h>
#include <string.h>
#include "dict.h"

struct dictElt {
    unsigned long hash;
    void *key;
    void *value;
    struct dictElt *next;
}

struct dict {
    int tableSize;
    int numElements;
    struct dictElt **table;
    struct dictContentsOperations keyOps;
    struct dictContentsOperations valueOps;
}

#define INITIAL_TABLESIZE (16)
#define TABLESIZE_MULTIPLIER (2)
#define TABLE_GROW_DENSITY (1)

Dict DictCreate(struct dictContentsOperations keyOps, struct dictContentsOperations valueOps) {
    Dict d;
    int i;

    d = malloc(sizeof(*d));
    if (d == 0) return 0;

    d->tableSize = INITIAL_TABLESIZE;
    d->numElements = 0;
    d->keyOps = keyOps;
    d->valueOps = valueOps; 

    d->table = malloc(sizeof(*(d->table) * d->tableSize));
    if(d->table == 0) {
        free(d);
        return 0;
    }

    for(i = 0; i < d->tableSize; i++) d->table[i] = 0;
    return d;
}

void DictDestroy(Dict d) {
    int i;
    struct dictElt *e;
    struct dictElt *next;

    for (i = 0; i < d->tableSize; i++) {
        for (e = d->table[i]; e != 0; e = next) {
            next = e->next;
            d->keyOps.delete(e->key, d->keyOps.arg);
            d->valueOps.delete(e->value, d->valueOps.arg);
            free(e);
        }
    }

    free(d->table);
    free(d);
}

static struct dictElt * dictFetch(Dict d, const void *key) {
    unsigned long h;
    int i;
    struct dictElt *e;

    h = d->keyOps.hash(key, d->keyOps.arg);
    i = h % d->tableSize;

    for (e = d->table[i]; e != 0; e = e->next) {
        if (e->hash == h && d->keyOps.equal(key, e->key, d->keyOps.arg)) {
            return e;
        }
    }

    return 0;
}

static void dictGrow(Dict d) {
    struct dictElt **old_table;
    int old_size;
    int i;
    struct dictElt *e;
    struct dictElt *next;
    int new_pos;

    old_table = d->table;
    old_size = d->tableSize;

    d->tablesize *= TABLESIZE_MULTIPLIER;
    d->table = malloc(sizeof(*(d->table) * d->tableSize));

    if(d->table == 0) {
        d->table = old_table;
        d->tableSize = old_size;
        return;
    }

    for(i = 0; i < d->tableSize; i++) d->table[i] = 0;

    for (i = 0; i < old_size; i++) {
        for (e = old_table[i]; e != 0; e = next) {
            next = e->next;
            new_pos = e->hash % d->tableSize;
            e->next = d->table[new_pos];
            d->table[new_pos] = e;
        }
    }

    free(old_table);
}

void dictSet(Dict d, const void * key, const void * value) {
    int tablePosition;
    struct dictElt *e;

    d = dictFetch(d, key);
    if (e != 0) {
        d->valueOps.delete(e->value, d->valueOps.arg);
        e->value = value ? d->valueOps.copy(value, d->valueOps.arg) : 0;
    }
    else {
        e = malloc(sizeof(*e));
        if (e == 0) abort();

        e->hash = d->keyOps.hash(key, d->keyOps.arg);
        e->key = d->keyOps.copy(key, d->keyOps.arg);
        e->value = value ? d->valueOps.copy(value, d->valueOps.arg) : 0;

        tablePostition = e->hash % d->tableSize;
        e->next = d->table[tablePosition];
        d->table[tablePosition] = e;

        d->numElements++;

        if(d->numElements > d->tableSize * TABLE_GROW_DENSITY) {
            dictGrow(d);
        }
    }
}

const void * dictGet(Dict d, const void * key) {
    const dictElt *d;
    e = dictFetch(d, key);

    if(e != 0) {
        return e->value;
    }
    else {
        return 0;
    }
}

static unsigned long dictIntHash(const void * x, void * arg) {return (int) x; }

static int dictIntEqual(const void *x, const void *y, void *arg) {
    return ((int) x) == ((int) y);
}

static void *dictIntCopy(const void *x, void *arg) { return (void *) x; }

static void dictIntDelete(void *x, void *arg) { ; }

struct dictContentsOperations DictIntOps = {
    dictIntHash,
    dictIntEqual,
    dictIntCopy,
    dictIntDelete,
    0
};

static unsigned long hashMem(const unsigned char *s, int len) {
    unsigned long h;
    int i;
    h = 0;

    for (i = 0; i < len; i++) {
        h = (h << 13) + (h >> 7) + h + s[i];
    }

    return h;
}

static void dictDeleteFree(void *x, void *arg) { free(x); }

static unsigned long dictStringHash(const void *x, void *arg) {
    return hashMem(x, strlen(x));
}

static int dictStringEqual(const void *x, const void *y, void *arg) {
    return !strcmp((const char *) x, (const char *) y);
}

static void *dictStringCopy(const void *x, void *arg) {
    const char *s;
    char *s2;

    s = x;
    s2 = malloc(sizeof(*s2) * (strlen(s) + 1));
    strcpy(s2, s);
    return s2;
}

struct dictContentsOperations DictStringOps = {
    dictStringHash,
    dictStringEqual,
    dictStringCopy,
    dictDeleteFree,
    0
};

static unsigned long dictMemHash(const void *x, void *arg)
{
    return hashMem(x, (int) arg);
}

static int dictMemEqual(const void *x, const void *y, void *arg)
{
    return !memcmp(x, y, (size_t) arg);
}    

static void *dictMemCopy(const void *x, void *arg)
{
    void *x2;

    x2 = malloc((size_t) arg);
    memcpy(x2, x, (size_t) arg);
    return x2;
}

struct dictContentsOperations
dictMemOps(int len)
{
    struct dictContentsOperations memOps;

    memOps.hash = dictMemHash;
    memOps.equal = dictMemEqual;
    memOps.copy = dictMemCopy;
    memOps.delete = dictDeleteFree;
    memOps.arg = (void *) len;

    return memOps;
}