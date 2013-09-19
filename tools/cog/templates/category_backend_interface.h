[[[cog import codegen.cog_common as t; t.init_models(model_dir, cog_category_file) ]]]
[[[end]]]
#ifndef _FILENAME_ROOT_backend_interface_h
#define _FILENAME_ROOT_backend_interface_h

struct	FILENAME_ROOT_backend_interface
{
	void (*init)();
	void (*finalise)();
	struct CATEGORY_NAME *(*create) (struct CATEGORY_NAME *initial_FILENAME_ROOT);
	struct CATEGORY_NAME *(*retrieve_from_id)(char *id);
	CATEGORY_NAME_list (*retrieve_from_filter)(struct CATEGORY_NAME_occi_filter *filter);
	void (*update)  (char *id, struct CATEGORY_NAME *updated_FILENAME_ROOT);
	void (*del)     (char *id);
	void (*delete_all_matching_filter) (struct CATEGORY_NAME_occi_filter *filter);	// Delete all FILENAME_ROOTs matching filter
    CATEGORY_NAME_id_list (*list) (struct CATEGORY_NAME_occi_filter *filter); // Return all ids of FILENAME_ROOTs matching filter
    [[[cog t.filter_by_name_header()]]]
    [[[end]]]
};

extern struct FILENAME_ROOT_backend_interface * CATEGORY_NAME_backend;

#endif
