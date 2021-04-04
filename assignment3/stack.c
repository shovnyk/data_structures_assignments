/* stack.c: a simple single linked list implementation of a stack */
#include <stdio.h>
#include <stdlib.h>
#include "mystack.h"

#define new(x) (x*) malloc (sizeof (x)) 
#define  RETURN_IF_NULL(x,y) if (x == NULL) {\
    fprintf (stderr, "malloc(): failed to allocate new memory.\n");\
    return y;\
}

/* check if stack is empty */
int is_empty (stack_t *top)
{
    return (top == NULL);
}

/* display stack from top to bottom */
void display (stack_t *top)
{
    stack_t *p;
    if (is_empty (top)) {
        printf ("Stack is empty.\n");
        return;
    }
    printf ("%d ", top->info);
    p = top->link;
    while (p != NULL) { 
        printf ("%d ", p->info); 
        p = p->link;
    }        
    putchar ('\n');
}

/* display the stack from bottom to top (recursively) in each step.
   Because of reccursive nature it cannot print a newline at the end */
int reverse_print (stack_t *top)
{
    if (is_empty (top)) { /* this is not a base case, this is simply
                             an edge case handler */
        printf ("Stack is empty\n");
        return 0;
    }
    if (top->link == NULL) { /* this is the base case : when there is 
                                only one item in the stack */
        printf ("%d ", top->info);
        return 1;
    }
    reverse_print (top->link);
    printf ("%d ", top->info);
    return 1;
}

/* push new value on top of the stack */
int push (stack_t **top, int info)
{ 
    stack_t *node = new (stack_t);
    RETURN_IF_NULL (node, 0);
    node->info = info;
    node->link = *top; /* top == NULL when empty so edge case handled */
    *top = node; 
    return 1;
} 

/* pop value of the top of the stack */
int pop (stack_t **top, int *popped)
{
    stack_t *tmp;
    if (is_empty (*top)) {
        return 0;
    }
    tmp = *top; 
    *top = (*top)->link; 
    *popped = tmp->info;
    free (tmp); 
    return 1;
}

/* deallocate remainining nodes/memory of the stack when done */
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

/* return size of the stack same as traversal of a single linked list */
int size_of_stack (stack_t *top)
{
    int count;
    stack_t *p;
    if (is_empty (top)) {
        return 0;
    }
    p = top;
    count = 0;
    while (p != NULL) {
        count++;
        p = p->link;
    }
    return count;
}
