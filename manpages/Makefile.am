#
# Manifests Makefile.am
#
# Copyright 2012, Mandriva
# All rights reserved
#
# Authors:
#   Alexandre Lissy <alissy@mandriva.com>
#

man7_COMPATIBLEONE = accords.7 compatibleone.7

man1_COMPATIBLEONE =  accords-config.1 co-broker.1 co-command.1 co-parser.1 \
	co-resolver.1 co-start.1 co-status.1 co-stop.1

install_man = install -m0644 -D

install-data-local: $(man1_COMPATIBLEONE) $(man7_COMPATIBLEONE)
	for man in $(man1_COMPATIBLEONE); do \
		$(install_man) $$man $(DESTDIR)$(mandir)/man1/$$man; \
	done
	for man in $(man7_COMPATIBLEONE); do \
		$(install_man) $$man $(DESTDIR)$(mandir)/man7/$$man; \
	done

uninstall-local:
	for man in $(man1_COMPATIBLEONE); do \
		rm $(DESTDIR)$(mandir)/man1/$$man; \
	done
	for man in $(man7_COMPATIBLEONE); do \
		rm $(DESTDIR)$(mandir)/man7/$$man; \
	done
