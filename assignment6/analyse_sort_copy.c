/* analyse_sort.c:
   C program to perform a best case, worst case and avergae case analysis
   of different sorting algorithms for varying input sizes controlled by
   3 parameters: start, stop and step.
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sorting_functions.c" /* implementations of various sorting 
                                  algorithms */
typedef struct array {
    int count;
    int *ptr;
} array_t ;

/* return a random integer in the inclusive range [lower,upper] */
int randint (int lower, int upper)
{
    static int called_before = 0;
    int n;
    if (!called_before) { /* set the seed only once */
        called_before = 1;
        srand (time (NULL));
    }
    n = rand (); 
    /* modulo operation returns one less than divisor so add 1 */
    return lower + n % (upper - lower + 1);
}

/* copy contents from the second half of the array into the first */
void arr_restore (int *arr, int len)
{
    for (int i = 0; i < len; i++) 
        arr[i] = arr[i + len];

}

/* call sorting function and measure its running time */
#define NANOSECONDS_PER_SECOND 1e9
long time_sort (int (*sort)(int *, int, int), int *arr, int len)
{
    long elapsed;
    struct timespec temp, start, end;
    clock_gettime (CLOCK_MONOTONIC, &start);
    sort (arr, 0, len - 1);
    clock_gettime (CLOCK_MONOTONIC, &end);
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = NANOSECONDS_PER_SECOND + end.tv_nsec - start.tv_nsec;
    } 
    else {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    elapsed = (temp.tv_sec * NANOSECONDS_PER_SECOND) + temp.tv_nsec;
    return elapsed;
}

/* allocate memory using malloc; if it fails exit with error code 2
   this should be avoided in a routine which writes to a file otherwise
   contents will be lost */
void *xmalloc (size_t siz)
{
    void *mem = malloc (siz);
    if (mem == NULL) 
        exit (2);
    return mem;
}

/* the characteristic O(n), O(n^2) and O(nlog n) running times are
   only observablle if the size of the input is large enough */
#define DEF_START_SIZE 10
#define DEF_STOP_SIZE 30000
#define DEF_STEP_SIZE 1000

/* print the timed data from sorting inputs for all cases */
int main (int argc, char **argv)
{
    int i, j, k, N;
    int start, stop, step;
    long ns_elapsed_b, ns_elapsed_w, ns_elapsed_a;

    extern struct algorithm algo[]; /* collection of sort function pointers */

    if (argc == 1) {
        start = DEF_START_SIZE;
        stop = DEF_STOP_SIZE;
        step = DEF_STEP_SIZE;
    } 
    else if (argc == 4) {
        start = atoi (argv[1]);
        stop = atoi (argv[2]);
        step = atoi (argv[3]);
    }
    else {
        fprintf (stderr, "Usage: %s [START STOP STEP]\n", argv[0]);
        exit (1);
    }
    N = (stop - start) / step + 1;

    array_t *best = (array_t *) xmalloc (sizeof (array_t) * N);
    array_t *worst = (array_t *) xmalloc (sizeof (array_t) * N);
    array_t *average = (array_t *) xmalloc (sizeof (array_t) * N);

    /* generate inputs */
    for (i = 0, j = start; i < N; i++, j += step) {
        best[i].count = j;
        worst[i].count = j;
        average[i].count = j;

        best[i].ptr = (int *) xmalloc (sizeof (int) * j);

        /* for worst and average cases we allocate double the necessary
           size N and make a copy of all the elements at an offset of N 
           For example: [3, 77,..., 9,  3, 77,..., 9] 
                         0   1 ...  N-1 N  N+1 ... 2N-1
        */
        worst[i].ptr = (int *) xmalloc (2 * sizeof (int) * j);
        average[i].ptr = (int *) xmalloc (2 * sizeof (int) * j);

        for (k = 0; k < j; k++) {
            best[i].ptr[k] = k + 1;
            worst[i].ptr[k] 
                = worst[i].ptr[k + worst[i].count] = j - k;
            average[i].ptr[k] 
                = average[i].ptr[k + average[i].count] = randint (1, j);
        }
    }

    /* print the data */
    for (i = 0; i < 5; i++) {
        /* for each (1) sorting algorithm */
            /* calculate running time for all (2) inputs cases for
               (3) best, worst and average cases */
        printf ("Analysis of %s:\n", algo[i].name);
        printf ("%11s %11s %11s %11s\n",
                "Inputs-size", "Best-case", "Worst-case", "Average-case");
        for (j = 0; j < N; j++) {
            ns_elapsed_b 
                = time_sort (algo[i].sort, best[j].ptr, best[j].count);

            /* For worst and average cases, we need to restore array to
               original state so the next sorting algorithm sees it identical
               to the current one */
            ns_elapsed_w 
                = time_sort (algo[i].sort, worst[j].ptr, worst[j].count);
            arr_restore (worst[j].ptr, worst[j].count);

            ns_elapsed_a 
                = time_sort (algo[i].sort, average[j].ptr, average[j].count);
            arr_restore (average[j].ptr, average[j].count);

            printf ("%11d %11ld %11ld %11ld\n", best[j].count, ns_elapsed_b,
                    ns_elapsed_w, ns_elapsed_a);
        }
        printf ("\n\n");
    }

    /* free the memory associated with the arrays */
    for (i = 0; i < N; i++) {
        free (best[i].ptr);
        free (worst[i].ptr);
        free (average[i].ptr);
    }
    free (best);
    free (worst);
    free (average);
    printf ("Memory successfully deallocated. Exiting process.\n");
    return 0;
}
