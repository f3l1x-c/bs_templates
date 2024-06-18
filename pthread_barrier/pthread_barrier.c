//
// Created by felix on 18.06.24.
//

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_barrier_t barrier;
} program_state;

typedef struct {
    int pid;
    program_state state;
} thread_arg;

/*
*     Thread function with arguments.
* */
void* thread_function_args(void* args) {
    thread_arg* arg = (thread_arg*)args;
    program_state* state = arg->state;

    pthread_mutex_lock(&state->mutex);
    // access shared data
    int serial_thread = pthread_barrier_wait(&state->barrier);
    if (serial_thread == PTHREAD_BARRIER_SERIAL_THREAD) {
        // do something in a single thread
    }
    pthread_barrier_wait(&state->barrier);

    pthread_mutex_unlock(&state->mutex);
    return NULL;
}

/*
*     Create pthreads with thread arguments.
* */
void create_pthreads_args(int num_threads, void* thread_function_args,
    pthread_t ptids[], thread_arg args[]) {
    for(int i = 0; i < num_threads; i++) {
        // last argument pthread_function argument.
        if (pthread_create(&ptids[i], NULL, thread_function, &args[i]) != 0) {
            fprintf(stderr, "Failed creating pthread.\n");
            exit(EXIT_FAILURE);
        }
    }
}

/*
*     Initialize mutex and barrier.
* */
void init_data(program_state* state) {
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Failed initializing mutex.\n");
        return EXIT_FAILURE;
    }
    state->mutex = mutex;

    if(pthread_barrier_init(&state->barrier, NULL, number_of_bees) < 0) {
        fprintf(stderr, "Failed initializing barrier.\n");
        return EXIT_FAILURE;
    }
}

/*
*     Destroy mutex and barrier.
* */
void free_data(program_state* state) {
    pthread_barrier_destroy(&state->barrier);
    pthread_mutex_destroy(&state->mutex);
    free(state);
}