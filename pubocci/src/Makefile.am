#
# PUBOCCI Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>
#
lib_LTLIBRARIES = libpubocci.la

libpubocci_la_SOURCES = occilogin.c occilogin.h \
    occipublisher.c occipublisher.h \
    occiresolver.c occiresolver.h

libpubocci_la_CFLAGS = $(CFLAGS_COCCI) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_COREST)
libpubocci_la_LIBADD = $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COXML) $(LD_COREST) -luuid -lpthread -lssl
