#
# TESTOS Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
# Copyright 2012, Prologue, All rights reserved
#
# Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>
#   Iain James Marshall <ijm667@hotmail.com>
#

bin_PROGRAMS = testdc

testdc_SOURCES = testdc.c

testdc_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COCARRIER) $(CFLAGS_PUBOCCI) $(CFLAGS_PROCCI)
testdc_LDFLAGS = $(LD_COCARRIER) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LD_COSQL) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS) $(LDFLAGS_POSTGRE)

