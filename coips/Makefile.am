#
# COIPS Makefile.am
#
# Copyright 2012, Prologue, All rights reserved
#
# Authors:
#   Iain James Marshall <ijm667@hotmail.com>
#

bin_PROGRAMS = coips
EXTRA_DIST = coips.h

coips_SOURCES = coips.c
coips_LDFLAGS = $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LD_COSQL) $(LD_PUBOCCI) $(LD_COPABR)
coips_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COSQL) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_PUBOCCI) $(CFLAGS_COPABR)

# Create auto-generated model files
model_h_files = saas.h
include $(top_srcdir)/tools/Makefile_generate_model.am
coips.c: $(model_c_files)

