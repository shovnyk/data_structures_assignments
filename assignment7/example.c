#include <stdio.h>
#include <stdlib.h>

#define EMPTY NULL
#define BUFF 1024

typedef struct b_tree_node {
    int data;
    struct b_tree_node *parent;
    struct b_tree_node *left;
    struct b_tree_node *right;
} bnode_t;

bnode_t *you;  /* global pointer to current position in the tree */

/* constructor to initialize a node */
bnode_t *getnode (int data)
{
    bnode_t *tmp = (bnode_t *) malloc (sizeof (bnode_t));
    if (tmp == NULL) 
        return NULL;
    tmp->data = data;
    tmp->parent = you;
    tmp->left = NULL;
    tmp->right = NULL;
    return tmp;
}

/* display contents */
void print_each_node (bnode_t *root) 
{
    if (root == NULL) 
        printf ("Empty");
    else {
        printf ("%d", root->data);
        if (root == you)
            printf ("(you are here) ");
        else 
            printf (" ");
    }
}

/* deallocate memory assocated with each node */
void delete_each_node (bnode_t *root)
{
    printf ("Deleting node %d\n", root->data);
    free (root);
}

/* visit root first, then left subtree, then right subtree */
void traverse_preorder (bnode_t *root, void (*visit)(bnode_t *root))
{
    visit (root);
    if (root->left != EMPTY)
        visit (root->left);
    if (root->right != EMPTY)
        visit (root->right);
}

/* visit left subtree, then right subtree, then root */
void traverse_postorder (bnode_t *root, void (*visit)(bnode_t *root))
{
    if (root->left != EMPTY)
        visit (root->left);
    if (root->right != EMPTY)
        visit (root->right);
    visit (root);
}

/* read user input and return an integer */
int getnum (const char *prompt) 
{
    int n;
    char input[BUFF];
    fputs (prompt, stdout);
    fgets (input, BUFF, stdin);
    sscanf (input, "%d", &n);
    return n;
}

/* like getchar() but consume trailing newlines */
int my_getchar ()
{
    char input[BUFF];
    fgets (input, BUFF, stdin);
    return input[0];
}

#define MENU "Commands:\n"\
             "---------\n"\
             "c\tCreate a new node\n"\
             "u\tGo up one level\n"\
             "d\tGo down one level\n"\
             "x\tDelete subtree starting from current node\n\n"
#define SUBMENU(prefix) "l\t" prefix " left subtree\n"\
                        "r\t" prefix " right subtree\n"
#define PROMPT ">> " 

int main (void)
{
    bnode_t *root = EMPTY, *tmp;
    you = root;
    printf (MENU);
    while (1) 
    {
        printf (PROMPT);
        switch (my_getchar ()) {
            case 'c':
                if (root == NULL) {
                    root = getnode (getnum ("Enter root node data: "));
                    you = root;
                }
                else {
                    printf (SUBMENU ("Create"));
                    switch (my_getchar ()) {
                        case 'l':
                            if (you->left != EMPTY) {
                                fprintf (stderr, "Cannot enter node\n");
                                continue;
                            }
                            you->left = getnode (getnum ("Enter data: "));
                            break;

                        case 'r':
                            if (you->right != EMPTY) {
                                fprintf (stderr, "Cannot enter node\n");
                                continue;
                            }
                            you->right = getnode (getnum ("Enter data: "));
                            break;
                    }
                }
                break;

            case 'x':
                tmp = you;
                you = you->parent;
                traverse_postorder (tmp, delete_each_node);
                break;

            case 'u':
                if (you != root)
                    you = you->parent;
                else
                    fprintf (stderr, "Already at root. cannot go higher\n");
                break;

            case 'd':
                printf (SUBMENU ("Go down"));
                switch (my_getchar ()) {
                    case 'l':
                        if (you->left != EMPTY)
                            you = you->left;
                        break;

                    case 'r':
                        if (you->right != EMPTY)
                            you = you->right;
                        break;
                }
                break;

            default: case 'h':
                printf (MENU);
                continue;

            case 'q':
                if (root != EMPTY) {
                    traverse_postorder (root, delete_each_node);
                    root = you = EMPTY;
                }
                exit (1);

            case 'p':
                traverse_preorder (root, print_each_node);
                putchar ('\n');
                break;
        }
    }
}
