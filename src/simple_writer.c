#include    <stdio.h>
#include    <sys/uio.h>
#include    <alloca.h>
#include    "simple_writer.h"

#define     IOV_IMM_THRES           16

#include    <stdio.h>

static inline
int do_log_write(struct simple_writer *w, struct mbuf *mb) {
    int ret = 1, iov_dyn = 0;
    uint32_t iov_idx = 0;
    struct iovec *iov = 
        (mb->nblks <= IOV_IMM_THRES) ?
        alloca(sizeof(struct iovec) * mb->nblks) :
        ((iov_dyn = 0), malloc(sizeof(struct iovec) * mb->nblks));
    if (unlikely(iov == NULL))
        return 0;
    list_foreach(&(mb->lead)) {
        __auto_type blk = intrusive_ref(struct mbuf);
        iov[iov_idx].iov_len = blk->size - 1;
        iov[iov_idx].iov_base = blk->blk;
        iov_idx++;
    }
    ret = writev(w->current_fd, iov, mb->nblks);
    if (unlikely(iov_dyn))
        free(iov);
    return ret > -1;
}

static
int sw_log_write(struct laft_writer *w, struct mbuf *mb) {
    __auto_type wimpl = log_writer_impl(w, struct simple_writer);
    time_t t = time(NULL);
    struct tm d;
    localtime_r(&t, &d);
    int wday_curr = wimpl->actualpath[wimpl->pathlen - 2] - '0';
    if (wday_curr != d.tm_wday) {
        close(wimpl->current_fd);
        sprintf(wimpl->actualpath, "%s.%d", wimpl->fullpath, d.tm_wday);
        wimpl->current_fd = open(wimpl->actualpath, O_RDWR|O_CREAT|O_APPEND|O_CLOEXEC, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
    }
    if (unlikely(wimpl->current_fd == -1))
        return 0;
    int ret = do_log_write(wimpl, mb);
    mbuf_destroy(mb);
    return ret;
}

static
void sw_dtor_hook(struct laft_writer *w) {
    __auto_type wimpl = log_writer_impl(w, struct simple_writer);
    if (wimpl->current_fd > -1)
        close(wimpl->current_fd);
    free(wimpl->fullpath);
    free(wimpl);
}

struct laft_writer *simple_writer_create(const char *logpath) {
    size_t len = strlen(logpath) + 2 + 1;
    struct simple_writer *w = malloc(sizeof(struct simple_writer) + sizeof(char) * len);
    if (unlikely(w == NULL))
        return NULL;
    w->fullpath = malloc(sizeof(char) * (len - 2));
    if (unlikely(w->fullpath == NULL))
        return free(w), NULL;
    time_t t = time(NULL);
    struct tm d;
    localtime_r(&t, &d);
    return 
        sprintf(w->actualpath, "%s.%d", logpath, d.tm_wday),
        (w->pathlen = len),
        (w->current_fd = open(w->actualpath, O_RDWR|O_CREAT|O_APPEND|O_CLOEXEC, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)),
        (w->writer_ctl_.log_write = sw_log_write),
        (w->writer_ctl_.dtor_hook = sw_dtor_hook),
        strcpy(w->fullpath, logpath),
        log_writer_from_ptr(w);
}
