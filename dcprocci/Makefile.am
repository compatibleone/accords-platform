#
# OSPROCCI Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
# Copyright 2012, Prologue, All rights reserved
#
# Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>
#   Iain James Marshall <ijm667@hotmail.com>
#

bin_PROGRAMS = dcprocci
EXTRA_DIST = proccidc.c dcmethod.c dcaction.c dccontract.c

dcprocci_SOURCES = dcprocci.c 

dcprocci_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COCARRIER) $(CFLAGS_PUBOCCI) $(CFLAGS_PROCCI) $(CFLAGS_COSACS) $(CFLAGS_COPABR)
dcprocci_LDFLAGS = $(LD_COCARRIER) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LD_COSQL) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)

# Create auto-generated model files
model_h_files = deltacloud.h dcconfig.h
include $(top_srcdir)/tools/Makefile_generate_model.am
dcprocci.c: $(model_c_files)

