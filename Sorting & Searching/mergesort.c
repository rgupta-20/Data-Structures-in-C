#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void merge(int n1, const int a1[], int n2, const int a2[], int out[]) {
    int i1 = 0;
    int i2 = 0;
    int iout = 0;

    while (i1 < n1 || i2 < n2) {
        if (i2 >= n2 || ((i1 < n1) && (a1[i1] < a1[i2]))) {
            out[iout++] = a1[i1++];
        }

        else {
            out[iout++] = a2[i2++];
        }
    }
}

void mergeSort(int n, const int a[], int out[]) {
    int *a1;
    int *a2;

    if (n < 2) {
        memcpy(out, a, sizeof(int) * n);
    }

    else {
        a1 = malloc(sizeof(int) * n);
        a2 = malloc(sizeof(int) * (n - n/2));

        mergeSort(n/2, a, a1);
        mergeSort(n - n/2, a + n/2, a2);

        merge(n/2, a1, n - n/2, a2, out);

        free(a1);
        free(a2);
    }
}