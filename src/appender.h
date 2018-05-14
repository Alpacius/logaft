#pragma once

#include    "stdc_common.h"
#include    "linux_common.h"
#include    "list.h"
#include    "encoder.h"
#include    "writer.h"

struct laft_appender {
    uint64_t id;
    intrusive;
    struct laft_encoder *e;
    struct laft_writer *w;
    // TODO rolling policy
};
