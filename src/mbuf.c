#include    "mbuf.h"

struct mbuf *mbuf_create_cstr(const char *cstr) {
    size_t cap = strlen(cstr) + 1;
    struct mbuf *mb = malloc(sizeof(struct mbuf) + sizeof(char) * cap);
    return mb ? 
        list_init(intrusion_from_ptr(mb)), 
        list_init(&(mb->lead)), 
        list_add_tail(intrusion_from_ptr(mb), &(mb->lead)),
        (mb->size = mb->cap = cap), 
        (mb->nblks = 1),
        (mb->refcnt = 1),
        memcpy(mb->blk, cstr, sizeof(char) * cap), 
        mb :
        NULL;
}

struct mbuf *mbuf_create_triv(size_t cap) {
    struct mbuf *mb = malloc(sizeof(struct mbuf) + sizeof(char) * cap);
    return mb ? 
        list_init(intrusion_from_ptr(mb)), 
        list_init(&(mb->lead)), 
        (mb->size = 0), 
        (mb->cap = cap), 
        list_add_tail(intrusion_from_ptr(mb), &(mb->lead)),
        (mb->nblks = 1),
        (mb->refcnt = 1),
        mb :
        NULL;
}

int mbuf_fill_cstr(struct mbuf *mb, const char *cstr) {
    size_t size = strlen(cstr) + 1;
    if (unlikely(size > mb->cap))
        return 0;
    memcpy(mb->blk, cstr, sizeof(char) * size), (mb->size = size);
    return 1;
}

void mbuf_borrow(struct mbuf *mb) {
    __atomic_add_fetch(&(mb->refcnt), 1, __ATOMIC_ACQ_REL);
}

void mbuf_destroy(struct mbuf *mb) {
    if (__atomic_sub_fetch(&(mb->refcnt), 1, __ATOMIC_ACQ_REL))
        return;
    list_del(intrusion_from_ptr(mb));
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
