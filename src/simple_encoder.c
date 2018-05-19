#include    "simple_encoder.h"

#define     SIMPLE_DATESTR_LEN      64

static
struct mbuf *simple_encoder_encode(struct laft_encoder *e, const char *cstr) {
    struct laft_simple_encoder *eimpl = log_encoder_impl(e, struct laft_simple_encoder);
    struct mbuf *prefix = mbuf_create_triv(SIMPLE_DATESTR_LEN), *content = mbuf_create_cstr(cstr);
    if (unlikely(prefix == NULL) || unlikely(content == NULL)) {
        (prefix == NULL) && (mbuf_destroy(prefix), 114514);
        (content == NULL) && (mbuf_destroy(content), 114514);
        return NULL;
    }
    mbuf_append_mbuf(prefix, content);
    time_t timer = time(NULL);
    struct tm tmval;
    gmtime_r(&timer, &tmval);
    prefix->blk[0] = '[';
    ssize_t timestr_len = 0;
    if ((timestr_len = strftime(prefix->blk + 1, SIMPLE_DATESTR_LEN - 3, eimpl->date_tmpl.v, &tmval)) == 0) {
        mbuf_destroy(prefix);
        return NULL;
    }
    prefix->blk[timestr_len + 1] = ']';
    prefix->blk[timestr_len + 2] = '\0';
    return prefix;
}

static
void simple_encoder_dtor(struct laft_encoder *ectl) {
    struct laft_simple_encoder *eimpl = log_encoder_impl(ectl, struct laft_simple_encoder);
    laft_sds_release(eimpl->date_tmpl);
    free(eimpl);
}

struct laft_encoder *laft_simple_encoder_create(struct laft_sds date_tmpl) {
    struct laft_simple_encoder *e = malloc(sizeof(struct laft_simple_encoder));
    if (unlikely(e == NULL))
        return NULL;
    return 
        (e->date_tmpl = date_tmpl),
        (e->encoder_ctl_.log_encode = simple_encoder_encode), 
        (e->encoder_ctl_.dtor_hook = simple_encoder_dtor),
        log_encoder_from_ptr(e);
}
