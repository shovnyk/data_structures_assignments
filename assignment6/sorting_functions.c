/* sorting_functions.c: 
   C program to  store all the implementations of  the different sorting
   function used in the file sort.c  This file should be included in the
   file.c using the preprocessor directive: #include sorting_functions.c
*/
#include <stdio.h>
#include <limits.h> /* INT_MAX is defined here */

/* swap two values by reference, this prevents integer overflow */
void swap (int *x, int *y)
{
    int temp = *y;
    *y = *x;
    *x = temp;
}

/* Bubble-sort:  compare and  exchange adjacent  elements over  multiple
   iterations to sort the entire array */
int bsort (int *arr, int begin_idx, int end_idx)
{
    int i, j, len; 
    if (arr == NULL) {
        return 0;
    }
    len = end_idx - begin_idx + 1;
    for (i = 0; i < (len - 1); i++) {
        for (j = (len - 1); j > i; j--) {
            if (arr[j] < arr[j - 1]) {
                swap (&arr[j], &arr[j - 1]);
            }
        } /* 2 nested loops => worst case time complexity: O(n^2) */ 
    }
    return 1;
}

/* Selection-sort: Select the smallest element and grow the sorted region */
int ssort (int *arr, int begin_idx, int end_idx)
{
    int i, j, min, len;
    if (arr == NULL) {
        return 0;
    }
    len = end_idx - begin_idx + 1;
    for (i = 0; i < (len - 1); i++) {
        min = i;
        for (j = (i + 1); j < len; j++) {
            if (arr[j] < arr[min]) {
                min = j;
            }
        }
        swap (&arr[i], &arr[min]);
    } /* 2 nested loops => worst case time complexity: O(n^2) */
    return 1;
}

/* Insertion-sort: Select  the smallest element  and insert it  into the
   sorted region */
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
    } /* 2 nested loops => worst case time complexity: O(n^2) */
    return 1;
}

/* helper function for merge sort procedure: merge two smaller subarrays
   into a larger sorted array */
void merge (int *A, int p, int q, int r)
{
    int nl = q - p + 1;
    int nr = r - q;
    int L[nl + 1], R[nr + 1]; /* C99 allows variable sied arrays */
    int i, j, k;
    for (i = 0; i < nl; i++) {
        L[i] = A[p + i];
    }
    for (j = 0; j < nr; j++) {
        R[j] = A[q + 1 + j];
    }
    L[nl] = INT_MAX; /* closest integer to "infinity" in C */
    R[nr] = INT_MAX;
    i = j = 0;
    for (k = p; k <= r; k++) {
        if (L[i] <= R[j])
            A[k] = L[i++];
        else 
            A[k] = R[j++];
    } /* requires single traversal so time complexity: O(n) */
}

/* Merge-sort: divide  an array  into smaller  subarrays and  merge them
   recursively in sorted order to obtain the final sorted array */
int msort (int *A, int p, int r) 
{
    int q;
    if (p < r && A != NULL) {
        q = p + ((r - p)/2);    /* divide: the array into subarrays */
        msort (A, p, q);        /* conquer: sort the smaller subarrays */
        msort (A, q + 1, r);
        merge (A, p, q, r);     /* combine: merge into single solution */
        return 1;
    } /* from recurrence relation, the time complexity is O(nlogn) */
    return 0;
}

/* helper  function for  quicksort procedure:  partition a  larger array
   into smaller  subarrays with a pivot  such that every element  in the
   left half is  smaller thant the pivot and every  element in the right
   half is larger.  */
int partition (int *A, int p, int r) 
{
    int i, j, pivot;
    pivot = A[r];
    i = (p - 1);
    for (j = p; j < r; j++) {
        if (A[j] <= pivot) {
            /* ++i will not work because of race condition */
            i++;       
            swap (&A[i], &A[j]);
        } 
    }/* time complexity: O(n) */
    swap (&A[i + 1], &A[r]);
    return (i + 1);
}

/* Quick-sort: partion a larger array into smaller subarrays and keep 
   on doing so until the entire array is sorted */
int my_qsort (int *A, int p, int r) 
{
    int q;
    if (p < r && A != NULL) {
        q = partition (A, p, r); /* divide: partition into two subarrays */
        my_qsort (A, p, q - 1);  /* conquer: sort the two subarrays */
        my_qsort (A, q + 1, r);  
        return 1;                /* combine: not necessary */
    } 
    /* from recurrence relation, avg case time complexity: O(nlogn) */
    return 0;
}

#ifndef BUFF 
#define BUFF 1024
#endif

/* a structure to encapsulate the function signature with its name */
struct algorithm {
    int (*sort)(int *, int, int);
    char name[BUFF];
} algo[] = {   
    {bsort, "Bubble Sort"},
    {ssort, "Selection Sort"},
    {isort, "Insertion Sort"},
    {msort, "Merge Sort"},
    {my_qsort, "Quick Sort"},
    {NULL, "\0"}
}; /* global variable; can be accessed by any other file */
