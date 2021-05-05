#include <stdio.h>
#include <stdlib.h>

#define HIGHLIGHT "\033[0;31m"
#define RESET "\033[0m"

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

int pretty_print (int *arr, int len, int found_index)
{
    int i;
    if (arr == NULL) {
        printf ("[]\n");
        return 0;
    }
    if (found_index == -1) { /* if match not found, then don't print */
        printf ("[%d", arr[0]);
        for (i = 1; i < len; i++) {
            printf (", %d", arr[i]);
        }
        printf ("]\n");
    }
    else {
        putchar ('[');
        for (i = 0; i < found_index; i++) {
            printf ("%d, ", arr[i]);
        }
        printf (HIGHLIGHT "%d" RESET, arr[i++]);
        for (; i < len; i++) {
            printf (", %d", arr[i]);
        }
        printf ("]\n");
    }
    return 1;
}

int main(int argc, char **argv)
{
    int key;
    int arr[] = {1,2,3,4,5,6,78};
    int len = sizeof (arr) / sizeof (arr[0]);
    if (argc < 2) {
        key = 78;
    } else {
        key = atoi (argv[1]);
    }
    printf ("len = %d, len - 1 = %d, key = %d\n", len, len - 1, key);
    pretty_print (arr, len, binsearch (arr, 0, len - 1, key)); 
    return 0;
}
