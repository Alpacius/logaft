#pragma once

#include    "stdc_common.h"
#include    "linux_common.h"
#include    "miscutils.h"
#include    "list.h"
#include    "tid.h"
#include    "cpbuffer.h"

struct cpr {
    struct cpbuffer r;
    pthread_mutex_t l;
};

struct pqueue {
    intrusive;
    uint32_t size;
    uint32_t lbidx;
    struct cpr q[];
};

static inline
struct pqueue *pqueue_init(struct pqueue *pq, uint32_t size) {
    (pq->size = size), (pq->lbidx = 0);
    for (uint32_t i = 0; i < size; i++)
        pthread_mutex_init(&(pq->q[i].l), NULL), cp_buffer_init(&(pq->q[i].r));
    return pq;
}

static inline
struct pqueue *pqueue_ruin(struct pqueue *pq) {
    return pq;
}

static inline
struct pqueue *pqueue_new(uint32_t size) {
    struct pqueue *pq = malloc(sizeof(struct pqueue) + sizeof(struct cpr) * size);
    return pq ? pqueue_init(pq, size) : NULL;
}

static inline
void pqueue_delete(struct pqueue *pq) {
    free(pqueue_ruin(pq));
}

static inline
uint32_t pqueue_token(struct pqueue *pq) {
   long tid = do_gettid();
   return tid > -1 ? (uint32_t) tid % pq->size : 0;
}

static inline
void pqueue_push_back(struct pqueue *pq, struct link_index *elt) {
    uint32_t idx = __atomic_fetch_add(&(pq->lbidx), 1, __ATOMIC_ACQ_REL) % pq->size;
    pthread_mutex_lock(&(pq->q[idx].l));
    cp_buffer_produce(&(pq->q[idx].r), elt);
    pthread_mutex_unlock(&(pq->q[idx].l));
}

static inline
struct link_index *pqueue_shift(struct pqueue *pq, uint32_t tok) {
    struct link_index *q = NULL;
    do {
        q = cp_buffer_consume(&(pq->q[tok].r));
    } while (!q);
    if (!list_is_empty(q)) {
        struct link_index *t = q->next;
        return list_del(t), t;
    } else 
        return NULL;
}

static inline
int pqueue_dump(struct pqueue *pq, uint32_t tok, struct link_index *t) {
    struct link_index *q = NULL;
    do {
        q = cp_buffer_consume(&(pq->q[tok].r));
    } while (!q);
    if (list_is_empty(q))
        return 0;
    list_move(t, q);
    return 1;
}
