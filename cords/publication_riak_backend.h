#ifndef _publication_riak_backend
#define _publication_riak_backend

#include "publication_backend_interface.h" // TODO Hack to allow building
#include "publication_node_backend.h"

struct publication_backend_interface *cords_publication_riak_backend_interface();

#endif
