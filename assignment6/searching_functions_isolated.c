#include <stdio.h>
int linsearch (int *A, int begin_idx, int end_idx, int key)
{
    int i, len = end_idx - begin_idx + 1;
    for (i = 0; i < len; i++) {
        if (A[i] == key) {
            return i;
        }
    }
    return -1;
}

int binsearch (int *A, int p, int r, int key) 
{
    int q;
    if (p <= r) {
        q = p + (r - p)/2;
        if (A[q] > key) {
            return binsearch (A, p, q - 1, key); 
        }
        if (A[q] < key) {
            return binsearch (A, q + 1, r, key); 
        }
        if (A[q] == key) { 
            return q;                            
        }
    }
    return -1;
}
