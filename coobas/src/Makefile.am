#
## COOBAS Makefile.am
#
## Copyright 2011, Mandriva, All rights reserved
#
## Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>

bin_PROGRAMS = coobas

coobas_SOURCES = coobas.c

coobas_CFLAGS = $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI)
coobas_LDADD = $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) -luuid -lpthread -lssl

