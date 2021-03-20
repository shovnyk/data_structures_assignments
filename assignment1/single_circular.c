/* A C program to implement a singly linked circular list */

#include <stdio.h> 
#include <stdlib.h>
#include <stdarg.h>

typedef struct node { 
    int info;
    struct node *link;
} list_t; 

#define  RETURN_IF_NULL(x,y) if (x == NULL) {\
    fprintf (stderr, "malloc(): failed to allocate memory\n");\
    return y;\
}

/* check if the list is empty */
int is_empty (list_t *end) {
    return (end == NULL);
}

/* print the contents of the list */
void display (list_t *end)
{
    list_t *p, *start = end->link;
    if (end == NULL) {
        printf ("List is empty\n");
        return;
    }
    p = start; 
    do {
        printf ("%d ", p->info);
        p = p->link;
    } while (p != start); /* reverse till it cycles back to start */
    putchar ('\n');
}

/* initialize a linked list with data collected as varargs */
list_t *create_list (int count, ...)
{
    va_list ap;

    va_start (ap, count);
    struct node *n, *temp, *start;
    n =  (struct node *)malloc (sizeof (struct node));
    RETURN_IF_NULL (n, NULL);
    n->info = va_arg (ap, int);
    start = temp = n;
    int i;
    for (i = 0; i < count - 1; i++)
    {
        n = (struct node *)malloc (sizeof (struct node));
        RETURN_IF_NULL (n, NULL);
        temp->link = n;
        n->info = va_arg (ap, int);
        temp = n;
    } 
    va_end (ap);

    /* set link of last node to point to first node */
    n->link = start; 
    /* return pointer to last element */
    return n; 
}

/* append node and connect it to starting node */
int insert_at_end (list_t **end, int info)
{   /* since we will be updating the end pointer, 
       we take in value by reference */
    struct node *newnode, *start;
    newnode = (struct node*)malloc (sizeof (struct node));
    RETURN_IF_NULL (newnode, 0);
    start = (*end)->link; 
    newnode->info = info;
    newnode->link = start;
    (*end)->link = newnode;
    (*end) = newnode; 
    printf ("Element '%d' has been added to the list at the end.\n", info);
    return 1;
}

/* insert element after a specified index */
int insert_after_index (list_t **end, int info, int index)
{
    struct node *newnode, *p;
    p = (*end)->link; /* start from the first node */
    newnode = (struct node*)malloc (sizeof (struct node));
    RETURN_IF_NULL (newnode, 0);
    newnode->info = info;
    int n = 0;
    while (n < index)
    {
        if (p == NULL) {
            free (newnode);
            return 0;
        }
        p = p->link;
    } 
    /* TODO do the actual insertion */
    return 1;
}
    
/* frees memory of all remaining nodes */
int destroy_list (list_t *end) 
{
    if (is_empty(end))
        return 0; /* nothing to free */
    struct node *p, *temp, *start;
    start = end->link;
    p = start;
    do 
    {
        temp = p->link;
        free (p);
        p = temp;
    } while (p != start);
    return 1;
}

                          /* end of implementation */

/* macro to safely get input from the user and store in variable  */
#define GET_NUM(from,into)  if (fgets (from, BUFF, stdin) == NULL) {\
    fprintf (stderr, "IO Error. Exiting process.\n");\
    exit (1);\
}\
else if (sscanf (from, "%d", &into) == 0) {\
    INVALID_INPUT();\
    continue;\
}
/* diplay help menu */
#define DISPLAY_MENU() printf ("c\tCreate a new list\n"\
                                "p\tPrint list\n"\
                                "i\tInsert element into list\n"\
                                "d\tDelete element from list\n"\
                                "h\tDisplay this help menu\n"\
                                "q\tQuit\n");
#define INVALID_INPUT() fprintf (stderr, "?\n")
#define BUFF 256
#define PROMPT ">> "

/* main routine to test the functions defined */
int main (void)
{
    int i, x;
    int choice, index, to_be_inserted;
    int initial_size, *initial_list; 

    node_t *l = NULL; /* list is initially empty */
    char cmd[BUFF], input[BUFF]; /* buffers to store user input */

    puts ("Implementation of a Singly Linked Linear List\nCommands:");
    DISPLAY_MENU ();
    while (1)
    {
        printf (PROMPT);
        if (fgets (cmd, BUFF, stdin) != NULL) {
            switch (cmd[0])
            {
                case 'c':
                    if (l  != NULL) { /* a list already exists */
                        fprintf (stderr, "!Overwriting existing list\n");
                        destroy_list (l);
                    } 
                    printf ("Enter the initial number of elements\n" PROMPT); 
                    GET_NUM (input, initial_size);
                    initial_list = (int *) malloc (sizeof (int));
                    RETURN_IF_NULL (initial_list, 1);
                    for (i = 0; i < initial_size; i++) {
                        GET_NUM (input, initial_list[i]);
                    } 
                    l = create_list (initial_list, initial_size); 
                    if (l != NULL) {
                        printf ("List created.\n");
                    }
                    free (initial_list);
                    break; 
                   
                case 'p':
                    display (l);
                    break;

                case 'i':
                    printf ("Insert what?\n");
                    GET_NUM (input, to_be_inserted);
                    printf ("Insert where?\n" 
                            "1\tBeginning of list\n"
                            "2\tEnd of list\n"
                            "3\tAfter index N\n" PROMPT);
                    GET_NUM (input, choice);
                    switch (choice) {
                        case 1:
                            if (insert_at_start (&l, to_be_inserted)) {
                                printf ("Element '%d' has been added "
                                        "to beginning.\n", to_be_inserted); 
                            }
                            break; 
                        case 2:
                            if (insert_at_end (l, to_be_inserted)) {
                                printf ("Element '%d' has been added "
                                        "to end.\n", to_be_inserted);
                            }
                            break;
                        case 3:
                            printf ("N = ? ");
                            GET_NUM (input, index);
                            x = insert_after_index (l, to_be_inserted, index);
                            if (x) { 
                                printf ("Element '%d' has been added after "
                                        "index %d.\n", to_be_inserted, index);
                            }
                            else if (x == -1) {
                                fprintf (stderr, "!Index out of bounds.\n");
                            }
                            break;

                        default:
                            INVALID_INPUT();
                    } 
                    break;

                case 'd':
                    printf ("Delete where?\n" 
                            "1\tBeginning of list\n"
                            "2\tEnd of list\n"
                            "3\tAt index N\n" PROMPT);
                    GET_NUM (input, choice);
                    switch (choice) {
                        case 1:
                            if (delete_at_start (&l, &x)) {
                                printf ("Element '%d' has been removed from" 
                                        " beginning.\n", x); 
                            }
                            break; 
                        case 2:
                            if (delete_at_end (l, &x)) {
                                printf ("Element '%d' has been removed from"
                                        "end.\n", x);
                            }
                            break;
                        case 3:
                            printf ("N = ? ");
                            GET_NUM (input, index); 
                            if (delete_at_index (l,index, &x)) { 
                                printf ("Element '%d' has been added after "
                                        "index %d.\n", x, index);
                            }
                            else {
                                fprintf (stderr, "!Index out of bounds.\n");
                            }
                            break;

                        default:
                            INVALID_INPUT();
                    } 
                    break;

                case 'q':
                    destroy_list (l);
                    printf ("List destroyed. Exiting process.\n");
                    exit (0);

                default: 
                    INVALID_INPUT();
                case 'h':
                    DISPLAY_MENU ();
                    continue;
            }
        }
        else {
            fprintf (stderr, "IO Error. Exiting process.\n");
            exit (1); 
        }
    }
    exit (0);
}
