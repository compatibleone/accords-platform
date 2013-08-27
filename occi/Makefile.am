#
# OCCI Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean Parpaillon <jparpaillon@mandriva.com>
#
lib_LTLIBRARIES = libocci.la
EXTRA_DIST = occiatb.c occiatb.h linknode.c linknode.h kindnode.c kindnode.h \
	     occiact.c occiact.h occibuilder.h occicat.c occicat.h occi.h    \
	     occihttp.c occijson.c occilink.c occilink.h occimixin.c         \
	     occimixin.h occipar.c occipar.h occipub.h occixml.c

libocci_la_SOURCES = occi.c cordsocci.c pubocci.c osonocci.c 
libocci_la_CFLAGS = $(CFLAGS_BACKEND) $(AM_CFLAGS) $(CFLAGS_COXML) $(CFLAGS_COREST) $(CFLAGS_CORDS)
libocci_la_LDFLAGS = $(LD_BACKEND)
