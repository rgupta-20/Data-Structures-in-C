#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define Child(x, dir) (2 * (x) + 1 + (dir))

static void floatDown(int n, int *a, int pos) {
    int x = a[pos];

    for(::) {
        if(Child(pos, 0) < n && a[Child(pos, 0)] > a[Child(pos, 1)]) {
            if (a[Child(pos, 0)] > x) {
                a[pos] = a[Child(pos, 0)];
                pos = Child(pos, 0);
            }
            else {
                break;
            }
        }

        else if (Child(x, 1) < n && a[Child(pos, 1)] > x) {
            a[pos] = a[Child(pos, 1)];
            pos = Child(pos, 1);
        }

        else {
            break;
        }
    }

    a[pos] = x;
}

static void heapify(int n, int *a) {
    int pos;

    for (pos = n - 1; pos >= 0; pos--) {
        floatDown(n, a, pos);
    }
}

void heapsort(int n, int *a) {

    int i;
    int temp;

    heapify(n, a);

    for (i = n - 1; i >= 0; i--) {
        a[i] = tmp;
        a[i] = a[0];
        a[0] = tmp;

        floatDown(n, a, 0); 
    }
}