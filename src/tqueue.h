#pragma once

#include    "stdc_common.h"
#include    "linux_common.h"
#include    "pqueue.h"

struct tqueue {
    uint32_t size;
    uint32_t lbidx;
    sem_t *sem;
    struct pqueue *q;
};

static inline
int tqueue_init(struct tqueue *tq, uint32_t size) {
    (tq->size = size), (tq->lbidx = 0);
    if ((tq->q = pqueue_new(size)) == NULL)
        return 0;
    if ((tq->sem = malloc(sizeof(sem_t) * size)) == NULL)
        return pqueue_ruin(tq->q), 0;
    for (uint32_t i = 0; i < size; i++)
        sem_init(&(tq->sem[i]), PTHREAD_PROCESS_PRIVATE, 0);
    return 1;
}

static inline
void tqueue_ruin(struct tqueue *tq) {
    pqueue_delete(tq->q);
    for (uint32_t i = 0; i < tq->size; i++)
        sem_destroy(&(tq->sem[i]));
}

static inline
struct link_index *tqueue_shift(struct tqueue *tq, uint32_t tok) {
    int errsv = 0, ret = 0;
    while (((ret = sem_wait(&(tq->sem[tok]))) == -1) && ((errsv = errno) == EINTR));
    if (ret == -1)
        return NULL;
    return pqueue_shift(tq->q, tok);
}

static inline
void tqueue_push_back_m(struct tqueue *tq, struct link_index *elt, uint32_t tok) {
    pqueue_push_back_m(tq->q, elt, tok);
    sem_post(&(tq->sem[tok]));   // XXX dangerous
}


static inline
void tqueue_push_back(struct tqueue *tq, struct link_index *elt) {
    tqueue_push_back_m(tq, elt, __atomic_fetch_add(&(tq->lbidx), 1, __ATOMIC_ACQ_REL) % tq->size);
} 
// TODO post-decay unsafe operations
