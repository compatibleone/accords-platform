#
# COMMAND Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean Parpaillon <jparpaillon@mandriva.com>
#

bin_PROGRAMS = command

EXTRA_DIST = 

command_SOURCES = command.c command.h

command_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COXML) $(CFLAGS_OCCI) $(CFLAGS_COREST) $(CFLAGS_COCCI) \
	$(CFLAGS_PUBOCCI) $(CFLAGS_CORDS) $(CFLAGS_COCARRIER) $(CFLAGS_COPABR) $(CFLAGS_COCSPI)
command_LDFLAGS = $(LD_COCSPI) $(LD_COPABR) $(LD_COCARRIER) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) \
	$(LD_CORDS) $(LD_COREST) $(LD_COXML)  $(LD_COREST) $(LDFLAGS_UUID) $(LDFLAGS_THREADS) \
	$(LDFLASG_SSL)
