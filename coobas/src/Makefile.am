#
## COOBAS Makefile.am
#
## Copyright 2011, Mandriva, All rights reserved
#
## Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>

bin_PROGRAMS = coobas

coobas_SOURCES = coobas.c

coobas_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_PUBOCCI) $(CFLAGS_COPABR)
coobas_LDFLAGS = $(AM_LDFLAGS) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)

