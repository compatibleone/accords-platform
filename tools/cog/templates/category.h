[[[cog import codegen.cog_common as t; t.init_models(model_dir, cog_category_file); t.category_h_guard() ]]]
[[[end]]]

[[[cog t.category_h_struct() ]]]
[[[end]]]
{
	[[[cog t.category_h_members() ]]]
	[[[end]]]
};

struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * allocate_
[[[cog t.category_name()]]]
[[[end]]] 
();
struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * liberate_
[[[cog t.category_name()]]]
[[[end]]] 
(struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * optr);

#endif
