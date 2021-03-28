/* hanoi.c: C program to solve the tower of hanoi problem via recursion 
    
   In the tower of hanoi problem we have to move disks stacked on one
   rod (A), in decreasing order of size from bottom to top, to another 
   stack (C) with the help of an intermediate rod (B) without ever
   moving a larger disk on top of a smaller disk.
*/

#include <stdio.h>
#include <stdlib.h>

/* move a disk with a certain number from one rod to another */
void move (int diskno, char from, char to)
{
    /* The actual implementation of this function can be varied.
       Here, we simply print to stdout. */
    printf ("Move disk %d from rod %c to rod %c.\n", diskno, 
            from, to);
}

/* recursively find the minimum number of moves required
   to move disks from one rod to another */ 
int hanoi (int n, char from, char to, char via, int *count)
{
    if (n < 1) {
        return 0;
    }
    if (n == 1)
    {   /* base case */
        move (1, from, to);
        (*count)++;          /* increment the counter after every move */
    } 
    else {
        hanoi (n - 1, from, via, to, count); /* recursive function call */
        move (n, from, to);
        (*count)++;
        hanoi (n - 1, via, to, from, count);
    }
    return 1;
}

#define BUFF 256
#define PROMPT "Enter the number of disks in the stack "\
               "or press q to quit.\n>> "
#define INVALID_INPUT "?\n"

int main (void)
{
    int moves, disks;
    char input[BUFF];
    while (1) 
    {
        printf (PROMPT);
        if (fgets (input, BUFF, stdin) == NULL) {
            /* IO error */
            fprintf (stderr, "!Could not read input.\n");
        }
        else if (input[0] == 'q') { 
            /* user enters q: exit from the program */
            break;
        }
        else if (sscanf (input, "%d", &disks)) {
            moves = 0;
            if (hanoi (disks, 'A', 'C', 'B', &moves))
                printf ("%d moves are required.\n", moves);
            else 
                printf ("!Number of disks must be positive.\n");
        }
        else {
            fprintf (stderr, INVALID_INPUT);
        }
    }
    printf ("!Exiting process.\n");
    exit (0); 
}
