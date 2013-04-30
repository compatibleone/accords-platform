#
# COPAAS Makefile.am
#
# Copyright 2012, Prologue, All rights reserved
#
# Authors:
#   Iain James Marshall <ijm667@hotmail.com>
#

bin_PROGRAMS = copaas
EXTRA_DIST = copaas.h

copaas_SOURCES = copaas.c 

copaas_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COCARRIER) $(CFLAGS_PUBOCCI) $(CFLAGS_PROCCI) $(CFLAGS_COSACS) $(CFLAGS_COPABR)
copaas_LDFLAGS = $(LD_COCARRIER) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)

# Create auto-generated model files
model_h_files = paas_application.h paas_application_deployable.h paas_application_manifest.h paas_application_version.h paas_application_version_instance.h paas_environment.h paas_environment_configuration.h paas_environment_link.h paas_environment_node.h paas_environment_relation.h paas_environment_template.h paas_environment_variable.h
include $(top_srcdir)/tools/Makefile_generate_model.am
copaas.c: $(model_c_files)

