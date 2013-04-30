bin_PROGRAMS = strukt

strukt_SOURCES = occirest.c struktsql.c strukt.c

CLEANFILES = _license.h _strukt_wrapper

_license.h: $(srcdir)/license.h
	cp $(srcdir)/license.h $@
	
_strukt_wrapper: $(srcdir)/strukt_wrapper
	cp $(srcdir)/strukt_wrapper $@

all-local: _license.h _strukt_wrapper
