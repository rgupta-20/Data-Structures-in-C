typedef struct dict *Dict;

struct dictContentsOperations {
    unsigned long (*hash) (const void *datum, void* arg);

    int (*equal) (const void * datum1, const void * datum2, void * arg);

    void *(*copy) (const void * datum, void * arg);

    void (*delete) (void *datum, void *arg);

    void * arg;
}

Dict dictCreate(struct dictContentsOperations keyOps, struct dictContentsOperations valueOps);

void dictDestroy(Dict d);

void dictSet(Dict d, const void *key, const void *value);

const void *dictGet(Dict d, const void *key);

struct dictContentsOperations DictIntOps;

struct dictContentsOperations DictStringOps;

struct dictContentsOperations dictMemOps(int size);