/* postfix.c: C program to parse and evaluate a user input expression 
   in postifix notation, using a stack.

   In postfix notation, the binary operator is written after the two 
   operands.  Postfix notation is easy to implement and does not 
   require any operator precedence or associativity rules like with 
   the usual infix form.
*/

#define _GNU_SOURCE 
#include <stdio.h>      /* declares snprintf(), defines macro BUFSIZ */
#include <math.h>       /* declares pow() */
#include <string.h>     /* decalres strncpy(), strtok(), strchr() */
#include <stdlib.h>

#include "stack.c"      /* include file containing stack routines */

/* prototypes for stack functions */
int   is_empty       (stack_t  *);
void  display        (stack_t  *);
int   push           (stack_t  **,  int  );
int   pop            (stack_t  **,  int  *);
int   destroy_stack  (stack_t  *);
int   size_of_stack  (stack_t  *);
int   reverse_print  (stack_t  *);

/* a wrapper for reverse_print to display the stack */
void show_steps (stack_t *top, char last_op) 
{
    printf ("= ");
    reverse_print (top); 
    printf ("%c\n", last_op);
    return;
}

/* trims newline character from string */
char *strim (char *str)
{
    int n = 0;
    while (str[n] != '\0') {
        if (str[n] == '\n')
            str[n] = '\0';
        n++;
    }
    return str;
}

/* Interprets and then calculates the result of the binary operator 
   acting on the operands.  Returns NULL on successful operation and 
   string indicating Math Error on failure */
char *op (int operand1, int operand2, char operator, int *result) 
{
    switch (operator) {
        case '+':
            *result =  (operand1 + operand2); 
            break;

        case '-':
            *result =  (operand1 - operand2);
            break;

        case '*':
            *result =  (operand1 * operand2);
            break;

        case '/': 
            if (operand2 == 0) { 
                /* handle /0 exception */
                return "Division by 0 is undefined";
            }
            *result =  (operand1 / operand2);
            break;

        case '^':
            if (operand1 == 0 && operand2 == 0) {
                /* handle 0^0 exception */
                return "Raising 0 to 0 is undefined";
            }
            *result =  (int)pow (operand1, operand2); 
            break;
    }
    return NULL;
}

/* Evaluates the expression present in the buffer and stores it in 
   variable passed by reference. Checks for 
       1. syntax error - an error in expression itself
       2. math errors  - arithmetic operation is undefined.
   and stores the message in errorbuff. Use snprint and strncpy to 
   prevent buffer overflow.  Variable 'intsteps' tells it whether 
   to show the intermediate steps. */
int eval
(char *exprbuff, int *output, char *errorbuff, size_t size, int intsteps)
{
    stack_t *s = NULL; 
    int num, x, y, result;
    char terminator;
    char *delimiter, *operators, *token, *exception;

    operators = "+-*/^";
    terminator = '$'; 
    delimiter = ", ";

    if (strchr (exprbuff, delimiter[0]) == NULL) {
        snprintf (errorbuff, size, "Syntax error: Missing delimiters. "
                  "Did you forget the '%c's?", delimiter[0]);
        return 0;
    } 
    token = strtok (exprbuff, delimiter); 
    while (token != NULL && token[0] != terminator)
    {
        if (strchr (operators, token[0]) != NULL) /* if token is operator */
        {
            if (intsteps)
                show_steps (s, token[0]);
            if (!pop (&s, &x) || /* pop second operand */
                !pop (&s, &y)    /* pop first operand */ ) {
                strncpy (errorbuff,
                         "Syntax error: Not enough operands.", size);
                return 0;
            }
            exception = op (y, x, token[0], &result); /* calculate */
            if (exception == NULL) { 
                push (&s, result); /* push back onto stack */
            }
            else {
                snprintf (errorbuff, size, "Math error: %s.", exception);
                destroy_stack (s);
                return 0;
            }
        }
        else if (sscanf (token, "%d", &num)) {
            /* if token is number, push and wait for next operator */
            push (&s, num);
        }
        else { 
            /* invalid token */
            snprintf (errorbuff, size, "Syntax error: Unrecognised "
                      "token '%s'.", strim (token));
            destroy_stack (s);
            return 0;
        }
        token = strtok (NULL, delimiter); /* get next token 
                                             from expression buffer */
    }
    if (size_of_stack (s) != 1) {
        strncpy (errorbuff, "Syntax error: Not enough operators.", size);
        destroy_stack (s);
        return 0;
    }
    pop (&s, output);
    destroy_stack (s);
    return 1;
}

#define MESSAGE \
    "Enter an expression in postfix notation.\n"\
    "Use , (comma) as a delimiter and '$' (dollar) as end of input.\n"\
    "The following operations are allowed:\n"\
    "\t+ addition\n"\
    "\t- subtraction\n"\
    "\t* multiplication\n"\
    "\t/ division\n"\
    "\t^ exponentiation\n"\
    "A valid expression can be: "\
    "'6, 2, 3, +, -, 3, 8, 2, /, +, *, 2, ^, 3, +, $'\n"\
    "Press h to display these help instructions. Press q to quit.\n"
#define PROMPT ">> "
#define ANS(x) printf ("= %d (ans)\n", x)

int main (void) 
{
    int output = 0;
    char input[BUFSIZ], msg[BUFSIZ];

    printf ("Program to evaluate a postfix expression.\n");
    printf (MESSAGE);
    while (1)
    {
        printf (PROMPT);
        if (fgets (input, BUFSIZ, stdin) == NULL) {
            fprintf (stderr, "Could not read input.\n");
            continue;
        }
        switch (input[0])
        {
            case 'q':
                printf ("Exiting process.\n"); 
                exit (0);
                
            case 'h':
                printf (MESSAGE);
                break;

            case '\n': /* empty user input */
                continue;

            default:
                if (eval (input, &output, msg, BUFSIZ, 1)) {
                    ANS (output);
                }
                else {
                    fputs (msg, stderr);
                    putchar ('\n');
                }
        } 
    }
    exit (1);
}
