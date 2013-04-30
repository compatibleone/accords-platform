#
# COSACS Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jamie Marshall <ijm667@hotmail.com>
#

bin_PROGRAMS = cosacs
EXTRA_DIST = 

cosacs_SOURCES = cosacs.c
cosacs_LDFLAGS = $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LD_PUBOCCI) 
cosacs_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COPABR) $(CFLAGS_PUBOCCI)

