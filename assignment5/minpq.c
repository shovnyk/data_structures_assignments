/* minpq.c: C program to implement a Minimum Priority Queue (PQ) using a
   singly linked list.  

   Unlike  a regular  queue, a  priority queue  does not  always provide
   FIFO  access. Instead,  in general  it always  accepts (enqueues)  an
   object  with some  indicator  of its  priority. When  it  is time  to
   remove((dequeu)  an object,  the  one with  the  highest priority  is
   removed  first. FIFO  rule comes  into play  when there  are multiple
   objects of  the same  priority. PQs  are typically  implemented using
   Heaps (a special kind  of Tree) but here we will  use a Singly Linked
   List.

   If we present  the rank using an  integer than we can  say that LOWER
   the value  of the integer, HIGHER  is its priority. Thus  in order to
   maitain the  min PQ we  must keep the list  in sorted order  with the
   node with  the highest  value of  rank, i.e.  lowest priority  at the
   beginning.

   Primitive operations: 
   ---------------------
   1. insert or enqueue: insert element into the PQ based on priority
   2. min_extract: extract the element with minimum priority (max rank)
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int rank;
    int data;
    struct node *next;
} queue_t;

/* check if the PQ is empty */
int is_empty (queue_t *q)
{
    return (q == NULL);
}

/* display the contents of the queue */
int display (queue_t *q)
{
    queue_t *p;
    if (is_empty (q)) {
        printf ("Queue is empty\n");
        return 0;
    }
    printf ("%5s\t%5s\n%5s\t%5s\n",
            "Rank","Data",
            "----","----");
    for (p = q; p != NULL; p = p->next) {
        printf ("%5d\t%5d\n", p->rank, p->data);
    }
    return 1;
}

/* insert and element in sorted order in the linked list */
int enqueue (queue_t **q, int data, int rank)
{
    /* dynamic allocation => overflow check not required */
    struct node *newnode, *p;
    newnode = (struct node *) malloc (sizeof (struct node*));
    if (newnode == NULL) {
        fprintf (stderr, "!malloc(); failed to allocate.\n");
        return 0;
    }
    newnode->data = data;
    newnode->rank = rank;

    /* case 1:  either first element is being inserted  or element being
                inserted has minimum priority */
    if (is_empty (*q) || (rank > (*q)->rank))
    {
        newnode->next = *q;
        *q = newnode;
        return 1;
    }

    /* case  2: iterate through  thel list  and find the  element having
                rank just higher than the one to be inserted and enqueue
                after it */
    p = *q;
    while ((p->next != NULL) &&
            (p->next->rank >= rank))
    {
        /* because of traversal over the entire list, in the worst case,
           this is an O(n) operation */
        p = p->next;
    }
    newnode->next = p->next;
    p->next = newnode;
    return 1;
}

/* remove min element from the PQ*/
int extract_min (queue_t **q, int *removed)
{
    /* because the list is already  sorted, this is a constant time O(1)
       operation */
    struct node *tmp;
    if (is_empty (*q)) { /* queue underflow */
        return 0;
    }
    /* this operation is equivalent to  removing from the beginning of a
       SLL */
    *removed = (*q)->data;
    tmp = (*q);
    *q = (*q)->next;
    free (tmp);
    return 1;
}

/* deallocate nodes in the PQ, not yet freed. */
int destroy_queue (queue_t *q)
{
    queue_t *p, *tmp;
    if (q == NULL) {
        return 0;
    }
    p = q;
    while (p != NULL) {
        tmp = p;
        p = p->next;
        free (tmp);
    }
    return 1;
}
                          /* end of implementation */
#define BUFF 256
#define PROMPT ">>> "
#define INVALID_INPUT "?\n"
#define IOE "!IO Error.\n"
#define MENU "Commands:\n"\
             "=========\n"\
             "i\tInsert element and set priority, into the queue\n"\
             "d\tExtract minimum element from the queue\n"\
             "p\tPrint queue\n"\
             "h\tDisplay this help menu\n"\
             "q\tQuit\n"

/* read use input from stdin and convert to numeric value, if
   unsuccessful return 0 */
int getnum (char *buff, size_t size, int *var)
{
    if (fgets (buff, size, stdin) == NULL) {
        fprintf (stderr, IOE);
        return 0;
    }
    else if (sscanf (buff, "%d", var) != 1) {
        fprintf (stderr, INVALID_INPUT);
        return 0;
    }
    return 1;
}

/* main routine to test the priority queue implementation */
int main (void)
{
    queue_t *q = NULL;
    char input[BUFF];
    int to_be_inserted, priority, min;
    printf ("Implementation of a Minimum Priority Queue using "
            "a Single Linked List.\n"
            "minimum priority = maximum rank\n");
    printf (MENU);
    while (1)
    {
        printf (PROMPT);
        if (fgets (input, BUFF, stdin))
        {
            switch (input[0]) {
                case 'i':
                    printf ("Insert what?\n");
                    if (!getnum (input, BUFF, &to_be_inserted)) {
                        continue;
                    }
                    printf ("What should be the priority of '%d'? ",
                            to_be_inserted);
                    if (!getnum (input, BUFF, &priority)) {
                        continue;
                    }
                    else {
                        if (enqueue (&q, to_be_inserted, priority)) {
                            printf ("Element '%d' has been inserted "
                                    "into the queue.\n", to_be_inserted);
                        } else {
                            printf ("Queue overflow: queue is full. "
                                    "Cannot insert new element.\n");
                        }
                    }
                    break;

                case 'd':
                    if (extract_min (&q, &min)) {
                        printf ("Minimum element '%d' has been extracted "
                                "from the queue.\n", min);
                    } else {
                        printf ("Queue underflow: queue is empty. "
                                "Cannot extract any element.\n");
                    }
                    break;

                case 'q':
                    if (destroy_queue (q)) {
                        printf ("!Destroyed exisiting queue.\n");
                    }
                    printf ("!Exiting process.\n");
                    exit (0);

                case 'p':
                    display (q);
                    break;

                default:
                    fprintf (stderr, INVALID_INPUT);
                case 'h':
                    printf (MENU);
                    break;
            }
        }
        else {
            fprintf (stderr, IOE);
            continue;
        }
    }
}
