#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

#define BLOCK 512
#define PID_OFFSET 2
#define SERVER_TIMEOUT 5
#define FILE_POSITION 1

bool isExecuted = true;

pid_t *process_queue;
int process_queue_pointer = 0;
int process_queue_size = 0;


void sigTermHandler(int sig) {
    if (sig == SIGTERM) {
        isExecuted = false;
    }
}

void invokeClient(bool increment) {
    kill(process_queue[process_queue_pointer], SIGCONT);

    if (increment) {
        process_queue_pointer = (process_queue_pointer + 1) % process_queue_size;
    }

    sleep(SERVER_TIMEOUT);
}

int main(int argc, char **argv) {

    if (argc < 3) {
        puts("Usage: server <file> <client0_pid> <client1_pid> ... <clientn_pid>");
        return 0;
    }

    if (signal(SIGTERM, sigTermHandler) == SIG_ERR) {
        puts("can`t bind SIGCONT handler");
        return 1;
    }

    FILE *buffer_f;
    int buffer_fd;
    if (!(buffer_f = fopen(argv[FILE_POSITION], "r"))) {
        puts("Can`t open file\n");
        return 1;
    }
    buffer_fd = fileno(buffer_f);

    pid_t server_pid = getpid();
    process_queue_size = argc - PID_OFFSET;
    process_queue = (pid_t *) calloc(process_queue_size, sizeof(pid_t));
    for (int i = 0; i < process_queue_size; i++) {
        process_queue[i] = (pid_t) strtol(argv[i + PID_OFFSET], NULL, 10);
    }

    printf("Server started with pid %d ! Listen  %d\n", server_pid, process_queue_size);
    printf("Invoke first client with pid %d\n", process_queue[process_queue_pointer]);
    invokeClient(false);

    char data[BLOCK];
    while (isExecuted && !flock(buffer_fd, LOCK_EX)) {
        printf("Server %d reads from %d:\n", server_pid, process_queue[process_queue_pointer]);

        int bytes;
        while ((bytes = (int) read(buffer_fd, data, sizeof(data)))) {
            printf("%.*s", bytes, data);
        }
        putchar('\n');

        fflush(buffer_f);
        fflush(stdout);
        rewind(buffer_f);

        flock(buffer_fd, LOCK_UN);

        invokeClient(true);
    }

    puts("Shut down server\n");
    fclose(buffer_f);
    free(process_queue);

    return 0;
}