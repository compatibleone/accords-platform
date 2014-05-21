#
# COCSPI Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>
#
lib_LTLIBRARIES = libcocspi.la

libcocspi_la_SOURCES = cspi.c cspi.h 
libcocspi_la_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_COXML) $(CFLAGS_COSQL) $(CFLAGS_COCCI) $(CFLAGS_OCCI) $(CFLAGS_PUBOCCI) $(CFLAGS_COPABR)
libcocspi_la_LDFLAGS = $(LD_COCCI) $(LD_COXML) $(LD_COSQL) $(LDFLAGS_UUID) $(LDFLAGS_THREADS)



