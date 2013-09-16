#
# OSOCCI Makefile.am
#
# Copyright 2012, Prologue, All rights reserved
#
# Authors:
#   Iain James Marshall <ijm667@hotmail.com>
#
bin_PROGRAMS = example
EXTRA_DIST = example_contract.c example_contract.h

example_SOURCES = example.c 

example_CFLAGS = $(CFLAGS_BACKEND) $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) \
                 $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COCARRIER) $(CFLAGS_PUBOCCI) \
                 $(CFLAGS_PROCCI) $(CFLAGS_COSACS) $(CFLAGS_COPABR) -Werror
example_LDFLAGS = $(LD_BACKEND) $(LD_COCARRIER) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) \
                  $(LD_COREST) $(LD_COXML) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)

