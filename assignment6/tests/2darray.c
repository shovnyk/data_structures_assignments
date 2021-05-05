#include <stdio.h>

#include "sorting_functions.c"

int main (void)
{
    char datafile[5][BUFF];
    for (int i = 0; i < 5; i++) {
        snprintf (datafile[i], BUFF, "data%c.txt", algo[i].name[0]);
    }
    for (int i = 0; i < 5; i++) {
        printf ("datafile[%d] = %s\n", i, datafile[i]);
    }
    return 0;
}
