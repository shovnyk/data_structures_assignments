/* traversal.c:
   C program  to traverse a  BST in (1)  pre-order (2) in-order  and (3)
   post-order.
*/
#include <stdio.h>
#include <stdlib.h>

#define EMPTY NULL
#define BUFF 1024

typedef struct binary_tree_node {
    char data;
    struct binary_tree_node *left;
    struct binary_tree_node *right;
} bnode_t;

/* allocate memory and initialize data for a node */
bnode_t *getnode (int data) {
    bnode_t *newnode = (bnode_t *) malloc (sizeof (bnode_t));
    if (newnode == NULL) 
        return NULL;
    newnode->data = data;
    newnode->left = EMPTY;
    newnode->right = EMPTY;
    return newnode;
}

/* insert node into BST based on property */
bnode_t *bst_insert (bnode_t *root, char data) 
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

/* function to implement pre-order traversal of the BST 
   first visit current node, then left subtree, then right subtree */
void traverse_preorder (bnode_t *root, void (*visit)(bnode_t *))
{
    if (root == EMPTY) {
        return;
    }
    visit (root);
    traverse_preorder (root->left, visit);
    traverse_preorder (root->right, visit);
}

/* function to implement in-order traversal of the BST 
   first visit left subtree, then current node, then right subtree */
void traverse_inorder (bnode_t *root, void (*visit)(bnode_t *))
{
    if (root == EMPTY) {
        return;
    }
    traverse_inorder (root->left, visit);
    visit (root);
    traverse_inorder (root->right, visit);
}

/* function to implement post-order traversal of the BST
   first visit left subtree, then right subtree, then current node */
void traverse_postorder (bnode_t *root, void (*visit)(bnode_t *))
{
    if (root == EMPTY) {
        return;
    }
    traverse_postorder (root->left, visit);
    traverse_postorder (root->right, visit);
    visit (root);
}

/* display contents of each node of the BST */
void print_each_node (bnode_t *root)
{
    printf ("%c ", root->data);
}

/* free memory associated with each node */
void delete_each_node (bnode_t *root)
{
    free (root);
}
                          /* end of implementation */
#define MENU "Commands:\n"\
             "---------\n"\
             "c\tCreate a new BST\n"\
             "a\tAdd element to exisiting BST\n"\
             "t\tTraverse existing BST and print every node\n"\
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
    char ch;
    bnode_t *root = EMPTY; /* start with an empty tree */
    printf (MENU);
    while (1) {
        switch (my_getchar (PROMPT)) {
            case 'c': 
                if (root != EMPTY) {
                    printf ("!BST already exists! Overwrite [y/n]?\n");
                    switch (my_getchar (PROMPT)) {
                        case 'y':
                            traverse_postorder (root, delete_each_node);
                            printf ("!BST has been deleted.\n");
                            root = EMPTY;
                            break;

                        case EOF:
                            fprintf (stderr, INVALID_INPUT);

                        case 'n': default:
                            continue;
                    }
                }
                while ((ch = my_getchar ("Enter data (q to quit): ")) != 'q'){
                    root = bst_insert (root, ch);
                }
                break;

            case 'a':
                root = bst_insert (root, my_getchar ("Add element? "));
                break;

            case 't':
                switch (my_getchar ("1\tPre-Order\n"
                                    "2\tIn-Order\n"
                                    "3\tPost-Order\n" PROMPT)) {
                    case '1':
                        traverse_preorder (root, print_each_node);
                        break;

                    case '2':
                        traverse_inorder (root, print_each_node);
                        break;

                    case '3':
                        traverse_postorder (root, print_each_node);
                        break;

                    default:
                        fprintf (stderr, INVALID_INPUT);
                        break;
                }
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
                    traverse_postorder (root, delete_each_node);
                    printf ("!BST has been deleted.\n");
                }
                exit (0);
        }
    }
}
