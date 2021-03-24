/* dynamic_stacks.c: C program to implement a dynamically sized stack 

   If we think of a stack as a single linked list with its first node
   being the top, then it is possible for us to make it accommodate 
   any number of new elements without overflow - by inserting new 
   nodes at the start. We would still need to check for undeflow.

*/

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int info;
    struct node *link;
} stack_t;

#define new(x) (x*) malloc (sizeof (x)) 
/* a macro to check if malloc fails and return an error code accordingly */
#define  RETURN_IF_NULL(x,y) if (x == NULL) {\
    fprintf (stderr, "malloc(): failed to allocate memory\n");\
    return y;\
}

/* check if the stack if empty:
   this is same operation as checking if an SLL is empty */
int is_empty (stack_t *top)
{
    return (top == NULL);
}

/* display the stack from top to bottom: 
   this is the same operation as traversing an SLL */
void display (stack_t *top)
{
#define  red     "\033[0;31m"
#define  reset   "\033[0m"
    stack_t *p;
    if (is_empty (top)) {
        printf ("Stack is empty.\n");
        return;
    }
    printf (red "%10d " reset " <-- top\n", top->info);
    p = top->link;
    while (p != NULL) { 
        printf ("%10d\n", p->info); 
        p = p->link;
    }        
    putchar ('\n');
}

/* push a new value onto the stack: 
   this is the same operation as inserting an element at the 
   beginning of an SLL */
int push (stack_t **top, int info)
{ 
    stack_t *node = new (stack_t);
    RETURN_IF_NULL (node, 0);
    node->info = info;
    node->link = *top; /* top == NULL when empty so edge case handled */
    *top = node; 
    return 1;
} 

/* pop a node off the top of the stack:
   this is the same operation as deletion from the beginning of 
   an SLL */
int pop (stack_t **top, int *popped)
{
    stack_t *tmp;
    if (is_empty (*top)) {
        fprintf (stderr, "!Stack Underflow. Cannot pop value anything.\n");
        return 0;
    }
    tmp = *top; 
    *top = (*top)->link; 
    *popped = tmp->info;
    free (tmp); 
    return 1;
}

/* destroy nodes that have not been popped:
   since memory is being dynamically alloatec it is important to
   free it to prevent memory leaks */
int destroy_stack (stack_t *top)
{
    stack_t *p, *tmp;
    if (is_empty (top)) {
        return 0; 
    }
    p = top;
    while (p != NULL) {
        tmp = p->link;
        free (p);
        p = tmp;
    }
    return 1;
}


                          /* end of implementation */
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
    stack_t *s = NULL; /* stack is initially empty */
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
                    display (s);
                    break;

                case 5:
                    if (!is_empty (s)) {
                        destroy_stack (s);
                        printf ("!Stack destroyed.\n");
                    }
                    printf ("!Exiting process.\n");
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
