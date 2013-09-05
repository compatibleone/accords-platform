#include "allocate.h"

#include "backend_common.h"
#include "occi.h"

void *allocate_pointer_array(int id_count) {
	return allocate(sizeof(char*) * id_count);
}

char *generate_id() {
    return occi_allocate_uuid();
}

