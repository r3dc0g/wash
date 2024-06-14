#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int current_opt;
    int line_numbers = 0;

    while ((current_opt = getopt(argc, argv, "hn:")) != -1) {
        switch (current_opt) {
            case 'h':
                printf("Usage: %s <filename>\n", argv[0]);
                return 0;
            case 'n':
                line_numbers = atoi(optarg);
                break;
            default:
                printf("Usage: %s <filename>\n", argv[0]);
                return 1;
        }
    }

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

    return 0;
}

