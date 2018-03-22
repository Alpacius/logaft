#pragma once

#include    "stdc_common.h"
#include    "logger.h"

struct lafft_logger_map;

struct laft_logger_map *laft_logger_map_create(uint64_t cap);
void laft_logger_map_destroy(struct laft_logger_map *m);
struct laft_logger *laft_logger_map_get(struct laft_logger_map *m, const char *name);
int laft_logger_map_put(struct laft_logger_map *m, const char *name, struct laft_logger *l);

