#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK 512

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("USAGE: ./bootstrap <clientcount> <client message> ... <client message>\n");
        _exit(1);
    }
    int status, pipefd[2];
    pipe(pipefd);
    pid_t child_pid = fork();
    int count = atoi(argv[1]);

    switch (child_pid) {
        case -1:
            perror("fork");
            _exit(1);
        case 0:
            for (int i = 0; i < count; i++) {
                switch (fork()) {
                    case -1:
                        perror("fork");
                        _exit(1);
                    case 0:
                        fflush(stdout);
                        close(pipefd[0]);
                        close(STDOUT_FILENO);
                        dup(pipefd[1]);
                        wait(&status);
                        execl("./client", "./client", argv[i + 2], NULL);
                        break;
                }
            }
            _exit(0);
        default:
            close(pipefd[1]);
            close(STDIN_FILENO);
            dup(pipefd[0]);
            wait(&status);
            execl("../bin/server", "../bin/server", NULL);
    }
    return 0;
}