/* ====================================================================
*
*  Name:                Garrett Crites
*  Course Number:       CSC 3350
*  Assignment Title:    wash - Extra Credit Coding Assignment
*  Professor:           Andy Cameron
*  Date:                June 13, 2024
*
*  ====================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

// The maximum length command
#define MAX_LINE 256


/*
* Function: pwd
* ----------------------------
*   Prints the current working directory to the screen
*
*   returns: 0
*/
int pwd() {

    // create cwd buffer
    char cwd[MAX_LINE];

    // get current working directory
    getcwd(cwd, sizeof(cwd));

    // print the current working directory
    printf("%s\n", cwd);
    fflush(stdout);
    return 0;
}

/*
* Function: pwd
* ----------------------------
*   Prints the help message
*
*/
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

/*
* Function: pwd
* ----------------------------
*   Prints the current working directory to the screen
*
*   returns: 1 if the command is a built-in command, 0 otherwise
*/
int fork_child(char *args[], char *filename) {

    // status of child process
    int status;

    // fork child
    pid_t pid = fork();

    // fork failed
    if (pid < 0) {
        fprintf(stderr, "wash: fork failed\n");
        fflush(stdout);
        return 1;
    } else if (pid == 0) {

        // child process

        // redirect output to file
        if (filename != NULL) {

            // error filename buffer
            char errorname[MAX_LINE];
            strcpy(errorname, filename);

            // append proper file extensions
            strcat(filename, ".output\0");
            strcat(errorname, ".error\0");

            // redirect stdout and stderr to file
            freopen(filename, "a+", stdout);
            freopen(errorname, "a+", stderr);

            // execute command
            execvp(args[0], args);

            // print error message if execvp fails
            fprintf(stderr, "wash: command not found\n");
            fflush(stdout);
            exit(1);
        }
        else {

            // execute command
            execvp(args[0], args);

            // print error message if execvp fails
            fprintf(stderr, "wash: command not found\n");
            fflush(stdout);
            return 1;
        }
    } else {

        // parent process

        wait(&status);
    }

    return 0;
}

/*
* Function: handle_builtin
* ----------------------------
*   Runs the built-in commands for the shell
*
*/
int handle_builtin(char *args[], int argc) {

    // exit built-in command
    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }

    // echo built-in command
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

    // cd built-in command
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

    // pwd built-in command
    if (strcmp(args[0], "pwd") == 0) {
        pwd();
        return 1;
    }

    // setpath built-in command
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

    // help built-in command
    if (strcmp(args[0], "help") == 0) {

        print_help();

        return 1;
    }


    return 0;
}

/*
* Function: parse
* ----------------------------
*   Parses the input from the user, evoking built-in
*   commands or forking a child process
*
*/
int parse(char *input) {

    // create array for arguments
    char *args[MAX_LINE];

    // create copies of input for parsing
    char input_copy[MAX_LINE];
    char redirect_copy[MAX_LINE];
    strcpy(input_copy, input);
    strcpy(redirect_copy, input);

    // determine if there is a redirect
    char *predicate = strtok(redirect_copy, ">");

    if (predicate[strlen(predicate) - 1] == ' ') {

        // remove trailing whitespace
        predicate[strlen(predicate) - 1] = '\0';

    }

    char *filename = strtok(NULL, ">");

    // variables to parse arguments
    int i = 0;
    char *argument;

    // if filename is NULL, then there
    // is no redirect
    if (filename != NULL) {

        // remove leading whitespace
        if (filename[0] == ' ') {
            filename++;
        }

        argument = strtok(predicate, " ");
        while (argument != NULL) {
            args[i] = argument;
            argument = strtok(NULL, " ");
            i++;
        }

    }
    else {
        argument = strtok(input_copy, " ");
        while (argument != NULL) {
            args[i] = argument;
            argument = strtok(NULL, " ");
            i++;
        }
    }

    args[i] = NULL;

    if (i == 0) {
        return 0;
    }

    // handle built-in commands or fork
    // for the child process
    if (!handle_builtin(args, i)) {
        fork_child(args, filename);
    }

    return 0;
}

/*
* Function: main
* ----------------------------
*   Runs the main shell loop
*
*/
int main(int argc, char *argv[]) {

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

        // get user, hostname, and current directory
        char *user = getenv("USER");
        char hostname[MAX_LINE / 2];
        gethostname(hostname, sizeof(hostname));
        char *current_dir = getcwd(NULL, MAX_LINE / 2);

        // print prompt
        printf("[%s@%s %s]$ ", user, hostname, current_dir);
        fflush(stdout);
        fflush(stdin);

        // get input from standard in
        fgets(input, MAX_LINE, stdin);

        // return if there is no input
        if (input[0] == '\n') {
            return 0;
        }

        // remove newline
        input[strlen(input) - 1] = '\0';

        // parse input
        parse(input);
        fflush(stdout);
        fflush(stdin);
    }

    return 0;
}
