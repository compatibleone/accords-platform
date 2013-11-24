#
# COES Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>
#

bin_PROGRAMS = cops

EXTRA_DIST   = copsoperation.c copsplacement.c copsquantity.c

cops_SOURCES = cops.c

cops_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_PUBOCCI) $(CFLAGS_COMONS) $(CFLAGS_COPABR) $(CFLAGS_COCSPI)
cops_LDFLAGS = $(LD_COCSPI) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LD_COSQL) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)

# Create auto-generated model files
model_h_files = quantity.h federation.h
include $(top_srcdir)/tools/Makefile_generate_model.am
cops.c: $(model_c_files)

