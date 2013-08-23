#
# OSOCCI Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
# Copyright 2012, Prologue, All rights reserved
#
# Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>
#   Iain James Marshall <ijm667@hotmail.com>
#

bin_PROGRAMS = slam
EXTRA_DIST = slamcontrol.c

slam_SOURCES = slam.c

slam_CFLAGS = $(CFLAGS_BACKEND) $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) \
              $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COCARRIER) $(CFLAGS_PUBOCCI) $(CFLAGS_PROCCI) $(CFLAGS_COSACS) $(CFLAGS_COPABR) $(CFLAGS_COMONS) $(CFLAGS_COCSPI)
slam_LDFLAGS = $(LD_BACKEND) $(LD_COCARRIER) $(LD_COCSPI) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) \
               $(LD_COREST) $(LD_COXML) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)

