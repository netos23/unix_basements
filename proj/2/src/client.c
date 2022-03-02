#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define FILE_POSITION 1
#define TRUNC_SIZE 5000

bool isExecuted = true;

void sigTermHandler(int sig) {
    if (sig == SIGTERM) {
        isExecuted = false;
    }
}

void sigContHandler(int sig) {
    // empty handler
}

int main(int argc, char **argv) {

    if (argc < 2) {
        puts("Usage: client <file> <message>");
        return 0;
    }

    if (signal(SIGTERM, sigTermHandler) == SIG_ERR) {
        puts("can`t bind SIGTERM handler");
        return 1;
    }

    if (signal(SIGCONT, sigContHandler) == SIG_ERR) {
        puts("can`t bind SIGCONT handler");
        return 1;
    }

    int buffer_fd;
    FILE *buffer_f;
    if (!(buffer_f = fopen(argv[FILE_POSITION], "w+"))) {
        puts("Can`t open file\n");
        return 1;
    }
    buffer_fd = fileno(buffer_f);
    pid_t pid = getpid();

    printf("Client started with pid %d! Waiting for server... \n", pid);
    pause();

    while (isExecuted && !flock(buffer_fd, LOCK_EX)) {
        printf("Client with pid %d writes:\n", pid);
        ftruncate(buffer_fd, 0);

        for (int i = 2; i < argc; i++) {
            char *buf = argv[i];
            int buflen = (int) strlen(buf);
            write(buffer_fd, buf, buflen);
            printf("%.*s ", buflen, buf);
        }
        putchar('\n');

        fflush(buffer_f);
        fflush(stdout);
        rewind(buffer_f);

        flock(buffer_fd, LOCK_UN);

        pause();
    }

    close(buffer_fd);
    puts("Shut down client\n");

    return 0;
}
