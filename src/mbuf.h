#pragma once

#include    "stdc_common.h"
#include    "linux_common.h"
#include    "list.h"

struct mbuf {
    intrusive;
    struct link_index lead;
    size_t size, cap;
    uint32_t nblks;
    char blk[];
};

// TODO alpacius - long cstr split
static inline
struct mbuf *mbuf_create(const char *cstr) {
    size_t cap = strlen(cstr) + 1;
    struct mbuf *mb = malloc(sizeof(struct mbuf) + sizeof(char) * cap);
    return mb ? 
        list_init(intrusion_from_ptr(mb)), 
        list_init(&(mb->lead)), 
        (mb->size = mb->cap = cap), 
        (mb->nblks = 1),
        memcpy(mb->blk, cstr, sizeof(char) * cap), 
        mb :
        NULL;
}

static inline
void mbuf_destroy(struct mbuf *mb) {
    if (!list_is_empty(&(mb->lead))) {
        list_foreach_remove(&(mb->lead)) {
            detach_current_iterator;
            struct mbuf *sub = intrusive_ref(struct mbuf);
            free(sub);
        }
    }
    free(mb);
}

static inline
int mbuf_append_cstr(struct mbuf *master, const char *cstr) {
    struct mbuf *slave = mbuf_create(cstr);
    if (unlikely(slave == NULL))
        return 0;
    return list_add_tail(intrusion_from_ptr(slave), &(master->lead)), (master->nblks++), 1;
}

static inline
int mbuf_append_mbuf(struct mbuf *master, struct mbuf *slave) {
    return list_add_tail(intrusion_from_ptr(slave), &(master->lead)), (master->nblks++), 1;
}

#define mbuf_append(m_, a_) _Generic((a_), const char *: mbuf_append_cstr((m_), (a_)), struct mbuf *: mbuf_append_mbuf((m_), (a_)))
