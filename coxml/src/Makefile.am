#
# COXML Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean Parpaillon <jparpaillon@mandriva.com>
#
lib_LTLIBRARIES = libcoxml.la
EXTRA_DIST = standard.h types.h

AM_CPPFLAGS = -DUNIX -Dunix

libcoxml_la_SOURCES = html.c \
                      document.c document.h \
                      element.c element.h \
                      xmlparse.c xmlparse.h \
                      allocate.c allocate.h
