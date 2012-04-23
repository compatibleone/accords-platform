#
# COREST Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>
#
lib_LTLIBRARIES = libcorest.la
EXTRA_DIST = global.h rest.h restpublic.h rests.c \
	restworker.c tlsconfig.h url.h \
	restc.c restheader.c restrequest.c restworker.h ssltls.c \
	connections.h restc.h restheader.h restrequest.h rests.h restworkers.c \
	ssltls.h restresponse.c restthread.c restx.c standard.h \
	restresponse.h restthread.h restx.h tlsconfig.c url.c

libcorest_la_SOURCES = base64.c base64.h \
    encode64.c encode64.h \
    socket.c \
    tlsrest.c \
    tlsload.c tlsload.h \
    restlog.c restlog.h \
    restserver.c \
    restclient.c restclient.h

libcorest_la_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COXML)
libcorest_la_LDFLAGS = $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)
