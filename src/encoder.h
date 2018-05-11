#pragma once

#include    "std_common.h"
#include    "linux_common.h"
#include    "miscutils.h"
#include    "mbuf.h"

struct laft_encoder {
    struct mbuf *log_encode(struct laft_encoder *, const char *);
};

#define     encoder                 struct laft_encoder encoder_ctl_
#define     encoder_from_imm(i_)    (&((i_).encoder_ctl_))
#define     encoder_from_ptr(p_)    (&((p_)->encoder_ctl_))
#define     encoder_impl(i_, t_)    container_of((i_), t_, encoder_ctl_)
