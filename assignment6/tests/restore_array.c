#include <stdio.h>
#include <stdlib.h>

/* idea:  allocate an array of  double the size (2*len)  of the intended
size  (len) and  use the  second half  to store  copies of  the elements
stored in  the first half.  Always refer to  the array through  its base
address (ie name) and pass len for operating on the array and not 2*len.
so that all operations are performed on the first half:
only the first half should be printed and only the first half should be
sorted. The second half can then be used to recover the original array
after it has been modified for eg: sorted */
 

void modify1 (int *arr, int len)
{
    for (int i = 0; i < len; i++) 
        arr[i] *= 2;
}

void modify2 (int *arr, int len)
{
    for (int i = 0; i < len; i++)
        arr[i] *= -1;
}

void modify3 (int *arr, int len)
{
    for (int i = 0; i < len; i++)
        arr[i] = 0;
}

void display (int *arr, int len)
{
    for (int i = 0; i < len; i++) 
        printf ("%d ", arr[i]);
    putchar ('\n');
}

void arr_restore (int *arr, int len)
{
    for (int i = 0; i < len; i++) 
        arr[i] = arr[i + len];
}

int main (int argc, char **argv)
{
    int len;
    int *arr;

    if (argc < 2) {
        fprintf (stderr, "usage: %s <element1> [element2]...\n", argv[0]);
        return 1;
    }

    len = argc - 1;
    arr = (int *) malloc (2 * sizeof (int) * len);
    if (arr == NULL) 
        return 1;

    /* making the array */
    for (int i = 0; i < len; i++) 
        arr[i] = arr[i + len] = atoi (argv[i + 1]); 

    display (arr, len); 

    void (*func[])(int *, int) = {modify1, modify2, modify3, NULL};

    for (int i = 0; i < 3; i++) {
        printf ("before modify%d(): ", i+1);
        display (arr, len);
        func[i] (arr, len);
        printf ("after modify%d(): ", i+1);
        display (arr, len);
        printf ("restoring original array.\n");
        arr_restore (arr, len);
    }

    free (arr);

    return 0;
}
