#include <stdio.h>
#include <stdlib.h>

void *xmalloc (size_t siz)
{
    void *mem = malloc (siz);
    if (mem == NULL) 
        exit (2);
    return mem;
}

int main (void)
{
    int *arr = (int *) xmalloc (10 * sizeof (int));
    free (arr);
    arr = (int *) xmalloc (-10 * sizeof (int));
    return 1;
}
