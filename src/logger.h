#pragma once

#include    "stdc_common.h"
#include    "linux_common.h"
#include    "list.h"
#include    "appender.h"
#include    "loglevel.h"

struct laft_logger {
    uint64_t id;
    intrusive;
    int level;
    struct link_index appenders;
};
