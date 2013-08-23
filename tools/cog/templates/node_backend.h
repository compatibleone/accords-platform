[[[cog import codegen.cog_common as t; t.init_models(model_dir, cog_category_file); t.node_h_guard() ]]]
[[[end]]]

 struct 
[[[cog t.filename_root()]]]
[[[end]]]
_backend_interface *
[[[cog t.category_name()]]]
[[[end]]]
_node_interface_func();

#endif
