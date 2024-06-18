#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <stddef.h> // for size_t
#include <pthread.h>
#include <stdbool.h>

typedef void* (*job_function)(void*);
typedef void* job_arg;
typedef pthread_mutex_t job_id;

/***
 * This is the stub of a simple job queue.
 */

typedef struct job_queue_entry {
    job_id done_info;
    job_function fun;
    job_arg args;

    struct job_queue_entry *next;
} job_queue_entry;

/***
 * This is the stub for the thread pool that uses the queue.
 * Implement at LEAST the Prototype functions below.
 */

typedef struct job_queue_init {
    job_queue_entry *head;
} job_queue_init;


typedef struct {
    size_t  size;
    pthread_t* threads;

    pthread_mutex_t lock_thread;
    pthread_mutex_t lock_queue;
    pthread_cond_t new_job;

    bool open;
    job_queue_init *job_queue;
} thread_pool;

// Prototypes for REQUIRED functions
void pool_create(thread_pool* pool, size_t size);
job_id pool_submit(thread_pool* pool, job_function start_routine,
                   job_arg arg); // You need to define a datatype for the job_id (chose wisely).
void pool_await(job_id id);
void pool_destroy(thread_pool* pool);

#endif
