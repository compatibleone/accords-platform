#
# OSPROCCI Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>
#

bin_PROGRAMS = osprocci
EXTRA_DIST = proccios.c oscontract.c quantum.c oskeypair.c

osprocci_SOURCES = osprocci.c

osprocci_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_COSQL) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COCARRIER) $(CFLAGS_PUBOCCI) $(CFLAGS_PROCCI) $(CFLAGS_COSACS) $(CFLAGS_COPABR)
osprocci_LDFLAGS = $(LD_COCARRIER) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LD_COSQL) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS) $(LDFLAGS_POSTGRE)

# Create auto-generated model files
model_h_files = openstack.h
include $(top_srcdir)/tools/Makefile_generate_model.am
osprocci.c: $(model_c_files)

