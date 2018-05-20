#pragma once

#include    "stdc_common.h"
#include    "linux_common.h"
#include    "list.h"
#include    "encoder.h"
#include    "writer.h"
#include    "recrouter.h"

struct laft_appender {
    uint32_t id;
    intrusive;
    struct laft_encoder *e;
    struct laft_writer *w;
    struct recrouter *r;
};
