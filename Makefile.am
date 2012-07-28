#
# ACCORDS Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean Parpaillon <jparpaillon@mandriva.com>
#
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = scripts
if ENABLE_COXML
SUBDIRS += coxml
endif
if ENABLE_OCCI
SUBDIRS += occi
endif
if ENABLE_CORDS
SUBDIRS += cords
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
if ENABLE_PAASPROCCI
SUBDIRS += paasprocci
endif
if ENABLE_COOL
SUBDIRS += cool
endif
if ENABLE_EXAMPLE
SUBDIRS += example
endif
if ENABLE_COES
SUBDIRS += coes
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
if ENABLE_DCPROCCI
SUBDIRS += dcprocci
endif
if ENABLE_PAPROCCI
SUBDIRS += paprocci
endif
if ENABLE_COMMAND
SUBDIRS += command
endif
if ENABLE_TESTDC
SUBDIRS += testdc
endif
if ENABLE_TESTOS
SUBDIRS += testos
endif
if ENABLE_TESTON
SUBDIRS += teston
endif
if ENABLE_TESTCP
SUBDIRS += testcp
endif
if ENABLE_TESTCB
SUBDIRS += testcb
endif
if ENABLE_TESTAZ
SUBDIRS += testaz
endif
if ENABLE_TESTPA
SUBDIRS += testpa
endif
if ENABLE_FILESERVER
SUBDIRS += fileserver
endif
if ENABLE_TESTRESOLVER
SUBDIRS += testresolver
endif

EXTRA_DIST = README INSTALL NEWS ChangeLog Certificates.crt example1.xml example2.xml example3.xml example4.xml example5.xml coips.xml slamtest.xml platform.xml manifest.xsd cords.xsd slam.xsd cordstypes.xsd accords.spec.in

SPEC=$(PACKAGE).spec
SPEC_DIR:=$(shell rpm -E '%{_specdir}')
SOURCE_DIR:=$(shell rpm -E '%{_sourcedir}')

spec:
	cp $(SPEC).in $(SPEC)
	sed -ri -e "s|_VERSION|$(VERSION)|g" $(SPEC)

dist-srpm: dist-gzip spec
	cp $(SPEC) $(SPEC_DIR)
	cp $(distdir).tar.gz $(SOURCE_DIR)
	rpmbuild -bs $(SPEC_DIR)/$(SPEC)
