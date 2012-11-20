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
PAPATH = ${TEMPDIR}/${PANAME}/
PATEST = ${TEMPDIR}/${PANAME}/.stamp
PAFILE = ${PANAME}.zip
PAURL = http://www.activeeon.com/public_content/releases/ProActive/${PAVERSION}/${PAFILE}

${PAFILE}:
	mkdir -p ${TEMPDIR} && curl ${PAURL} -o ${PAFILE}

${PATEST}: ${PAFILE}
	cd ${TEMPDIR} && unzip ${PAFILE}
	touch ${PATEST}

proactive: ${PATEST}
	cp ${TEMPDIR}/${PANAME}/dist/lib/*.jar .

distdir: proactive

dist: proactive

clean:
	rm ${PAFILE} || true
	rm *.jar || true
	rm -rf ${TEMPDIR}/${PANAME} || true

javadir = $(datadir)/java/accords

java_DATA = $(wildcard *.jar)

EXTRA_DIST = ${java_DATA}
