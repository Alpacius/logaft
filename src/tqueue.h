#pragma once

#include    "stdc_common.h"
#include    "linux_common.h"
#include    "pqueue.h"

struct tqueue {
    uint32_t size;
    int *evfds;
    struct pqueue *q;
};

static inline
int tqueue_init(struct tqueue *tq, uint32_t size) {
    // TODO implementation
    return 1;
}

static inline
void tqueue_ruin(struct tqueue *tq) {
    pqueue_delete(tq->q);
}

static inline
struct link_index *tqueue_shift(struct tqueue *tq, uint32_t tok) {
    // TODO implementation
    return NULL;
}

static inline
void tqueue_push_back(struct tqueue *tq, struct link_index *elt) {
    pqueue_push_back(tq->q, elt);
    // TODO implementation
} 
