#
# ACCORDS Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
# Copyright 2012, Prologue, All rights reserved
#
# Authors:
#   Jean Parpaillon <jparpaillon@mandriva.com>
#   Iain James Marshall <ijm667@hotmail.com>
#
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = scripts
SUBDIRS += manifests
SUBDIRS += manpages

if ENABLE_COXML
SUBDIRS += coxml
endif
if ENABLE_CORDS
SUBDIRS += cords
endif
if ENABLE_OCCI
SUBDIRS += occi
endif
if ENABLE_COCARRIER
SUBDIRS += cocarrier
endif
if ENABLE_COCCI
SUBDIRS += cocci
endif
if ENABLE_COPABR
SUBDIRS += copabr
endif
if ENABLE_COCSPI
SUBDIRS += cocspi
endif
if ENABLE_PUBOCCI
SUBDIRS += pubocci
endif
if ENABLE_COREST
SUBDIRS += corest
endif
if ENABLE_COSS
SUBDIRS += coss
endif
if ENABLE_EZVM
SUBDIRS += ezvm
endif
if ENABLE_COSACS
SUBDIRS += cosacs
endif
if ENABLE_COIPS
SUBDIRS += coips
endif
if ENABLE_COSCHED
SUBDIRS += cosched
endif
if ENABLE_SLAM
SUBDIRS += slam
endif
if ENABLE_TESTORGA
SUBDIRS += testorga
endif
if ENABLE_ORGABAS
SUBDIRS += orgabas
endif
if ENABLE_COPAAS
SUBDIRS += copaas
endif
if ENABLE_PAASPROCCI
SUBDIRS += paasprocci
endif
if ENABLE_COOL
SUBDIRS += cool
endif
if ENABLE_CONAGIOS
SUBDIRS += conagios
endif
if ENABLE_EXAMPLE
SUBDIRS += example
endif
if ENABLE_COIDMS
SUBDIRS += coidms
endif
if ENABLE_COPS
SUBDIRS += cops
endif
if ENABLE_COEES
SUBDIRS += coees
endif
if ENABLE_COOBAS
SUBDIRS += coobas
endif
if ENABLE_COMONS
SUBDIRS += comons
endif
if ENABLE_CONETS
SUBDIRS += conets
endif
if ENABLE_PUBLISHER
SUBDIRS += publisher
endif
if ENABLE_PARSER
SUBDIRS += parser
endif
if ENABLE_BROKER
SUBDIRS += broker
endif
if ENABLE_PROCCI
SUBDIRS += procci
endif
if ENABLE_OSPROCCI
SUBDIRS += osprocci
endif
if ENABLE_OSOCCIPROCCI
SUBDIRS += osocciprocci
endif
if ENABLE_TESTOSOCCI
SUBDIRS += testosocci
endif
if ENABLE_ONPROCCI
SUBDIRS += onprocci
endif
if ENABLE_AZPROCCI
SUBDIRS += azprocci
endif
if ENABLE_CNPROCCI
SUBDIRS += cnprocci
endif
if ENABLE_EZIPROCCI
SUBDIRS += eziprocci
endif
if ENABLE_COEASY
SUBDIRS += coeasy
endif
if ENABLE_OAPROCCI
SUBDIRS += oaprocci
endif
if ENABLE_DUMMYPROCCI
SUBDIRS += dummyprocci
endif
if ENABLE_COMODEL
SUBDIRS += comodel
endif
if ENABLE_DCPROCCI
SUBDIRS += dcprocci
endif
if ENABLE_PAPROCCI
SUBDIRS += paprocci
endif
if ENABLE_COMMAND
SUBDIRS += command
endif
if ENABLE_TESTOCCI
SUBDIRS += testocci
endif
if ENABLE_TESTOS
SUBDIRS += testos
endif
if ENABLE_TESTON
SUBDIRS += teston
endif
if ENABLE_TESTCN
SUBDIRS += testcn
endif
if ENABLE_TESTHDFS
SUBDIRS += testhdfs
endif
if ENABLE_TESTCP
SUBDIRS += testcp
endif
if ENABLE_TESTCB
SUBDIRS += testcb
endif
if ENABLE_FILESERVER
SUBDIRS += fileserver
endif
if ENABLE_TESTRESOLVER
SUBDIRS += testresolver
endif
if ENABLE_TESTREST
SUBDIRS += testrest
endif
if ENABLE_TESTSUITE
SUBDIRS += testsuite
endif
if ENABLE_TESTAZ
SUBDIRS += testaz
endif
if ENABLE_TESTDC
SUBDIRS += testdc
endif
if ENABLE_PYACCORDS
SUBDIRS += pyaccords pyaccordsSDK
endif

EXTRA_DIST = README INSTALL NEWS ChangeLog Certificates.crt coips.xml slamtest.xml manifest.xsd cords.xsd slam.xsd cordstypes.xsd compatibleone.spec.in accounts.xml

if ENABLE_RPM
ACCORDS_SPEC=compatibleone.spec
PAPROCCI_SPEC=compatibleone-paprocci-java.spec
PAARCH=compatibleone-paprocci-java-$(VERSION)
PAPROCCI_SRCS=$(PAARCH).tar.gz
SPEC_DIR:=$(shell rpm -E '%{_specdir}')
SOURCE_DIR:=$(shell rpm -E '%{_sourcedir}')

TEMPDIR = /tmp/ProActive
PAVERSION = 3.3.0
PANAME = ProActiveScheduling-${PAVERSION}_bin
PAROOT = ${TEMPDIR}/$(PAARCH)/
PAPATH = ${TEMPDIR}/${PANAME}/
PATEST = ${TEMPDIR}/${PANAME}/.stamp
PAFILE = ${PANAME}.zip
PAURL = http://www.activeeon.com/public_content/releases/ProActive/${PAVERSION}/${PAFILE}

${PAFILE}:
	mkdir -p ${TEMPDIR} && curl ${PAURL} -o ${PAFILE}

${PATEST}: ${PAFILE}
	unzip ${PAFILE} -d ${TEMPDIR} && touch ${PATEST}

$(ACCORDS_SPEC): $(ACCORDS_SPEC).in
	cp $(ACCORDS_SPEC).in $(ACCORDS_SPEC)
	sed -ri -e "s|_VERSION|$(VERSION)|g" $(ACCORDS_SPEC)

$(PAPROCCI_SRCS): ${PATEST}
	rm $(PAPROCCI_SRCS) || true
	mkdir -p $(PAROOT)
	for jar in ProActive.jar ProActive_ResourceManager.jar \
		ProActive_SRM-common.jar ProActive_Scheduler-client.jar \
		ProActive_Scheduler-core.jar bouncycastle.jar \
		commons-cli-1.1.jar commons-httpclient-3.1.jar http-2.0.4.jar \
		javassist.jar log4j.jar trilead-ssh2.jar \
		hibernate-jpa-2.0-api-1.0.1.Final.jar \
	; do \
		cp ${TEMPDIR}/${PANAME}/dist/lib/$$jar $(PAROOT); \
	done
	cd $(TEMPDIR) && tar czf $(abs_top_srcdir)/$(PAPROCCI_SRCS) $(PAARCH)/

$(PAPROCCI_SPEC): $(PAPROCCI_SPEC).in
	cp $(PAPROCCI_SPEC).in $(PAPROCCI_SPEC)
	sed -ri -e "s|_VERSION|$(VERSION)|g" $(PAPROCCI_SPEC)

paprocci-srpm: $(PAPROCCI_SPEC) $(PAPROCCI_SRCS)

dist-srpm: dist-gzip $(ACCORDS_SPEC) paprocci-srpm
	cp $(ACCORDS_SPEC) $(SPEC_DIR)
	cp $(PAPROCCI_SPEC) $(SPEC_DIR)
	cp $(distdir).tar.gz $(SOURCE_DIR)
	cp $(PAPROCCI_SRCS) $(SOURCE_DIR)
	rpmbuild -bs $(SPEC_DIR)/$(ACCORDS_SPEC)
	rpmbuild -bs $(SPEC_DIR)/$(PAPROCCI_SPEC)
endif # ENABLE_RPM


