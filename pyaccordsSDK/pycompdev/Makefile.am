#
# ACCORDS Makefile.am
#
# Copyright 2012, Mandriva, All rights reserved
#
# Authors:
#   Alexandre Lissy <alissy@mandriva.com>
#

EXTRA_DIST = $(wildcard *.c) $(wildcard *.h) $(wildcard *.py)

all-am:
	python setup.py build

install-data-local:
	python setup.py install --root="${DESTDIR}" --prefix="${prefix}"
