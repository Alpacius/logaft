#pragma once

#include    "stdc_common.h"
#include    "miscutils.h"
#include    "list.h"

struct tasklet {
    intrusive;
    int (*execute)(struct tasklet *);
    void (*dtor_hook)(struct tasklet *);
};

#define     laft_tasklet            struct tasklet tasklet_ctl_
#define     tasklet_from_imm(i_)    (&((i_).tasklet_ctl_))
#define     tasklet_from_ptr(p_)    (&((p_)->tasklet_ctl_))
#define     tasklet_host(p_, t_)    container_of((p_), t_, tasklet_ctl_)
