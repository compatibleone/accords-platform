#
# COCARRIER Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>
#
lib_LTLIBRARIES = libcocarrier.la

libcocarrier_la_SOURCES = azclient.c azclient.h \
    osclient.c osclient.h \
    onclient.c onclient.h \
    json.c json.h

libcocarrier_la_CFLAGS = $(CFLAGS_COREST) $(CFLAGS_COXML) $(CFLAGS_COCCI)
libcocarrier_la_LIBADD = $(LD_COREST) $(LD_COXML) -luuid -lpthread -lssl
