#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_LINE 256

int main(int argc, char **argv) {

    if (argc < 2) {
        char *line;
        size_t length = 0;
        while (getline(&line, &length, stdin) != -1) {
            fprintf(stdout, "%s", line);
        }
        return 0;
    }

    int current_opt;
    int line_numbers = 0;

    while ((current_opt = getopt(argc, argv, "hn:")) != -1) {
        switch (current_opt) {
            case 'h':
                printf("Usage: %s [-h] [-n N] [filename]\n", argv[0]);
                return 0;
            case 'n':
                line_numbers = atoi(optarg);
                break;
            default:
                printf("Usage: %s [-h] [-n N] [filename]\n", argv[0]);
                return 1;
        }
    }

    if (argc < 4 && line_numbers > 0) {
        char line[MAX_LINE];
        size_t length = 0;
        FILE *temp = tmpfile();
        while (fgets(line, MAX_LINE, stdin) != NULL) {
            fputs(line, temp);
            fflush(stdin);
        }

        int i = 0;
        while (!feof(temp) && i < line_numbers) {
            char *temp_line;
            size_t temp_length = 0;
            getline(&temp_line, &temp_length, temp);
            fprintf(stdout, "%s", temp_line);
            i++;
        }
    }
    else {
        FILE *file = fopen(argv[argc - 1], "r");
        if (file == NULL) {
            printf("Error: Cannot open file %s\n", argv[argc - 1]);
            return 1;
        }

        int i = 0;
        while (!feof(file) && i < line_numbers) {
            char *line;
            size_t length = 0;
            getline(&line, &length, file);
            fprintf(stdout, "%s", line);
            i++;
        }
    }


    return 0;
}

