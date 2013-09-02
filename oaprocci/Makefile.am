#
# OAPROCCI Makefile.am
#
# Copyright 2013, Prologue, All rights reserved
#
# Authors:
#   Iain James Marshall <ijm667@hotmail.com>
#

bin_PROGRAMS = oaprocci
EXTRA_DIST = 

# Create auto-generated model files
model_h_files = onapp.h oaconfig.h onapp_extras.h
include $(top_srcdir)/tools/Makefile_generate_model.am

oaprocci_SOURCES = oaprocci.c onapp_helpers.c

oaprocci_CFLAGS = $(CFLAGS_BACKEND) $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) \
                  $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COCARRIER) $(CFLAGS_PUBOCCI) $(CFLAGS_PROCCI) $(CFLAGS_COSACS) $(CFLAGS_COPABR) $(CFLAGS_COMONS)
oaprocci_LDFLAGS = $(LD_BACKEND) $(LD_COCARRIER) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) \
                   $(LD_COREST) $(LD_COXML) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)

oaprocci.c: $(model_c_files)