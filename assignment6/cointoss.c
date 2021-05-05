#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int randint (int lower, int upper)
{
    static int called_before = 0;
    int n;
    if (!called_before) { 
        called_before = 1;
        srand (time (NULL));
    }
    n = rand (); 
    return lower + n % (upper - lower + 1);
}

int main (void)
{
    int heads = 0, tails = 0, tosses = 0;
    while (1) {
        puts ("Tossing coin...");
        srand (time (NULL));
        if (randint(0,1)) {
            printf ("Heads\n");
            heads++;
        }
        else {
            printf ("Tails\n");
            tails++;
        }
        sleep (1);
        tosses++;
        printf ("Heads = %d, Tails = %d\nP(H) = %f, P(T) = %f",
                heads, tails, (heads/(double)tosses), (tails/(double)tosses));
        putchar ('\n');
    }
}
