#
# BROKER Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>
#

bin_PROGRAMS = broker
EXTRA_DIST = brokerplan.c brokerservice.c brokerprovider.c

broker_SOURCES = broker.c broker.h

broker_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) \
                $(CFLAGS_BACKEND) $(CFLAGS_COCCI) $(CFLAGS_PUBOCCI) $(CFLAGS_COPABR)
broker_LDFLAGS = $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) \
                 $(LD_BACKEND) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)
