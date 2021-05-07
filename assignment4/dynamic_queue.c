/* dynamic_queue.c: C program to implement a queue using dynamic memory 
   allocation. This has the advantage of avoiding overflow. We would
   still need to check for underflow. 

   For this we are going to be using a single linked list implementation.
   In order to maintain the FIFO behaviour we will need to add to the 
   end of the list and remove from the beginning. Thus queue operations
   become list operations:
    1. enqueue() = insert_at_end()
    2. dequeue() = delete_from_start()
*/ 
#include <stdio.h>
#include <stdlib.h>

typedef struct node { /* defintion for a node */
    int info;
    struct node *link;
} node_t;

typedef struct { /* header node for the queue storing front and rear */
    struct node *front; 
    struct node *rear;
} queue_t ;

/* check if the queue is empty */
int is_empty (queue_t *q)
{
    return (q->front == NULL) && (q->rear == NULL);
}

/* display the contents of the queue */
int display (queue_t *q)
{
   node_t *p;
   if (is_empty (q)) {
       printf ("Queue is empty.\n");
       return 0;
   }
   p = q->front; /* queue = everything between front and rear */
   while (p != NULL) {
       printf ("%d ", p->info);
       p = p->link;
   }
   putchar ('\n');
   return 1;
}

/* insert element into the rear of the queue */
int enqueue (queue_t *q, int info)
{
   node_t *newnode;
   newnode = (node_t *) malloc (sizeof (node_t));
   if (newnode == NULL) {
       fprintf (stderr, "malloc() failed to allocate");
       return 0;
   }
   newnode->info = info;
   newnode->link = NULL;
   if (is_empty (q)) { /* for empty queue, make front point to new node */
       q->front = newnode;
   }
   else { /* for exisiting queue, make previous node point to new */
       q->rear->link = newnode;
   }
   q->rear = newnode;
   return 1;
}

/* remove elements from the front of the queue */
int dequeue (queue_t *q, int *removed)
{
    node_t *tmp;
    int only_one_node;
    if (is_empty (q)) { /* underflow, cannot dequeue */
        return 0;
    }
    only_one_node = (q->front == q->rear);
    tmp = q->front;
    q->front = q->front->link;
    *removed = tmp->info;
    free (tmp);
    if (only_one_node) 
        q->rear = NULL;
    return 1;
}

/* deallocate undequeued memeory to prevent memory leaks */
int destroy_queue (queue_t *q)
{
    node_t *a, *tmp;
    if (is_empty (q)) { /* nothing to free */
        return 0;
    }
    a = q->front;
    while (a != NULL) {
        tmp = a;
        a = a->link;
        free (tmp);
    }
    q->front = q->rear = NULL;  /* set header to make queue empty */
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

/* main routine to test the queue implementation */
int main (void)
{
    char input[BUFF];
    int to_be_enqueued, dequeued;
    queue_t q = {NULL, NULL};

    printf ("Implementation of a dynamically sized queue.\n");
    printf (MENU);
    while (1) 
    {
        printf (PROMPT);
        if (fgets (input, BUFF, stdin)) 
        {
            switch (input[0]) {
                case 'i':
                    printf ("Insert what?\n");
                    if (!getnum (input, BUFF, &to_be_enqueued)) { 
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
