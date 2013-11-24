#
# COSCHED Makefile.am
#
# Copyright 2012, Prologue, All rights reserved
#
# Authors:
#   Iain James Marshall <ijm667@hotmail.com>
#

bin_PROGRAMS = cosched

cosched_SOURCES = cosched.c
cosched_LDFLAGS = $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LD_COSQL) $(LD_PUBOCCI) $(LD_COPABR) $(LD_COCSPI)
cosched_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_COSQL) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_PUBOCCI) $(CFLAGS_COPABR) $(CFLAGS_COCSPI)

# Create all auto-generated model files
model_h_files = counter.h schedule.h timer.h
include $(top_srcdir)/tools/Makefile_generate_model.am
cosched.c: $(model_c_files)

