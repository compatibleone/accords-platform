#
# COCARRIER Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>
#
lib_LTLIBRARIES = libcocarrier.la
EXTRA_DIST = dataelement.c dataelement.h \
	eucahack.c osconfig.c

libcocarrier_la_SOURCES = azclient.c azclient.h \
    osclient.c osclient.h \
    onclient.c onclient.h \
    dcclient.c dcclient.h \
    json.c json.h 

libcocarrier_la_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_COXML) $(CFLAGS_COCCI) $(CFLAGS_CORDS) 
libcocarrier_la_LDFLAGS = $(LD_COXML) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)
