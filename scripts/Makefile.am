#
# ACCORDS Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean Parpaillon <jparpaillon@mandriva.com>
#
pkgsysconfdir=$(sysconfdir)/$(PACKAGE)
dist_pkgsysconf_DATA=cords_user.xml accords.ini

scripts=accords

aliases = \
	co-start co-broker co-command co-parser co-resolver co-status co-stop \
	run-azprocci run-broker run-coes run-coees run-coips run-comons run-conets run-coobas \
	run-coss run-ezvm run-fileserver run-onprocci run-osprocci run-parser run-procci \
	run-cosched run-publisher run-testosocci run-osocciprocci run-dcprocci run-slam \
	run-cosacs accords-config

EXTRA_DIST = accords.in openssl.cnf accords.ini publication.xml accords-config.in accords.init
CLEANFILES = $(scripts) $(aliases)

# Edit for local run (without installation)
localpath = $(PATH_AZPROCCI):$(PATH_BROKER):$(PATH_COES):$(PATH_COMONS):$(PATH_CONETS):$(PATH_COOBAS):$(PATH_COSS):$(PATH_EZVM):$(PATH_FILESERVER):$(PATH_ONPROCCI):$(PATH_OSPROCCI):$(PATH_PARSER):$(PATH_PROCCI):$(PATH_COSCHED):$(PATH_PUBLISHER):$(builddir)
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

setaliases = sed \
	-e 's|@aliases[@]|$(aliases)|g'

all-local: $(scripts)
	chmod 755 accords
	for alias in $(aliases); do \
	  ln -fs accords $$alias; \
	done

install-exec-local: accords.in Makefile accords.init
	mkdir -p $(DESTDIR)$(bindir)
	srcdir=''; \
	  test -f ./accords.in || srcdir=$(srcdir)/; \
	  $(installedit) $${srcdir}accords.in >$(DESTDIR)$(bindir)/accords
	chmod 755 $(DESTDIR)$(bindir)/accords
	for alias in $(aliases); do \
	  ln -fs accords $(DESTDIR)$(bindir)/$$alias; \
	done
	install -p -m 0755 -D accords.init $(DESTDIR)$(sysconfdir)/init.d/accords

uninstall-local:
	for alias in $(aliases); do \
	  rm -f $(DESTDIR)$(bindir)/$$alias; \
	done
	rm -f $(DESTDIR)$(bindir)/accords
	rm -f $(DESTDIR)$(sysconfdir)/init.d/accords

accords: accords.in
accords: Makefile
	rm -f $@ $@.tmp
	srcdir=''; \
	  test -f ./$@.in || srcdir=$(srcdir)/; \
	  $(installedit) $${srcdir}$@.in | $(setaliases) >$@.tmp
	mv $@.tmp $@
