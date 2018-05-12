#pragma once

#include    "stdc_common.h"
#include    "linux_common.h"
#include    "list.h"
#include    "appender.h"

#define     LAFT_LOGLEVEL_FATAL     0
#define     LAFT_LOGLEVEL_ERROR     1
#define     LAFT_LOGLEVEL_WARN      2
#define     LAFT_LOGLEVEL_INFO      3
#define     LAFT_LOGLEVEL_TRACE     4
#define     LAFT_LOGLEVEL_DEBUG     5

#define     LAFT_LOGLEVEL_SINK     -1

struct laft_logger {
    uint64_t id;
    intrusive;
    int level;
    struct link_index appenders;
};
