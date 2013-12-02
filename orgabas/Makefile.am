#
## ORGABAS Makefile.am
#
## Copyright 2013, Prologue, All rights reserved
#
## Authors:
#   Iain James Marshall, Prologue <ijm667@hotmail.com>

bin_PROGRAMS = orgabas

orgabas_SOURCES = orgabas.c
extra_DISTRIB = ldapclient.c ldapclient.h ldapuser.c orgaccount.c orgatrans.c orga-client.xml

orgabas_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COREST) $(CFLAGS_BROKER) $(CFLAGS_OCCI) $(CFLAGS_COXML)\
	 $(CFLAGS_CORDS) $(CFLAGS_COCCI) $(CFLAGS_PUBOCCI) $(CFLAGS_COPABR) $(CFLAGS_COMONS) \
         $(CFLAGS_TESTORGA) $(CFLAGS_COCSPI)
orgabas_LDFLAGS = $(LD_COCSPI) $(LD_COPABR) $(LD_PUBOCCI) $(LD_COCCI) $(LD_OCCI) $(LD_CORDS) $(LD_COREST) $(LDFLAGS_TESTORGA) $(LD_COXML) $(LD_COSQL) $(LDFLAGS_UUID) $(LDFLAGS_SSL) $(LDFLAGS_THREADS) $(LDFLAGS_LDAP) $(LDFLAGS_POSTGRE)


