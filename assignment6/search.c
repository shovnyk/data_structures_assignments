/* search.c:
   C program to implement and  compare linear and binary sort algorithms
   using a singly linked list implementation.

   Linear search:  a simple searching  algorithm where we  compare each 
   element of the list to the item being searched for.                  

   Binary search: looking  at the middle of the list  for the item. This
   only works for sorted lists and is much faster.
*/
#define _GNU_SOURCE /* timing functions are GNU extensions */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <time.h>

/* ansi escape color codes */
#define  ANSI_RED    "\033[01;31m"
#define  ANSI_GREEN  "\033[01;32m"
#define  ANSI_BLUE   "\033[01;34m"
#define  HIGHLIGHT   ANSI_RED
#define  ANSI_RESET  "\033[0m"

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
        printf (HIGHLIGHT "%d" ANSI_RESET, arr[i++]);
        for (; i < len; i++) {
            printf (", %d", arr[i]);
        }
        printf ("]\n");
    }
    return 1;
}

/* simple linear search algorithm (stdlib already declares a linear 
   search routine by the name of lsearch */
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

/* binary search algorithm (stdlib already defines a binary search 
   routine by the name of bsearch */ 
int binsearch (int *A, int p, int r, int key) 
{
    int q;
    if (p <= r) {
        q = p + (r - p)/2;
        if (A[q] > key) {
            return binsearch (A, p, q - 1, key); /* search in lower half */
        }
        if (A[q] < key) {
            return binsearch (A, q + 1, r, key); /* search in upper half */
        }
        if (A[q] == key) { 
            return q;                            /* match found, return */
        }
    }
    return -1;
}

/* mergesort for sorting the array before binary search can be performed */
void merge (int *A, int p, int q, int r) /* helper function for msort */
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
int sort (int *A, int p, int r)  /* standard implementation of merge sort */
{
    int q;
    if (p < r && A != NULL) {
        q = p + ((r - p)/2);
        sort (A, p, q);    
        sort (A, q + 1, r);
        merge (A, p, q, r); 
        return 1;
    }
    return 0;
}

/* run and time the code using function pointers and clock_gettime */
#define NANOSECONDS_PER_SECOND 1e9
long int time_search
(int (*search)(int *, int, int, int), int *arr, int len, int key, int *retval)
{
    long elapsed;
    struct timespec diff, before, after;

    clock_gettime (CLOCK_MONOTONIC, &before);
    *retval = search (arr, 0, len - 1, key);
    clock_gettime (CLOCK_MONOTONIC, &after);

    if ((after.tv_nsec - before.tv_nsec) < 0) {
        diff.tv_sec = after.tv_sec - before.tv_sec - 1;
        diff.tv_nsec = NANOSECONDS_PER_SECOND + after.tv_nsec - before.tv_nsec;
    } 
    else {
        diff.tv_sec = after.tv_sec - before.tv_sec;
        diff.tv_nsec = after.tv_nsec - before.tv_nsec;
    }
    elapsed = (diff.tv_sec * NANOSECONDS_PER_SECOND) + diff.tv_nsec;
    return elapsed;
}
                          /* end of implementation */
#define BUFF 1024
#define MENU "Commands:\n"\
             "---------\n"\
             "s\tSearch for a key in an array\n"\
             "c\tCompare performances of linear and binary search\n"\
             "h\tDisplay this help menu\n"\
             "q\tQuit\n\n"
#define PROMPT ">> "
#define INVALID_INPUT "?\n"
#define IOE "!IO Error\n"
#define INIT_CAP 100

/* procedure to convert a user input buffer into an array if possible */
int *getarr (char *buff, char *delim, int *len)
{
    char *token;
    size_t capacity, i;
    int elem, *arr;
    if (strrchr (buff, '\n') == NULL) {
        fprintf (stderr, "Warning: input too long, exceeds %d bytes", BUFF);
    }
    capacity = INIT_CAP;
    arr = (int *) malloc (sizeof (int) * capacity);
    if (arr == NULL) {
        perror ("malloc");
        *len = -1;
        return NULL;
    }
    i = 0;
    token = strtok (buff, delim); 
    while ((token != NULL)) {
        if (sscanf (token, "%d", &elem) != 1) {
            fprintf (stderr, INVALID_INPUT); 
            *len = -1;
            return NULL;
        }
        if (i == capacity) { 
            capacity *= 2; 
            arr = (int *) realloc (arr, sizeof (int) * capacity);
            if (arr == NULL) { 
                perror ("realloc"); 
                break;
            }
        }
        arr[i++] = elem;
        token = strtok (NULL, delim);
    }
    *len = i;
    return arr;
}

/* get character from standard input stream safely */
int my_getchar (char *buffer, size_t bsiz)
{
    if (fgets (buffer, bsiz, stdin) == NULL) {
        return EOF;
    }
    return buffer[0];
}

/* procedure to read and convert user input into a valid integer and 
   store it inside variable supplied by caller */
int getnum (char *buffer, size_t siz, int *var)
{
    if (fgets (buffer, siz, stdin) == NULL) {
        fprintf (stderr, IOE);
        return 0;
    }
    else if (sscanf (buffer, "%d", var) == 0) {
        fprintf (stderr, INVALID_INPUT);
        return 0;
    }
    return 1;
}

/* copy the contents of the one array into another */
int *arrcpy (int *arr, int len)
{
    int i;
    int *copy = (int *) malloc (sizeof (int) * len);
    if (copy == NULL) {
        return NULL;
    }
    for (i = 0; i < len; i++) {
        copy[i] = arr[i];
    }
    return copy; 
}

int main (void)
{
    long int elapsed;
    char input[BUFF] = {'\0'}, choice;
    int *arr = NULL, *copy = NULL, len, key, index, i;

    int (*search)(int *, int, int, int); /* function pointer */
    struct algorithm {
        int (*search)(int *, int, int, int);
        char name[BUFF];
        int need_to_sort;
    }
    a[] = {
        {linsearch, "Linear Search", 0},
        {binsearch, "Binary Search", 1}
    };

    printf ("Implementation of linear and binary search algorithms:\n");
    printf (MENU);
    while (1) {
        printf (PROMPT);
        choice = my_getchar (input, BUFF);  
        switch (choice) {
            case 's':
                printf ("Array (comma/space delimited) = ");
                if (fgets (input, BUFF, stdin) == NULL) {
                    fprintf (stderr, IOE);
                    continue;
                }
                arr = getarr (input, ", \n", &len);
                if (arr == NULL) 
                    continue;
                printf ("Key = ");
                if (!getnum (input, BUFF, &key)) 
                    continue;
                printf ("What algorithm?\n"
                        "b\tBinary search\n"
                        "l\tLinear search\n" PROMPT);
                choice = my_getchar (input, BUFF);
                switch (choice) {
                    case 'b':
                        search = binsearch;
                        sort (arr, 0, len - 1);
                        printf ("!Array sorted prior to searching.\n");
                        break;

                    case 'l':
                        search = linsearch;
                        break;

                    default:
                        fprintf (stderr, INVALID_INPUT);
                        break;
                } 
                index = search (arr, 0, len - 1, key);
                if (index == -1) {
                    printf ("Key='%d' NOT FOUND in array: ", key);
                }
                else {
                    printf ("Key='%d' FOUND at index=%d: ", key, index);
                }
                pretty_print (arr, len, index);
                free (arr);
                break;
                
            case 'c':
                printf ("Array (comma/space delimited) = ");
                if (fgets (input, BUFF, stdin) == NULL) {
                    fprintf (stderr, IOE);
                    continue;
                }
                arr = getarr (input, ", \n", &len);
                if (arr == NULL) 
                    continue;
                printf ("Key = ");
                if (!getnum (input, BUFF, &key)) 
                    continue;
                copy = arrcpy (arr, len);
                printf ("\n   %-13s  %-16s  %s\n\n",
                        "Algorithm", "Time elapsed (ns)", "Index");
                for (i = 0; i < 2; i++) {
                    if (a[i].need_to_sort) {
                        sort (arr, 0, len - 1);
                    }
                    elapsed = time_search (a[i].search, arr, len, key, &index);
                    printf ("%d  %-14s  %-17ld ", i+1, a[i].name, elapsed);
                    printf ("%d%s ", index, 
                            (index == -1) ? "(not found):" : ":");
                    pretty_print (arr, len, index);
                    free (arr);
                    arr = arrcpy (copy, len);
                }
                putchar ('\n');
                free (arr);
                free (copy);
                break;

            case 'q':
                printf ("!Exiting process.\n");
                exit (0);
                break;

            case EOF:
                fprintf (stderr, IOE);
                break;

            default:
                fprintf (stderr, INVALID_INPUT);
            case 'h':
                printf (MENU);
                break;
        }
    }
}
