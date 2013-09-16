# This makefile will be parsed to create Makefile.in


lib_LTLIBRARIES = libbackend.la

# Build the model files
model_h_files = xlink.h windowsazure.h cnconfig.h computenext.h publication.h authitem.h \
    easiclouds_application.h easiclouds_connection.h easiclouds_description.h easiclouds_extra.h \
    easiclouds_instance.h easiclouds_keypair.h easiclouds_link.h easiclouds_metadata.h \
    easiclouds_model.h easiclouds_node.h easiclouds_parameters.h \
    easiclouds_postconf.h easiclouds_server.h easiclouds_input.h easiclouds_output.h \
    saas.h
include $(top_srcdir)/tools/Makefile_generate_model.am

libbackend_la_SOURCES = backend_common.c link_backend.c backend_profiling.c riak_backend.c \
    xlink_node_backend.c occi_rest_xlink.c xlink.c \
    cnconfig_node_backend.c occi_rest_cnconfig.c cnconfig.c \
    computenext_node_backend.c occi_rest_computenext.c computenext.c \
    windowsazure_node_backend.c occi_rest_windowsazure.c windowsazure.c \
    publication_riak_backend.c occi_rest_publication.c publication.c \
    authitem_node_backend.c occi_rest_authitem.c authitem.c \
    easiclouds_application_node_backend.c occi_rest_easiclouds_application.c easiclouds_application.c \
    easiclouds_connection_node_backend.c occi_rest_easiclouds_connection.c easiclouds_connection.c \
    easiclouds_description_node_backend.c occi_rest_easiclouds_description.c easiclouds_description.c \
    easiclouds_extra_node_backend.c occi_rest_easiclouds_extra.c easiclouds_extra.c \
    easiclouds_instance_node_backend.c occi_rest_easiclouds_instance.c easiclouds_instance.c \
    easiclouds_keypair_node_backend.c occi_rest_easiclouds_keypair.c easiclouds_keypair.c \
    easiclouds_link_node_backend.c occi_rest_easiclouds_link.c easiclouds_link.c \
    easiclouds_metadata_node_backend.c occi_rest_easiclouds_metadata.c easiclouds_metadata.c \
    easiclouds_model_node_backend.c occi_rest_easiclouds_model.c easiclouds_model.c \
    easiclouds_node_node_backend.c occi_rest_easiclouds_node.c easiclouds_node.c \
    easiclouds_parameters_node_backend.c occi_rest_easiclouds_parameters.c easiclouds_parameters.c \
    easiclouds_postconf_node_backend.c occi_rest_easiclouds_postconf.c easiclouds_postconf.c \
    easiclouds_server_node_backend.c occi_rest_easiclouds_server.c easiclouds_server.c \
    easiclouds_input_node_backend.c occi_rest_easiclouds_input.c easiclouds_input.c \
    easiclouds_output_node_backend.c occi_rest_easiclouds_output.c easiclouds_output.c \
    saas_node_backend.c occi_rest_saas.c saas.c 
libbackend_la_CFLAGS = $(CFLAGS_BACKEND) $(CFLAGS_COXML) $(CFLAGS_OCCI) $(CFLAGS_COREST) -Werror
libbackend_la_LDFLAGS = -lcurl 

#json-c
JSON_C_DIR=/usr/local
libbackend_la_CFLAGS += -I$(JSON_C_DIR)/include/json-c
libbackend_la_LDFLAGS+= -L$(JSON_C_DIR)/lib -ljson-c

link_backend.c : $(model_c_files) generator_input.txt
