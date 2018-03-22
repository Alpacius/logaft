#include    "stdc_common.h"
#include    "linux_common.h"
#include    "loggerconf.h"
#include    "loggermap.h"
#include    "simple_encoder.h"
#include    "simple_writer.h"


#define     DEFAULT_LOGGERMAP_CAP       64


static struct laft_logger_map *loggers;
static int factory_initflag = 0;

struct appender_tmp_idx {
    struct laft_appender *ainst;
    struct laft_appender_config *aconf;
};

static inline
int logger_conf_init(const struct laft_config *conf) {
    // 1. appenders construction
    struct link_index aidx;
    list_init(&aidx);
    uint64_t nappenders = 0;
    list_foreach(&(conf->appenders))
        nappenders++;
    if (nappenders == 0)
        nappenders = 1;
    struct appender_tmp_idx *apile = alloca(sizeof(struct appender_tmp_idx) * nappenders);
    uint64_t apile_idx = 0;
    list_foreach(&(conf->appenders)) {
        __auto_type aconf = intrusive_ref(struct laft_appender_config);
        struct laft_encoder *e = laft_simple_encoder_create(laft_sds_literal(aconf->dateformat));
        struct laft_writer *w = simple_writer_create(aconf->logpath);
        struct laft_appender *ainst = laft_appender_create(e, w);
        (apile[apile_idx].ainst = ainst), (apile[apile_idx].aconf = aconf), (apile_idx++);

    }
    // 2. loggers construction
    list_foreach(&(conf->loggers)) {
        __auto_type lconf = intrusive_ref(struct laft_logger_config);
        // XXX Yeah, brutal, yeah
        struct laft_appender *fetch_appender(const char *name) {
            for (uint64_t idx = 0; idx < nappenders; idx++) {
                __auto_type assoc = apile[idx];
                if (strcmp(name, assoc.aconf->name) == 0)
                    return assoc.ainst;
            }
            return NULL;
        }
        struct link_index alist;
        list_init(&alist);
        for (uint64_t idx = 0; idx < lconf->n_appenders; idx++) {
            struct laft_appender *a = fetch_appender(lconf->appenders[idx]);
            if (likely(a != NULL))
                list_add_tail(intrusion_from_ptr(a), &alist);
        }
        struct laft_logger *linst = laft_logger_create(lconf->level, &alist);
        laft_logger_map_put(loggers, lconf->name, linst);
    }
    return 1;
}

int logger_factory_init(const struct laft_config *conf) {
    if (unlikely(factory_initflag))
        return 1;
    loggers = laft_logger_map_create(DEFAULT_LOGGERMAP_CAP);
    if (unlikely(loggers == NULL))
        return 0;
    logger_conf_init(conf);
    return (factory_initflag = 1);
}

struct laft_logger *laft_get_logger(const char *name) {
    if (unlikely(loggers == NULL))
        return NULL;
    return laft_logger_map_get(loggers, name);
}
