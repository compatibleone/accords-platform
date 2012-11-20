#
# PAPROCCI Makefile.am
#
# Copyright 2012, Mandriva, All rights reserved
#
# Authors:
#   Alexandre Lissy <alissy@mandriva.com>
#

TEMPDIR = /tmp/ProActive
PAVERSION = 3.3.0
PANAME = ProActiveScheduling-${PAVERSION}_bin
PAFILE = ${PANAME}.zip
PAURL = http://www.activeeon.com/public_content/releases/ProActive/${PAVERSION}/${PAFILE}

${TEMPDIR}/${PAFILE}:
	curl ${PAURL} -o ${TEMPDIR}/${PAFILE}

extract: ${TEMPDIR}/${PAFILE}
	cd ${TEMPDIR} && unzip ${PAFILE}

proactive: extract
	cp ${TEMPDIR}/${PANAME}/dist/lib/*.jar .

dist: proactive

clean:
	rm ${TEMPDIR}/${PAFILE} || true
	rm -rf ${TEMPDIR}/${PANAME} || true

javadir = $(datadir)/java/accords

java_DATA = $(wildcard *.jar)

EXTRA_DIST = ${java_DATA}
