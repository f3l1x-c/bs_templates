//
// Created by john on 17.06.24.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    int thread_id;
    int *shared_counter;
    pthread_mutex_t *mutex;
    pthread_cond_t *cond;
    int *threads_completed;
    int num_threads;
} ThreadData;

void *thread_function(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    
    // Lock the pthread_mutex_cond before modifying the shared resource
    pthread_mutex_lock(data->mutex);
    
    // Modify the shared resource
    (*(data->shared_counter))++;
    printf("Thread %d incremented counter to %d\n", data->thread_id, *(data->shared_counter));
    
    (*(data->threads_completed))++;
    if (*(data->threads_completed) == data->num_threads) {
        pthread_cond_signal(data->cond);
    }
    // Unlock the pthread_mutex_cond after modification
    pthread_mutex_unlock(data->mutex);

    pthread_exit(NULL);
}

int main() {

    int num_threads = 10;
    pthread_t* threads = (pthread_t*)malloc(num_threads*sizeof(pthread_t));
    ThreadData* thread_data = (ThreadData*)malloc(num_threads*sizeof(ThreadData));
    int shared_counter = 0;
    int threads_completed = 0;
    pthread_mutex_t mutex;
    pthread_cond_t cond;


    // Initialize the pthread_mutex_cond
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        fprintf(stderr, "pthread_mutex_init\n");
        return EXIT_FAILURE;
    }

    // Initialize the cond
    if (pthread_cond_init(&cond, NULL) != 0) {
        fprintf(stderr, "Error pthread_cond_init\n");
        return EXIT_FAILURE;
    }

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        thread_data[i] = (ThreadData){.thread_id=i,
                        .shared_counter=&shared_counter,
                        .mutex=&mutex,
                        .cond=&cond,
                        .threads_completed=&threads_completed,
                        .num_threads= num_threads};
        if (pthread_create(&threads[i], NULL, thread_function, (void *)&thread_data[i]) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return 1;
        }
    }
    // Wait for all threads to signal_handling completion
    pthread_mutex_lock(&mutex);
    while (threads_completed < num_threads) {
        pthread_cond_wait(&cond, &mutex);
        printf("cond_signal received!\n");
    }
    pthread_mutex_unlock(&mutex);

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Error joining thread %d\n", i);
            return 1;
        }
    }

    // Destroy the pthread_mutex_cond
    if (pthread_mutex_destroy(&mutex) != 0) {
        fprintf(stderr, "pthread_mutex_destroy\n");
        return EXIT_FAILURE;
    }
    // Destroy the cond
    if (pthread_cond_destroy(&cond) != 0) {
        fprintf(stderr, "pthread_cond_destroy\n");
        return EXIT_FAILURE;
    }

    printf("Final counter value: %d\n", shared_counter);
    return EXIT_SUCCESS;
}