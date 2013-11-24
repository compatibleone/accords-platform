#
# COCARRIER Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>
#
lib_LTLIBRARIES = libcopabr.la
EXTRA_DIST = csa.c csa.h \
	cse.c cse.h \
	csp.c csp.h \
	cosacsctrl.c cosacsctrl.h \
	cpxsd.c cpxsd.h

libcopabr_la_SOURCES = cp.c cp.h cb.c cb.h
libcopabr_la_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_COXML) $(CFLAGS_COCCI) $(CFLAGS_OCCI) $(CFLAGS_PUBOCCI)
libcopabr_la_LDFLAGS = $(LD_COCCI) $(LD_CORDS) $(LD_COXML) $(LD_COSQL) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)

