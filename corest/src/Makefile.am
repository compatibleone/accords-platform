#
# COREST Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>
#
lib_LTLIBRARIES = libcorest.la

libcorest_la_SOURCES = base64.c base64.h \
    encode64.c encode64.h \
    socket.c socket.h \
    tlsrest.c tlsrest.h \
    tlsload.c tlsload.h \
    restlog.c restlog.h \
    restserver.c restserver.h \
    restclient.c restclient.h

libcorest_la_CFLAGS = $(CFLAGS_COXML)
libcorest_la_LDFLAGS = $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)
