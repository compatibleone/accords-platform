#
# COPAAS Makefile.am
#
# Copyright 2012, Prologue, All rights reserved
#
# Authors:
#   Iain James Marshall <ijm667@hotmail.com>
#

bin_PROGRAMS = copaas
EXTRA_DIST = occipaas_application.c \
occipaas_config_option.c \
occipaas_config_template.c \
occipaas_deployable.c \
occipaas_environment.c \
occipaas_link.c \
occipaas_manifest.c \
occipaas_node.c \
occipaas_version.c \
occipaas_version_instance.c \
paas_application.c \
paas_application.h \
paas_config_option.c \
paas_config_option.h \
paas_config_template.c \
paas_config_template.h \
paas_deployable.c \
paas_deployable.h \
paas_environment.c \
paas_environment.h \
paas_link.c \
paas_link.h \
paas_manifest.c \
paas_manifest.h \
paas_node.c \
paas_node.h \
paas_version.c \
paas_version.h \
paas_version_instance.c \
paas_version_instance.h

copaas_SOURCES = copaas.c 

copaas_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COCARRIER) $(CFLAGS_PUBOCCI) $(CFLAGS_PROCCI) $(CFLAGS_COSACS) $(CFLAGS_COPABR) 
copaas_LDFLAGS = $(LD_COCARRIER) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)

