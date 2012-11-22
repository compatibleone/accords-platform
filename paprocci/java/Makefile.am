#
# PAPROCCI Makefile.am
#
# Copyright 2012, Mandriva, All rights reserved
#
# Authors:
#   Alexandre Lissy <alissy@mandriva.com>
#

EXTRA_DIST = build.xml src/

javadir = $(datadir)/java/accords

java_DATA = dist/co-proactive-procci.jar lib/*.jar

dist/co-proactive-procci.jar:
	ant dist

clean:
	rm dist/co-proactive-procci.jar || true

all-am: dist/co-proactive-procci.jar
