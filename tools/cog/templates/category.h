[[[cog import codegen.cog_common as t; t.init_models(model_dir, cog_category_file); t.category_h_guard() ]]]
[[[end]]]

[[[cog t.category_h_struct() ]]]
[[[end]]]
{
	[[[cog t.category_h_members() ]]]
	[[[end]]]
};

struct CATEGORY_NAME * allocate_CATEGORY_NAME();
struct CATEGORY_NAME * liberate_CATEGORY_NAME(struct CATEGORY_NAME * optr);
struct CATEGORY_NAME * reset_CATEGORY_NAME(struct CATEGORY_NAME * sptr);

#endif
