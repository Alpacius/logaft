#include    "taskexecutor.h"

static void *task_worker_loop(void *arg);

int task_executor_init(struct task_executor *e, uint32_t s, struct recrouter *r) {
    (e->logrec_router = r), (e->size = s), __atomic_store_n(&(e->active), 0, __ATOMIC_ACQ_REL);
    if (tqueue_init(&(e->tasks), s) < 1)
        return 0;
    pthread_barrier_init(&(e->active_barrier), NULL, size);
    for (uint32_t i = 0; i < s; i++)
        pthread_create(&(e->workers[i]), NULL, task_worker_loop, e);
    __atomic_store_n(&(e->active), 1, __ATOMIC_ACQ_REL);
    return 1;
}

void task_executor_ruin(struct task_executor *e, struct link_index *t) {
    __atomic_store_n(&(e->active), 0, __ATOMIC_ACQ_REL);
    void *tret = NULL;
    for (uint32_t i = 0; i < e->size; i++)
        pthread_join(&(e->workers[i]), &tret);
    if (t) {
        // TODO dump tasks for further deletion
    }
    tqueue_ruin(&(e->tasks));
}

struct task_executor *task_executor_create(uint32_t size, struct recrouter *r) {
    struct task_executor *e = malloc(sizeof(struct task_executor) + sizeof(pthread_t) * size);
    return e ? (task_executor_init(e, size, r) ? e : (free(e), NULL)) : NULL;
}

void task_executor_delete(struct task_executor *e, struct link_index *t) {
    task_executor_ruin(e, t);
    free(e);
}

int task_executor_sumbit(struct task_executor *e, struct tasklet *t) {
    return tqueue_push_back(&(e->tasks), intrusion_from_ptr(t)), 0;
}

static
void *task_worker_loop(void *arg) {
    struct task_executor *e = arg;
    pthread_barrier_wait(&(e->active_barrier));
    while (__atomic_load_n(&(e->active), __ATOMIC_ACQUIRE)) {
        // FIXME brutal implementation
        struct link_index *tref = tqueue_shift(&(e->tasks), do_gettid() % e->size);
        if (unlikely(tref == NULL))
            continue;
        struct tasklet *task = intruded_val(tref, struct tasklet);
        task->execute(task);
        task->dtor_hook(task);
    }
    return NULL;
}
