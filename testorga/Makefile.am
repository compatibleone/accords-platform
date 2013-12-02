#
# TESTORGA Makefile.am
#
# Copyright 2013, Prologue, All rights reserved
#
# Authors:
#   Iain James Marshall <ijm667@hotmail.com>
#

bin_PROGRAMS = testorga

testorga_SOURCES = testorga.c orgaclient.c 
extra_DISTRIB = orgaconfig.c

testorga_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COCARRIER) $(CFLAGS_PUBOCCI) $(CFLAGS_PROCCI) $(CFLAGS_COSACS) $(CFLAGS_COPABR) 
testorga_LDFLAGS = $(LD_COCARRIER) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LD_COSQL) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS) $(LDFLAGS_POSTGRE)




