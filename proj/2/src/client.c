#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/file.h>

#define BLOCK 512

void main() {
    int fd = open("data", O_RDWR);
    char data[BLOCK];
    while (1) {
        if (flock(fd, LOCK_EX) != -1) {
            int sz = read(fd, data, sizeof(data));
            while (sz > 0) {
                sz = read(fd, data, sizeof(data));
            }
            printf("input: ");
            scanf("%s", data);
            sz = write(fd, data, sizeof(data));
            printf("Client writes : %s\n", data);
            flock(fd, LOCK_UN);
            sleep(1);
        }
    }
    close(fd);
}
