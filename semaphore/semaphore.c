//
// Created by john on 17.06.24.
//

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 5

typedef struct {
  sem_t *semaphore;
  int thread_id;
} thread_data_t;

void *thread_function(void *arg) {
  thread_data_t *data = (thread_data_t *)arg;
  int thread_id = data->thread_id;
  sem_t *semaphore = data->semaphore;

  printf("Thread %d: Waiting to enter critical section...\n", thread_id);
  sem_wait(semaphore);

  // Critical section
  printf("Thread %d: Inside critical section\n", thread_id);
  sleep(1);

  // Critical section end
  // (increment) semaphore
  sem_post(semaphore);

  printf("Thread %d: Leaving critical section\n", thread_id);
  return NULL;
}

int main() {
  pthread_t threads[NUM_THREADS];
  thread_data_t thread_data[NUM_THREADS];
  sem_t semaphore;

  if (sem_init(&semaphore, 0, 1) != 0) {
    perror("sem_init");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < NUM_THREADS; ++i) {
    thread_data[i] = (thread_data_t){.semaphore = &semaphore,
                    .thread_id = i + 1};
    if (pthread_create(&threads[i], NULL, thread_function, &thread_data[i]) !=
        0) {
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < NUM_THREADS; ++i) {
    if (pthread_join(threads[i], NULL) != 0) {
      perror("pthread_join");
      exit(EXIT_FAILURE);
    }
  }

  if (sem_destroy(&semaphore) != 0) {
    perror("sem_destroy");
    exit(EXIT_FAILURE);
  }

  printf("All threads have finished execution.\n");
  return EXIT_SUCCESS;
}