/* queue.h: include information for queue implementation using single linked lists */

#ifndef MY_QUEUE_H      /* header include guard */
#define MY_QUEUE_H 1

/* struct definitions */
typedef struct node {
    int info;
    struct node *link;
} node_t;

typedef struct {
    struct node *front; 
    struct node *rear;
} queue_t ;

/* function declarations */
int is_empty (queue_t *);
int display (queue_t *);
int enqueue (queue_t *, int );
int dequeue (queue_t *, int *);
int destroy_queue (queue_t *);

#endif 
