CC=gcc -std=gnu11
LIBS=-lpthread
lib_LTLIBRARIES=liblogaft.la
liblogaft_la_SOURCES=appender.c logger.c \
					 mbuf.c \
					 simple_encoder.c simple_rrouter.c simple_writer.c \
					 singletons.c taskexecutor.c \
					 tid.c
logaftincludedir=$(includedir)/logaft
logaftinclude_HEADERS=logaft.h logger.h stdc_common.h linux_common.h list.h \
					  miscutils.h appender.h encoder.h mbuf.h loglevel.h writer.h recrouter.h \
					  simple_encoder.h simple_writer.h sds.h
