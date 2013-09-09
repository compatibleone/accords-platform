#
# PROCCI Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>
#

bin_PROGRAMS = procci
EXTRA_DIST = proccicontract.c

procci_SOURCES = procci.c procci.h

procci_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI) \
                $(CFLAGS_BACKEND) $(CFLAGS_COPABR) $(CFLAGS_PUBOCCI) -Werror
procci_LDFLAGS = $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LDFLAGS_UUID) \
                 $(LD_BACKEND) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)
