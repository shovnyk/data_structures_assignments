/* check.c */
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

/* "constructor" to allocate memory for a node and inialize it with data */
bnode_t *getnode (int data)
{
    bnode_t *tmp = (bnode_t *) malloc (sizeof (bnode_t));
    if (tmp == NULL) 
        return NULL;
    tmp->data = data;
    tmp->parent = you;
    tmp->left = EMPTY;
    tmp->right = EMPTY;
    return tmp;
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

/* like getchar but print a prompt and consume trailing newlines */
int my_getchar (const char *prompt)
{
    char input[BUFF];
    fputs (prompt, stdout);
    if (fgets (input, BUFF, stdin) == NULL)
        return EOF;
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
        switch (my_getchar (PROMPT)) {
            case 'c':
                if (root == EMPTY) {
                    root = getnode (getnum ("Enter root node data: "));
                    you = root;
                    break;
                }
                switch (my_getchar (SUBMENU ("Create"))) {
                    case 'l':
                        if (you->left != EMPTY) {
                            fprintf (stderr, "Left node already full\n");
                            continue;
                        }
                        you->left = getnode (getnum ("Enter data: "));
                        break;

                    case 'r':
                        if (you->right != EMPTY) {
                            fprintf (stderr, "Right node already full\n");
                            continue;
                        }
                        you->right = getnode (getnum ("Enter data: "));
                        break;
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
                switch (my_getchar (SUBMENU ("Go down"))) {
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
