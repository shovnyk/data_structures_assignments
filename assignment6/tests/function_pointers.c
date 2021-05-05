#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* example 1 */
int add (int x, int y)
{
    return x + y;
}

int mult (int x, int y)
{
    return (x * y);
}

int sub (int x, int y)
{
    return (x - y);
}

int divide (int x, int y)
{
    if (y != 0)
        return (x / y);
    return -1;
}

void swap (int *x, int *y)
{
    int temp = *y;
    *y = *x;
    *x = temp;
}
int bsort (int *arr, int begin_idx, int end_idx)
{
    int i, j, len; 
    if (arr == NULL) {
        return 0;
    }
    len = end_idx - begin_idx + 1;
    for (i = 0; i < (len - 1); i++) {
        for (j = (len - 1); j > i; j--) {
            if (arr[j] < arr[j - 1]) {
                swap (&arr[j], &arr[j - 1]);
            }
        }
    }
    return 1;
}

int main(void) 
{  
    puts ("Example 1");
                        /* step 1: declare pointer */

    /* the syntax is very easy: it is almost identical to the function 
       declaration except the name of the function is a pointer preceded
       by a '*' and inside PARENTHESES 
        
                   return-type (*pointer-name) (argslist...)
    */
    int (*p)(int, int); /* declare a pointer to a function of name 'p' hence
                           '*p' and accepting arguments of type int, int */
                        
                        /* step 2: assign pointer */
    p = &add;           /* assign to pointer p the starting 'address'
                           of the instructions for the function add()
                           which accepts argument of type (int, int) */

                        /* step 3: use this function */
    int c = (*p)(2, 3); /* derefence this function as if doing so were to 
                           give you the function add itself! and then use
                           it as you have used add()! */
    printf ("c = %d\n", c);

    p = mult;          /* since mult has the same signature: (int, int) 
                           so it can be assigned to p as well. Note that
                           the use of the & operator is optional */
    c = p(2, 3);       /* note that dereferencing using * is  optional*/

    printf ("c = %d\n", c);
    
    puts ("Example 2");

    int arr[] = {99, 80, -99, 1, 0, 2, 3, 900};
    int len = sizeof (arr) / sizeof (arr[0]);
    int (*sort)(int *, int, int);
    sort = bsort;
    for (int i = 0; i < len; i++) 
        printf ("%d ", arr[i]);
    putchar ('\n');
    sort(arr, 0, len - 1);
    for (int i = 0; i < len; i++)
        printf ("%d ", arr[i]);
    putchar ('\n');

    puts ("Example 3");

    /* finally let's take a look at an array of function pointers */

    int (*f[])(int, int) = {add, mult, sub, divide}; 
    /* assigning the names of the function using an array initializer
       this means that f[0] is add, f[1] is mult, f[2] is sub and 
       f[3] is divide.
    */

    int x = 10, y = 20;
    printf ("%d + %d = %d\n", x, y, f[0](x, y));
    printf ("%d * %d = %d\n", x, y, f[1](x, y));
    printf ("%d - %d = %d\n", x, y, f[2](x, y));
    printf ("%d / %d = %d\n", x, y, f[3](x, y));

    return 0;

}
