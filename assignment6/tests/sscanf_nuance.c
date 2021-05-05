#include <stdio.h>
#include <string.h>


/* I have always thought sscanf returned 0 whenever it failed to convert a 
   character into the specified format. Turns out if sscanf gets only a
   newline character and cannot convert it into say a nunmber, it will
   return EOF (from the man page: "The value EOF is returned if the end of
   the input is reached before either the first successful conversion or a 
   matching failure occurs.") 
   So the appropriate error checking condition to see that a single number
   was successfully converted can be :

   if (sscanf (token, "%d", &elem) != 1) { error handling }

   or
   n = sscanf (token, "%d", &elem);
   if (n == 0 || n == EOF) { error handling }
   */
int main (void)
{
    int elem;
    char input[100];
    fgets (input, 100, stdin);
    printf ("number of ... converted: %d\n", sscanf (input, "%d", &elem));
    return 0;
}

/* run this program and simply press enter. The result should be -1 */
