#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#define BLOCK 512

int main() {
    char data[BLOCK];
    int sz;
    while ((sz = (int) read(STDIN_FILENO, data, sizeof(data)))) {
        printf("Server reads : %s,serverPID: %d\n", data, getpid());
        fflush(NULL);
    }
    printf("File is empty, server PID:%d\n", getpid());
    _exit(0);
}