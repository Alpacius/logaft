#pragma once

#include    "stdc_common.h"
#include    "linux_common.h"
#include    "miscutils.h"
#include    "list.h"
#include    "appender.h"

struct recrouter {
    intrusive;
    uint32_t (*routing_tok)(struct laft_appender *);
};

#define     rrouter                 struct recrouter recrouter_ctl_
#define     rrouter_from_imm(i_)    (&((i_).recrouter_ctl_))
#define     rrouter_from_ptr(p_)    (&((p_)->recrouter_ctl_))
#define     rrouter_impl(p_, t_)    container_of((p_), t_, recrouter_ctl_)
