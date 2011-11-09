#
# COCARRIER Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>
#
lib_LTLIBRARIES = libcopabr.la

libcopabr_la_SOURCES = cp.o cp.h cb.c cb.h
libcopabr_la_CFLAGS = $(CFLAGS_COREST) $(CFLAGS_COXML) $(CFLAGS_COCCI) $(CFLAGS_OCCI) $(CFLAGS_PUBOCCI)
libcopabr_la_LIBADD = $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) -luuid -lpthread -lssl
