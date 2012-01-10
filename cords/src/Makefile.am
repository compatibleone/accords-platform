#
# CORDS Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean Parpaillon <jparpaillon@mandriva.com>
#
lib_LTLIBRARIES = libcords.la
EXTRA_DIST = account.c azconfig.h cordselements.h instruction.c \
	node.c package.c publication.h placement.c placement.h tarif.c \
	account.h carrier.c cords.h instruction.h \
	node.h package.h report.c tarif.h \
	action.c carrier.h cordspublic.h invoice.c \
	onconfig.c parameter.c report.h transaction.c \
	action.h compute.c domain.c invoice.h onconfig.h parameter.h \
	requirement.c transaction.h agency.c \
	compute.h domain.h ipaddress.c opennebula.c \
	plan.c requirement.h user.c agency.h \
	configuration.c enquiry.c ipaddress.h opennebula.h plan.h \
	security.c user.h algorithm.c configuration.h enquiry.h license.h \
	openstack.c price.c security.h value.c algorithm.h constraint.c \
	event.c manifest.c openstack.h price.h service.c value.h \
	appliance.c constraint.h event.h manifest.h operation.c profile.c \
	service.h windowsazure.c appliance.h contract.c image.c monitor.c \
	optimise.c profile.h storage.c windowsazure.h authorise.c \
	contract.h image.h monitor.h optimise.h provider.c storage.h xlink.c \
	authorise.h infrastructure.c network.c osconfig.c provider.h system.c \
	xlink.h azconfig.c infrastructure.h network.h osconfig.h publication.c \
	system.h iprange.c iprange.h instance.c instance.h operator.h operator.c \
	vm.c vm.h

libcords_la_SOURCES = cords2occi.c cordsbase.c
libcords_la_CFLAGS = $(AM_CFLAGS) $(CFLAGS_COXML) $(CFLAGS_COREST) $(CFLAGS_OCCI)
libcords_la_LDFLAGS = $(LD_COXML)
