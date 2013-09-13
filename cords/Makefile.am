#
# CORDS Makefile.am
#
# Copyright 2011, Mandriva, All rights reserved
#
# Authors:
#   Jean Parpaillon <jparpaillon@mandriva.com>
#
lib_LTLIBRARIES = libcords.la
EXTRA_DIST = operation.c cordselements.h cords.h cordspublic.h

# Build the model files
model_h_files = azconfig.h paconfig.h placement.h account.h instruction.h		\
		node.h package.h tarif.h carrier.h report.h action.h invoice.h onconfig.h 			\
		parameter.h transaction.h compute.h domain.h requirement.h agency.h ipaddress.h		\
		opennebula.h plan.h user.h configuration.h enquiry.h security.h algorithm.h			\
		openstack.h price.h value.h constraint.h event.h manifest.h service.h appliance.h	\
		profile.h windowsazure.h contract.h image.h monitor.h optimise.h storage.h			\
		authorise.h provider.h infrastructure.h network.h osconfig.h system.h	\
		iprange.h instance.h operator.h vm.h port.h firewall.h import.h agreement.h			\
		metadata.h file.h script.h interface.h session.h consumer.h connection.h stream.h	\
		probe.h metric.h alert.h quota.h application.h variable.h business.h zone.h			\
		guarantee.h term.h deltacloud.h terms.h amazonEc2.h control.h			\
		ec2config.h gw.h intercloudGW.h linkgw.h packet.h penalty.h release.h volume.h

include $(top_srcdir)/tools/Makefile_generate_model.am

libcords_la_SOURCES = cords2occi.c cordsbase.c
libcords_la_CFLAGS = $(CFLAGS_CORDS) $(CFLAGS_BACKEND) $(AM_CFLAGS) $(CFLAGS_COXML) $(CFLAGS_COREST) $(CFLAGS_OCCI) -Werror
libcords_la_LDFLAGS = $(LD_COXML) $(LD_BACKEND) -lcurl 

#json-c
JSON_C_DIR=/usr/local
libcords_la_CFLAGS += -I$(JSON_C_DIR)/include/json-c
libcords_la_LDFLAGS+= -L$(JSON_C_DIR)/lib -ljson-c

cords2occi.c cordsbase.c: $(model_c_files)
