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
libcords_la_LDFLAGS = $(LD_COXML) $(LD_BACKEND) -lcurl 

#json-c
JSON_C_DIR=/usr/local
libcords_la_CFLAGS += -I$(JSON_C_DIR)/include/json-c
libcords_la_LDFLAGS+= -L$(JSON_C_DIR)/lib -ljson-c

