#
# COSACS Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jamie Marshall <ijm667@hotmail.com>
#

bin_PROGRAMS = cool
EXTRA_DIST = cool.h

cool_SOURCES = cool.c
cool_LDFLAGS = $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS) $(LD_COPABR) $(LD_COCCI) $(LD_OCCI) $(LD_COREST) $(LD_COXML) $(LD_PUBOCCI)
cool_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COPABR) $(CFLAGS_PUBOCCI) $(CFLAGS_COMONS)

# Create auto-generated model files

model_h_files = job.h workload.h

include $(top_srcdir)/tools/Makefile_generate_model.am

# xlink has to be built separately, because it needs the "--cords link" option whereas the ones above do not.
xlink.h : %.h : $(top_srcdir)/model/*.xml $(MODEL_CODEGEN)
	$(MODEL_CODEGEN) -t Header -o . $(top_srcdir)/model -f $@

occixlink.c xlink.c: xlink.h
	$(MODEL_STRUKT) --license $(srcdir)/../strukt/license.h --nosql --cords link $(subst .c,,$@)
	mv $@ occi$@
	$(MODEL_STRUKT) --license $(srcdir)/../strukt/license.h --nosql --xml --occi $(subst .c,,$@)

all_xlink_files = xlink.c xlink.h xlink.xsd occixlink.c
CLEANFILES += $(all_xlink_files)

cool.c: $(model_c_files) xlink.c

