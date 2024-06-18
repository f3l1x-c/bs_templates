//
// Created by felix on 18.06.24.
//
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

#include "myqueue_pair.h"

#define BASE 10

typedef struct {
    int flower_field_width;
    int flower_field_height;
    int harvested_flowers;
    bool** flowers;
    bool hive_destroyed;
    myqueue *flower_queue;
    pthread_barrier_t barrier;
    pthread_mutex_t mutex;
} flower_state;

typedef struct {
    int bee_id;
    flower_state* state;
} bee_arg;


int random_range(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void *bee_thread_function(void* args) {
    bee_arg* arg = (bee_arg*)args;
    flower_state* state = arg->state;
    while(true) {
        if (state->harvested_flowers == (state->flower_field_height * state->flower_field_width))
            break;

        if (!myqueue_is_empty(state->flower_queue)) {
            // There are flowers to collect nectar from
            pthread_mutex_lock(&state->mutex);
            Pair flower_location = myqueue_pop(state->flower_queue);
            pthread_mutex_unlock(&state->mutex);
            int x = flower_location.x;
            int y = flower_location.y;

            // Simulate flying to the flower location
            printf("Bee %d flying to food source at position (%d,%d).\n", arg->bee_id, x, y);
            usleep(random_range(100000, 500000)); // usleep uses microseconds

            if (state->flowers[y][x]) {
                // Flower has nectar
                state->flowers[y][x] = false;
                state->harvested_flowers++;
                printf("Bee %d collected nectar at position (%d,%d) and reports potential food sources: ", arg->bee_id, x, y);
            } else {
                // Flower has already been harvested
                printf("Bee %d could not find nectar at position (%d,%d) and reports potential food sources: ", arg->bee_id, x, y);
            }
            pthread_mutex_lock(&state->mutex);
            // Check surrounding cells for harvestable flowers and add them to the queue
            if (x > 0) {
                myqueue_push(state->flower_queue, (Pair){x - 1, y}); // Left
                printf("(%d,%d)", x-1, y);
            }

            if (x < state->flower_field_width - 1) {
                myqueue_push(state->flower_queue, (Pair){x + 1, y}); // Right
                printf("(%d,%d)", x+1, y);
            }

            if (y > 0) {
                myqueue_push(state->flower_queue, (Pair){x, y - 1}); // Top
                printf("(%d,%d)", x, y-1);
            }

            if (y < state->flower_field_height - 1) {
                myqueue_push(state->flower_queue, (Pair){x, y + 1}); // Bottom
                printf("(%d,%d)", x, y+1);
            }
            printf(".\n");
            // Unlock the mutex after accessing shared data
            pthread_mutex_unlock(&state->mutex);
        } else {
            // There are no flowers to collect nectar from
            printf("Bee %d is working in beehive.\n", arg->bee_id);
            usleep(random_range(100000, 500000)); // usleep uses microseconds
        }

        int serial_thread = pthread_barrier_wait(&state->barrier);
        if (serial_thread == PTHREAD_BARRIER_SERIAL_THREAD) {
            if ((rand() % 10) == 0) {
                printf("Bees encounter a bear and engage in a fight.\n");
				if ((rand() % 10) <= 4){
                    printf("Bear destroys the beehive.\n");
                    state->hive_destroyed = true;
                }
                else
                    printf("The bees successfully repel the bear and resume their work.\n");
            }
        }
        pthread_barrier_wait(&state->barrier);

        if(state->hive_destroyed)
            break;
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage %s <flower field width> <flower field height> <number of bees>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int flower_field_width;
    int flower_field_height;
    int number_of_bees;

    if (sscanf(argv[1], "%d", &flower_field_width) != 1) {
        fprintf(stderr, "Failed parsing flower field width.\n");
        return EXIT_FAILURE;
    }
    if (sscanf(argv[2], "%d", &flower_field_height) != 1) {
        fprintf(stderr, "Failed parsing flower field height.\n");
        return EXIT_FAILURE;
    }
    if (sscanf(argv[3], "%d", &number_of_bees) != 1) {
        fprintf(stderr, "Failed parsing number of bees.\n");
        return EXIT_FAILURE;
    }

    bool** flowers = malloc(flower_field_height * sizeof(bool*));
    for (int i = 0; i < flower_field_height; i++) {
        flowers[i] = malloc(flower_field_width * sizeof(bool));
        for (int j = 0; j < flower_field_width; j++)
            flowers[i][j] = true;
    }

    myqueue flower_queue;
    myqueue_init(&flower_queue);

    pthread_t bee_threads[number_of_bees];
    bee_arg bee_args[number_of_bees];

    flower_state* state = malloc(sizeof(flower_state));
    state->flower_field_width = flower_field_width;
    state->flower_field_height = flower_field_height;
    state->flowers = flowers;
    state->hive_destroyed = false;
    state->flower_queue = &flower_queue;
    state->harvested_flowers = 0;

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Failed initialize mutex.\n");
        return EXIT_FAILURE;
    }
    state->mutex = mutex;
    if(pthread_barrier_init(&state->barrier, NULL, number_of_bees) < 0) {
        fprintf(stderr, "Failed initializing barrier.\n");
        return EXIT_FAILURE;
    }

    myqueue_push(state->flower_queue, (Pair){random_range(0, flower_field_width-1),random_range(0, flower_field_height-1)});
    for (int i = 0; i < number_of_bees; i++) {
        bee_args[i].bee_id = i;
        bee_args[i].state = state;

        if (pthread_create(&bee_threads[i], NULL, bee_thread_function, &bee_args[i]) != 0) {
            fprintf(stderr, "Failed creating bee thread.\n");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < number_of_bees; i++)
        pthread_join(bee_threads[i], NULL);
    printf("%d bees collected nectar from %d/%d flowers.\n", number_of_bees, state->harvested_flowers, flower_field_width*flower_field_height);
    if(state->hive_destroyed)
        printf("Beehive was destroyed.\n");
    for (int i = 0; i < flower_field_height; i++)
        free(flowers[i]);
    free(flowers);
    pthread_barrier_destroy(&state->barrier);
    pthread_mutex_destroy(&state->mutex);
    return 0;
}