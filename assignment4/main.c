#include <stdio.h>
#include <stdlib.h>
                          /* end of implementation */
#define BUFF 256
#define PROMPT ">>> "
#define INVALID_INPUT "?\n"
#define IOE "!IO Error.\n"
#define MENU "Commands:\n"\
             "=========\n"\
             "i\tInsert element into queue\n"\
             "d\tDelete element from queue\n"\
             "p\tPrint queue\n"\
             "h\tDisplay this help menu\n"\
             "q\tQuit\n"

/* read use input from stdin and convert to numeric value, if
   unsuccessful return 0 */
int get_num (char *buff, size_t size, int *var)
{
    if (fgets (buff, size, stdin) == NULL) {
        fprintf (stderr, IOE);
        return 0;
    }
    else if (sscanf (buff, "%d", var) == 0) {
        fprintf (stderr, INVALID_INPUT);
        return 0;
    }
    return 1;
}

/* main routine to test the queue implementation */
int main (void)
{
    char input[BUFF];
    int to_be_enqueued, dequeued;
    queue_t q = { /* initially, */
        -1,       /* front = -1 */ 
        -1,       /* rear  = -1 */
        {0}       /* queue is empty */
    };

    printf ("Fixed Sized Implementation of a Queue.\n");
    printf (MENU);
    while (1) 
    {
        printf (PROMPT);
        if (fgets (input, BUFF, stdin)) 
        {
            switch (input[0]) {
                case 'i':
                    printf ("Insert what?\n");
                    if (!get_num (input, BUFF, &to_be_enqueued)) { 
                        continue;
                    }
                    else {
                        if (enqueue (&q, to_be_enqueued)) {
                            printf ("Element '%d' has been inserted "
                                    "into the queue.\n", to_be_enqueued);
                        } else {
                            printf ("Queue overflow: queue is full. "
                                    "Cannot insert new element.\n");
                        }
                    }
                    break;

                case 'd':
                    if (dequeue (&q, &dequeued)) {
                        printf ("Element '%d' has been removed "
                                "from the queue.\n", dequeued);
                    } else {
                        printf ("Queue underflow: queue is empty. "
                                "Cannot remove any element.\n");
                    } 
                    break;

                case 'q':
                    printf ("!Exiting process.\n");
                    exit (0);

                case 'p':
                    display (&q);
                    break;

                default:
                    fprintf (stderr, INVALID_INPUT);
                case 'h':
                    printf (MENU);
                    break; 
            }
        }
        else {
            fprintf (stderr, IOE);
            continue;
        }
    }
}
