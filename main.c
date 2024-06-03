#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* The maximum length command */

int main(void)
{
    char *args[MAX_LINE/2 + 1]; /* command line arguments */
    int should_run = 1; /* flag to determine when to exit program */
    char input[MAX_LINE];
    int i = 0;
    int status;
    pid_t pid;

    // Loop for the shell
    while (should_run) {

        // print prompt
        printf("wash > ");
        fflush(stdout);

        // get input from standard in
        fgets(input, MAX_LINE, stdin);
        input[strlen(input) - 1] = '\0'; // remove newline

        if (strcmp(input, "exit") == 0) {
            should_run = 0;
            break;
        }

        //
        char *token = strtok(input, " ");
        while (token != NULL) {
            args[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        args[i] = NULL;

        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Fork failed\n");
            return 1;
        } else if (pid == 0) {
            execvp(args[0], args);
            fprintf(stderr, "wash: \n");
            return 1;
        } else {
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}
