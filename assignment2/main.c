#define MENU "Commands:\n"\
             "---------\n"\
             "1.\tPush\n"\
             "2.\tPop\n"\
             "3.\tPrint the stack from top to bottom\n"\
             "4.\tDisplay this help menu\n"\
             "5.\tQuit\n"
#define INVALID_INPUT "?\n"
#define BUFF 256
#define PROMPT "--> "

/* function to get numeric input from user from stdin */
char getnum (char *buff, size_t size, int *var)
{
    if (fgets (buff, size, stdin) == NULL) {
        fprintf (stderr, "!IO Error.\n");
        return 0;
    }
    else if (sscanf (buff, "%d", var) != 1) {
        fprintf (stderr, INVALID_INPUT);
        return buff[0];
    }
    return 1;
}

/* main routine to test the functions defined */
int main (void)
{ 
    stack_t s = {-1, {0}};
    int cmd, tmp = 0;
    char input[BUFF];  /* for storing user input */
    int to_be_pushed = 0, popped_value;

    printf ("\nImplementation of a Stack with fixed memory\n");
    printf (MENU);
    while (1)
    {
        printf (PROMPT);
        tmp = getnum (input, BUFF, &cmd);
        if (tmp == 0) {
            fprintf (stderr, "!IO Error.\n");
            continue;
        }
        else if (tmp == 1){ /* assuming user cannot enter ascii 1 through
                               keyboard */
            switch (cmd)
            {
                case 1:
                    printf ("Push what?\n" PROMPT);
                    if (getnum (input, BUFF, &to_be_pushed) != 1) 
                        continue;
                    if (push (&s, to_be_pushed)) {
                        printf ("Element '%d' has been pushed.\n", 
                                to_be_pushed);
                    }
                    break;

                case 2:
                    if (pop (&s, &popped_value)) {
                        printf ("Element '%d' has been popped.\n",
                                popped_value);
                    }
                    break;

                case 3:
                    display (&s);
                    break;

                case 5:
                    printf ("Exiting process.\n");
                    exit (0);

                default: 
                    fprintf (stderr, INVALID_INPUT);
                case 4:
                    printf (MENU);
                    continue;
            }
        }
        else {
            continue;
        }
    }
    exit (0);
}
