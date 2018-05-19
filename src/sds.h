#pragma once

#include    "stdc_common.h"

struct laft_sds {
    union {
        char *d;
        const char *c;
    } ref;
    const char *v;
    void (*dtor_hook_)(void *);
};

#define laft_sds_literal(l_) ({ const char *l__ = (l_); struct laft_sds ret_ = { .dtor_hook_ = NULL, .ref.c = l__, .v = l__ }; ret_; })
#define laft_sds_ref(r_) ({ char *r__ = (r_); struct laft_sds ret_ = { .dtor_hook_ = free, .ref.d = r__, .v = r__ }; ret_; })
#define laft_sds_release(s_) ({ struct laft_sds s__ = (s_); if (s__.dtor_hook_) s__.dtor_hook_(s__.ref.d); 1; })
