[[[cog import codegen.cog_common as t; t.init_models(model_dir, cog_category_file)]]]
[[[end]]]
#ifndef _FILENAME_ROOT_riak_backend
#define _FILENAME_ROOT_riak_backend

#include "FILENAME_ROOT_backend_interface.h" // TODO Hack to allow building
#include "FILENAME_ROOT_node_backend.h"

struct FILENAME_ROOT_backend_interface *CATEGORY_NAME_riak_backend_interface();

#endif
