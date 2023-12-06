#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#define FIFO_NAME1 "sTc_fifo"
#define FIFO_NAME2 "cTs_fifo"
#define STR_LEN 100
#define DONE "Done"
char sr[STR_LEN];
FILE *fsr;
FILE *fsw;
pthread_t th;
int i, count;

void *reverse(void *arg)
{

    // print inside thread function

    i = 0;
    const char *s = (char *)arg;
    char *rev = malloc((strlen(s)) * sizeof(char));
    // fclose(fsw);
    //  writing

    if (strcmp(s, "exit") == 0)
    {
        fprintf(fsw, " %s\n", DONE);
        fflush(fsw);
    }
    else
    {

        // reverse s array to rev array
        count = strlen(s) - 1;
        for (i = 0; i < strlen(s); i++)
        {
            rev[i] = s[count];
            count--;
        }
        rev[i] = '\0';

        fprintf(fsw, " %s\n", rev);
        fflush(fsw);
    }
    return (void *)EXIT_SUCCESS;
}

int main()
{
    // create the fifo's

    // reading
    if (mkfifo(FIFO_NAME2, 0777) == -1 &&
        errno != EEXIST)
    {
        perror("cannot create fifo file");
        exit(EXIT_FAILURE);
    }
    printf("Server: FIFO 1 created\n");
     printf("The server ready to comunicate\n");

    if (!(fsr = fopen(FIFO_NAME2, "r")))
    {

        perror("cannot open fifo file for r");
        exit(EXIT_FAILURE);
    }

    // writing
    if (mkfifo(FIFO_NAME1, 0777) == -1 &&
        errno != EEXIST)
    {
        perror("cannot create fifo file");
        exit(EXIT_FAILURE);
    }
    printf("Server: FIFO 2 created\n");
    if (!(fsw = fopen(FIFO_NAME1, "w")))
    {

        perror("cannot open fifo file for r");
        exit(EXIT_FAILURE);
    }

   
   //copy from the file to array sr
    while (fscanf(fsr, " %s", sr) != EOF)
    {

        if (pthread_create(&th, NULL, reverse, (void *)sr) != 0)
        {
            printf("Error!!");
            exit(1);
        }
        pthread_join(th, NULL);
    }

    return EXIT_SUCCESS;
}