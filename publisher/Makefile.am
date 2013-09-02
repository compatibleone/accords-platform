#
# PUBLISHER Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>
#

bin_PROGRAMS = publisher

publisher_SOURCES = publisher.c publisher.h

publisher_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) \
                   $(CFLAGS_BACKEND) $(CFLAGS_COCCI) $(CFLAGS_PUBOCCI)
publisher_LDFLAGS = $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) \
                    $(LD_BACKEND) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)


