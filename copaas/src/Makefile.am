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
occipaas_application_deployable.c \
occipaas_application_environment.c \
occipaas_link.c \
occipaas_application_manifest.c \
occipaas_node.c \
occipaas_application_version.c \
occipaas_application_version_instance.c \
paas_application.c \
paas_application.h \
paas_application_deployable.c \
paas_application_deployable.h \
paas_application_environment.c \
paas_application_environment.h \
paas_link.c \
paas_link.h \
paas_application_manifest.c \
paas_application_manifest.h \
paas_node.c \
paas_node.h \
paas_application_version.c \
paas_application_version.h \
paas_application_version_instance.c \
paas_application_version_instance.h \
copaas.h \
occipaas_configuration_option.c \
occipaas_application_configuration_template.c \
occipaas_relation.c \
paas_configuration_option.c \
paas_configuration_option.h \
paas_application_configuration_template.c \
paas_application_configuration_template.h \
paas_relation.c \
paas_relation.h

copaas_SOURCES = copaas.c 

copaas_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COCARRIER) $(CFLAGS_PUBOCCI) $(CFLAGS_PROCCI) $(CFLAGS_COSACS) $(CFLAGS_COPABR) 
copaas_LDFLAGS = $(LD_COCARRIER) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)

