#
# TESTOS Makefile.am
#
# Copyright 2012, Compatible One
#
# Authors:
#   Iain James Marshall <ijm667@hotmail.com>
#

bin_PROGRAMS = testosocci

testosocci_SOURCES = testosocci.c

testosocci_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI) \
                    $(CFLAGS_COCARRIER) $(CFLAGS_PUBOCCI) $(CFLAGS_PROCCI)
testosocci_LDFLAGS = $(LD_COCARRIER) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) \
                     $(LD_BACKEND) $(LD_COXML) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)

