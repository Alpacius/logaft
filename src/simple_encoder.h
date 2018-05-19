#pragma once

#include    "stdc_common.h"
#include    "linux_common.h"
#include    "list.h"
#include    "sds.h"
#include    "encoder.h"

struct laft_simple_encoder {
    log_encoder;
    struct laft_sds date_tmpl;
};

struct laft_encoder *laft_simple_encoder_create(struct laft_sds date_tmpl);

#define laft_default_encoder laft_simple_encoder_create(laft_sds_literal("%F %T"))
