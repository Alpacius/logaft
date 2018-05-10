#pragma once

#include    "stdc_common.h"
#include    "linux_common.h"
#include    "list.h"

struct mbuf;

struct mbuf *mbuf_create_cstr(const char *cstr);
void mbuf_destroy(struct mbuf *mb);
int mbuf_append_cstr(struct mbuf *master, const char *cstr);
int mbuf_append_mbuf(struct mbuf *master, struct mbuf *slave);

#define mbuf_append(m_, a_) _Generic((a_), const char *: mbuf_append_cstr((m_), (a_)), struct mbuf *: mbuf_append_mbuf((m_), (a_)))
