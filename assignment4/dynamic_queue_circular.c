/* 
   dynamic_queue_circular.c: C program to implement a dynamic queue using 
   a circular linked list. With a circular linked list we have the 
   advantage of being able to access the front and the rear at all times.
   The front of the queue then is simply the next node after the rear.
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int info;
    struct node *link;
} queue_t;

/* check if the queue is empty */
int is_empty (queue_t *q)
{
    return (q == NULL);
}

/* display the conents of the queue */
int display (queue_t *q)
{
    queue_t *front, *p;
    if (is_empty (q)) {
        printf ("Queue is empty.\n");
        return 0;
    }
    p = front = q->link; /* start from the front */
    do {
        printf ("%d ", p->info);
        p = p->link;
    } while (p != front); /* iterate till the front is encountered again */
    putchar ('\n');
    return 1;    
}

/* insert elements at the front fo the queue:
   same as inserting at the end of a circular list */
int enqueue (queue_t **q, int info)
{
    queue_t *newnode;
    newnode = (queue_t *) malloc (sizeof (queue_t));
    if (newnode == NULL) {
       fprintf (stderr, "malloc() failed to allocate");
       return 0;
    }
    newnode->info = info;
    if (is_empty (*q)) {
        newnode->link = newnode;
        *q = newnode;
        return 1;
    }
    newnode->link = (*q)->link;
    (*q)->link = newnode;
    *q = (*q)->link;
    return 1;
}

/* remove elements from the end of the queue: 
   same as removing elements from the beginning of the circular list */
int dequeue (queue_t **q, int *removed)
{
    queue_t *front;
    if (is_empty (*q)) { /* empty */
        return 0;
    }
    if (*q == (*q)->link) { /* only one node */
        *removed = (*q)->info;
        free (*q);
        *q = NULL;
        return 1;
    } 
    front = (*q)->link;
    (*q)->link = front->link;
    *removed = front->info;
    free (front);
    return 1;
}

/* deallocate remaining nodes in the queue */
int destroy_queue (queue_t **q) 
{
    queue_t *p, *tmp, *front;
    if (is_empty (*q)) {
        return 0; /* nothing to free */
    }
    p = front = (*q)->link; /* start from the front */
    do {
        tmp = p->link;
        free (p);
        p = tmp;
    } while (p != front);
    *q = NULL;
    return 1;
}
                          /* end of implementation */
#define BUFF 256
#define PROMPT ">>> "
#define INVALID_INPUT "?\n"
#define IOE "!IO Error.\n"
#define MENU "Commands:\n"\
             "=========\n"\
             "i\tInsert element into queue\n"\
             "d\tDelete element from queue\n"\
             "p\tPrint queue\n"\
             "h\tDisplay this help menu\n"\
             "q\tQuit\n"

/* read use input from stdin and convert to numeric value, if
   unsuccessful return 0 */
int get_num (char *buff, size_t size, int *var)
{
    if (fgets (buff, size, stdin) == NULL) {
        fprintf (stderr, IOE);
        return 0;
    }
    else if (sscanf (buff, "%d", var) == 0) {
        fprintf (stderr, INVALID_INPUT);
        return 0;
    }
    return 1;
}

/* main routine to test the queue implementation */
int main (void)
{
    char input[BUFF];
    int to_be_enqueued, dequeued;
    queue_t *q = NULL;

    printf ("Alternate implementation of a dynamically sized queue.\n");
    printf (MENU);
    while (1) 
    {
        printf (PROMPT);
        if (fgets (input, BUFF, stdin)) 
        {
            switch (input[0]) {
                case 'i':
                    printf ("Insert what?\n");
                    if (!get_num (input, BUFF, &to_be_enqueued)) { 
                        continue;
                    }
                    else {
                        if (enqueue (&q, to_be_enqueued)) {
                            printf ("Element '%d' has been inserted "
                                    "into the queue.\n", to_be_enqueued);
                        } else {
                            printf ("Queue overflow: queue is full. "
                                    "Cannot insert new element.\n");
                        }
                    }
                    break;

                case 'd':
                    if (dequeue (&q, &dequeued)) {
                        printf ("Element '%d' has been removed "
                                "from the queue.\n", dequeued);
                    } else {
                        printf ("Queue underflow: queue is empty. "
                                "Cannot remove any element.\n");
                    } 
                    break;

                case 'q':
                    if (destroy_queue (&q)) { 
                        printf ("!Queue destroyed.\n");
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
