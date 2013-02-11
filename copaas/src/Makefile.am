#
# COPAAS Makefile.am
#
# Copyright 2012, Prologue, All rights reserved
#
# Authors:
#   Iain James Marshall <ijm667@hotmail.com>
#

bin_PROGRAMS = copaas
EXTRA_DIST = copaas.h \
paas_application_deployable.h \
paas_application.h \
paas_application_manifest.h \
paas_application_version.h \
paas_application_version_instance.h \
paas_environment_configuration.h \
paas_environment.h \
paas_environment_link.h \
paas_environment_node.h \
paas_environment_relation.h \
paas_environment_template.h \
paas_environment_variable.h \
paas_application_deployable.c \
paas_application.c \
paas_application_manifest.c \
paas_application_version.c \
paas_application_version_instance.c \
paas_environment_configuration.c \
paas_environment.c \
paas_environment_link.c \
paas_environment_node.c \
paas_environment_relation.c \
paas_environment_template.c \
paas_environment_variable.c \
occipaas_application_deployable.c \
occipaas_application.c \
occipaas_application_manifest.c \
occipaas_application_version.c \
occipaas_application_version_instance.c \
occipaas_environment_configuration.c \
occipaas_environment.c \
occipaas_environment_link.c \
occipaas_environment_node.c \
occipaas_environment_relation.c \
occipaas_environment_template.c \
occipaas_environment_variable.c

copaas_SOURCES = copaas.c 

copaas_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COCARRIER) $(CFLAGS_PUBOCCI) $(CFLAGS_PROCCI) $(CFLAGS_COSACS) $(CFLAGS_COPABR) 
copaas_LDFLAGS = $(LD_COCARRIER) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)



