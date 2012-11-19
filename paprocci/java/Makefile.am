#
# PAPROCCI Makefile.am
#
# Copyright 2012, Mandriva, All rights reserved
#
# Authors:
#   Alexandre Lissy <alissy@mandriva.com>
#

SUBDIRS = libs

javadir = $(datadir)/java/accords

java_DATA = dist/co-proactive-procci.jar

dist/co-proactive-procci.jar:
	ant dist -Dpalib.dir=libs

clean:
	rm dist/co-proactive-procci.jar || true

all-am: dist/co-proactive-procci.jar
