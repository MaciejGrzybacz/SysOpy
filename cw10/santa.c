#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Mutex and condition variable declarations for Santa.
pthread_mutex_t mutex_santa = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_santa = PTHREAD_COND_INITIALIZER;

// Mutexes for each reindeer.
pthread_mutex_t mutexes_reindeer[9] = {
        PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER
};

// Global count of reindeer back from vacation.
int count_reindeer_back = 0;
pthread_mutex_t mutex_reindeer_count = PTHREAD_MUTEX_INITIALIZER;

// Thread identifiers for Santa and reindeer.
pthread_t thread_santa;
pthread_t threads_reindeer[9];

// Handler for reindeer threads.
void* handle_reindeer(void* arg) {
    int reindeer_id = *(int*)arg;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    pthread_mutex_lock(&mutexes_reindeer[reindeer_id]);
    while (1) {
        sleep(rand() % 6 + 5); // Simulate reindeer returning at random times.

        pthread_mutex_lock(&mutex_reindeer_count);
        printf("Reindeer: total waiting %d, ID %d\n", count_reindeer_back, reindeer_id);

        count_reindeer_back++;
        if (count_reindeer_back == 9) {
            printf("Reindeer: Santa wake-up call, ID %d\n", reindeer_id);
            pthread_cond_signal(&cond_santa);
            count_reindeer_back = 0;
        }

        pthread_mutex_unlock(&mutex_reindeer_count);

        pthread_mutex_lock(&mutexes_reindeer[reindeer_id]);

        printf("Reindeer: off to vacation, ID %d\n", reindeer_id);
    }

    return NULL;
}

// Handler for Santa's thread.
void* handle_santa(void* arg) {
    for (int i = 0; i < 4; i++) {
        pthread_cond_wait(&cond_santa, &mutex_santa);
        printf("Santa: awoken\n");

        printf("Santa: distributing toys\n");
        sleep(rand() % 3 + 2); // Simulate time taken to distribute toys.

        for (int j = 0; j < 9; j++) {
            pthread_mutex_unlock(&mutexes_reindeer[j]);
        }

        printf("Santa: sleeping again\n");
    }

    for (int j = 0; j < 9; j++) {
        pthread_cancel(threads_reindeer[j]);
    }

    return NULL;
}

int main() {
    int reindeer_ids[9];
    pthread_create(&thread_santa, NULL, handle_santa, NULL);
    for (int i = 0; i < 9; i++) {
        reindeer_ids[i] = i;
        pthread_create(&threads_reindeer[i], NULL, handle_reindeer, &reindeer_ids[i]);
    }

    pthread_join(thread_santa, NULL);
    for (int i = 0; i < 9; i++) {
        pthread_join(threads_reindeer[i], NULL);
    }

    printf("Operation complete\n");

    return 0;
}
