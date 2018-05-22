#pragma once

#include    "stdc_common.h"
#include    "linux_common.h"
#include    "list.h"
#include    "appender.h"
#include    "loglevel.h"

struct laft_logger {
    uint32_t id;
    intrusive;
    int level;
    struct link_index appenders;
};

struct laft_logger *laft_logger_create(int level, struct link_index *appenders);
void laft_logger_destroy(struct laft_logger *l);
int laft_logger_request(struct laft_logger *l, int level, char *content);
