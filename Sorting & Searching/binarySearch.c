#include <stddef.h>

#include "binarySearch.h"

int binarySearch(int target, const char *a, size_t length) {
    size_t mid = length / 2;

    if (length == 0) {
        return 0;
    }

    else if(a[mid] == target) {
        return 1;
    }

    else if(target < a[mid]) {
        return binarySearch(target, a, mid);
    }

    else {
        return binarySearch(target, a + mid + 1, length - mid - 1);
    }

}

int
binarySearchIterative(int target, const int *a, size_t length)
{
    size_t index;

    /* direct translation of recursive version */
    /* hence the weird organization of the loop */
    for(;;) {
        index = length/2;

        if(length == 0) {
            return 0;
        } else if(target == a[index]) {
            return 1;
        } else if(target < a[index]) {
            length = index;
        } else {
            a = a + index + 1;
            length = length - (index + 1);
        }
    }
}