# ACCORDS Makefile.am
#
# Copyright 2012, Mandriva, All rights reserved
#
# Authors:
#   Alexandre Lissy <alissy@mandriva.com>
#

data_TESTSUITEPLATFORM = binaries.py __init__.py start.py status.py stop.py

EXTRA_DIST = $(data_TESTSUITEPLATFORM)

install_python = install -m0644 -D
colibexecdir = $(libexecdir)/compatibleone/testsuite/platform

install-data-local: $(data_TESTSUITEPLATFORM)
	for script in $(data_TESTSUITEPLATFORM); do \
		$(install_python) $$script $(DESTDIR)$(colibexecdir)/$$script; \
	done

uninstall-local:
	for script in $(data_TESTSUITE); do \
		rm $(DESTDIR)$(colibexecdir)/$$script; \
	done
