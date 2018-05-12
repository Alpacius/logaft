#pragma once

#include    "stdc_common.h"
#include    "linux_common.h"
#include    "tqueue.h"
#include    "recrouter.h"
#include    "tid.h"

struct tasklet {
    intrusive;
    int (*execute)(struct tasklet *);
    void (*dtor_hook)(struct tasklet *);
};

#define     laft_tasklet            struct tasklet tasklet_ctl_
#define     tasklet_from_imm(i_)    (&((i_).tasklet_ctl_))
#define     tasklet_from_ptr(p_)    (&((p_)->tasklet_ctl_))
#define     tasklet_host(p_, t_)    container_of((p_), t_, tasklet_ctl_)

struct task_executor {
    int active;
    pthread_barrier_t active_barrier;
    uint32_t size;
    struct recrouter *logrec_router;
    struct tqueue tasks;
    pthread_t workers[];
};
