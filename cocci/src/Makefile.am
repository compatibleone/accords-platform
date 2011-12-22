#
# COCCI Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>
#
lib_LTLIBRARIES = libcocci.la
EXTRA_DIST = occic.c occic.h \
	occim.c occim.h \
	occipublisher.h \
	occirescat.h occiresolver.h \
	occirespro.h \
	urlpublic.h

libcocci_la_SOURCES = occiclient.c occiclient.h \
    occiserver.c accords.c accords.h

libcocci_la_CFLAGS = $(AM_CFLAGS) $(CFLAGS_OCCI) $(CFLAGS_COREST) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_PUBOCCI)
libcocci_la_LDFLAGS = $(LD_COREST) $(LD_OCCI) $(LD_CORDS) $(LD_COXML) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)
