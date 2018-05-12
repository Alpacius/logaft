#pragma once

#include    "stdc_common.h"
#include    "linux_common.h"
#include    "pqueue.h"

struct tqueue {
    uint32_t size;
    sem_t sem;
    struct pqueue *q;
};

static inline
int tqueue_init(struct tqueue *tq, uint32_t size) {
    tq->size = size;
    if ((tq->q = pqueue_new(size)) == NULL)
        return 0;
    if (sem_init(&(tq->sem), PTHREAD_PROCESS_PRIVATE, 0) == -1)
        return pqueue_delete(tq->q), 0;
    return 1;
}

static inline
void tqueue_ruin(struct tqueue *tq) {
    pqueue_delete(tq->q), sem_destroy(&(tq->sem));
}

static inline
struct link_index *tqueue_shift(struct tqueue *tq, uint32_t tok) {
    int errsv = 0, ret = 0;
    while (((ret = sem_wait(&(tq->sem))) == -1) && ((errsv = errno) == EINTR));
    if (ret == -1)
        return NULL;
    return pqueue_shift(tq->q, tok);
}

static inline
void tqueue_push_back(struct tqueue *tq, struct link_index *elt) {
    pqueue_push_back(tq->q, elt);
    sem_post(&(tq->sem));   // XXX dangerous
} 
