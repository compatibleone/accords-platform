#
# COCCI Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>
#
lib_LTLIBRARIES = libcocci.la

libcocci_la_SOURCES = occiclient.c occiclient.h \
    occiserver.c occiserver.h

libcocci_la_CFLAGS = $(CFLAGS_OCCI) $(CFLAGS_COREST) $(CFLAGS_COXML) $(CFLAGS_CORDS)
libcocci_la_LIBADD = $(LD_COREST) $(LD_OCCI) $(LD_CORDS) -lpthread -luuid -lssl $(LD_COXML)
