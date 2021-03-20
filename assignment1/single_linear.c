/* single_linear.c: C program to implement a singly linked linear list */      

#include <stdio.h>  /* for printf and fprintf */
#include <stdlib.h> /* for malloc and exit */

/* a macro to check if malloc fails and return an error code accordingly */
#define  RETURN_IF_NULL(x,y) if (x == NULL) {\
    fprintf (stderr, "malloc(): failed to allocate memory\n");\
    return y;\
}

typedef struct node { 
    int info;
    struct node *link;
} node_t;  /* typedef for ease of use and readability */

/* check if the list is empty */
int is_empty (node_t *start) {
    return (start == NULL);
}

/* print the contents of the list */
void display (node_t *start)
{
    node_t *p = start;
    if (is_empty(start)) {
        printf ("List is empty\n");
        return;
    }
    while (p != NULL)
    {
        printf ("%d ", p->info);
        p = p->link;
    }
    putchar ('\n');
}

/* initialize a linked list with data collected in an array */
node_t *create_list (int *arr, int len)
{
    int i;
    node_t *n, *temp, *start;
    n = (node_t *) malloc (sizeof (node_t));
    RETURN_IF_NULL (n, NULL);

    n->info = arr[0];
    start = temp = n;
    for (i = 1; i < len; i++)
    {
        n = (node_t *) malloc (sizeof (node_t));
        RETURN_IF_NULL (n, NULL);
        temp->link = n;
        n->info = arr[i];
        temp = n;
    } 
    n->link = NULL;
    return start;
}

/* add a node at the beginning of the list */
int insert_at_start (node_t **start, int info)
{ 
    node_t *newnode = (node_t *) malloc (sizeof (node_t));
    RETURN_IF_NULL (newnode, 0);
    newnode->info = info;
    newnode->link = *start; /* start = NULL if list is empty so it will 
                               handle that edge case as well */
    *start = newnode; 
    return 1;
} 

/* add a node anywhere in the list */
int insert_at_end (node_t *start, int info)
{
    node_t *p = start, *newnode;
    newnode = (node_t *) malloc (sizeof (node_t));
    RETURN_IF_NULL (newnode, 0);
    newnode->info = info;
    newnode->link = NULL; 
    while (p->link != NULL) {
        p = p->link;
    } 
    p->link = newnode; 
    return 1;
}

/* add a node after a certain index */
int insert_after_index (node_t *start, int info, int index)
{
    node_t *p, *newnode;
    p = start;
    newnode = (node_t *) malloc (sizeof (node_t));
    RETURN_IF_NULL (newnode, 0);
    newnode->info = info; 
    int n = 0;
    while (n < index)
    {
        if (p == NULL) {    
            /* index exceeds number of nodes, return */
            free (newnode);
            return -1; 
        }
        p = p->link;
        n++;
    } 
    newnode->link = p->link;
    p->link = newnode; 
    return 1;
}

/* remove node start  */
int delete_at_start (node_t **start, int *removed)
{
    if (is_empty (*start)) {
        fprintf (stderr, "Underflow: list is empty. Cannot delete!\n");
        return 0;
    }
    node_t *tmp = *start; 
    /* handles edge case when only one node in list */
    *start = (*start)->link; 
    *removed = tmp->info; /* store removed element before freeing*/
    free (tmp); 
    return 1;
}

/* remove elements from the end of the list */
int delete_at_end (node_t *start, int *removed)
{
    node_t *tmp, *p = start;
    if (is_empty (start)) {
        fprintf (stderr, "Underflow: list is empty. Cannot delete!\n");
        return 0;
    }
    while ((p->link)->link != NULL)
    {
        p = p->link;
    }
    /* at this point, p is the second last element in the list */
    tmp = p->link; 
    p->link = NULL; 
    *removed = tmp->info; 
    free (tmp);
    return 1; 
}

/* remove elements at arbitrary index */
int delete_at_index (node_t *start, int n1, int *removed)
{
    int i;
    node_t *tmp,*p = start;
    for (i = 0; i < n1; i++)
    {
        tmp = p;
        p = p->link;
        if (p == NULL) /* index exceeds number of nodes, return */
            return 0;
    }
    tmp->link = p->link;
    *removed = p->info; 
    free (p);
    return 1;
}

/* frees memory of all remaining nodes */
int destroy_list (node_t *start) 
{
    node_t *p = start, *temp;
    if (is_empty(start))
        return 0; /* nothing to free */
    while (p != NULL)
    {
        temp = p->link;
        free (p);
        p = temp;
    }
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
