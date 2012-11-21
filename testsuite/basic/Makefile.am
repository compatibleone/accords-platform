# ACCORDS Makefile.am
#
# Copyright 2012, Mandriva, All rights reserved
#
# Authors:
#   Alexandre Lissy <alissy@mandriva.com>
#

data_TESTSUITEBASIC = accords.py cocheck.py command.py coobas.py coplatform.py \
	coresolver.py costart.py example.py paasprocci.py onprocci.py parser.py \
	testaz.py teston.py azprocci.py cocommand.py comons.py cool.py coprovider.py \
	cosacs.py costatus.py ezvm.py osocciprocci.py procci.py testbin.py testosocci.py \
	broker.py coees.py conagios.py copaas.py coprovision.py cosched.py costop.py \
	fileserver.py osprocci.py publisher.py testcb.py testos.py cobroker.py coips.py \
	conets.py coparser.py cops.py coss.py dcprocci.py __init__.py paprocci.py \
	slam.py testdc.py testresolver.py

EXTRA_DIST = $(data_TESTSUITEBASIC)

install_python = install -m0644 -D
colibexecdir = $(libexecdir)/compatibleone/testsuite/basic

install-data-local: $(data_TESTSUITEBASIC)
	for script in $(data_TESTSUITEBASIC); do \
		$(install_python) $$script $(DESTDIR)$(colibexecdir)/$$script; \
	done

uninstall-local:
	for script in $(data_TESTSUITE); do \
		rm $(DESTDIR)$(colibexecdir)/$$script; \
	done
