#include    "simple_encoder.h"

#define     SIMPLE_DATESTR_LEN      64

static const char *level_desc_tbl[] = {
    [LAFT_LOGLEVEL_FATAL] = "[FATAL]",
    [LAFT_LOGLEVEL_ERROR] = "[ERROR]",
    [LAFT_LOGLEVEL_WARN] = "[WARN]",
    [LAFT_LOGLEVEL_INFO] = "[INFO]",
    [LAFT_LOGLEVEL_TRACE] = "[TRACE]",
    [LAFT_LOGLEVEL_DEBUG] = "[DEBUG]"
};

static
struct mbuf *simple_encoder_encode(int level, struct laft_encoder *e, const char *cstr) {
    if (unlikely(level < 0))
        return NULL;
    struct laft_simple_encoder *eimpl = log_encoder_impl(e, struct laft_simple_encoder);
    struct mbuf *lv = mbuf_create_cstr(level_desc_tbl[level]), *prefix = mbuf_create_triv(SIMPLE_DATESTR_LEN), *content = mbuf_create_cstrln(cstr);
    if (unlikely(lv == NULL), (prefix == NULL) || unlikely(content == NULL)) {
        (lv == NULL) && (mbuf_destroy(lv), 114514);
        (prefix == NULL) && (mbuf_destroy(prefix), 114514);
        (content == NULL) && (mbuf_destroy(content), 114514);
        return NULL;
    }
    mbuf_append_mbuf(lv, prefix);
    mbuf_append_mbuf(lv, content);
    time_t timer = time(NULL);
    struct tm tmval;
    localtime_r(&timer, &tmval);
    prefix->blk[0] = '[';
    ssize_t timestr_len = 0;
    if ((timestr_len = strftime(prefix->blk + 1, SIMPLE_DATESTR_LEN - 4, eimpl->date_tmpl.v, &tmval)) == 0) {
        mbuf_destroy(prefix);
        return NULL;
    }
    prefix->blk[timestr_len + 1] = ']';
    prefix->blk[timestr_len + 2] = ' ';
    prefix->blk[timestr_len + 3] = '\0';
    prefix->size = strlen(prefix->blk) + 1;
    return lv;
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
