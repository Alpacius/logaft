#pragma once

#include    "stdc_common.h"
#include    "linux_common.h"
#include    "miscutils.h"
#include    "mbuf.h"

struct laft_writer {
    int (*log_write)(struct laft_writer *, struct mbuf *);
    void (*dtor_hook)(struct laft_writer *);
};

#define     log_writer                  struct laft_writer writer_ctl_
#define     log_writer_from_imm(i_)     (&((i_).writer_ctl_))
#define     log_writer_from_ptr(p_)     (&((p_)->writer_ctl_))
#define     log_writer_impl(p_, t_)     container_of((p_), t_, writer_ctl_)
