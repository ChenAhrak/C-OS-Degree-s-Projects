#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

#define FIFO_NAME1 "sTc_fifo"
#define FIFO_NAME2 "cTs_fifo"
#define STR_LEN 100
char cw[STR_LEN];
char cr[STR_LEN];
FILE *fcr;
FILE *fcw;

void catcher(int signum)
{
    printf("\nCaught signal %d\n", signum);
    fcr = fopen(FIFO_NAME1, "r");
    while (fscanf(fcr, " %s", cr) != EOF)
    {
        printf("Got: %s\n", cr);
        if (strcmp(cr, "Done") == 0)
        {
            break;
        }
    }

    exit(0);
}

int main()
{
    signal(SIGINT, catcher);

    if (!(fcw = fopen(FIFO_NAME2, "w")))
    {
        perror("cannot open fifo file for w");
        exit(EXIT_FAILURE);
    }

    printf("The client ready to communicate\n");

    // writing
    while (fgets(cw, STR_LEN, stdin) != NULL)
    {
        // printf("Got: %s\n", s);
        fprintf(fcw, " %s\n", cw);
        fflush(fcw); // <== important
        if (strcmp(cw, "exit\n") == 0)
        {
            break;
        }
    }

    if (!(fcr = fopen(FIFO_NAME1, "r")))
    {
        perror("cannot open fifo file for r");
        exit(EXIT_FAILURE);
    }

    // copy from reading to array cr
    while (fscanf(fcr, " %s", cr) != EOF)
    {
        printf("Got: %s\n", cr);
        if (strcmp(cr, "Done") == 0)
        {
            break;
        }
    }

    return EXIT_SUCCESS;
}