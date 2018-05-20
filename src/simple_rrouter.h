#include    "recrouter.h"

struct simple_rrouter {
    rrouter;
    uint32_t mod_size;
};

struct recrouter *simple_rrouter_create(uint32_t mod_size);
