/* fixed_sized_stacks.c: C program to implement a fixed sized stack

   Stacks have two primitive operations: 
   1. push() which puts values on the top
   2. pop() which removes values from the top

   In this implementation we will use arrays having a predefined
   size to design stack that implements these operations. We will
   need to check for both overflow, when the stack if full and 
   undeflow, when the stack is empty. A top pointer always needs
   to be maintained.

*/

#include <stdio.h>
#include <stdlib.h>

#define CAPACITY 5        /* set a size limit to the stack */

typedef struct stack {
    int top;
    int items[CAPACITY];
} stack_t;                /* typedef for ease of use */

int is_empty (stack_t *s) /* check if the stack is empty */
{
    return (s->top == -1); /* for an empty stack, top must be -1 */
}

int is_full (stack_t *s)  /* check if the stack if full  */
{
    return (s->top == (CAPACITY - 1));
}

/* print the contents of the stack from top to bottom */
void display (stack_t *s) 
{
/* colors for better visualization */
#define  red     "\033[0;31m"
#define  reset   "\033[0m"
    int i;
    if (is_empty (s)) {
        printf ("Stack is empty.\n");
        return;
    }
    printf (red "%10d " reset " <-- top\n", (s->items)[s->top]);
    for (i = (s->top) - 1; i >= 0; i--) {
        printf ("%10d\n", (s->items)[i]);
    }
    putchar ('\n');
}

/* push an element on top of the stack and update top */
int push (stack_t *s, int info)
{
    if (is_full (s)) {
        fprintf (stderr, "!Stack Overflow. "
                "Cannot push new value '%d'\n", info);
        return 0;
    }
    /* move top to "upward" first and then assign new value.
       For this use pre increment operator */
    (s->items)[++(s->top)] = info;  
    return 1;
}

/* pop the element at the top of the stack and update top */
int pop (stack_t *s, int *popped) 
{
   if (is_empty (s)) {
       fprintf (stderr, "!Stack Underflow. Cannot pop anything.\n");
       return 0;
   }
   /* get popped value before decrementing top. For this, prefer
      post decrement operator */
   *popped = (s->items)[(s->top)--];
   return 1;
}

                          /*end of implementation */
#define MENU "Commands:\n"\
             "---------\n"\
             "1.\tPush\n"\
             "2.\tPop\n"\
             "3.\tPrint the stack from top to bottom\n"\
             "4.\tDisplay this help menu\n"\
             "5.\tQuit\n"
#define INVALID_INPUT "?\n"
#define BUFF 256
#define PROMPT "--> "

/* function to get numeric input from user from stdin */
char getnum (char *buff, size_t size, int *var)
{
    if (fgets (buff, size, stdin) == NULL) {
        fprintf (stderr, "!IO Error.\n");
        return 0;
    }
    else if (sscanf (buff, "%d", var) == 0) {
        fprintf (stderr, INVALID_INPUT);
        return buff[0];
    }
    return 1;
}

/* main routine to test the functions defined */
int main (void)
{ 
    stack_t s = {-1, {0}};
    int cmd, tmp = 0;
    char input[BUFF];  /* for storing user input */
    int to_be_pushed = 0, popped_value;

    printf ("\nImplementation of a Stack with fixed memory\n");
    printf (MENU);
    while (1)
    {
        printf (PROMPT);
        tmp = getnum (input, BUFF, &cmd);
        if (tmp == 0) {
            fprintf (stderr, "!IO Error.\n");
            continue;
        }
        else if (tmp == 1){ /* assuming user cannot enter ascii 1 through
                               keyboard */
            switch (cmd)
            {
                case 1:
                    printf ("Push what?\n" PROMPT);
                    if (getnum (input, BUFF, &to_be_pushed) != 1) 
                        continue;
                    if (push (&s, to_be_pushed)) {
                        printf ("Element '%d' has been pushed.\n", 
                                to_be_pushed);
                    }
                    break;

                case 2:
                    if (pop (&s, &popped_value)) {
                        printf ("Element '%d' has been popped.\n",
                                popped_value);
                    }
                    break;

                case 3:
                    display (&s);
                    break;

                case 5:
                    printf ("Exiting process.\n");
                    exit (0);

                default: 
                    fprintf (stderr, INVALID_INPUT);
                case 4:
                    printf (MENU);
                    continue;
            }
        }
        else {
            continue;
        }
    }
    exit (0);
}
