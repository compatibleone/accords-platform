#
# COCCI Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>
#   Iain James Marshall <ijm667@hotmail.com>
#
lib_LTLIBRARIES = libcocci.la
EXTRA_DIST = occic.c occic.h \
	stdnode.c stdnode.h \
	occim.c occim.h \
	occirescat.h occiresolver.h \
	occirespro.h \
	urlpublic.h \
	occiauth.h occiauth.c authcache.h

libcocci_la_SOURCES = occiclient.c occiserver.c occiclient.h \
    accords.c accords.h occibody.c occibody.h

libcocci_la_CFLAGS = $(AM_CFLAGS) $(CFLAGS_OCCI) $(CFLAGS_COREST) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_PUBOCCI) $(CFLAGS_COCARRIER)
libcocci_la_LDFLAGS = $(LD_COCARRIER) $(LD_OCCI) $(LD_COXML) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)

# Create auto-generated model files
model_h_files = authitem.h
include $(top_srcdir)/tools/Makefile_generate_model.am
occiclient.c: $(model_c_files)

