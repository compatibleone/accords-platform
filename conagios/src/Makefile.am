#
# CONAGIOS Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jamie Marshall <ijm667@hotmail.com>
#

bin_PROGRAMS = conagios
EXTRA_DIST = nagioscommand.c \
nagioscommand.h \
nagioscontact.c \
nagioscontactgroup.c \
nagioscontactgroup.h \
nagioscontact.h \
nagioshost.c \
nagioshostdependancy.c \
nagioshostdependancy.h \
nagioshostescalation.c \
nagioshostescalation.h \
nagioshostextinfo.c \
nagioshostextinfo.h \
nagioshostgroup.c \
nagioshostgroup.h \
nagioshost.h \
nagiosservice.c \
nagiosservicedependancy.c \
nagiosservicedependancy.h \
nagiosserviceescalation.c \
nagiosserviceescalation.h \
nagiosservicegroup.c \
nagiosservicegroup.h \
nagiosservice.h \
nagiosseviceextinfo.c \
nagiosseviceextinfo.h \
nagiostimeperiod.c \
nagiostimeperiod.h \
occinagioscommand.c \
occinagioscontact.c \
occinagioscontactgroup.c \
occinagioshost.c \
occinagioshostdependancy.c \
occinagioshostescalation.c \
occinagioshostextinfo.c \
occinagioshostgroup.c \
occinagiosservice.c \
occinagiosservicedependancy.c \
occinagiosserviceescalation.c \
occinagiosservicegroup.c \
occinagiosseviceextinfo.c \
occinagiostimeperiod.c

conagios_SOURCES = conagios.c
conagios_LDFLAGS = $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LD_PUBOCCI) $(LD_COPABR)
conagios_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COPABR) $(CFLAGS_PUBOCCI)




