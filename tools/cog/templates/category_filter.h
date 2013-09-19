[[[cog import codegen.cog_common as t; t.init_models(model_dir, cog_category_file) ]]]
[[[end]]]

#ifndef _
 [[[cog t.category_name()]]]
 [[[end]]]
_occi_filter_h
#define _
 [[[cog t.category_name()]]]
 [[[end]]]
_occi_filter_h

struct CATEGORY_NAME_occi_filter
{
	[[[cog t.category_filters() ]]]
	[[[end]]]
};

unsigned CATEGORY_NAME_count_filters(struct CATEGORY_NAME_occi_filter *filter);

#endif
