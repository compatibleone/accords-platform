#
# CORDS Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean Parpaillon <jparpaillon@mandriva.com>
#
lib_LTLIBRARIES = libcords.la
EXTRA_DIST = operation.c cordselements.h cords.h cordspublic.h

libcords_la_SOURCES = cords2occi.c cordsbase.c
libcords_la_CFLAGS = $(CFLAGS_BACKEND) $(CFLAGS_CORDS) $(AM_CFLAGS) $(CFLAGS_COXML) $(CFLAGS_COREST) $(CFLAGS_OCCI) -Werror
libcords_la_LIBADD = $(LD_COXML) $(LD_BACKEND) -lcurl

#json-c
libcords_la_CFLAGS += ${JSONC_CFLAGS}
libcords_la_LIBADD += ${JSONC_LIBS}

