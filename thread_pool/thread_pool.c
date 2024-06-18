#include "thread_pool.h"
#include <stdlib.h>
#include <stdio.h>

void *thread_pool_party(void* args){
    thread_pool *whirlpool = (thread_pool*)args;

    while (whirlpool->open){
        // only one thread wait for new job
        pthread_mutex_lock(&whirlpool->lock_thread);

        // wait for new job entry
        while(whirlpool->job_queue->head == NULL && whirlpool->open){
            // sleep until condition for new job is set
            pthread_cond_wait(&whirlpool->new_job, &whirlpool->lock_thread);
        }
        if (!whirlpool->open){break;}

        // get new job (function and args)
        pthread_mutex_lock(&whirlpool->lock_queue);
        job_queue_entry *next_job = whirlpool->job_queue->head;
        whirlpool->job_queue->head = next_job->next;

        pthread_mutex_unlock(&whirlpool->lock_queue);
        pthread_mutex_unlock(&whirlpool->lock_thread);

        // run the job
        next_job->fun(next_job->args);

        // set current job to done
        printf("Unlock job %p\n", (void*)&next_job->done_info);
        pthread_mutex_unlock(&next_job->done_info);

        // set job to state done
        // free(next_job);
    }

    return NULL;
}

void pool_create(thread_pool* pool, size_t size){
    pool->size = size;
    pool->open = true;
    pool->threads = malloc(size * sizeof(pthread_t));

    pool->job_queue = malloc(sizeof (job_queue_init));
    pool->job_queue->head = NULL;

    pthread_cond_init(&(pool->new_job), NULL);
    pthread_mutex_init(&(pool->lock_thread), NULL);
    pthread_mutex_init(&(pool->lock_queue), NULL);

    // jump threads into the pool
    for (size_t cnt = 0; cnt<size; cnt++){
        pthread_create(&(pool->threads[cnt]), NULL, thread_pool_party, pool);
    }

}

job_id pool_submit(thread_pool* pool, job_function start_routine, job_arg arg) {
    // create new job
    job_queue_entry *new_job = malloc(sizeof (struct job_queue_entry));

    // generate new unique id
    pthread_mutex_init(&(new_job->done_info), NULL);
    pthread_mutex_lock(&(new_job->done_info));

    // set fun, args and next to NULL because it will be the last item for now in the queue
    new_job->fun = start_routine;
    new_job->args = arg;
    new_job->next = NULL;

    // look the queue
    pthread_mutex_lock(&pool->lock_queue);

    // when head is NULL it will be the next job to execute
    if (pool->job_queue->head == NULL){
        // put job into queue
        pool->job_queue->head = new_job;

    } else {
        // shift to next free entry of the queue
        job_queue_entry *tmp = pool->job_queue->head;
        while (tmp->next != NULL){
            tmp = tmp->next;
        }
        tmp->next = new_job;
    }

    // send signal to work
    pthread_mutex_unlock(&pool->lock_queue);
    pthread_cond_signal(&pool->new_job);

    printf("Return job %p\n", (void*)&(new_job->done_info));
    return new_job->done_info;
}

void pool_await(job_id id) {
    printf("Wait for unlock job %p\n", (void*)&id);
    pthread_mutex_lock(&id);
    pthread_mutex_unlock(&id);
}

void pool_destroy(thread_pool* pool) {
    pthread_mutex_lock(&(pool->lock_thread));
    pthread_cond_signal(&pool->new_job);
    pool->open = false;
    pthread_mutex_unlock(&(pool->lock_thread));

    // free threads
    for (size_t i = 0; i < pool->size; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    free(pool->threads);
    free(pool->job_queue);
    pthread_cond_destroy(&(pool->new_job));
    pthread_mutex_destroy(&(pool->lock_thread));
    pthread_mutex_destroy(&(pool->lock_queue));
}
