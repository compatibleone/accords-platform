#
# COXML Makefile.am
#
# Copyright 2013, CompatibleOne, All rights reserved
#
# Authors:
#   Iain James Marshall <ijm667@hotmail.com>
#
lib_LTLIBRARIES = libcosql.la
EXTRA_DIST = occisql.h stubmysql.h stubmysql.c

AM_CPPFLAGS = -DUNIX -Dunix

libcosql_la_SOURCES = occisql.c 

libcosql_la_LDFLAGS = $(LDFLAGS_MYSQL)

