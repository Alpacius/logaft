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


static struct tasklet *append_tasklet_create(struct laft_appender *a, char *csref, int level);


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

void laft_appender_destroy(struct laft_appender *a) {
    a->e->dtor_hook(a->e);
    a->w->dtor_hook(a->w);
    free(a);
}

int laft_log_append(struct laft_appender *a, int level, char *content) {
    struct tasklet *t = append_tasklet_create(a, content, level);       // TODO epoch hint
    if (unlikely(t == NULL))
        return 0;
    struct task_executor *x = common_executor();
    int ret = task_executor_submit_m(x, t, a->r->routing_tok(a->r, a)); 
    return ret ?
        1 :
        t->dtor_hook(t), 0;
}

static
int append_tasklet_execute(struct tasklet *t) {
    struct append_tasklet *timpl = tasklet_host(t, struct append_tasklet);
    struct laft_encoder *e = timpl->a->e;
    struct mbuf *blks = e->log_encode(timpl->level, e, timpl->content.v);
    if (unlikely(blks == NULL))
        return 0;
    struct laft_writer *w = timpl->a->w;
    return w->log_write(w, blks);
}

static
void append_tasklet_dtor_hook(struct tasklet *t) {
    struct append_tasklet *timpl = tasklet_host(t, struct append_tasklet);
    laft_sds_release(timpl->content);
    free(timpl);
}

static
struct tasklet *append_tasklet_create(struct laft_appender *a, char *csref, int level) {
    struct append_tasklet *t = malloc(sizeof(struct append_tasklet));
    if (unlikely(t == NULL))
        return NULL;
    size_t cslen = strlen(csref) + 1;
    char *csref_ = malloc(sizeof(char) * cslen);
    memcpy(csref_, csref, cslen);
    return
        (t->a = a),
        (t->tasklet_ctl_.execute = append_tasklet_execute),
        (t->tasklet_ctl_.dtor_hook = append_tasklet_dtor_hook),
        (t->level = level),
        (t->content = laft_sds_ref(csref_)),
        tasklet_from_ptr(t);
}
