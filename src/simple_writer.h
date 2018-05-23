#pragma once

#include    "writer.h"

struct simple_writer {
    log_writer;
    const char *fullpath;
    int current_fd;
    size_t pathlen;
    char actualpath[];
};
