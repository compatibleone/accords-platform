[[[cog import codegen.cog_common as t; t.init_models(model_dir); t.category_h_guard(cog_category_file) ]]]
[[[end]]]

[[[cog t.category_h_struct(cog_category_file) ]]]
[[[end]]]
{
	[[[cog t.category_h_members(cog_category_file) ]]]
	[[[end]]]
}

#endif
