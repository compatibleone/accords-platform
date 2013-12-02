#
# COMODEL Makefile.am
#
# Copyright 2013, Prologue, All rights reserved
#
# Authors:
#   Iain James Marshall <ijm667@hotmail.com>
#

bin_PROGRAMS = comodel

EXTRA_DIST = 

comodel_SOURCES = comodel.c comodel.h strukt.c strukt.h genoccixml.c genoccisql.c gencomp.c

comodel_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COXML) $(CFLAGS_OCCI) $(CFLAGS_COREST) $(CFLAGS_COCCI) \
	$(CFLAGS_PUBOCCI) $(CFLAGS_CORDS) $(CFLAGS_COCARRIER) $(CFLAGS_COPABR) $(CFLAGS_COCSPI)
comodel_LDFLAGS = $(LD_COCSPI) $(LD_COPABR) $(LD_COCARRIER) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML)  $(LD_COSQL) $(LD_COREST) $(LDFLAGS_UUID) $(LDFLAGS_THREADS) $(LDFLASG_SSL) $(LDFLAGS_POSTGRE)



