//
// Created by felix on 17.06.24.
//

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef struct {
    int pid;
} thread_arg;

/*
*     Thread function without arguments.
* */
void* thread_function() {
    // do something
    return NULL;
}

/*
*     Thread function with arguments.
* */
void* thread_function_args(void* args) {
    thread_arg* arg = (thread_arg*)args;
    // do something
    return NULL;
}

/*
*     Create pthreads without thread arguments.
* */
void create_pthreads(int num_threads, void* thread_function, pthread_t ptids[]) {
    for(int i = 0; i < num_threads; i++) {
        // last argument pthread_function argument.
        if (pthread_create(&ptids[i], NULL, thread_function, NULL) != 0) {
            fprintf(stderr, "Failed creating pthread.\n");
            exit(EXIT_FAILURE);
        }
    }
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
*     Join pthreads.
* */
void join_pthreads(int num_threads, pthread_t ptids[]) {
    for(int i = 0; i < num_threads; i++) {
        if (pthread_join(ptids[i], NULL) != 0) {
            fprintf(stderr, "Failed joining pthread.\n");
            exit(EXIT_FAILURE);
        }
    }
}