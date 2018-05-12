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
    tq->size = size;
    if (unlikely((tq->evfds = malloc(sizeof(int) * size)) == NULL))
        return 0;
    for (uint32_t i = 0; i < size; i++)
        if ((tq->evfds[i] = eventfd(0, EFD_CLOEXEC)) == -1) {
            for (uint32_t j = 0; j < i; j++)
                close(tq->evfds[j]);
            return free(tq->evfds), 0;
        }
    if (unlikely((tq->q = pqueue_new(size)) == NULL)) {
        for (uint32_t j = 0; j < size; j++)
            close(tq->evfds[j]);
        return 0;
    }
    return 1;
}

static inline
void tqueue_ruin(struct tqueue *tq) {
    for (uint32_t i = 0; i < tq->size; i++)
        close(tq->evfds[i]);
    pqueue_delete(tq->q);
}

static inline
int tqueue_dump(struct tqueue *tq, uint32_t tok, struct link_index *t) {
    uint64_t sink;
    return (read(tq->evfds[tok], &sink, sizeof(sink)) > -1) ?
        pqueue_dump(tq->q, tok, t) :
        0;
}

static inline
void tqueue_push_back(struct tqueue *tq, struct link_index *elt) {
    pqueue_push_back(tq->q, elt);
} 
