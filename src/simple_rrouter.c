#include    "simple_rrouter.h"
#include    "appender.h"


static
uint32_t sr_routing_tok(struct recrouter *r, void *seed) {
    struct simple_rrouter *sr = rrouter_impl(r, struct simple_rrouter);
    struct laft_appender *a = seed;
    return a->id % sr->mod_size;
}

static
void sr_dtor_hook(struct recrouter *r) {
    struct simple_rrouter *sr = rrouter_impl(r, struct simple_rrouter);
    free(sr);
}

struct recrouter *simple_rrouter_create(uint32_t mod_size) {
    struct simple_rrouter *r = malloc(sizeof(struct simple_rrouter));
    if (unlikely(r == NULL))
        return NULL;
    return 
        (r->mod_size = mod_size), 
        (r->recrouter_ctl_.routing_tok = sr_routing_tok), 
        (r->recrouter_ctl_.dtor_hook = sr_dtor_hook), 
        rrouter_from_ptr(r);
}
