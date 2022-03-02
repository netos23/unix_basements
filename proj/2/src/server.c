#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/file.h>
#define BLOCK 512
void main() {
    int fd = open("./assets/data", O_RDONLY);
    char data[BLOCK];
    while (1) {
        if (flock(fd, LOCK_SH) != -1) {
            int sz = read(fd, data, sizeof(data));
            if (sz > 0)
                while (sz != 0) {
                    printf("Server reads : %s\n", data);
                    sz = read(fd, data, sizeof(data));
                    sleep(1);
                }
            else
                printf("File is empty\n");
            flock(fd, LOCK_UN);
            sleep(1);
        }
    }
    close(fd);
}