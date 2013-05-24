[[[cog import codegen.category_h_template as t; t.category_h_guard(cog_category_file) ]]]
[[[end]]]

[[[cog t.category_h_struct(cog_category_file) ]]]
[[[end]]]
{
	[[[cog t.category_h_members(cog_category_file) ]]]
	[[[end]]]
}

#endif
