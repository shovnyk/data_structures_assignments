/* single_circular.c: C program to implement a singly linked circular list */

#include <stdio.h> 
#include <stdlib.h>

#define  RETURN_IF_NULL(x,y) if (x == NULL) {\
    fprintf (stderr, "malloc(): failed to allocate memory\n");\
    return y;\
}
/* list empty = nothing to delete, report underflow  */
#define CHECK_UNDERFLOW(x) if (is_empty (x)) {\
    fprintf (stderr, "!Underflow. List is empty. Cannot delete.\n");\
    return 0;\
}

typedef struct node { 
    int info;
    struct node *link;
} node_t; 

int is_empty (node_t *end) {
    return (end == NULL);
}

void display (node_t *end)
{
    node_t *p, *start; 
    if (is_empty (end)) {
        printf ("List is empty\n");
        return;
    }
    p = start = end->link;
    do {
        printf ("%d ", p->info);
        p = p->link;
    } while (p != start); /* reverse till it cycles back to start */
    putchar ('\n');
}

node_t *create_list (int *arr, int len)
{
    int i = 0;
    node_t *n, *tmp, *start;
    n =  (node_t *)malloc (sizeof (node_t));
    RETURN_IF_NULL (n, NULL);
    n->info = arr[i];
    start = tmp = n;
    for (i = 1; i < len; i++)
    {
        n = (node_t *)malloc (sizeof (node_t));
        RETURN_IF_NULL (n, NULL);
        tmp->link = n;
        n->info = arr[i];
        tmp = n;
    } 
    /* set link of last node to point to first node */
    n->link = start; 
    /* return pointer to last element */
    return n; 
}

int insert_at_start (node_t **end, int info)
{
    node_t *newnode = (node_t *) malloc (sizeof (node_t));
    RETURN_IF_NULL (newnode, 0);
    newnode->info = info;
    if (is_empty (*end)) {
        newnode->link = newnode;
        (*end) = newnode; /* make end point to new node */
        return 1;
    }
    /* add an element after end but don't move the end pointer */
    newnode->link = (*end)->link;
    (*end)->link = newnode;
    return 1;
}    

int insert_at_end (node_t **end, int info)
{
    /* same as before, except the end pointer is set
       to point to the newly inserted node */
    if (!insert_at_start (end, info)) {
        return 0;
    }
    (*end) = (*end)->link;
    return 1;
}

int insert_after_index (node_t **end, int info, size_t index)
{
    size_t n;
    node_t *newnode, *p, *start;
    if (is_empty (*end)) {
        return -1;
    }
    p = start = (*end)->link; /* start from the first node */
    newnode = (node_t*) malloc (sizeof (node_t));
    RETURN_IF_NULL (newnode, 0);
    newnode->info = info;
    n = 0;
    do {
        if (n++ == index) {
            newnode->link = p->link;
            p->link = newnode;
            if (p == *end) {
                /* if inserted after last node, end pointer should 
                   be updated or else it will be logically added
                   at the beginning */
                *end = newnode;
            }
            return 1;
        }
        p = p->link;
    } while (p != start);
    free (newnode); /* could not be inserted */
    return -1;
}

int delete_at_start (node_t **end, int *removed)
{
    node_t *start; 
    CHECK_UNDERFLOW (*end);     /* if empty */
    if (*end == (*end)->link)   /* if only one node */
    {
        *removed = (*end)->info;
        free (*end);
        *end = NULL;
        return 1;
    }
    /* make the last node point to the second node */
    start = (*end)->link;
    (*end)->link = start->link;
    *removed = start->info;
    free (start);
    return 1;
}

int delete_at_end (node_t **end, int *removed)
{
    node_t *p;
    CHECK_UNDERFLOW (*end);     /* if empty */
    if (*end == (*end)->link)   /* if only one node */
    {
        *removed = (*end)->info;
        free (*end);
        *end = NULL;
        return 1;
    } 
    p = (*end)->link;
    while ((p->link) != *end) {
        p = p->link;
    }
    /* traverse to the second last node */
    p->link = (*end)->link; 
    *removed = (*end)->info;
    free (*end);
    *end = p; /* update end pointer to second last node */
    return 1; 
}

int delete_at_index (node_t **end, size_t index, int *removed)
{ 
    size_t i;
    node_t *p, *start, *tmp;
    if (index == 0) {
        return delete_at_start (end, removed);
    }
    CHECK_UNDERFLOW (*end);
    p = start = (*end)->link;
    i = 0;
    do {
        if (i++ == (index - 1)) {
            tmp = p->link;
            p->link = (p->link)->link;
            *removed = tmp->info;
            free (tmp);
            if (p->link == start) { 
                /* if last node is being deleted, update the
                   end pointer to point to second last node */
                *end = p;
            } 
            return 1;
        }
        p = p->link;
    } while (p != start);
    return -1;
}

int destroy_list (node_t *end) 
{
    if (is_empty(end))
        return 0; /* nothing to free */
    node_t *p, *tmp, *start;
    start = end->link;
    p = start;
    do 
    {
        tmp = p->link;
        free (p);
        p = tmp;
    } while (p != start);
    return 1;
} 
                          /* end of implementation */

/* macro to get input from the user and store in variable  */
#define GET_NUM(from,into)  if (fgets (from, BUFF, stdin) == NULL) {\
    fprintf (stderr, "!IO Error. Exiting process.\n");\
    exit (1);\
}\
else if ((sscanf (from, "%d", &into) == 0)) {\
    fprintf (stderr, INVALID_INPUT);\
    continue;\
}

#define MENU "Commands:\n"\
             "---------\n"\
             "c\tCreate a new list\n"\
             "p\tPrint list\n"\
             "i\tInsert element into list\n"\
             "d\tDelete element from list\n"\
             "h\tDisplay this help menu\n"\
             "q\tQuit\n"
#define INVALID_INPUT "?\n"
#define BUFF 256
#define PROMPT ">> "

/* main routine to test the functions defined */
int main (void)
{
    int i, x, y;
    int choice, index, to_be_inserted;
    int initial_size, *initial_list; 

    node_t *l = NULL; /* list is initially empty */
    char cmd[BUFF], input[BUFF]; /* buffers to store user input */

    printf ("\nImplementation of a Circular Linked Linear List\n");
    printf (MENU);
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
                    if (initial_size == 0) {
                        fprintf (stderr, "!Provide a non zero size\n");
                        continue;
                    }
                    initial_list = (int *) malloc (sizeof (int) * initial_size); 
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
                            if (insert_at_end (&l, to_be_inserted)) {
                                printf ("Element '%d' has been added "
                                        "to end.\n", to_be_inserted);
                            }
                            break;
                        case 3:
                            printf ("N = ? ");
                            GET_NUM (input, index);
                            x = insert_after_index (&l, to_be_inserted, index);
                            if (x == 1) { 
                                printf ("Element '%d' has been added after "
                                        "index %d.\n", to_be_inserted, index);
                            }
                            else if (x == -1) {
                                fprintf (stderr, "!Index out of bounds.\n");
                            }
                            break;

                        default:
                            fprintf (stderr, INVALID_INPUT);
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
                                printf ("Element '%d' has been removed " 
                                        "from beginning.\n", x); 
                            }
                            break; 
                        case 2:
                            if (delete_at_end (&l, &x)) {
                                printf ("Element '%d' has been removed "
                                        "from the end.\n", x);
                            }
                            break;
                        case 3:
                            printf ("N = ? ");
                            GET_NUM (input, index); 
                            y = delete_at_index (&l,index, &x);
                            if (y == 1) { 
                                printf ("Element '%d' has been removed "
                                        "at index %d.\n", x, index);
                            }
                            else if (y == -1) {
                                fprintf (stderr, "!Index out of bounds.\n");
                            }
                            break;

                        default:
                            fprintf (stderr, INVALID_INPUT);
                    } 
                    break;

                case 'q':
                    if (l != NULL) { /* deallocate the list only 
                                        if it exists */
                        destroy_list (l);
                        printf ("List destroyed.\n");
                    }
                    printf ("Exiting process.\n");
                    exit (0);

                default: 
                    fprintf (stderr, INVALID_INPUT);
                case 'h':
                    printf (MENU);
                    continue;
            }
        }
        else {
            fprintf (stderr, "!IO Error. Exiting process.\n");
            exit (1); 
        }
    }
    exit (0);
}
