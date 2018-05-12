#pragma once

#include    "stdc_common.h"
#include    "linux_common.h"
#include    "pqueue.h"

struct tqueue {
    int evfd;
    struct pqueue *q;
};

static inline
int tqueue_init(struct tqueue *tq, uint32_t size) {
    if ((tq->evfd = eventfd(0, EFD_CLOEXEC)) == -1)
        return 0;
    if (unlikely((tq->q = pqueue_new(size)) == NULL))
        return close(tq->evfd) > -1;
    return 1;
}

static inline
void tqueue_ruin(struct tqueue *tq) {
    pqueue_delete(tq->q), close(tq->evfd);
}

static inline
struct link_index *tqueue_shift(struct tqueue *tq, uint32_t tok) {
    uint64_t sink;
    return (read(tq->evfd, &sink, sizeof(sink)) > -1) ?
        pqueue_shift(tq->q, tok) :
        NULL;
}

static inline
int tqueue_dump(struct tqueue *tq, uint32_t tok, struct link_index *t) {
    uint64_t sink;
    return (read(tq->evfd, &sink, sizeof(sink)) > -1) ?
        pqueue_dump(tq->q, tok, t) :
        0;
}

static inline
void tqueue_push_back(struct tqueue *tq, struct link_index *elt) {
    pqueue_push_back(tq->q, elt);
} 
