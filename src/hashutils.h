#pragma once

#include    "stdc_common.h"

static inline
uint64_t djb_hashing_cstr(const char *cstr) {
    uint64_t hval = 5381;
    for (uint64_t idx = 0; cstr[idx]; idx++)
        hval = ((hval << 5) + hval) + cstr[idx];
    return hval;
}
