#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>   /* for clock_gettime() */
#include <limits.h> /* for INT_MAX */
#include <stdlib.h> /* for atoi() */

void swap (int *x, int *y)
{
    int temp = *y;
    *y = *x;
    *x = temp;
}

int isort (int *arr, int begin_idx, int end_idx)
{
    int i, j, key, len;
    if (arr == NULL) {
        return 0;
    }
    len = end_idx - begin_idx + 1;
    for (i = 1; i < len; i++)
    {
        key = arr[i];
        j = i - 1;
        while ((j >= 0) && (arr[j] > key)) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
    return 1;
}

void merge (int *A, int p, int q, int r)
{
    int nl = q - p + 1;
    int nr = r - q;
    int L[nl + 1], R[nr + 1];
    int i, j, k;
    for (i = 0; i < nl; i++) {
        L[i] = A[p + i];
    }
    for (j = 0; j < nr; j++) {
        R[j] = A[q + 1 + j];
    }
    L[nl] = INT_MAX;
    R[nr] = INT_MAX;
    i = j = 0;
    for (k = p; k <= r; k++) {
        if (L[i] <= R[j])
            A[k] = L[i++];
        else 
            A[k] = R[j++];
    }
}

int msort (int *A, int p, int r) 
{
    int q;
    if (p < r && A != NULL) {
        q = (p + r) / 2;
        msort (A, p, q);
        msort (A, q + 1, r);
        merge (A, p, q, r);
        return 1;
    }
    return 0;
}

void display (int *arr, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf ("%d ", arr[i]);
    }
    putchar ('\n');
}

/* let's use function pointers to time the function: this basically 
   acts as a kind of wrapper around the sort function */
long int time_func (int (*sort)(int *, int, int), int *arr, int len)
{
    /* use clock_get because it return time in nanoseconds precision */
    struct timespec before, after;
    long int elapsed;
    long unsigned int before_ns, after_ns;

    clock_gettime (CLOCK_MONOTONIC, &before);
    sort (arr, 0, len - 1);
    clock_gettime (CLOCK_MONOTONIC, &after);

    before_ns = (before.tv_sec * 1e9) + before.tv_nsec;
    after_ns = (before.tv_sec * 1e9) + after.tv_nsec;
    elapsed = after_ns - before_ns;
   
    return elapsed;
}

int main (int argc, char **argv)
{
    int len = argc - 1;
    int arr[len];
    int brr[len]; /* C99 allows variable sized arrays */

    if (argc < 2) {
        fprintf (stderr, "%s: provide an array as argument.\n", argv[0]);
        return 1;
    }
    
    for (int i = 1; i < argc; i++) {
        arr[i - 1] = brr[i - 1] =  atoi (argv[i]);
    }

    puts ("before insetion sort: ");
    display (arr, len);

    printf ("%ld nanoseconds elapsed\n", time_func (isort, arr, len));

    puts ("after insertion sort: ");
    display (arr, len);

    printf ("\n---\n");

    puts ("before merge sort: ");
    display (brr, len);

    printf ("%ld nanoseconds elapsed\n", time_func (msort, brr, len));

    puts ("after merge sort: ");
    display (brr, len);

    return 0;
}
