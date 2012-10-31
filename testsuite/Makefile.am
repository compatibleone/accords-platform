#
# ACCORDS Makefile.am
#
# Copyright 2012, Mandriva, All rights reserved
#
# Authors:
#   Alexandre Lissy <alissy@mandriva.com>
#

SUBDIRS = basic platform

data_TESTSUITE = compatibleone-testsuite.py dirs.py __init__.py utils.py

EXTRA_DIST = $(data_TESTSUITE)

install_python = install -m0644 -D
colibexecdir = $(libexecdir)/compatibleone/testsuite

install-data-local: $(data_TESTSUITE)
	for script in $(data_TESTSUITE); do \
		$(install_python) $$script $(DESTDIR)$(colibexecdir)/$$script; \
	done

uninstall-local:
	for script in $(data_TESTSUITE); do \
		rm $(DESTDIR)$(colibexecdir)/$$script; \
	done
