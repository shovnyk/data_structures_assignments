/* queue.c:
   Implementation of a dynamically sized queue. This has been done using
   a singly linked list  a header node to point to the  start as well as
   the end  of the list  for constant time access  to both the  front as
   well as the end. queue = list:
                     1. enqueue = insert_at_end or append
                     2. dequeue = delete_at_start
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

/* initialize an empty queue */
queue_t *queue_init ()
{
    queue_t *tmp = (queue_t *) malloc (sizeof (queue_t));
    if (tmp == NULL) {
        perror ("malloc");
        return NULL;
    }
    tmp->front = NULL;
    tmp->rear = NULL;
    return tmp;
}

/* display the contents of the queue */
int queue_print (const char *before, queue_t *q, const char *after)
{
   node_t *p;
   if (is_empty (q)) {
       printf ("Queue is empty.\n");
       return 0;
   }
   fputs (before, stdout);
   p = q->front; /* queue = everything between front and rear */
   while (p != NULL) {
       printf ("%d ", p->info);
       p = p->link;
   }
   fputs (after, stdout);
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
