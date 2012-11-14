#
# PAPROCCI Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean-Philippe Braun <jpbraun@mandriva.com>
#

#JDK=/home/cone/bin/jdk1.6.0_25
#cc -g -I$JDK/include/ -I$JDK/include/linux/ -L$JDK/jre/lib/i386 -L$JDK/jre/lib/i386/client -ljava $CURRDIR/invoke.c -o cprocci

bin_PROGRAMS = paprocci
EXTRA_DIST = proccipa.c

paprocci_SOURCES = paprocci.c paclient.c paclient.h invoke.c invoke.h pacontract.c

paprocci_CFLAGS =  $(CFLAGS_PAPROCCI) $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COCARRIER) $(CFLAGS_PUBOCCI) $(CFLAGS_PROCCI) $(CFLAGS_COPABR)   
paprocci_LDFLAGS = $(LDFLAGS_PAPROCCI) $(LD_COCARRIER) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS)  

#Working
#paprocci_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML) $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_COCARRIER) $(CFLAGS_PUBOCCI) $(CFLAGS_PROCCI) $(CFLAGS_COPABR) -I/home/cone/bin/jdk1.6.0_25/include/ -I/home/cone/bin/jdk1.6.0_25/include/linux/ 
#paprocci_LDFLAGS = $(LD_COCARRIER) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LD_COXML) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS) -L/home/cone/bin/jdk1.6.0_25/jre/lib/i386 -L/home/cone/bin/jdk1.6.0_25/jre/lib/i386/client -ljvm -ljava -lverify

