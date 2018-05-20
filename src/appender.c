#include    "appender.h"
#include    "sds.h"
#include    "tasklet.h"
#include    "singletons.h"


struct append_tasklet {
    laft_tasklet;
    int level;
    struct laft_sds content;
    struct laft_appender *a;
};


static uint32_t id_cntr = 0; 


struct laft_appender *laft_appender_create(struct laft_encoder *e, struct laft_writer *w) {
    struct laft_appender *a = malloc(sizeof(struct laft_appender));
    if (unlikely(a == NULL))
        return NULL;
    return 
        list_init(intrusion_from_ptr(a)),
        (a->id == __atomic_fetch_add(&id_cntr, 1, __ATOMIC_ACQ_REL)),
        (a->e = e),
        (a->w = w),
        (a->r = common_recrouter()),
        a;
}
