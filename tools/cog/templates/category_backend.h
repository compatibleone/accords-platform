[[[cog import codegen.cog_common as t; t.init_models(model_dir, cog_category_file) ]]]
[[[end]]]
#ifndef _
[[[cog t.filename_root()]]]
[[[end]]] 
_backend_h
#define _
[[[cog t.filename_root()]]]
[[[end]]] 
_backend_h

typedef struct  
[[[cog t.category_name()]]]
[[[end]]] 
_id_list_t
{
	int count;
	char **ids;
}  
[[[cog t.category_name()]]]
[[[end]]] 
_id_list;

typedef struct 
[[[cog t.filename_root()]]]
[[[end]]] 
_list_t
{
	int count;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 **
[[[cog t.filename_root()]]]
[[[end]]] 
s;
} 
[[[cog t.filename_root()]]]
[[[end]]] 
_list;

struct	
[[[cog t.filename_root()]]]
[[[end]]] 
_backend_interface
{
	void (*init)();
	void (*finalise)();
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 *(*create)  (int allocate_uuid, struct 
[[[cog t.category_name()]]]
[[[end]]] 
 *initial_
[[[cog t.filename_root()]]]
[[[end]]] 
);
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 *(*retrieve_from_id)(char *id);
	
[[[cog t.filename_root()]]]
[[[end]]] 
_list (*retrieve_from_filter)(struct 
[[[cog t.category_name()]]]
[[[end]]] 
_occi_filter *filter);
	void (*update)  (char *id, struct 
[[[cog t.category_name()]]]
[[[end]]] 
 *updated_
[[[cog t.filename_root()]]]
[[[end]]] 
);
	void (*del)     (char *id);
	void (*delete_all_matching_filter) (struct 
[[[cog t.category_name()]]]
[[[end]]] 
_occi_filter *filter);	// Delete all 
[[[cog t.filename_root()]]]
[[[end]]] 
s matching filter
	 
[[[cog t.category_name()]]]
[[[end]]] 
_id_list (*list) (struct 
[[[cog t.category_name()]]]
[[[end]]] 
_occi_filter *filter); // Return all ids of 
[[[cog t.filename_root()]]]
[[[end]]] 
s matching filter
};

void 
[[[cog t.category_name()]]]
[[[end]]] 
_free_id_list( 
[[[cog t.category_name()]]]
[[[end]]] 
_id_list *list);
void free_
[[[cog t.filename_root()]]]
[[[end]]] 
_list(
[[[cog t.filename_root()]]]
[[[end]]] 
_list *list);

#endif
