#include    "singletons.h"
#include    "simple_rrouter.h"


#define     N_LOG_THREADS       1

static struct task_executor *default_executor = NULL;
static struct recrouter *default_rrouter = NULL;

static int laft_running = 0;

int laft_init(void) {
    if (!laft_running) {
        default_executor = task_executor_create(N_LOG_THREADS);
        default_rrouter = simple_rrouter_create(N_LOG_THREADS);
        laft_running = 1;
    }
    return 0;
}

int laft_ruin(void) {
    task_executor_delete(default_executor, NULL);
    default_rrouter->dtor_hook(default_rrouter);
}

struct task_executor *common_executor(void) {
    return default_executor;
}

struct recrouter *common_recrouter(void) {
    return default_rrouter;
}
