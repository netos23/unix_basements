#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
    FILE *file;

    if (argc < 2) {
        puts("Usage: client <file> <message>\n");
        return 0;
    }

    if ((file = fopen(argv[1], "w")) == NULL) {
        puts("Can`t create file with given name");
        return 1;
    }

    fprintf(file, "Message from %d :\n", getpid());
    for (int i = 2; i < argc; i++) {
        fputs(argv[i], file);
        fputc('\n', file);
    }
    fclose(file);

    return 0;
}

