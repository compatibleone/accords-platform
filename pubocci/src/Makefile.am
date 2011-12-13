#
# PUBOCCI Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>
#
lib_LTLIBRARIES = libpubocci.la
EXTRA_DIST = autopub.c cordslang.h occirescat.c occirespro.h \
	autopub.h occipub.c occirescat.h occirespro.c

libpubocci_la_SOURCES = occilogin.c occilogin.h \
    occipublisher.c occipublisher.h \
    occiresolver.c

libpubocci_la_CFLAGS = $(CFLAGS_COCCI) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_COREST) $(CFLAGS_COPABR)
libpubocci_la_LDFLAGS = $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COXML) $(LD_COREST) $(LD_COPABR) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)
