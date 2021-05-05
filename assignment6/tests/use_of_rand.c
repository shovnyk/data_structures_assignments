#include <stdio.h>
#include <stdlib.h> /* for rand() and atoi() */
#include <time.h>

int randint (int lower, int upper)
{
    static int called_before = 0;
    int n;
    if (!called_before) {
        called_before = 1;
        printf ("calling time()\n");
        srand (time (NULL));
    }
    n = rand ();
    return lower + n % (upper - lower + 1);
}

int main (int argc, char **argv)
{
    int len = 10;
    int *arr = (int *) malloc (len * sizeof (int));
    if (arr == NULL) {
        perror ("malloc");
        return 1;
    }
    int i;
    for (i = 0; i < len; i++) {
       arr[i] = randint (10, 200); 
    }
    for (i = 0; i < len; i++) {
        printf ("%d ", arr[i]);
    }
    free (arr);
    return 0;
}
