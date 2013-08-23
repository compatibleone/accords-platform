#include "allocate.h"

#include "backend_common.h"

void *allocate_pointer_array(int id_count) {
	return allocate(sizeof(char*) * id_count);
}

