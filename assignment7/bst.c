/* bst.c:
   C program to interactively take user  input to create a binary search
   tree and delete nodes from it.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EMPTY NULL
#define INVALID_INPUT "?\n"
#define BUFF 1024
#define IOE "!IO Error"

typedef struct binary_tree_node {
    int data;
    struct binary_tree_node *left;
    struct binary_tree_node *right;
} bnode_t;

/* allocate memory for new node */
bnode_t *getnode (int data)
{
    bnode_t *newnode = (bnode_t *) malloc (sizeof (bnode_t));
    if (newnode == NULL)
        return NULL;
    newnode->data = data;
    newnode->left = EMPTY;
    newnode->right = EMPTY;
    return newnode;
}

/* insert nodes into the binary tree following the BST property */
bnode_t *bst_insert (bnode_t *root, int data)
{
    if (root == EMPTY) {
        root = getnode (data);
    }
    else if (data <= root->data) {
        root->left = bst_insert (root->left, data);
    }
    else {
        root->right = bst_insert (root->right, data);
    }
    return root;
}

/* create a BST from user input buffer */
bnode_t *bst_create (char *buff, char *delim)
{
    int num;
    bnode_t *root = EMPTY;
    char *token = strtok (buff, delim);
    while (token != NULL) {
        if (sscanf (token, "%d", &num) != 1) {
            fprintf (stderr, INVALID_INPUT);
            return NULL;
        }
        root = bst_insert (root, num);
        token = strtok (NULL, delim);
    }
    return root;
}

/* print the contents of the tree: pre-order traversal */
void bst_print (bnode_t *root)
{
    if (root == EMPTY) {
        printf ("Empty");
        return;
    }
    printf ("%d ", root->data);
    if (root->left != EMPTY) {
        bst_print (root->left);
    }
    if (root->right != EMPTY) {
        bst_print (root->right);
    }
}

/* free nodes of the bst: post-order traversal */
void bst_destroy (bnode_t *root)
{
    if (root == EMPTY)
        return;
    if (root->left != EMPTY) {
        bst_destroy (root->left);
    }
    if (root->right != EMPTY) {
        bst_destroy (root->right);
    }
    free (root);
}

/* search for an item in the BST using binary search */
bnode_t *bst_search (bnode_t *root, int key)
{
    if (root == EMPTY)  /* reached a leaf node but did not find data */
        return NULL;
    if (root->data == key) { /* current node contains data */
        return root;
    }
    if (key < root->data) {  /* search in left subtree */
        return bst_search (root->left, key);
    } 
    return bst_search (root->right, key); /* search in right subtree */
}

/* find the minimum element in the subtree rooted at 'root' */
int find_min (bnode_t *root)
{
    /* minimum element is always present in the left subtree for
       the given root and if there is no left subtree, then the 
       root is itself the minimum element */
    if (root == EMPTY)
        return 0;
    else if (root->left == EMPTY)
        return root->data;
    return find_min (root->left);
}

/* delete a node by its contents; return the updated position of the root */
bnode_t *bst_remove (bnode_t *root, int data)
{
    /* first perform a binary search to find the data */
    int val;
    bnode_t *tmp = NULL;
    if (root == EMPTY)
        return root;
    else if (data < root->data) 
        root->left = bst_remove (root->left, data);
    else if (data > root->data)
        root->right = bst_remove (root->right, data);
    else { 
        /* case 1: no child */
        if (root->left == EMPTY && root->right == EMPTY) {
            printf ("!Deleting leaf node\n");
            free (root);
            root = EMPTY;
        }
        /* case 2: one chld with left subtree empty */
        else if (root->left == EMPTY) {
            printf ("!Deleting element with one child.\n");
            tmp = root;
            root = root->right;
            free (tmp);
        }
        /* case 2: one child with right subtree empty */
        else if (root->right == EMPTY) {
            printf ("!Deleting element with one child.\n");
            tmp = root;
            root = root->left;
            free (tmp);
        }
        /* case 3: two children */
        else {
            printf ("!Deleting element with two children.\n");
            /* 1. find the minimum element in the right subtree */
            val = find_min (root->right);
            /* 2. assign it to the data field of the current node */
            root->data = val;
            /* 3. delete the duplicate from the right subtree */
            root->right = bst_remove (root->right, val);
        }
    }
    return root;
}
                          /* end of implementation */
#define MENU "Commands:\n"\
             "---------\n"\
             "c\tCreate a new BST\n"\
             "d\tDelete a node from the BST\n"\
             "p\tPrint the contents of the BST\n"\
             "h\tDisplay this help menu\n"\
             "q\tQuit\n\n"
#define PROMPT ">> "
#define INVALID_INPUT "?\n"

/* like getchar but print a prompt and consume trailing newlines */
int my_getchar (const char *prompt)
{
    char input[BUFF];
    fputs (prompt, stdout);
    if (fgets (input, BUFF, stdin) == NULL)
        return EOF;
    return input[0];
}

/* interactive program to make, traverse and delete a BST */
int main (void)
{
    char input[BUFF];
    int to_be_deleted;
    bnode_t *root = EMPTY; /* start with an empty tree */
    printf (MENU);
    while (1) {
        switch (my_getchar (PROMPT)) {
            case 'c': 
                if (root != EMPTY) {
                    printf ("!BST already exists! Overwrite [y/n]?\n");
                    switch (my_getchar (PROMPT)) {
                        case 'y':
                            bst_destroy (root);
                            printf ("!BST has been deleted.\n");
                            root = EMPTY;
                            break;

                        case EOF:
                            fprintf (stderr, INVALID_INPUT);

                        case 'n': default:
                            continue;
                    }
                }
                printf ("Enter data(space/comma separated): ");
                if (fgets (input, BUFF, stdin) == NULL) {
                    fprintf (stderr, IOE);
                    continue;
                }
                root = bst_create (input, ", \n");
                break;

            case 'd':
                printf ("Delete what? ");
                if (fgets (input, BUFF, stdin) == NULL) {
                    fprintf (stderr, IOE);
                    continue;
                }
                if (sscanf (input, "%d", &to_be_deleted) != 1) {
                    fprintf (stderr, INVALID_INPUT);
                    continue;
                }
                root = bst_remove (root, to_be_deleted);
                break;

            case 'p':
                bst_print (root);
                putchar ('\n');
                break;

            case 'h': 
                printf (MENU);
                break;

            case EOF: default:
                fprintf (stderr, INVALID_INPUT);
                break;

            case 'q':
                if (root != EMPTY) {
                    bst_destroy (root);
                    printf ("!BST has been destroyed.\n");
                }
                exit (0);
        }
    }
}
