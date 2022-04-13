#include <unistd.h>
#include <stdio.h>
#include <sys/fcntl.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("USAGE: ./client <message>\n");
        _exit(1);
    }

    int fd_fifo;
    if ((fd_fifo = open("/tmp/pipe0", O_WRONLY)) == -1) {
        return -1;
    }

    while (1) {
        for (int i = 1; i < argc; i++) {
            write(fd_fifo, argv[i], sizeof(argv[i]));
        }
    }
    _exit(0);
}