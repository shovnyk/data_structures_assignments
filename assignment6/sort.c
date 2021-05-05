/* sort.c: 
   C program  to interactively sort  multiple arrays read as  input from
   the user  using an algorithm also  chosen by the user.  The following
   sorting algorithms are demonstrated:
       1. Bubble-sort
       2. Selection-sort
       3. Insertion-sort
       4. Merge-sort
       5. Quick-sort
   Additionally, the  user is also  displayed the running time  of these
   different  algorithms for  comparision at  different input  sizes and
   levels of initial sortedness. 
*/
#define _GNU_SOURCE /* since clock_getttime is a GNU extension */
#include <stdio.h> 
#include <stdlib.h> /* malloc, realloc */
#include <string.h> /* strtok, strrchr */
#include <time.h>   /* clock_getttime */

#include "sorting_functions.c" /* include file containing all sorting
                                  algorithms into this file */

/* display an array passed with its length */
int display (int *arr, int len)
{
    int i;
    if (len < 0) {
        fprintf (stderr, "!Array length cannot be negative\n");
        return 0;
    }
    if (len == 0) {
        printf ("[]\n");
    }
    else {
        printf ("[%d", arr[0]);
        for (i = 1; i < len; i++) {
            printf (", %d", arr[i]);
        }
        printf ("]\n");
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

/* wrapper function around the sort function to both sort the function
   and also time the code */
#define NANOSECONDS_PER_SECOND 1e9
long time_sort (int (*sort)(int *, int, int), int *arr, int len)
{
    long elapsed;
    struct timespec diff, before, after;

    clock_gettime (CLOCK_MONOTONIC, &before);
    sort (arr, 0, len - 1);
    clock_gettime (CLOCK_MONOTONIC, &after);

    /* gettime can cause wrap around of the nanosecond parameter 
       leading to negative values, this is how we can avoid it: */
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
#define BUFF 1024
#define PROMPT ">> "
#define MENU "Commands\n"\
             "--------\n"\
             "s\tSort an array using an algorithm of your choice\n"\
             "c\tCompare running time of different algorithms\n"\
             "h\tDisplay this help menu\n"\
             "q\tQuit\n\n"
#define INVALID_INPUT "!Invalid Input\n"
#define IOE "!IO Error\n"
#define INIT_CAP 100

/* procedure to parse user input buffer and construct an array: collects
   tokens from the  input buffer, converts into integer  and stores into
   array by dynamically adjusting size and returns effective length */
int *getarr (char *buff, char *delim, int *len)
{
    char *token;
    size_t capacity, i;
    int elem, *arr;
    if (strrchr (buff, '\n') == NULL) {  /* search from end of string */
        /* no newline found  => user input too long */
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
        if (i == capacity) { /* if capacity is being exceeded, double it */
            capacity *= 2; 
            arr = (int *) realloc (arr, sizeof (int) * capacity);
            if (arr == NULL) { 
                perror ("realloc"); 
                break; /* return whatever has been converted */
            }
        }
        arr[i++] = elem;
        token = strtok (NULL, delim);
    }
    *len = i;
    return arr;
}

/* procedure to imitate the standard getchar() but take care of trailing
   newlines for reading user input safely */
int my_getchar (char *buffer, size_t bsiz)
{
    if (fgets (buffer, bsiz, stdin) == NULL) {
        return EOF;
    }
    return buffer[0];
}

/* main routine: interactive program */
int main (void) 
{
    char input[BUFF] = {'\0'}; /* initiliaze all bytes to zero to check 
                                  later if a newline was entered */
    long int elapsed;
    int *arr = NULL, *copy = NULL, len, choice, i;

    extern struct algorithm algo[];  /* external variable declared in
                                        sorting_functions.c */
    printf ("Implementation of various sorting algorithms:\n"
            "bubble-sort, selection-sort, insertion-sort, "
            "merge-sort and quick-sort.\n");
    printf (MENU);
    while (1) {
        printf (PROMPT);
        choice = my_getchar (input, BUFF);
        switch (choice)
        {
            case 's':
                printf ("Array (comma/space delimited) = ");
                if (fgets (input, BUFF, stdin) == NULL) {
                    fprintf (stderr, IOE);
                    continue;
                }
                arr = getarr (input, ", \n", &len);
                if (arr == NULL) 
                    continue;
                printf ("What algorithm?\n"
                        "b\tBubble sort\n"
                        "s\tSelection sort\n"
                        "i\tInsertion sort\n"
                        "m\tMerge sort\n"
                        "q\tQuick sort\n" PROMPT);
                choice = my_getchar (input, BUFF); 
                printf ("\nBefore sorting: ");
                display (arr, len);
                printf ("After sorting: ");
                switch (choice) {
                    case 'b':
                        elapsed = time_sort (bsort, arr, len);
                        display (arr, len);
                        printf ("Time elapsed (ns): %ld\n", elapsed);
                        break;

                    case 's':
                        elapsed = time_sort (ssort, arr, len);
                        display (arr, len);
                        printf ("Time elapsed (ns): %ld\n", elapsed);
                        break;

                    case 'i':
                        elapsed = time_sort (isort, arr, len);
                        display (arr, len);
                        printf ("Time elapsed (ns): %ld\n", elapsed);
                        break;

                    case 'm':
                        elapsed = time_sort (msort, arr, len);
                        display (arr, len);
                        printf ("Time elapsed (ns): %ld\n", elapsed);
                        break;

                    case 'q':
                        elapsed = time_sort (my_qsort, arr, len);
                        display (arr, len);
                        printf ("Time elapsed (ns): %ld\n", elapsed);
                        break;

                    case EOF:
                        fprintf (stderr, IOE);
                        break;

                    default: 
                        fprintf (stderr, INVALID_INPUT);
                }
                free (arr); /* deallocate to preven memory leaks */
                putchar ('\n');
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
                copy = arrcpy (arr, len);
                printf ("\n   %-14s  %-16s  %s\n\n",
                        "Algorithm", "Time elapsed (ns)", "Sorted array");
                for (i = 0; i < 5; i++) {
                    elapsed = time_sort (algo[i].sort, arr, len);
                    printf ("%d  %-14s  %-17ld ", i+1, algo[i].name, elapsed);
                    display (arr, len);
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
