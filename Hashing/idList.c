#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "idList.h"

#define OVERHEAD (1.1)
#define NULL_ID (-1)

struct idList {
    int size;
    int ids[1];
};

IDList IDListCreate(int n, int unsortedList[]) {
    IDList list;
    int size;
    int i;
    int probe;

    size = (int) (n * OVERHEAD + 1);

    list = malloc(sizeof(*list) + sizeof(int) * (size - 1));
    if (list == 0) return 0;

    list->size = size;

    for(i = 0; i < size; i++) {
        list->ids[i] = NULL_ID;
    }
    
    for(i = 0; i < n; i++) {
        assert(unsortedIdList[i] >= MIN_ID);
        assert(unsortedIdList[i] <= MAX_ID);

        for (probe = unsortedList[i] % list->size; list->ids[probe] != NULL_ID; probe = (probe + 1) % list->size);
        
        assert(list->ids[probe] == NULL_ID);

        list->ids[probe] = unsortedList[i];
        
    }   
}

void IDListDestroy(IDList list) {
    free(list);
}

int idListContains(int id, IDList list) {
    int probe;

    for (probe = id % list->size; list->ids[probe] != NULL_ID; probe = (probe + 1) % size) {
        if (list->ids[probe] == id) return 1;
    }

    return 0;
}