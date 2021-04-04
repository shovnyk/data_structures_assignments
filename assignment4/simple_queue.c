/*  simple_queue.c: C program to implement a fixed sized queue without 
    shifting operation of the elements. 

    A queue is a list like data structure which allows first in first out
    or 'FIFO' access to its elements. It is inspired by queues that form in
    real life and are effective for addressing problems which exhibit such
    patterns.  As such it has two primitive operations:

     i. insertion/enqueue() - which inserts elements at the end or 'rear'
    ii. deletiton/dequeue() - which removes elements from the FRONT
*/
#include <stdio.h>
#include <stdlib.h>

#define CAPACTITY 10 /* define a fixed size for the queue */

typedef struct queue {
    int front;
    int rear;
    int item[CAPACTITY];
} queue_t;

/* check if the queue is empty */
int is_empty (queue_t *q)
{
    /* the queue can be empty in two cases: */
    return (q->front == -1) /* either it has just been initialized */
        || (q->front == q->rear + 1); /* or all elements have been deleted */
}

/* check id the queue is full */
int is_full (queue_t *q)
{
    return (q->rear == (CAPACTITY - 1)); /* the rear points to last index */
}

/* print the contents of the queue from front to rear */
int display (queue_t *q)
{
    int i;
    if (is_empty (q)) {
        printf ("Queue is empty.\n");
        return 0;
    }
    /* at any point of time the queue is effectively all the elements
       between front and rear (inclusive) */
    for (i = q->front; i <= q->rear; i++) {
        printf ("%d ", (q->item)[i]);
    }
    putchar ('\n');
    return 1;
}

/* insert an element into the rear of the queue */
int enqueue (queue_t *q, int info) 
{
    if (is_full (q)) { /* overflow */
        return 0;
    }
    if (is_empty (q)) {
        (q->front)++;  
        /* if first element is being added, increment front */
    }
    /* move rear one position ahead and store new value */
    /* pre increment allows us to do in a single step */
    (q->item)[++(q->rear)] = info;  
    return 1; 
}

/* delete an element from the front of the queue */
int dequeue (queue_t *q, int *removed)
{
    if (is_empty (q)) { /* underflow */
        return 0;
    }
    /* increment front by one so the previous value is effectivley lost */
    /* post increment allows us to store the value before incrementing */
    *removed = (q->item)[(q->front)++]; 
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
    queue_t q = { /* initially, */
        -1,       /* front = -1 */ 
        -1,       /* rear  = -1 */
        {0}       /* queue is empty */
    };

    printf ("Implementation of a fixed sized queue.\n");
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
                    printf ("!Exiting process.\n");
                    exit (0);

                case 'p':
                    display (&q);
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
