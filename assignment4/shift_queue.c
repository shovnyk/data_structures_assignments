/* 
   shift_queue.c: A fixed memory queue implementation using arrays in which
   when the element is deleted all the elements are deleted, the remaining 
   are shifted to take its place.  
*/
#include <stdio.h>
#include <stdlib.h>

#ifndef CAPACITY 
#define CAPACITY 100
#endif

typedef struct queue {
    /* for this implementation, front is always = 0 */
    int rear;
    int item[CAPACITY];
} queue_t;

/* check if the queue is empty */
int is_empty (queue_t *q)
{
    return (q->rear == -1);
}

/* check id the queue is full */
int is_full (queue_t *q)
{
    return (q->rear == (CAPACITY - 1));
}

/* print the contents of the queue from front to rear */
int display (queue_t *q)
{
    int i;
    if (is_empty (q)) {
        printf ("Queue is empty.\n");
        return 0;
    }
    for (i = 0; i <= q->rear; i++) {
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
    (q->item)[++(q->rear)] = info;  
    return 1; 
}

/* delete an element from the front of the queue. 
   This is of order O(n) */
int dequeue (queue_t *q, int *removed)
{
    int i;
    if (is_empty (q)) { /* underflow */
        return 0;
    }
    *removed = (q->item)[0];  /* store removed element */
    /* shift all the elements to replace the element at the front */
    for (i = 0; i < q->rear; i++) {
        (q->item)[i] = (q->item)[i + 1];
    }
    (q->rear)--; /* update the rear to point to the last element */
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
    queue_t q = { /* initialize the queue */
        -1,       /* rear = -1 */
        {0}       /* set all the values to 0 */
    }; 
    printf ("Alternate implementation of a queue of fixed size = %d.\n",
            CAPACITY);
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
