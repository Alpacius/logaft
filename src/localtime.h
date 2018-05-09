#pragma once

#include    "./stdc_common.h"
#include    "./linux_common.h"


static inline
uint64_t epoch_current_ms(void) {
    struct timespec timeval;
    clock_gettime(CLOCK_REALTIME_COARSE, &timeval);
    return ((uint64_t) timeval.tv_sec) * 1000 + ((uint64_t) timeval.tv_nsec) / (1000 * 1000);
}

static inline
uint64_t epoch_adjusted_ms(int64_t diff) {
    return epoch_current_ms() + diff;
}
