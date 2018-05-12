#pragma once

#include    "std_common.h"
#include    "linux_common.h"
#include    "miscutils.h"
#include    "mbuf.h"

struct laft_encoder {
    struct mbuf *(*log_encode)(struct laft_encoder *, const char *);
    void (*dtor_hook)(struct laft_encoder *);
};

#define     encoder                 struct laft_encoder encoder_ctl_
#define     encoder_from_imm(i_)    (&((i_).encoder_ctl_))
#define     encoder_from_ptr(p_)    (&((p_)->encoder_ctl_))
#define     encoder_impl(p_, t_)    container_of((p_), t_, encoder_ctl_)
