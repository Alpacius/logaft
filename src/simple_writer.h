#pragma once

#include    "writer.h"

struct simple_writer {
    log_writer;
    char *fullpath;     // TODO opt
    int current_fd;
    size_t pathlen;
    char actualpath[];
};

struct laft_writer *simple_writer_create(const char *logpath);
