#include    "singletons.h"
#include    "simple_rrouter.h"


#define     N_LOG_THREADS       2

static struct task_executor *default_executor = NULL;
static struct recrouter *default_rrouter = NULL;

int laft_init(void) {
    // TODO refactor
    default_executor = task_executor_create(N_LOG_THREADS);
    default_rrouter = simple_rrouter_create(N_LOG_THREADS);
    return 0;
}

struct task_executor *common_executor(void) {
    return default_executor;
}

struct recrouter *common_recrouter(void) {
    return default_rrouter;
}
