#pragma once

#include    "stdc_common.h"
#include    "linux_common.h"
#include    "logger.h"
#include    "list.h"

struct laft_appender_config {
    intrusive;
    const char *name;
    const char *dateformat;
    const char *logpath;
};

struct laft_logger_config {
    intrusive;
    const char *name;
    int level;
    uint64_t n_appenders;
    const char **appenders;

};

struct laft_config {
    struct link_index appenders;
    struct link_index loggers;
};
