#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 256 /* The maximum length command */

int pwd() {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("%s\n", cwd);
    fflush(stdout);
    return 0;
}

void print_help() {
    printf("The Washington Shell is a simple shell that supports the following commands:\n\n");
    printf("\t-exit\t\t\texits the shell\n");
    printf("\t-echo [string]\t\tprints the arguments to the screen\n");
    printf("\t-cd [dir]\t\tchanges the current directory\n");
    printf("\t-pwd\t\t\tprints the current working directory\n");
    printf("\t-setpath <path>\t\tsets the path to look for executables\n");
    printf("\t-help\t\t\tprints this help message\n\n");
    fflush(stdout);
}

int fork_child(char **args) {
    int status;
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        fflush(stdout);
        return 1;
    } else if (pid == 0) {
        execvp(args[0], args);
        fprintf(stderr, "wash: command not found\n");
        fflush(stdout);
        return 1;
    } else {
        waitpid(pid, &status, 0);
    }

    return 0;
}

int handle_builtin(char **args, int argc) {

    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }

    if (strcmp(args[0], "echo") == 0) {

        int i = 1;
        while (args[i] != NULL) {
            printf("%s ", args[i]);
            i++;
        }

        printf("\n");
        fflush(stdout);
        return 1;
    }

    if (strcmp(args[0], "cd") == 0) {

        if (args[1] == NULL) {
            chdir(getenv("HOME"));
            fflush(stdout);
        } else {
            if (chdir(args[1]) != 0) {
                fprintf(stderr, "wash: directory not found\n");
                fflush(stdout);
            }
        }

        return 1;
    }

    if (strcmp(args[0], "pwd") == 0) {
        pwd();
        return 1;
    }

    if (strcmp(args[0], "setpath") == 0) {

        if (args[1] == NULL) {
            fprintf(stderr, "wash: no path specified\n");
            fflush(stdout);
            return 1;
        }

        char path[MAX_LINE];
        strcpy(path, args[1]);

        int i = 2;
        while (args[i] != NULL) {
            strcat(path, ":");
            strcat(path, args[i]);
            i++;
        }

        setenv("PATH", path, 1);

        return 1;
    }

    if (strcmp(args[0], "help") == 0) {

        print_help();

        return 1;
    }

    if (argc > 1 && strcmp(args[1], ">")) {
        if (args[2] == NULL) {
            fprintf(stderr, "wash: no output file specified\n");
            fflush(stdout);
            return 1;
        }

        char *output = strcat(".output", args[2]);
        FILE *fd = fopen(output, "w");

        fork_child(args);

        return 1;
    }

    return 0;
}

int parse(char *input) {

    char *args[MAX_LINE / 2 + 1];
    char *token = strtok(input, " ");
    int i = 0;

    while (token != NULL) {
        args[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    args[i] = NULL;

    if (i == 0) {
        return 0;
    }

    if (!handle_builtin(args, i - 1))
        fork_child(args);

    return 0;
}


int main(int argc, char **argv) {

    char input[MAX_LINE];
    int status;

    if (argc > 1) {

        if (strcmp(argv[1], "-h") == 0) {
            print_help();

        }

        return 0;
    }

    // Loop for the shell
    while (1) {

        int j = 0;
        char *current_dir = getcwd(NULL, MAX_LINE / 2);

        printf("%s > ", current_dir);
        fflush(stdout);
        fflush(stdin);

        // get input from standard in
        fgets(input, MAX_LINE, stdin);

        if (input[0] == '\n') {
            return 0;
        }

        // remove newline
        input[strlen(input) - 1] = '\0';

        parse(input);
    }

    return 0;
}