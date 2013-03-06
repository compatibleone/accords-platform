#
# EZIPROCCI Makefile.am
#
# Copyright 2012, Prologue, All rights reserved
#
# Authors:
#   Iain James Marshall <ijm667@hotmail.com>
#

bin_PROGRAMS = coeasy
EXTRA_DIST = easiclouds_application.c \
easiclouds_connection.c \
easiclouds_description.c \
easiclouds_extra.c \
easiclouds_instance.c \
easiclouds_keypair.c \
easiclouds_link.c \
easiclouds_model.c \
easiclouds_node.c \
easiclouds_parameters.c \
easiclouds_postconf.c \
easiclouds_server.c \
occieasiclouds_application.c \
occieasiclouds_connection.c \
occieasiclouds_description.c \
occieasiclouds_extra.c \
occieasiclouds_instance.c \
occieasiclouds_keypair.c \
occieasiclouds_link.c \
occieasiclouds_model.c \
occieasiclouds_node.c \
occieasiclouds_parameters.c \
occieasiclouds_postconf.c \
occieasiclouds_server.c

coeasy_SOURCES = coeasy.c

coeasy_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COCARRIER) $(CFLAGS_PUBOCCI) $(CFLAGS_PROCCI) $(CFLAGS_COSACS) $(CFLAGS_COPABR) $(CFLAGS_COMONS)
coeasy_LDFLAGS = $(LD_COCARRIER) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)


