#
# CONAGIOS Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jamie Marshall <ijm667@hotmail.com>
#

bin_PROGRAMS = conagios

conagios_SOURCES = conagios.c
conagios_LDFLAGS = $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LD_COSQL) $(LD_PUBOCCI) $(LD_COPABR) $(LDFLAGS_POSTGRE)
conagios_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_COSQL) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COPABR) $(CFLAGS_PUBOCCI)

# Create auto-generated model files
model_h_files = nagioscommand.h nagioscontact.h nagioscontactgroup.h nagioshost.h nagioshostdependancy.h nagioshostescalation.h nagioshostextinfo.h nagioshostgroup.h nagiosservice.h nagiosserviceescalation.h nagiosservicegroup.h nagiostimeperiod.h nagiosservicedependancy.h nagiosserviceextinfo.h
include $(top_srcdir)/tools/Makefile_generate_model.am
conagios.c: $(model_c_files)

