#include    "logger.h"
#include    "singletons.h"


static uint32_t id_cnt = 0; 


struct laft_logger *laft_logger_create(int level, struct link_index *appenders) {
    struct laft_logger *l = malloc(sizeof(struct laft_logger));
    if (unlikely(l == NULL))
        return NULL;
    return 
        list_move(&(l->appenders), appenders),
        list_init(intrusion_from_ptr(l)),
        (l->id = __atomic_fetch_add(&id_cnt, 1, __ATOMIC_ACQ_REL)),
        (l->level = level),
        l;
}

void laft_logger_destroy(struct laft_logger *l) {
    list_foreach_remove(&(l->appenders)) {
        struct laft_appender *a = intrusive_ref(struct laft_appender);
        detach_current_iterator;
        laft_appender_destroy(a);
    }
    free(l);
}

int laft_logger_request(struct laft_logger *l, int level, char *content) {
    if (level > l->level)
        return 0;
    list_foreach(&(l->appenders)) {
        // TODO mbuf encoding optimization
        struct laft_appender *a = intrusive_ref(struct laft_appender);
        laft_log_append(a, level, content);
    }
    return 1;
}
