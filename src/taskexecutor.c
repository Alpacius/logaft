#include    "taskexecutor.h"

static void *task_worker_loop(void *arg);

int task_executor_init(struct task_executor *e, uint32_t s, struct recrouter *r) {
    // TODO implementation
    return 1;
}

void task_executor_ruin(struct task_executor *e) {
    // TODO implementation
}

struct task_executor *task_executor_create(uint32_t size, struct recrouter *r) {
    struct task_executor *e = malloc(sizeof(struct task_executor) + sizeof(pthread_t) * size);
    return e ? task_executor_init(e, size, r) : NULL;
}

void task_executor_delete(struct task_executor *e, list_ctl_t *t) {
    // TODO dump tasks for further deletion
    task_executor_ruin(e);
    free(e);
}

int task_executor_sumbit(struct task_executor *e, struct tasklet *t) {
    // TODO implementation
    return 0;
}

static
void *task_worker_loop(void *arg) {
    // TODO implementation
    return NULL;
}
