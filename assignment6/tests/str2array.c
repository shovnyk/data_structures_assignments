/* routine to convert a string buffer consisiting of comma or space 
   separated numbers into an array */
#include <stdio.h>
#include <stdlib.h> /* for malloc and realloc */
#include <string.h> /* for strtok */

#define BUFF 1024
#define INIT_CAP 3 /* set this to a reasonably high value */

int *getarr (char *buff, char *delim, int *len)
{
    char *token;
    size_t capacity, i;
    int elem, *arr;
    if (strchr (buff, '\n') == NULL) {
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
    printf ("array capacity has been set to %lu\n", capacity);
    i = 0;
    token = strtok (buff, delim); 
    while ((token != NULL)) {
        if (sscanf (token, "%d", &elem) != 1) {
            fprintf (stderr, "invalid input\n"); 
            *len = -1;
            return NULL;
        }
        printf ("read in element %d\n", elem);
        if (i == capacity) { /* if capacity is being exceeded, double it */
            capacity *= 2; 
            arr = (int *) realloc (arr, sizeof (int) * capacity);
            if (arr == NULL) { 
                perror ("realloc"); 
                break; /* return whatever has been converted */
            }
            printf ("array capacity has been doubled to %lu\n", capacity);
        }
        arr[i++] = elem;
        token = strtok (NULL, delim);
    }
    *len = i;
    return arr;
}

int main (void)
{ 
    char input[BUFF];
    int *arr, len;
    printf ("Array (space/comma delimited)? = " );
    if (fgets (input, BUFF, stdin) == NULL) {
        fprintf (stderr, "IO Error\n");
        exit (1);
    }
    arr = getarr (input, " ,\n", &len);
    if (arr == NULL) {
        fprintf (stderr, "failed to generate array from input buffer.\n"
                "Exiting process");
        exit (1);
    }
    for (int i = 0; i < len; i++) {
        printf ("%d ", arr[i]);
    }
    putchar ('\n');
    free (arr);
    return 0;
}

/* edge cases: trailig white spaces. There are 2 ways in which you can 
   resolve this:
   1. putting a newline in the delimiter string before passing it to
      strtok(). ie the delimiter string is " ,\n" instead of " ,"

                                      OR

   2. check for a newline before entering the loop that processes
      tokens:
                token = strtok (buff, " ,");
                while (token != NULL && token[0] != '\n') {
                    ...
                }
*/
