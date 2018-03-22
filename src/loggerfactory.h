#pragma once

#include    "loggerconf.h"

int logger_factory_init(const struct laft_config *conf);
struct laft_logger *laft_get_logger(const char *name);
