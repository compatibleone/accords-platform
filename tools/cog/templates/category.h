[[[cog import codegen.cog_common as t; t.init_models(model_dir, cog_category_file); t.category_h_guard() ]]]
[[[end]]]

[[[cog t.category_h_struct() ]]]
[[[end]]]
{
	[[[cog t.category_h_members() ]]]
	[[[end]]]
};

#endif
