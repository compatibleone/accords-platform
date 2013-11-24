#
# EZIPROCCI Makefile.am
#
# Copyright 2012, Prologue, All rights reserved
#
# Authors:
#   Iain James Marshall <ijm667@hotmail.com>
#

bin_PROGRAMS = coeasy
EXTRA_DIST = 

coeasy_SOURCES = coeasy.c

coeasy_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_COSQL) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COCARRIER) $(CFLAGS_PUBOCCI) $(CFLAGS_PROCCI) $(CFLAGS_COSACS) $(CFLAGS_COPABR) $(CFLAGS_COMONS)
coeasy_LDFLAGS = $(LD_COCARRIER) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LD_COSQL) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)

# Create auto-generated model files
model_h_files = easiclouds_application.h easiclouds_connection.h easiclouds_description.h easiclouds_extra.h easiclouds_instance.h easiclouds_keypair.h easiclouds_link.h easiclouds_metadata.h easiclouds_model.h easiclouds_node.h easiclouds_parameters.h easiclouds_postconf.h easiclouds_server.h easiclouds_input.h easiclouds_output.h
include $(top_srcdir)/tools/Makefile_generate_model.am
coeasy.c: $(model_c_files)

