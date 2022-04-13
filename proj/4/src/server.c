#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
//#include <linux/fs.h>


#define FILE_PATTERN "/tmp/%s"
#define BLOCK 512
#define FILENAME_LEN 100

#define LOG_PATH "/tmp/log.txt"
#define LOCK_PATH "/tmp/server.lock"

bool isExecuted = true;

/*

typedef struct IntList {
    int *files;
    int size;
    int capacity;
} int_list_t;

typedef struct IntListIterator {
    int_list_t *list;
    int pointer;
} int_list_itr_t;

int_list_t *create_list() {

}


void relocate() {

}

void add() {

}

void delete_list() {

}*/

void sigTermHandler(int sig) {
    if (sig == SIGTERM) {
        isExecuted = false;
    }
}


int main(int argc, char **argv) {
    if (argc != 2) {
        puts("Usage: ./server <file>");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid == -1) {
        return EXIT_FAILURE;
    }
    if (pid) {
        exit(EXIT_SUCCESS);
    }


    if (setsid() == -1 && chdir("/") == -1) {
        return EXIT_FAILURE;
    }
    for (int i = 0; i < 1000; i++) {
        close(i);
    }
    open("/dev/null", O_RDWR);
    dup(0);
    dup(0);

    char filename[FILENAME_LEN];
    memset(filename, '\0', sizeof(filename));
    sprintf(filename, FILE_PATTERN, argv[1]);

    int data_buffer_fd;
    unlink(filename);

    if ((mkfifo(filename, 0777)) == -1) {
        return -1;
    }
    if ((data_buffer_fd = open(filename, O_RDONLY)) == -1) {
        return -1;
    }


    remove(LOG_PATH);
    FILE *log = fopen(LOG_PATH, "w");
    fputs("log created\n", log);

/*
    int lock = open(LOCK_PATH, O_WRONLY);
    if (!lockf(lock, F_LOCK, 100)) {
        fputs("server already created\n", log);
        fflush(NULL);
        close(lock);
        fclose(log);
        close(data_buffer_fd);
        return EXIT_FAILURE;
    }

    char pid_buffer[3];
    memset(pid_buffer, '\0', sizeof(pid_buffer));
    sprintf(filename, "%d", getpid());
    write(lock, pid_buffer, sizeof(pid_buffer));
*/

    if (signal(SIGTERM, sigTermHandler) == SIG_ERR) {
        fputs("can`t bind SIGCONT handler\n", log);

        //  lockf(lock, F_UNLOCK);
        //  close(lock);
        fclose(log);
        close(data_buffer_fd);

        return EXIT_FAILURE;
    }

    char data[BLOCK];
    int sz;
    memset(data, '\0', sizeof(data));

    while (isExecuted) {
        while ((sz = (int) read(data_buffer_fd, data, sizeof(data)))) {
            fprintf(log, "%.*s\n", sz, data);
            fflush(NULL);
        }
    }

    // lockf(lock, F_ULOCK, 100);
    //close(lock);
    fclose(log);
    close(data_buffer_fd);


    return 0;
}