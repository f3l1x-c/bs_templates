//
// Created by felix on 17.06.24.
//

#ifndef MYQUEUE_H_
#define MYQUEUE_H_

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/queue.h> // see queue(7) & stailq(3)

typedef struct {
    int x;
    int y;
} Pair;

struct myqueue_entry {
    Pair value;
    STAILQ_ENTRY(myqueue_entry) entries;
};

STAILQ_HEAD(myqueue_head, myqueue_entry);

typedef struct myqueue_head myqueue;

static void myqueue_init(myqueue* q) {
    STAILQ_INIT(q);
}

static bool myqueue_is_empty(myqueue* q) {
    return STAILQ_EMPTY(q);
}

static void myqueue_push(myqueue* q, Pair value) {
    struct myqueue_entry* entry = malloc(sizeof(struct myqueue_entry));
    entry->value = value;
    STAILQ_INSERT_TAIL(q, entry, entries);
}

static Pair myqueue_pop(myqueue* q) {
    assert(!myqueue_is_empty(q));
    struct myqueue_entry* entry = STAILQ_FIRST(q);
    const Pair value = entry->value;
    STAILQ_REMOVE_HEAD(q, entries);
    free(entry);
    return value;
}

#endif