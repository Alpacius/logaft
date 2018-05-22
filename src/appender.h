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

struct laft_appender *laft_appender_create(struct laft_encoder *e, struct laft_writer *w);
int laft_log_append(struct laft_appender *a, int level, char *content);
void laft_appender_destroy(struct laft_appender *a);
