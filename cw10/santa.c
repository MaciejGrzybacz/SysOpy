#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t santa_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t santa_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t reindeers_mutexes[9] = {
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER
};

int reindeers_back_count = 0;
pthread_mutex_t reindeers_back_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_t santa_thread;
pthread_t reindeers_threads[9];

void* reindeer_thread_handler(void* arg) {
    int id = *(int*)arg;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    pthread_mutex_lock(&reindeers_mutexes[id]);
    while (1) {
        sleep(rand() % 6 + 5);

        pthread_mutex_lock(&reindeers_back_mutex);
        printf("Reindeer: waiting %d reindeers, %d\n", reindeers_back_count, id);

        reindeers_back_count++;
        if (reindeers_back_count == 9) {
            printf("Reindeer: waking up Santa, %d\n", id);
            pthread_cond_signal(&santa_cond);
            reindeers_back_count = 0;
        }

        pthread_mutex_unlock(&reindeers_back_mutex);

        pthread_mutex_lock(&reindeers_mutexes[id]);

        printf("Reindeer: going on vacation, %d\n", id);
    }

    return NULL;
}

void* santa_thread_handler(void* arg) {
    for (int i = 0; i < 4; i++) {
        pthread_cond_wait(&santa_cond, &santa_mutex);
        printf("Santa: waking up\n");

        printf("Santa: delivering toys\n");
        sleep(rand() % 3 + 2);

        for (int j = 0; j < 9; j++) {
            pthread_mutex_unlock(&reindeers_mutexes[j]);
        }

        printf("Santa: going to sleep\n");
    }

    for (int j = 0; j < 9; j++) {
        pthread_cancel(reindeers_threads[j]);
    }

    return NULL;
}

int main() {
    int ids[9];
    pthread_create(&santa_thread, NULL, santa_thread_handler, NULL);
    for (int i = 0; i < 9; i++) {
        ids[i] = i;
        pthread_create(&reindeers_threads[i], NULL, reindeer_thread_handler, &ids[i]);
    }

    pthread_join(santa_thread, NULL);
    for (int i = 0; i < 9; i++) {
        pthread_join(reindeers_threads[i], NULL);
    }

    printf("End\n");

    return 0;
}
