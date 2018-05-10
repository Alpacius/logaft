#include    "mbuf.h"

struct mbuf {
    intrusive;
    struct link_index lead;
    size_t size, cap;
    uint32_t nblks;
    char blk[];
};

struct mbuf *mbuf_create_cstr(const char *cstr) {
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

void mbuf_destroy(struct mbuf *mb) {
    if (!list_is_empty(&(mb->lead)))
        list_foreach_remove(&(mb->lead)) {
            detach_current_iterator;
            struct mbuf *sub = intrusive_ref(struct mbuf);
            free(sub);
        }
    free(mb);
}

int mbuf_append_cstr(struct mbuf *master, const char *cstr) {
    struct mbuf *slave = mbuf_create_cstr(cstr);
    if (unlikely(slave == NULL))
        return 0;
    return list_add_tail(intrusion_from_ptr(slave), &(master->lead)), (master->nblks++), 1;
}

int mbuf_append_mbuf(struct mbuf *master, struct mbuf *slave) {
    return list_add_tail(intrusion_from_ptr(slave), &(master->lead)), (master->nblks++), 1;
}

