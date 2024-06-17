//
// Created by felix on 17.06.24.
//

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void create_pthreads(int num_threads, void* thread_function, pthread_t ptids[]) {
    for(int i = 0; i < num_threads; i++) {
        // last argument pthread_function argument.
        if (pthread_create(&ptids[i], NULL, thread_function, NULL) != 0) {
            fprintf(stderr, "Failed creating pthread.\n");
            exit(EXIT_FAILURE);
        }
    }
}

void join_pthreads(int num_threads, pthread_t ptids[]) {
    for(int i = 0; i < num_threads; i++) {
        if (pthread_join(ptids[i], NULL) != 0) {
            fprintf(stderr, "Failed joining pthread.\n");
            exit(EXIT_FAILURE);
        }
    }
}