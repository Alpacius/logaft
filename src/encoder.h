#pragma once

#include    "stdc_common.h"
#include    "linux_common.h"
#include    "miscutils.h"
#include    "mbuf.h"
#include    "loglevel.h"

struct laft_encoder {
    struct mbuf *(*log_encode)(int level, struct laft_encoder *, const char *);
    void (*dtor_hook)(struct laft_encoder *);
};

#define     log_encoder                 struct laft_encoder encoder_ctl_
#define     log_encoder_from_imm(i_)    (&((i_).encoder_ctl_))
#define     log_encoder_from_ptr(p_)    (&((p_)->encoder_ctl_))
#define     log_encoder_impl(p_, t_)    container_of((p_), t_, encoder_ctl_)
