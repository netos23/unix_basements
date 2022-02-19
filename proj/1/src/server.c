#include <stdio.h>

/**
 * Use a number limited by the range of an integer
 * */
#define BUFFER_LENGTH 100

int main(int argc, char **argv) {
    FILE *file;
    char buffer[BUFFER_LENGTH];
    int read;

    if (argc != 2) {
        puts("Usage: server <file>\n");
        return 0;
    }

    if ((file = fopen(argv[1], "r")) == NULL) {
        puts("Can`t open file");
        return 1;
    }

    while ((read = (int) fread(buffer, sizeof(char), BUFFER_LENGTH, file))) {
        printf("%.*s", read, buffer);
    }
    fclose(file);

    return 0;
}