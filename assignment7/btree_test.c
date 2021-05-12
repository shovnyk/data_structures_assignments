#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node *left;
    struct node *right;
} bnode_t;

/* allocate memory for a leaf node and initialize it to a given data */
bnode_t *getnode (int data)
{
    bnode_t *tmp = (bnode_t *) malloc (sizeof (bnode_t));
    if (tmp == NULL)
        return NULL;
    tmp->data = data;
    tmp->right = tmp->left = NULL;
    return tmp;
}

/* check if a node has 0 children */
int is_leaf (bnode_t *root)
{
    return (root->left == NULL) && (root->right == NULL);
}

/* helper  function for  the  is_strict and  is_complete functions  that
   counts the number  of leaf and non  leaf nodes in a  binary tree. For
   this we have used preorder traversal */
void count_nodes (bnode_t *root, int *leaf, int *nonleaf)
{
    if (root == NULL)
        return;
    if (is_leaf (root)) (*leaf)++; /* first check if current node is leaf */
    else (*nonleaf)++;
    count_nodes (root->left, leaf, nonleaf); /* count in left subtree */
    count_nodes (root->right, leaf, nonleaf); /* count in right subtree */
}

/* check  if a  tree is  a strictly  binary tree  using the  property: a
   strictly binary tree with N non leaf nodes has N + 1 leaf nodes */
int is_strict (bnode_t *root)
{
    int case1, case2;
    int leaf_nodes = 0;
    int nonleaf_nodes = 0;
    count_nodes (root, &leaf_nodes, &nonleaf_nodes);

    /* empty binary tree is a strict binary tree */
    case1 = (leaf_nodes + nonleaf_nodes == 0);

    /* property of strict binary tree: the number of leaf nodes
       should be one more than the number of nodes */
    case2 = (leaf_nodes == (nonleaf_nodes + 1));
    return case1 || case2;
}

/* free nodes of the binary tree using post order traversal */
int is_complete (bnode_t *root, int index, int nodes)
{
   if (root == NULL) /* empty binary tree is complete */
       return 1;
   else if (index >= nodes) 
       return 0;
   /* check if the left and right subtrees are complete */
   return is_complete (root->left, 2*index + 1, nodes) 
       && is_complete (root->right, 2*index + 2, nodes);
}

/* print information to stdout regarding whether the tree is strictly 
   binary or complete */
void btree_info (bnode_t *root)
{
    int leaf_nodes, nonleaf_nodes, total_nodes;

    /* check if the tree is strictly binary */
    if (is_strict (root))
        printf ("Strictly binary ");
    else 
        printf ("Not strictly binary ");

    leaf_nodes = nonleaf_nodes = total_nodes = 0;
    count_nodes (root, &leaf_nodes, &nonleaf_nodes);
    total_nodes = leaf_nodes + nonleaf_nodes;

    if (is_complete (root, 0, total_nodes))
        printf ("and complete.\n");
    else 
        printf ("and not complete.\n");
}

/* free nodes of exisiting binary tree using post order traversal */
void destroy_btree (bnode_t *root)
{
    if (root == NULL)
        return;
    destroy_btree (root->left);
    destroy_btree (root->right);
    free (root);
}

int main (void)
{
    bnode_t *root = NULL; /* start with an empty tree */

    btree_info (root); /* check for an empty tree */

    /* hardcode values for the first binary tree */
    root = getnode (200);
    root->left = getnode (300);
    root->right = getnode (50);
    root->left->left = getnode (10);
    root->left->right = getnode (90);
    btree_info (root);

    /* insert another node to the right of 50 and check again */
    root->right->right = getnode (777);
    btree_info (root);

    destroy_btree (root); /* delete current tree before creating a new one */

    root =  getnode (1);
    root->left = getnode (2);
    root->left->left = getnode (4);
    root->right = getnode (3);
    root->right->right = getnode (5);
    btree_info (root);

    destroy_btree (root);
    return 0;
}
