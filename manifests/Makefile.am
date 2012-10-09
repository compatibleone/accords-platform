#
# Manifests Makefile.am
#
# Copyright 2012, Mandriva, CityPassenger
# All rights reserved
#
# Authors:
#   Alexandre Lissy <alissy@mandriva.com>
#   Vincent Renardias <vrenardias@citypassenger.com>
#

scripts=build-install

EXTRA_DIST = openssl.cnf build-install.in

CLEANFILES = $(scripts)

localedit = sed \
        -e 's|@sitepath[@]|$(localpath)|g' \
	-e 's|@bindir[@]|$(bindir)|g' \
        -e 's|@pkgsysconfdir[@]|$(pkgsysconfdir)|g'

# Edit for normal run (installed)
installpath = $(pkglibexecdir)
installedit = sed \
        -e 's|@sitepath[@]|$(installpath)|g' \
	-e 's|@bindir[@]|$(bindir)|g' \
        -e 's|@pkgsysconfdir[@]|$(pkgsysconfdir)|g'


build-install: build-install.in
build-install: Makefile
	rm -f $@ $@.tmp
	srcdir=''; \
	  test -f ./$@.in || srcdir=$(srcdir)/; \
	  $(installedit) $${srcdir}$@.in >$@.tmp
	mv $@.tmp $@

all-local: $(scripts)
	chmod 755 build-install

