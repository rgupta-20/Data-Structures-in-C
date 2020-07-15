#ifndef _OBJECT_H
#define _OBJECT_H

// truncated version of an object
// real object will have more fields after methods
// we expose this for implementers
struct object {
    const struct methods *methods;
};

typedef struct object Object;

struct methods {
    Object *(*clone)(const Object *self);
    void (*print)(const Object *self);
    void (*destroy)(Object *self);
};

#endif