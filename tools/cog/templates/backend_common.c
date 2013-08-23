//[[[cog pass]]] No need for this, it's just a little nicer for the build system to handle this file as a cog template
//[[[end]]]
 
#ifndef _backend_common_c
#define _backend_common_c

#include "allocate.h"

void *allocate_pointer_array(int id_count) {
	return allocate(sizeof(char*) * id_count);
}

#endif
