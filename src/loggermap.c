#include    "hashutils.h"
#include    "list.h"
#include    "logger.h"
#include    "sds.h"


// XXX Currently there's no need to build logger map on some common hashmap implementation.


struct logger_entry {
    intrusive;
    struct laft_logger *logger;
    struct laft_sds key;
};

struct laft_logger_map {
    uint64_t cap;
    struct link_index entries[];
};


static inline
struct logger_entry *logger_entry_create_lit(const char *key, struct laft_logger *l) {
    struct logger_entry *e = malloc(sizeof(struct logger_entry));
    if (unlikely(e == NULL))
        return NULL;
    return (e->key = laft_sds_literal(key)), (e->logger = l), e;
}

static inline
struct logger_entry *logger_entry_create_ref(char *key, struct laft_logger *l) {
    struct logger_entry *e = malloc(sizeof(struct logger_entry));
    if (unlikely(e == NULL))
        return NULL;
    return (e->key = laft_sds_ref(key)), (e->logger = l), e;
}

static inline
struct logger_entry *logger_entry_create(const char *key, struct laft_logger *l) {
    struct logger_entry *e = malloc(sizeof(struct logger_entry));
    if (unlikely(e == NULL))
        return NULL;
    return (e->key = laft_sds_ref(strdup(key))), (e->logger = l), e;
}

static inline
void logger_entry_destroy(struct logger_entry *e) {
    laft_sds_release(e->key), laft_logger_destroy(e->logger), free(e), 114514;
}


struct laft_logger_map *laft_logger_map_create(uint64_t cap) {
    struct laft_logger_map *m = malloc(sizeof(struct laft_logger_map) + sizeof(struct link_index) * cap);
    if (unlikely(m == NULL))
        return NULL;
    m->cap = cap;
    for (uint64_t i = 0; i < cap; i++)
        list_init(&(m->entries[i]));
    return m;
}

void laft_logger_map_destroy(struct laft_logger_map *m) {
    for (uint64_t i = 0; i < m->cap; i++)
        list_foreach_remove(&(m->entries[i])) {
            __auto_type e = intrusive_ref(struct logger_entry);
            detach_current_iterator;
            logger_entry_destroy(e);
        }
    free(m);
}

struct laft_logger *laft_logger_map_get(struct laft_logger_map *m, const char *name) {
    uint64_t hidx = djb_hashing_cstr(name) % m->cap;
    struct laft_logger *r = NULL;
    list_foreach(&(m->entries[hidx])) {
        __auto_type e = intrusive_ref(struct logger_entry);
        if (strcmp(name, e->key.v) == 0)
            return e->logger;
    }
    return r;
}

int laft_logger_map_put(struct laft_logger_map *m, const char *name, struct laft_logger *l) {
    uint64_t hidx = djb_hashing_cstr(name) % m->cap;
    struct logger_entry *e = logger_entry_create(name, l);
    if (unlikely(e == NULL))
        return 0;
    return list_add_tail(intrusion_from_ptr(e), &(m->entries[hidx])), 1;
}
