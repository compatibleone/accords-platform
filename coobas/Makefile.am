#
## COOBAS Makefile.am
#
## Copyright 2011, Mandriva, All rights reserved
#
## Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>

bin_PROGRAMS = coobas

coobas_SOURCES = coobas.c

coobas_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML)\
	 $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_PUBOCCI) $(CFLAGS_COPABR) $(CFLAGS_COMONS) $(CFLAGS_COCSPI) \
	 $(CFLAGS_BACKEND)
coobas_LDFLAGS = $(LD_COCSPI) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST)\
	 $(LD_COXML) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS) $(LD_BACKEND)

