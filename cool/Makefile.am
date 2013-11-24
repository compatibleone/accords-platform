#
# COSACS Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jamie Marshall <ijm667@hotmail.com>
#

bin_PROGRAMS = cool
EXTRA_DIST = cool.h

cool_SOURCES = cool.c
cool_LDFLAGS = $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LD_COSQL) $(LD_PUBOCCI) $(LD_COPABR)
cool_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_COSQL) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COPABR) $(CFLAGS_PUBOCCI) $(CFLAGS_COMONS)

# Create auto-generated model files
model_h_files = job.h workload.h
include $(top_srcdir)/tools/Makefile_generate_model.am
cool.c: $(model_c_files)

