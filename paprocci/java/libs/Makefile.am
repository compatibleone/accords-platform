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
	unzip ${PAFILE} -d ${TEMPDIR} && touch ${PATEST}

proactive: ${PATEST}
	for jar in ProActive.jar ProActive_ResourceManager.jar \
		ProActive_SRM-common-client.jar ProActive_SRM-common.jar \
		ProActive_Scheduler-client.jar ProActive_Scheduler-core.jar \
		bouncycastle.jar commons-cli-1.1.jar commons-httpclient-3.1.jar \
		http-2.0.4.jar javassist.jar log4j.jar trilead-ssh2.jar \
		hibernate-jpa-2.0-api-1.0.1.Final.jar \
	; do \
		cp ${TEMPDIR}/${PANAME}/dist/lib/$$jar $(distdir); \
	done

dist-hook: proactive

clean:
	rm ${PAFILE} || true
	rm *.jar || true
	rm -rf ${TEMPDIR}/${PANAME} || true

javadir = $(datadir)/java/accords

dist_java_DATA = $(wildcard *.jar)
