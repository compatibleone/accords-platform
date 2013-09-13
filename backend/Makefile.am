# This makefile will be parsed to create Makefile.in


lib_LTLIBRARIES = libbackend.la

# Build the model files
model_h_files = xlink.h windowsazure.h
include $(top_srcdir)/tools/Makefile_generate_model.am

libbackend_la_SOURCES = backend_common.c link_backend.c backend_profiling.c riak_backend.c \
   xlink_node_backend.c occi_rest_xlink.c xlink.c \
   windowsazure_node_backend.c occi_rest_windowsazure.c windowsazure.c
libbackend_la_CFLAGS = $(CFLAGS_BACKEND) $(CFLAGS_COXML) $(CFLAGS_OCCI) $(CFLAGS_COREST) -Werror
libbackend_la_LDFLAGS = -lcurl 

link_backend.c : $(model_c_files)
