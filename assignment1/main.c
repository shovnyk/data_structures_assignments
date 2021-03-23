/* macro to get input from the user and store in variable  */
#define GET_NUM(from,into)  if (fgets (from, BUFF, stdin) == NULL) {\
    fprintf (stderr, "!IO Error. Exiting process.\n");\
    exit (1);\
}\
else if ((sscanf (from, "%d", &into) == 0)) {\
    fprintf (stderr, INVALID_INPUT);\
    continue;\
}

#define MENU "Commands:\n"\
             "---------\n"\
             "c\tCreate a new list\n"\
             "p\tPrint list\n"\
             "i\tInsert element into list\n"\
             "d\tDelete element from list\n"\
             "h\tDisplay this help menu\n"\
             "q\tQuit\n"
#define INVALID_INPUT "?\n"
#define BUFF 256
#define PROMPT ">> "

/* main routine to test the functions defined */
int main (void)
{
    int i, x, y;
    int choice, index, to_be_inserted;
    int initial_size, *initial_list; 

    node_t *l = NULL; /* list is initially empty */
    char cmd[BUFF], input[BUFF]; /* buffers to store user input */

    printf ("\nImplementation of a XXX List\n");
    printf (MENU);
    while (1)
    {
        printf (PROMPT);
        if (fgets (cmd, BUFF, stdin) != NULL) {
            switch (cmd[0])
            {
                case 'c':
                    if (l  != NULL) { /* a list already exists */
                        fprintf (stderr, "!Overwriting existing list\n");
                        destroy_list (l);
                    } 
                    printf ("Enter the initial number of elements\n" PROMPT); 
                    GET_NUM (input, initial_size);
                    if (initial_size == 0) {
                        fprintf (stderr, "!Provide a non zero size\n");
                        continue;
                    }
                    initial_list = (int *) malloc (sizeof (int) * initial_size); 
                    for (i = 0; i < initial_size; i++) {
                        GET_NUM (input, initial_list[i]);
                    } 
                    l = create_list (initial_list, initial_size); 
                    if (l != NULL) {
                        printf ("List created.\n");
                    }
                    free (initial_list);
                    break; 
                   
                case 'p':
                    display (l);
                    break;

                case 'i':
                    printf ("Insert what?\n");
                    GET_NUM (input, to_be_inserted);
                    printf ("Insert where?\n" 
                            "1\tBeginning of list\n"
                            "2\tEnd of list\n"
                            "3\tAfter index N\n" PROMPT);
                    GET_NUM (input, choice);
                    switch (choice) {
                        case 1:
                            if (insert_at_start (&l, to_be_inserted)) {
                                printf ("Element '%d' has been added "
                                        "to beginning.\n", to_be_inserted); 
                            }
                            break; 
                        case 2:
                            if (insert_at_end (&l, to_be_inserted)) {
                                printf ("Element '%d' has been added "
                                        "to end.\n", to_be_inserted);
                            }
                            break;
                        case 3:
                            printf ("N = ? ");
                            GET_NUM (input, index);
                            x = insert_after_index (&l, to_be_inserted, index);
                            if (x == 1) { 
                                printf ("Element '%d' has been added after "
                                        "index %d.\n", to_be_inserted, index);
                            }
                            else if (x == -1) {
                                fprintf (stderr, "!Index out of bounds.\n");
                            }
                            break;

                        default:
                            fprintf (stderr, INVALID_INPUT);
                    } 
                    break;

                case 'd':
                    printf ("Delete where?\n" 
                            "1\tBeginning of list\n"
                            "2\tEnd of list\n"
                            "3\tAt index N\n" PROMPT);
                    GET_NUM (input, choice);
                    switch (choice) {
                        case 1:
                            if (delete_at_start (&l, &x)) {
                                printf ("Element '%d' has been removed " 
                                        "from beginning.\n", x); 
                            }
                            break; 
                        case 2:
                            if (delete_at_end (&l, &x)) {
                                printf ("Element '%d' has been removed "
                                        "from the end.\n", x);
                            }
                            break;
                        case 3:
                            printf ("N = ? ");
                            GET_NUM (input, index); 
                            y = delete_at_index (&l,index, &x);
                            if (y == 1) { 
                                printf ("Element '%d' has been removed "
                                        "at index %d.\n", x, index);
                            }
                            else if (y == -1) {
                                fprintf (stderr, "!Index out of bounds.\n");
                            }
                            break;

                        default:
                            fprintf (stderr, INVALID_INPUT);
                    } 
                    break;

                case 'q':
                    if (l != NULL) { /* deallocate the list only 
                                        if it exists */
                        destroy_list (l);
                        printf ("List destroyed.\n");
                    }
                    printf ("Exiting process.\n");
                    exit (0);

                default: 
                    fprintf (stderr, INVALID_INPUT);
                case 'h':
                    printf (MENU);
                    continue;
            }
        }
        else {
            fprintf (stderr, "!IO Error. Exiting process.\n");
            exit (1); 
        }
    }
    exit (0);
}
