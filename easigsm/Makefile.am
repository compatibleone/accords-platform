#
# EASIGSM Makefile.am
#
# Copyright 2013, All rights reserved
#
# Authors:
#   Iain James Marshall <ijm667@hotmail.com>
#

bin_PROGRAMS = easigsm
EXTRA_DIST = 

easigsm_SOURCES = easigsm.c

easigsm_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COCARRIER) $(CFLAGS_PUBOCCI) $(CFLAGS_PROCCI) $(CFLAGS_COSACS) $(CFLAGS_COPABR) $(CFLAGS_COMONS) $(CFLAGS_COCSPI)
easigsm_LDFLAGS = $(LD_COCARRIER) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCSPI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LD_COSQL) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)

# Create auto-generated model files
model_h_files = ecslam_serviceTemplate.h gsm_agreement.h gsm_instance.h ecslam_negotiation.h ecslam_agreement.h
include $(top_srcdir)/tools/Makefile_generate_model.am
easigsm.c: $(model_c_files)


