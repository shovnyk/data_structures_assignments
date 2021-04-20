/* cq.c: C program to implement a Circular Queue
   
   a Circular Queue, like a Queue, is also a (First  In First Out) data 
   structture which  allows the total size  of the array (in  the array 
   implementation) to be used, while keeping  the time of access of the 
   first  and  last elements  the  same.  Thus  it  is space  and  time 
   efficient.                                                           
*/
#include <stdio.h>
#include <stdlib.h>

/* allow size to be changed via preprocessor before compiling */
#ifndef CAPACITY 
#define CAPACITY 100
#endif

typedef struct queue {
    int front;
    int rear;
    int item[CAPACITY];
} queue_t;

/* check if the queue is full */
int is_full (queue_t *q)
{
    /* 2 cases in which queue may be full: */
    int case1 = (q->front == (q->rear + 1)); 
    int case2 =  ((q->front == 0) && (q->rear == (CAPACITY - 1)));
    return case1 || case2;
}

/* check if the queue is empty */
int is_empty (queue_t *q)
{
    /* before removing only element, we set both front and rear
       to -1. But front is never -1, again so we only check if
       front is -1 */
    return (q->front == -1);
}

/* display the contents of the queue, everything between front and rear */
int display (queue_t *q) 
{
    int i;
    if (is_empty (q)) {
        printf ("Queue is empty\n");
        return 0;
    }
    if (q->rear >= q->front) {
       for (i = q->front; i <= q->rear; i++) {
            printf ("%d ", (q->item)[i]);
       }
    }
    /* for linear queue, rear always >= front, here not always so.
       In such cases, we need to print to the end and loop around 
       back to rear */
    else {
        i = q->front;
        do {
            printf ("%d ", (q->item)[i]);
            i = (i + 1) % CAPACITY;
        } 
        while (i != (q->rear + 1));
    }
    putchar('\n');
    return 1;
}

/* insert an element at the rear of the queue */
int enqueue (queue_t *q, int data)
{
    if (is_full (q)) { /* queue overflow */
        return 0;
    }
    /* arithmetic mmodulo operation resets rear to 0 after it has 
       attained the value CAPACITY - 1 */
    q->rear = (q->rear + 1) % CAPACITY;
    (q->item)[q->rear] = data; 
    if (q->front == -1) { /* if empty, increment front */
        q->front = 0;
    }
    return 1;
}

/* remove an element from the front of the queue */
int dequeue (queue_t *q, int *removed)
{
    if (is_empty (q)) { /* queue underflow */
        return 0;
    }
    *removed = (q->item)[q->front]; 
    if (q->front == q->rear) { /* queue has only one element */
    /* set front and rear to -1 to indicate empty list */
        q->front = -1;
        q->rear = -1;
    }
    else  /* front should loop around back to the end */
        q->front = (q->front + 1) % CAPACITY; 
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
    queue_t q = { /* initially, we set the queue to be empty: */
        -1,       /* front = -1, */ 
        -1,       /* rear  = -1, */
        {0}       /* say all the elements are zero initially */
    }; 
    printf ("Implementation of a Circular Queue of fixed size " 
            "= %d.\n", CAPACITY);
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
