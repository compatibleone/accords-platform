#
# COCARRIER Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>
#
lib_LTLIBRARIES = libcocarrier.la
EXTRA_DIST = dataelement.c dataelement.h eucahack.c 

libcocarrier_la_SOURCES = azclient.c azclient.h \
    list.c list.h \
    hmac_sha2.c hmac_sha2.h \
    sha2.c sha2.h \
    osclient.c osclient.h \
    onclient.c onclient.h \
    dcclient.c dcclient.h \
    co_json.c co_json.h 

libcocarrier_la_CFLAGS = $(CFLAGS_BACKEND) $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_COXML) $(CFLAGS_COCCI) $(CFLAGS_CORDS) 
libcocarrier_la_LDFLAGS = $(LD_BACKEND) $(LD_COXML) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)
