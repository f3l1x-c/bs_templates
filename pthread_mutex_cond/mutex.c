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
} ThreadData;

void *thread_function(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    
    // Lock the pthread_mutex_cond before modifying the shared resource
    pthread_mutex_lock(data->mutex);
    
    // Modify the shared resource
    (*(data->shared_counter))++;
    printf("Thread %d incremented counter to %d\n", data->thread_id, *(data->shared_counter));
    
    // Unlock the pthread_mutex_cond after modification
    pthread_mutex_unlock(data->mutex);

    pthread_exit(NULL);
}

int main() {

    int num_threads = 10;
    pthread_t* threads = (pthread_t*)malloc(num_threads*sizeof(pthread_t));
    ThreadData* thread_data = (ThreadData*)malloc(num_threads*sizeof(ThreadData));
    int shared_counter = 0;
    pthread_mutex_t mutex;

    // Initialize the pthread_mutex_cond
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        fprintf(stderr, "Error pthread_mutex_init\n");
        return EXIT_FAILURE;
    }

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        thread_data[i] = (ThreadData){.thread_id=i,
                        .shared_counter=&shared_counter,
                        .mutex=&mutex};
        if (pthread_create(&threads[i], NULL, thread_function, (void *)&thread_data[i]) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return 1;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Error joining thread %d\n", i);
            return 1;
        }
    }

    // Destroy the pthread_mutex_cond
    if (pthread_mutex_destroy(&mutex) != 0) {
        fprintf(stderr, "Error pthread_mutex_destroy\n");
        return EXIT_FAILURE;
    }

    printf("Final counter value: %d\n", shared_counter);
    return EXIT_SUCCESS;
}