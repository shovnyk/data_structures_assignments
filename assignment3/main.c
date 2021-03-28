

#define BUFF 256
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
#define INVALID_INPUT "?\n"
#define SYNTAX_ERROR(...) fprintf (stderr, "!Syntax error: " __VA_ARGS__);

int main (void) 
{
    char input[BUFF];
    int output = 0, x;
    printf ("Program to evaluate a postfix expression.\n");
    printf (MESSAGE);
    while (1)
    {
        printf (PROMPT);
        if (fgets (input, BUFF, stdin) == NULL) {
            fprintf (stderr, "!Could not read input.\n");
            continue;
        }
        switch (input[0])
        {
            case 'q':
                /* deallocate stack before freeing */
                printf ("!Exiting process.\n"); 
                exit (0);
                
            case 'h':
                printf (MESSAGE);
                break;

            case '\n':
                continue;

            default:
                x = eval (input, &output); /* evaluate the expression */
                switch (x) {
                    case 1: /* user cannot enter ascii 1 from keyboard */
                        ans (output);
                        break;

                    case 2:
                        SYNTAX_ERROR ("Not enough operators.\n'");
                        break;

                    case 0:
                        SYNTAX_ERROR ("Possible missing operator.\n");
                        break;

                    default:
                        SYNTAX_ERROR ("Unrecognised token '%c'.\n", x);
                        break;
                }
        } 
    }
}
