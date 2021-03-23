/* double_circular.c: 
   C program to implement a doubly linked circular list */      

#include <stdio.h> 
#include <stdlib.h> 

#define  RETURN_IF_NULL(x,y) if (x == NULL) {\
    fprintf (stderr, "malloc(): failed to allocate memory\n");\
    return y;\
}
#define CHECK_UNDERFLOW(x) if (is_empty (x)) {\
    fprintf (stderr, "!Underflow. List is empty. Cannot delete.\n");\
    return 0;\
}

#define new(x) (x*)malloc (sizeof (x))

typedef struct node { 
    int info;
    struct node *prev;
    struct node *next;
} node_t;

int is_empty (node_t *start)
{
    return (start == NULL);
}

/* initialize a linked list with data collected in an array */
node_t *create_list (int *arr, int len)
{
    int i;
    node_t *p, *temp, *start;
    start = new (node_t);
    RETURN_IF_NULL (start, NULL);
    p = start;
    p->info = arr[0];
    for (i = 1; i < len; i++)
    {
        temp = new (node_t);
        RETURN_IF_NULL (temp, NULL);
        temp->info = arr[i];
        temp->prev = p;
        p->next = temp;
        p = temp;
    }
    p->next = start; /* next pointer of last node points to
                        first node */
    start->prev = p; /* prev pointer of first node points to
                        last node */
    return start;
}

void display (node_t *start)
{
    node_t *p;
    if (is_empty (start)) {
        printf ("List empty.\n");
        return;
    }
    p = start;
    do { /* start from the beginning */
        printf ("%d ", p->info);
        p = p->next;
    } while (p != start); /* stop upon returning to beginning */
    putchar ('\n');
    return;
}

/* routine to handle insertion if the list is empty */
int insert_into_empty (node_t **start, int info)
{
    node_t *newnode = new (node_t);
    RETURN_IF_NULL (newnode, 0);
    newnode->info = info;
    *start = newnode;
    newnode->next = newnode;
    newnode->prev = newnode;
    return 1;
}

int insert_at_start (node_t **start, int info)
{
    node_t *newnode, *end;
    if (is_empty (*start)) {
        return insert_into_empty (start, info);
    }
    newnode  = new (node_t);
    RETURN_IF_NULL (newnode, 0);
    end = (*start)->prev;
    newnode->info = info;
    newnode->next = *start;
    (*start)->prev = newnode;
    newnode->prev = end;
    end->next = newnode;
    *start = newnode;
    return 1;
}

/* because we have access to the end, we can can insert an element
   in constant time. This is an advantage of circular lists. */
int insert_at_end (node_t **start, int info)
{
    node_t *newnode, *end;
    if (is_empty (*start)) {
        return insert_into_empty (start, info);
    }
    newnode = new (node_t);
    RETURN_IF_NULL (newnode, 0);
    end = (*start)->prev;
    newnode->info = info;
    end->next = newnode;
    newnode->prev = end;
    newnode->next = *start;
    (*start)->prev = newnode;
    return 1;
}

int insert_after_index (node_t **start, int info, size_t index)
{
    size_t n;
    node_t *newnode, *p, *end;
    if (is_empty (*start)) {
        return -1; /* -1 means out of bounds */
    }
    newnode = new (node_t);
    RETURN_IF_NULL (newnode, 0);
    end = (*start)->prev;
    newnode->info = info;
    p = *start; n = 0;
    while (p != end) {
        if (n++ == index) {
            newnode->prev = p;
            newnode->next = p->next;
            (p->next)->prev = newnode;
            p->next = newnode;
            return 1;
        }
        p = p->next;
    }
    if (n == index) { /* index is at the last node */ 
        newnode->prev = p;
        p->next = newnode; 
        newnode->next = *start;
        (*start)->prev = newnode; 
        return 1;
    }
    free (newnode);
    return -1;  /* index out of bounds, could not be inserted */
}

int delete_at_start (node_t **start, int *removed)
{
    node_t *end;
    CHECK_UNDERFLOW (*start); /* report underflow if empty */
    if (*start == (*start)->next) { /* if only one node */
        *removed = (*start)->info;
        free (*start);
        *start = NULL;
        return 1;
    }
    end = (*start)->prev;
    *removed = (*start)->info;
    *start = (*start)->next;
    free ((*start)->prev);
    (*start)->prev = end;
    end->next = *start;
    return 1;
}

/* we have access to the end, so its removal is a constant time 
   operation */
int delete_at_end (node_t **start, int *removed)
{
    node_t *secondlast, *end;
    CHECK_UNDERFLOW (*start); /* if list is empty  */
    if (*start == (*start)->next) { /* if only one node */
        *removed = (*start)->info;
        free (*start);
        *start = NULL;
        return 1;
    }
    end = (*start)->prev;
    secondlast = (end->prev);
    (*start)->prev = secondlast;
    secondlast->next = (*start);
    *removed = end->info;
    free (end);
    return 1;
}

int delete_at_index (node_t **start, size_t index, int *removed)
{
    size_t n;
    node_t *p, *end;
    if (index == 0) {
        return delete_at_start (start, removed);
    } 
    CHECK_UNDERFLOW (*start); 
    end = (*start)->prev;
    p = *start; n = 0;
    while (p != end) {
        if (n++ == index) {
            (p->prev)->next = p->next;
            (p->next)->prev = p->prev;
            *removed = p->info;
            free (p);
            return 1;
        }
        p = p->next;
    }
    if (n == index) { /* delete from the end */
        (p->prev)->next = *start;
        (*start)->prev = p->prev;
        *removed = p->info;
        free (p);
        return 1;
    }
    return -1;  /* index out of bounds, could not be inserted */
}

/* frees memory of all remaining nodes */
int destroy_list (node_t *start) 
{
    int same, adjc, i;
    node_t *p, *q;
    if (is_empty (start)) { /* nothing to free */
        return 0;
    }
    p = start;              /* start one from the beginning */
    q = start->prev;        /* start one from the end */
    same = adjc = i = 0;
    while (same = (p == q), adjc = (p->next == q), !same && !adjc) {
        /* Keep freeing from both ends of the list. 
           This will on average require half as many runs as the number of
           elements */ 
        p = p->next;    /* move to the next node */
        free (p->prev); /* free the previous node */
        q = q->prev; 
        free (q->next);
    }
    free (p);
    if (adjc) {
        free (q);
    }
    return 1; 
}
                          /* end of implementation */
/* macro to get input from the user and store in variable */
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

    printf ("\nImplementation of a Doubly Linked Circular List\n");
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
