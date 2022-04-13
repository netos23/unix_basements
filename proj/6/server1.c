#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

#define ERR_LEN 100
#define BUF_LEN 500
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

char buffer[BUF_LEN];
int avail = 0;
int consumed = 0;
bool done = false;

typedef struct {
    pthread_t tid;
    pthread_mutex_t mtx;
    pthread_cond_t cond;
} thread;


void errExitEN(int s, const char *msg);

static void *messageSupplier(void *arg) {
    int s;
    char *message = (char *) arg;

    while (true) {
        sleep(1);

        if ((s = pthread_mutex_lock(&mtx))) {
            errExitEN(s, "pthread_mutex_lock");
        }

        memset(buffer, '\0', BUF_LEN);
        avail = sprintf(buffer, "%s", message);
        consumed++;


        if ((s = pthread_mutex_unlock(&mtx))) {
            errExitEN(s, "pthread_mutex_unlock");
        }

        if ((s = pthread_cond_signal(&cond))) {
            errExitEN(s, "pthread_cond_signal");
        }
    }
}

void errExitEN(int s, const char *msg) {
    char err_buffer[ERR_LEN];
    memset(err_buffer, '\0', ERR_LEN);
    sprintf(err_buffer, "error: %d %s\n", s, msg);

    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    int s;

    for (int j = 1; j < argc; j++) {
        pthread_t tid;
        if ((s = pthread_create(&tid, NULL, messageSupplier, argv[j]))) {
            errExitEN(s, "pthread_create");
        }
    }

    while (!done) {

        if ((s = pthread_mutex_lock(&mtx))) {
            errExitEN(s, "pthread_mutex_lock");
        }

        while (avail == 0) {
            if ((s = pthread_cond_wait(&cond, &mtx))) {
                errExitEN(s, "pthread_cond_wait");
            }
        }
        consumed--;
        puts(buffer);
        printf("%d", consumed);
        avail = 0;

        if ((s = pthread_mutex_unlock(&mtx))) {
            errExitEN(s, "pthread_mutex_unlock");
        }
    }
    exit(EXIT_SUCCESS);
}