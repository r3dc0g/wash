#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* The maximum length command */

void exit_shell() {
    exit(0);
}

int fork_child(char **args) {
    int status;
    pid_t pid = fork();
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

    return 0;
}

int handle_builtin(char *input) {

    if (strcmp(input, "exit") == 0) {
        exit_shell();
    }

    if (strcmp(input, "echo") == 0) {
        // echo user input
    }

    if (strcmp(input, "cd") == 0) {
        // change directory
    }

    if (strcmp(input, "pwd") == 0) {
        // print working directory
    }

    if (strcmp(input, "setpath") == 0) {
        // looks for executables to run
    }

    return 0;
}

int parse(char *input) {

    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    char *token = strtok(input, " ");
    int i = 0;

    while (token != NULL) {
        args[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    args[i] = NULL;

    handle_builtin(input);
    fork_child(args);

    return 0;
}


int main(void)
{
    int should_run = 1; /* flag to determine when to exit program */
    char input[MAX_LINE];
    int status;

    // Loop for the shell
    while (should_run) {

        int j = 0;
        /* parse input */

        // print prompt
        printf("wash > ");
        fflush(stdout);

        // get input from standard in
        fgets(input, MAX_LINE, stdin);
        // remove newline
        input[strlen(input) - 1] = '\0';


    }

    return 0;
}
