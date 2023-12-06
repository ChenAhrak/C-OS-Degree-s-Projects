#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define STR_LEN 100
#define DONE "Done"
int pipeStoC[2];
int pipeCtoS[2];
pid_t pid;
char str[STR_LEN];
int status;

void catcherChild(int signum)
{

    while (read(pipeStoC[0], str, STR_LEN) > 0)
    {
        printf("\nReversed string: %s\n", str);
        if (strcmp(str, DONE) == 0)
            break;
    }

    close(pipeCtoS[1]); // Close write end
    close(pipeStoC[0]); // Close read end

    exit(0);
}

void printAll()
{

    while (read(pipeStoC[0], str, STR_LEN) > 0)
    {
        printf("Reversed string: %s\n", str);
        if (strcmp(str, DONE) == 0)
            break;
    }
    close(pipeCtoS[1]); // Close write end
    close(pipeStoC[0]); // Close read end
                        // Wait for child process to complete
}

void reverse_string(char *str)
{
    int i, j;
    char temp;
    int len = strlen(str);

    for (i = 0, j = len - 1; i < j; i++, j--)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

int main()
{
    signal(SIGINT, catcherChild);

    if (pipe(pipeCtoS) == -1)
    {
        perror("Error creating pipe");
        exit(EXIT_FAILURE);
    }

    if (pipe(pipeStoC) == -1)
    {
        perror("Error creating pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1)
    {
        perror("error forking");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {

        // Child process (server)
        close(pipeCtoS[1]); // Close unused write end
        close(pipeStoC[0]); // Close unused read end

        printf("The server is ready to communicate\n");
        while (read(pipeCtoS[0], str, STR_LEN) > 0)
        {
            if (strcmp(str, "exit") == 0)
            {
                strcpy(str, DONE);
                write(pipeStoC[1], str, STR_LEN);
                break;
            }

            else
            {
                reverse_string(str);
                write(pipeStoC[1], str, STR_LEN);
            }
        }

        close(pipeStoC[1]); // Close write end
        close(pipeCtoS[0]); // Close read end
        exit(EXIT_SUCCESS);
    }
    else
    {
        // Parent process (client)
        close(pipeCtoS[0]); // Close unused read end
        close(pipeStoC[1]); // Close unused write end

        printf("The client is ready to communicate\n");

        while (fgets(str, STR_LEN, stdin) != NULL)
        {
            str[strcspn(str, "\n")] = '\0'; // Remove trailing newline character

            write(pipeCtoS[1], str, STR_LEN);

            if (strcmp(str, "exit") == 0)
                break;

            printf("Enter a string (or 'exit' to quit):\n");
        }

        /// Wait for the child
        wait(&status);
        if (WIFEXITED(status))
        {
            int es = WEXITSTATUS(status);
        }
        // Read and print from server
        printAll();

        return EXIT_SUCCESS;
    }
}