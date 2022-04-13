#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define MAX_MSG_SIZE 1024

static pthread_cond_t threadDied = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER;

static int totThreads = 0;
static int numLive = 0;

static int numUnjoined = 0;

static char buffer[BUF_SIZE];
static int buf_flag = 0;

enum tstate {
    TS_ALIVE,
    TS_TERMINATED,
    TS_JOINED
};

static struct {
    pthread_t tid;
    enum tstate state;
} *thread_mutex_t;

static struct {
    int delay;
    char text[MAX_MSG_SIZE];
} *msg;


static void *threadFunc(void *arg) {
    int idx = (int) arg;
    int s;
    sleep(msg[idx].delay);


    s = pthread_mutex_lock(&threadMutex);
    if (s != 0) {
        printf("mutex lock err");
        exit(1);
    }

    while (buf_flag) {
        s = pthread_mutex_unlock(&threadMutex);
        if (s != 0) {
            printf("mutex unlock err");
            exit(1);
        }
        printf("buffer is busy\n");
        sleep(1);
        s = pthread_mutex_lock(&threadMutex);
        if (s != 0) {
            printf("mutex lock err");
            exit(1);
        }
    }

    sprintf(buffer, "%s", msg[idx].text);
    buf_flag = 1;
    numUnjoined++;
    thread_mutex_t[idx].state = TS_TERMINATED;


    s = pthread_mutex_unlock(&threadMutex);
    if (s != 0) {
        printf("mutex unlock err");
        exit(1);
    }

    s = pthread_cond_signal(&threadDied);
    if (s != 0) {
        printf("signal err");
        exit(1);
    }
    return arg;
}


int main(int argc, char *argv[]) {
    int idx, s;
    int msg_counter = 0;
    void *res;
    char str_res[MAX_MSG_SIZE];


    if ((argc < 2 || strcmp(argv[1], "--help") == 0) || (argc % 2 == 0)) {
        printf("arg: %d usg err\n", argc);
        exit(1);
    }

    msg = calloc(argc / 2, sizeof(*msg));
    if (msg == NULL) {
        printf("calloc err");
        exit(1);
    }

    thread_mutex_t = calloc(argc / 2, sizeof(*thread_mutex_t));
    if (thread_mutex_t == NULL) {
        printf("calloc err");
        exit(1);
    }

    for (idx = 1; idx < argc; idx += 2) {
        msg[msg_counter].delay = (int) strtol(argv[idx], NULL, 10);
        sprintf(msg[msg_counter].text, "%s", argv[idx + 1]);

        thread_mutex_t[msg_counter].state = TS_ALIVE;
        s = pthread_create(&thread_mutex_t[msg_counter].tid, NULL, threadFunc, (void *) msg_counter);
        if (s != 0) {
            printf("pthread create err");
            exit(1);
        }
        //printf("msg idx %d with delay %d and text %s\n", msg_counter, msg[msg_counter].delay, msg[msg_counter].text);

        msg_counter++;
    }

    totThreads = argc / 2;
    numLive = totThreads;

    while (numLive > 0) {
        s = pthread_mutex_lock(&threadMutex);
        if (s != 0) {
            printf("mutex lock err");
            exit(1);
        }

        while (numUnjoined == 0) {
            s = pthread_cond_wait(&threadDied, &threadMutex);
            if (s != 0) {
                printf("wait err");
                exit(1);
            }
        }

        for (idx = 0; idx < totThreads; idx++) {
            if (thread_mutex_t[idx].state == TS_TERMINATED) {
                s = pthread_join(thread_mutex_t[idx].tid, &res);
                if (s != 0) {
                    printf("join err");
                    exit(1);
                }

                printf("msg from thread_mutex_t %d: %s\n", (int) res, buffer);
                buf_flag = 0;

                thread_mutex_t[idx].state = TS_JOINED;
                numLive--;
                numUnjoined--;

                printf("Reaped thread_mutex_t %d (remind=%d)\n", idx, numLive);
            }
        }

        s = pthread_mutex_unlock(&threadMutex);
        if (s != 0) {
            printf("mutex unlock err");
            exit(1);
        }

    }

    exit(0);
}