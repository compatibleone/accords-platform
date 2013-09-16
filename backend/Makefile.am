# This makefile will be parsed to create Makefile.in


lib_LTLIBRARIES = libbackend.la

# Build the model files
model_h_files = xlink.h windowsazure.h cnconfig.h computenext.h publication.h authitem.h
include $(top_srcdir)/tools/Makefile_generate_model.am

libbackend_la_SOURCES = backend_common.c link_backend.c backend_profiling.c riak_backend.c \
    xlink_node_backend.c occi_rest_xlink.c xlink.c \
    cnconfig_node_backend.c occi_rest_cnconfig.c cnconfig.c \
    computenext_node_backend.c occi_rest_computenext.c computenext.c \
    windowsazure_node_backend.c occi_rest_windowsazure.c windowsazure.c \
    publication_riak_backend.c occi_rest_publication.c publication.c \
    authitem_node_backend.c occi_rest_authitem.c authitem.c 
libbackend_la_CFLAGS = $(CFLAGS_BACKEND) $(CFLAGS_COXML) $(CFLAGS_OCCI) $(CFLAGS_COREST) -Werror
libbackend_la_LDFLAGS = -lcurl 

#json-c
JSON_C_DIR=/usr/local
libbackend_la_CFLAGS += -I$(JSON_C_DIR)/include/json-c
libbackend_la_LDFLAGS+= -L$(JSON_C_DIR)/lib -ljson-c

link_backend.c : $(model_c_files)
