/* bfs.c:
   C program to implement Breadth-First-Search,  which will start at any
   node and  then explore all other  nodes at a given  distance and then
   explore the  nodes connected  to the starting  node, if  they haven't
   already been  explored. This process  will continue until all  of the
   nodes of the graph have been explored.
*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>

/* queue implementation */
#include "queue.c"

/* nodes will be colored based on how much they have been explored:
        1. not discovered = white
        2. discovered and exploring = gray
        3. explored = black */
typedef enum {
    white, gray, black
} color_t;

#define INFINITY INT_MAX
#define EMPTY NULL

#define VERTEX_FMT "%s"
typedef char * vertex_t;

/* define a structure for a Graph for encapsulating vertices and edges */
typedef struct {
    int no_of_vertices;
    vertex_t *V;
    int **Adj;
} graph_t;

/* structure for the BFS tree */
typedef struct {
    int *dist;
    int *parent;
} bfs_tree_t;

/* perform linear search on a given array */
int linsearch (int *arr, int len, int key) {
    if (arr == NULL)
        return -1;
    int i;
    for (i = 0; i < len; i++) {
        if (arr[i] == key) {
            return i;
        }
    }
    return -1;
}

/* print the BFS spanning tree recursively in a directory-tree style */
void bfs_tree_print (graph_t *G, bfs_tree_t *tree, int root)
{
#define ONE_INDENT "  "
#define LEVEL_PREFIX "+*-"
#define LEVEL_PREFIX_N 3
    int i, tmp;
    for (i = 0; i < (tree->dist)[root]; i++) {
        fputs (ONE_INDENT, stdout);
    }
    putchar (LEVEL_PREFIX[tree->dist[root] % LEVEL_PREFIX_N]);
    printf (" " VERTEX_FMT "\n", (G->V)[root]);

    i = linsearch (tree->parent, (G->no_of_vertices), root);
    while (i != -1)
    {
        bfs_tree_print (G, tree, i);
        if (i == (G->no_of_vertices - 1)) { 
            break; /* if at last index, do not increment further */
        }
        i++;
        tmp = linsearch (tree->parent + i, G->no_of_vertices - i, root);
        i = (tmp == -1) ? -1 : i + tmp;
    }
}

/* Breadth-First-Search: create and return a list obtained from BFS of the 
   tree and also simultaneously grow a BFS tree. Show some intermediate 
   outputs */
int *bfs (graph_t *G, int s, bfs_tree_t *tree)
{
    int i, u, v, j = 0;
    
    int *bfs_order = (int *)malloc (sizeof (int) * G->no_of_vertices);
    if (bfs_order == NULL) 
        return NULL;

    if (tree == NULL) {
        tree = (bfs_tree_t*)malloc (sizeof (bfs_tree_t));
    }
    tree->dist = (int *)malloc (sizeof (int) * G->no_of_vertices);
    tree->parent = (int *)malloc (sizeof (int) * G->no_of_vertices);
    color_t *color = (color_t *)malloc (sizeof (color_t) * G->no_of_vertices);

    if (tree->dist == NULL || tree->parent == NULL || color == NULL)
        return NULL;

    /* intialize for all vertices except source vertex */
    for (u = 0; u < G->no_of_vertices; u++) {
        if (u == s) continue;
        color[u] = white;
        (tree->dist)[u] = INFINITY;
        (tree->parent)[u] = -1;
    } 
    /* now initialize for source */
    color[s] = gray;
    (tree->dist)[s] = 0;
    (tree->parent)[s] = -1;

    /* start with an empty queue */
    queue_t *q = queue_init ();
    enqueue (q, s);

    /* start exploring and show intermediate outputs */
    while (!is_empty (q))
    {
        queue_print ("Contents of the queue: ", q, "\n");
        dequeue (q, &u);
        bfs_order[j++] = u; /* store into list */ 
        printf ("Exploring node: " VERTEX_FMT "...\n", (G->V)[u]);
        
        for (i = 0; v = (G->Adj)[u][i], v != -1; i++) {
            if (color[v] == white) {
                color[v]= gray;
                (tree->dist)[v] = (tree->dist)[u] + 1;
                (tree->parent)[v] = u;
                enqueue (q, v); 
                printf ("\tDiscovered vertex: " VERTEX_FMT "\n", (G->V)[v]);
            }
        }
        color[u] = black; 
        printf ("Finished exploring node: " VERTEX_FMT ".\n\n", (G->V)[u]);
    }
    /* free allocated memory wherever necessary */
    free (color); /* queue is already empty: no need to free */
    return bfs_order;
}

/* build an adjacency list for a certain vertex. Terminate each list 
   with a -1 */
int *make_adjacency_list (int count, ...)
{ 
    int i;
    va_list ap;
    int *adj = (int *) malloc ((count + 1) * sizeof (int));
    if (adj == NULL)
        return adj;
    va_start (ap, count);
    for (i = 0; i < count; i++) {
        adj[i] = va_arg (ap, int);
    }
    adj[i++] = -1;
    return adj;
}
                          /* end of implementation */
int main (void)
{
    int **Adj;
    int source, nV;
    vertex_t V[] = {"A", "B", "C", "D", "E", "F", "G", "H", NULL};
    nV = (sizeof (V)) / sizeof (V[0]) - 1;

    /* build the adjacency lists for the graph specification */
    Adj = (int **) malloc (sizeof (int *) * nV);
    Adj[0] = make_adjacency_list (3, 2, 3, 6);
    Adj[1] = make_adjacency_list (2, 3, 4);
    Adj[2] = make_adjacency_list (2, 0, 7);
    Adj[3] = make_adjacency_list (4, 0, 1, 4, 7);
    Adj[4] = make_adjacency_list (3, 1, 3, 5);
    Adj[5] = make_adjacency_list (3, 4, 6, 7);
    Adj[6] = make_adjacency_list (2, 0, 5);
    Adj[7] = make_adjacency_list (3, 2, 3, 5);

    /* build the graph from the vertex and adjacency lists */
    graph_t G = {nV, V, Adj};

    /* BFS the graph starting from certain node and grow the BFS tree */
    bfs_tree_t t = {NULL, NULL};
    source = 4; 
    int *bfs_order = bfs (&G, source, &t);
    
    /* print the nodes in proper BFS order */
    printf ("BFS Order: ");
    for (int i = 0; i < nV; i++)
        printf ("%s ", G.V[bfs_order[i]]);
    putchar ('\n');

    /* print the final BFS tree starting at node source */
    printf ("BFS Tree:\n");
    bfs_tree_print (&G, &t, source);

    /* deallocate memory when done */
    free (bfs_order);
    free (t.dist);
    free (t.parent);
    for (int i = 0; i < nV; i++) 
        free (G.Adj[i]);
    free (G.Adj);
    return 0;
}
