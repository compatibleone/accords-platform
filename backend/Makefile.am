# This makefile will be parsed to create Makefile.in


lib_LTLIBRARIES = libbackend.la

# Build the model files
model_h_files = xlink.h
include $(top_srcdir)/tools/Makefile_generate_model.am

libbackend_la_SOURCES = backend_common.c link_backend.c backend_profiling.c \
    $(abs_builddir)/xlink_node_backend.c $(abs_builddir)/occi_xlink_rest.c $(abs_builddir)/xlink.c
libbackend_la_CFLAGS = $(CFLAGS_BACKEND) $(CFLAGS_COXML) $(CFLAGS_OCCI) $(CFLAGS_COREST) $(CFLAGS_CORDS) -Werror
libbackend_la_LDFLAGS = $(LD_CORDS)

link_backend.c : $(model_c_files)
