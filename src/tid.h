#pragma once

#define     _GNU_SOURCE
#include    <unistd.h>
#include    <sys/syscall.h>

#include    "miscutils.h"

long do_gettid(void);
