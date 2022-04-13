#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BLOCK 512

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("USAGE: ../bin/client <message>\n");
        _exit(1);
    }
    int sz;
    int pid = getpid();
    char mypid[BLOCK];
    for (int i = 1; i < argc; i++) {
        sprintf(mypid, "Message:%s,clientPID: %d", argv[i], pid);
        sz = write(STDOUT_FILENO, mypid, sizeof(mypid));
        fflush(NULL);
    }
    _exit(0);
}