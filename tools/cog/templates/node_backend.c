[[[cog import codegen.cog_common as t; t.init_models(model_dir, cog_category_file); t.h_include() ]]]
[[[end]]]

#include "
[[[cog t.filename_root()]]]
[[[end]]] 
_occi_filter.h"
#include "
[[[cog t.filename_root()]]]
[[[end]]] 
_backend_interface.h"
#include "
[[[cog t.filename_root()]]]
[[[end]]] 
_node_backend.h"
#include "backend_common.h"

/*	--------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   m a n a g e m e n t   s t r u c t u r e 	*/
/*	--------------------------------------------------------------------	*/
private pthread_mutex_t list_
[[[cog t.category_name()]]]
[[[end]]] 
_control=PTHREAD_MUTEX_INITIALIZER;
private struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * 
[[[cog t.category_name()]]]
[[[end]]] 
_first = (struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node *) 0;
private struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * 
[[[cog t.category_name()]]]
[[[end]]] 
_last  = (struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node *) 0;
public struct  occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * occi_first_
[[[cog t.node_category()]]]
[[[end]]] 
_node() { return( 
[[[cog t.category_name()]]]
[[[end]]] 
_first ); }
public struct  occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * occi_last_
[[[cog t.node_category()]]]
[[[end]]] 
_node() { return( 
[[[cog t.category_name()]]]
[[[end]]] 
_last ); }

private void  
[[[cog t.category_name()]]]
[[[end]]] 
_init();
private void 
[[[cog t.category_name()]]]
[[[end]]] 
_finalise();
private struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * 
 [[[cog t.category_name()]]]
 [[[end]]] 
_create  (int allocate_uuid, struct 
[[[cog t.category_name()]]]
[[[end]]] 
 *initial_
[[[cog t.filename_root()]]]
[[[end]]] 
);
private struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * 
 [[[cog t.category_name()]]]
 [[[end]]] 
_retrieve_from_id(char *id);
private 
[[[cog t.filename_root()]]]
[[[end]]] 
_list  
[[[cog t.category_name()]]]
[[[end]]] 
_retrieve_from_filter(struct 
[[[cog t.category_name()]]]
[[[end]]] 
_occi_filter *filter);
private void  
[[[cog t.category_name()]]]
[[[end]]] 
_update (char *id, struct 
[[[cog t.category_name()]]]
[[[end]]] 
 *updated_
[[[cog t.filename_root()]]]
[[[end]]] 
);
private void  
[[[cog t.category_name()]]]
[[[end]]] 
_del    (char *id);
private void  
[[[cog t.category_name()]]]
[[[end]]] 
_delete_all_matching_filter (struct 
[[[cog t.category_name()]]]
[[[end]]] 
_occi_filter *filter);
private  
[[[cog t.category_name()]]]
[[[end]]] 
_id_list  
[[[cog t.category_name()]]]
[[[end]]] 
_list_ids (struct 
[[[cog t.category_name()]]]
[[[end]]] 
_occi_filter *filter);
[[[cog t.retrieve_from_name_declaration()]]]
[[[end]]]

struct 
[[[cog t.filename_root()]]]
[[[end]]] 
_backend_interface *  
[[[cog t.category_name()]]]
[[[end]]] 
_node_interface_func() {
	struct 
[[[cog t.filename_root()]]]
[[[end]]] 
_backend_interface node_interface =
	   { 
[[[cog t.category_name()]]]
[[[end]]] 
_init,
         
[[[cog t.category_name()]]]
[[[end]]] 
_finalise,
         
[[[cog t.category_name()]]]
[[[end]]] 
_create,
         
[[[cog t.category_name()]]]
[[[end]]] 
_retrieve_from_id,
         
[[[cog t.category_name()]]]
[[[end]]] 
_retrieve_from_filter,
         
[[[cog t.category_name()]]]
[[[end]]] 
_update,
         
[[[cog t.category_name()]]]
[[[end]]] 
_del,
         
[[[cog t.category_name()]]]
[[[end]]] 
_delete_all_matching_filter,
         
[[[cog t.category_name()]]]
[[[end]]] 
_list_ids,
         [[[cog t.retrieve_from_name_add_to_struct()]]]
         [[[end]]]
	struct 
[[[cog t.filename_root()]]]
[[[end]]] 
_backend_interface *interface_ptr =
			(struct 
[[[cog t.filename_root()]]]
[[[end]]] 
_backend_interface *) malloc (sizeof(struct 
[[[cog t.filename_root()]]]
[[[end]]] 
_backend_interface));
	interface_ptr->init = node_interface.init;
	interface_ptr->finalise = node_interface.finalise;
	interface_ptr->create = node_interface.create;
	interface_ptr->retrieve_from_id = node_interface.retrieve_from_id;
	interface_ptr->retrieve_from_filter = node_interface.retrieve_from_filter;
	interface_ptr->update = node_interface.update;
	interface_ptr->del = node_interface.del;
	interface_ptr->delete_all_matching_filter = node_interface.delete_all_matching_filter;
	interface_ptr->list = node_interface.list;
	return interface_ptr;
}

/*	----------------------------------------------	*/
/*	o c c i   c a t e g o r y   d r o p   n o d e 	*/
/*	----------------------------------------------	*/
private struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * ll_drop_
[[[cog t.category_name()]]]
[[[end]]] 
_node(struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr) {
	if ( nptr ) {
	if (!( nptr->previous ))
		
[[[cog t.category_name()]]]
[[[end]]] 
_first = nptr->next;
	else	nptr->previous->next = nptr->next;
	if (!( nptr->next ))
		
[[[cog t.category_name()]]]
[[[end]]] 
_last = nptr->previous;
	else	nptr->next->previous = nptr->previous;
		liberate_occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node( nptr );
		}
	return((struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node *)0);
}
private struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * drop_
[[[cog t.category_name()]]]
[[[end]]] 
_node(struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr) {
	pthread_mutex_lock( &list_
[[[cog t.category_name()]]]
[[[end]]] 
_control );
	nptr = ll_drop_
[[[cog t.category_name()]]]
[[[end]]] 
_node( nptr );
	pthread_mutex_unlock( &list_
[[[cog t.category_name()]]]
[[[end]]] 
_control );
	return(nptr);
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   l o c a t e   n o d e 	*/
/*	--------------------------------------------------	*/
private struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * ll_locate_
[[[cog t.category_name()]]]
[[[end]]] 
_node(char * id) {
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * pptr;
	for ( nptr = 
[[[cog t.category_name()]]]
[[[end]]] 
_first;
		nptr != (struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		else if (!( pptr->id )) continue;
		else if (!( strcmp(pptr->id,id) )) break;
		}
	return( nptr );
}

private struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * locate_
[[[cog t.category_name()]]]
[[[end]]] 
_node(char * id) {
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	pthread_mutex_lock( &list_
[[[cog t.category_name()]]]
[[[end]]] 
_control );
	nptr = ll_locate_
[[[cog t.category_name()]]]
[[[end]]] 
_node(id);
	pthread_mutex_unlock( &list_
[[[cog t.category_name()]]]
[[[end]]] 
_control );
	return( nptr );
}

[[[cog t.retrieve_from_name_locate_funcs()]]]
[[[end]]]

/*	--------------------------------------------	*/
/*	o c c i   c a t e g o r y   a d d   n o d e 	*/
/*	--------------------------------------------	*/
private struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * ll_add_
[[[cog t.category_name()]]]
[[[end]]] 
_node(int mode, struct 
[[[cog t.category_name()]]]
[[[end]]] 
 *
[[[cog t.filename_root()]]]
[[[end]]] 
) {
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	if (!( nptr = allocate_occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node() ))
		return( nptr );
	else	{
		nptr->contents = 
[[[cog t.filename_root()]]]
[[[end]]] 
;
		if (( mode != 0 ) && (!( 
[[[cog t.filename_root()]]]
[[[end]]] 
->id = occi_allocate_uuid())))
			return( liberate_occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node(nptr) );
		else	{
			if (!( nptr->previous = 
[[[cog t.category_name()]]]
[[[end]]] 
_last ))
				
[[[cog t.category_name()]]]
[[[end]]] 
_first = nptr;
			else	nptr->previous->next = nptr;
			
[[[cog t.category_name()]]]
[[[end]]] 
_last = nptr;
			return( nptr );
			}
		}
}
private struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * add_
[[[cog t.category_name()]]]
[[[end]]] 
_node(int mode, struct 
[[[cog t.category_name()]]]
[[[end]]] 
 *
[[[cog t.filename_root()]]]
[[[end]]] 
) {
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	pthread_mutex_lock( &list_
[[[cog t.category_name()]]]
[[[end]]] 
_control );
	nptr = ll_add_
[[[cog t.category_name()]]]
[[[end]]] 
_node( mode, 
[[[cog t.filename_root()]]]
[[[end]]] 
 );
	pthread_mutex_unlock( &list_
[[[cog t.category_name()]]]
[[[end]]] 
_control );
	return(nptr);
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   p a s s 	*/
/*	--------------------------------------------------	*/
private int pass_
[[[cog t.category_name()]]]
[[[end]]] 
_filter(
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * pptr, struct 
[[[cog t.category_name()]]]
[[[end]]] 
_occi_filter *filter) {
	[[[cog t.pass_category_filter()]]]
	[[[end]]]
	return(1);
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   l o a d 	*/
/*	------------------------------------------------------------------------------------------	*/
private char*autosave_
[[[cog t.category_name()]]]
[[[end]]] 
_name="
[[[cog t.category_name()]]]
[[[end]]] 
.xml";
private void autoload_
[[[cog t.category_name()]]]
[[[end]]] 
_nodes() {
	char * fn=autosave_
[[[cog t.category_name()]]]
[[[end]]] 
_name;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * 
[[[cog t.filename_root()]]]
[[[end]]] 
;
	struct xml_element * document;
	struct xml_element * eptr;
	struct xml_element * vptr;
	struct xml_atribut  * aptr;
	if (!( document = document_parse_file(fn)))
		return;
	if ((eptr = document_element(document,"
[[[cog t.category_name()]]]
[[[end]]] 
s")) != (struct xml_element *) 0) {
		for (vptr=eptr->first; vptr != (struct xml_element *) 0; vptr=vptr->next) {
			if (!( vptr->name )) continue;
			else if ( strcmp( vptr->name, "
[[[cog t.category_name()]]]
[[[end]]] 
" ) ) continue;
			if(!(
[[[cog t.filename_root()]]]
[[[end]]] 
 = allocate_
[[[cog t.category_name()]]]
[[[end]]] 
())) { break; }
			[[[cog t.load_attributes()]]]
			[[[end]]]
     		if (!( add_
[[[cog t.category_name()]]]
[[[end]]] 
_node(0, 
[[[cog t.filename_root()]]]
[[[end]]] 
))) {
     			liberate_
[[[cog t.category_name()]]]
[[[end]]] 
(
[[[cog t.filename_root()]]]
[[[end]]] 
);
     			break;
     		}
		}
	}
	document = document_drop( document );
	return;
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   s a v e 	*/
/*	------------------------------------------------------------------------------------------	*/
public  void set_autosave_
[[[cog t.category_name()]]]
[[[end]]] 
_name(char * fn) {
	autosave_
[[[cog t.category_name()]]]
[[[end]]] 
_name = fn;	return;
}
public  void autosave_
[[[cog t.category_name()]]]
[[[end]]] 
_nodes() {
	char * fn=autosave_
[[[cog t.category_name()]]]
[[[end]]] 
_name;	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * pptr;
	FILE * h;
	pthread_mutex_lock( &list_
[[[cog t.category_name()]]]
[[[end]]] 
_control );
	if (( h = fopen(fn,"w")) != (FILE *) 0) {
	fprintf(h,"<
[[[cog t.category_name()]]]
[[[end]]] 
s>\n");
	for ( nptr = 
[[[cog t.category_name()]]]
[[[end]]] 
_first;
		nptr != (struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		fprintf(h,"<
[[[cog t.category_name()]]]
[[[end]]] 
\n");
		[[[cog t.save_attributes()]]]
		[[[end]]]
		fprintf(h," />\n");
		}
	fprintf(h,"</
[[[cog t.category_name()]]]
[[[end]]] 
s>\n");
	fclose(h);
	}
	pthread_mutex_unlock( &list_
[[[cog t.category_name()]]]
[[[end]]] 
_control );
	return;
}


private struct 
[[[cog t.category_name()]]]
[[[end]]] 
 *clone_
[[[cog t.category_name()]]]
[[[end]]] 
(struct 
[[[cog t.category_name()]]]
[[[end]]] 
 *original) {
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 *copy;
	if (!(copy = allocate_
[[[cog t.category_name()]]]
[[[end]]] 
())) { return NULL; }
	memset(copy, 0, sizeof(struct 
[[[cog t.category_name()]]]
[[[end]]] 
));
	int success = 1;
	[[[cog t.clone()]]]
	[[[end]]]
	if(!success) {
		liberate_
[[[cog t.category_name()]]]
[[[end]]] 
(copy);
		return NULL;
	}
	return copy;
}

private struct 
[[[cog t.category_name()]]]
[[[end]]] 
 *  
[[[cog t.category_name()]]]
[[[end]]] 
_retrieve_from_id(char *id) {
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node *node = locate_
[[[cog t.category_name()]]]
[[[end]]] 
_node(id);
	if (node) {
		return clone_
[[[cog t.category_name()]]]
[[[end]]] 
(node->contents);
	}
	return NULL;
}

private struct 
[[[cog t.category_name()]]]
[[[end]]] 
 *  
[[[cog t.category_name()]]]
[[[end]]] 
_create(int allocate_uuid, struct 
[[[cog t.category_name()]]]
[[[end]]] 
 *initial_
[[[cog t.filename_root()]]]
[[[end]]] 
) {
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 *new_
[[[cog t.filename_root()]]]
[[[end]]] 
 = clone_
[[[cog t.category_name()]]]
[[[end]]] 
(initial_
[[[cog t.filename_root()]]]
[[[end]]] 
);
	add_
[[[cog t.category_name()]]]
[[[end]]] 
_node(1, new_
[[[cog t.filename_root()]]]
[[[end]]] 
);
	autosave_
[[[cog t.category_name()]]]
[[[end]]] 
_nodes();
	return new_
[[[cog t.filename_root()]]]
[[[end]]] 
;
}

private void   
[[[cog t.category_name()]]]
[[[end]]] 
_del(char *id) {
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node *node = locate_
[[[cog t.category_name()]]]
[[[end]]] 
_node(id);
	if(node) {
		drop_
[[[cog t.category_name()]]]
[[[end]]] 
_node( node );
	}
	autosave_
[[[cog t.category_name()]]]
[[[end]]] 
_nodes();
}

void   
[[[cog t.category_name()]]]
[[[end]]] 
_rebase_node_contents(struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node* node, struct 
[[[cog t.category_name()]]]
[[[end]]] 
* updated_
[[[cog t.filename_root()]]]
[[[end]]] 
) {
	pthread_mutex_lock( &list_
[[[cog t.category_name()]]]
[[[end]]] 
_control );
	if (node->contents) {
		liberate_
[[[cog t.category_name()]]]
[[[end]]] 
(node->contents);
	}
	node->contents = updated_
[[[cog t.filename_root()]]]
[[[end]]] 
;
	pthread_mutex_unlock( &list_
[[[cog t.category_name()]]]
[[[end]]] 
_control );
}

private void   
[[[cog t.category_name()]]]
[[[end]]] 
_update(char *id, struct 
[[[cog t.category_name()]]]
[[[end]]] 
 *updated_
[[[cog t.filename_root()]]]
[[[end]]] 
) {
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node *node = locate_
[[[cog t.category_name()]]]
[[[end]]] 
_node(id);
	if(node) {
		
[[[cog t.category_name()]]]
[[[end]]]
_rebase_node_contents(node, updated_
[[[cog t.filename_root()]]]
[[[end]]] 
);
	}
	autosave_
[[[cog t.category_name()]]]
[[[end]]] 
_nodes();
}

int   
[[[cog t.category_name()]]]
[[[end]]] 
_count_filter_matches(struct 
[[[cog t.category_name()]]]
[[[end]]] 
_occi_filter* filter) {
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * pptr;
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * sptr;
	// First pass counts the size of the list
	int id_count = 0;
	for (sptr = 
[[[cog t.category_name()]]]
[[[end]]] 
_first; sptr != (struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node*) 0;
			sptr = sptr->next) {
		if (!(pptr = sptr->contents))
			continue;

		if (pass_
[[[cog t.category_name()]]]
[[[end]]] 
_filter(pptr, filter)) {
			id_count++;
		}
	}
	return id_count;
}

private  
[[[cog t.category_name()]]]
[[[end]]] 
_id_list   
[[[cog t.category_name()]]]
[[[end]]] 
_list_ids(struct 
[[[cog t.category_name()]]]
[[[end]]] 
_occi_filter *filter) {
	 
[[[cog t.category_name()]]]
[[[end]]] 
_id_list ids;
	ids.count = 0;

	int id_count = 
[[[cog t.category_name()]]]
[[[end]]]
_count_filter_matches(filter);

	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * sptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * pptr;
	if (!(ids.ids = allocate_pointer_array(id_count))) {
		return ids;
	}
	for ( sptr = 
[[[cog t.category_name()]]]
[[[end]]] 
_first;
			sptr != (struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node *) 0;
			sptr = sptr->next ) {
			if (!( pptr = sptr->contents ))
				continue;
			if ( pass_
[[[cog t.category_name()]]]
[[[end]]] 
_filter( pptr, filter ) ) {
				if(!(ids.ids[ids.count++] = allocate_string(pptr->id))) {
					 
[[[cog t.category_name()]]]
[[[end]]] 
_free_id_list(&ids);
					return ids;
				}
			}
	}
	return ids;
}

private 
[[[cog t.filename_root()]]]
[[[end]]] 
_list   
[[[cog t.category_name()]]]
[[[end]]] 
_retrieve_from_filter(struct 
[[[cog t.category_name()]]]
[[[end]]] 
_occi_filter *filter) {
	
[[[cog t.filename_root()]]]
[[[end]]] 
_list 
[[[cog t.filename_root()]]]
[[[end]]] 
s;
	
[[[cog t.filename_root()]]]
[[[end]]] 
s.count = 0;

	int pub_count = 
[[[cog t.category_name()]]]
[[[end]]]
_count_filter_matches(filter);

	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * sptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * pptr;
	if (!(
[[[cog t.filename_root()]]]
[[[end]]] 
s.
[[[cog t.filename_root()]]]
[[[end]]] 
s = allocate_pointer_array(pub_count))) {
		return 
[[[cog t.filename_root()]]]
[[[end]]] 
s;
	}
	for ( sptr = 
[[[cog t.category_name()]]]
[[[end]]] 
_first;
			sptr != (struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node *) 0;
			sptr = sptr->next ) {
			if (!( pptr = sptr->contents ))
				continue;
			if ( pass_
[[[cog t.category_name()]]]
[[[end]]] 
_filter( pptr, filter ) ) {
				if(!(
[[[cog t.filename_root()]]]
[[[end]]] 
s.
[[[cog t.filename_root()]]]
[[[end]]] 
s[
[[[cog t.filename_root()]]]
[[[end]]] 
s.count++] = pptr)) {
					free_
[[[cog t.filename_root()]]]
[[[end]]] 
_list(&
[[[cog t.filename_root()]]]
[[[end]]] 
s);
					return 
[[[cog t.filename_root()]]]
[[[end]]] 
s;
				}
			}
	}
	return 
[[[cog t.filename_root()]]]
[[[end]]] 
s;
}

void  
[[[cog t.category_name()]]]
[[[end]]] 
_free_id_list(
[[[cog t.category_name()]]]
[[[end]]] 
_id_list *list) {
	int i;
	for(i = 0; i < list->count; i++) {
		liberate(list->ids[i]);
	}
	liberate(list->ids);
	list->count = 0;
}

void 
free_
[[[cog t.filename_root()]]]
[[[end]]] 
_list(
[[[cog t.filename_root()]]]
[[[end]]] 
_list *list) {
	liberate(list->
[[[cog t.filename_root()]]]
[[[end]]] 
s);
	list->count = 0;
}

void  
[[[cog t.category_name()]]]
[[[end]]]
_delete_all_matching_filter(struct 
[[[cog t.category_name()]]]
[[[end]]] 
_occi_filter *filter) {
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node* nptr;
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node* sptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
* pptr;
	nptr = 
[[[cog t.category_name()]]]
[[[end]]] 
_first;
	while (nptr != (struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node*) 0) {
		if ((!(pptr = nptr->contents))
				|| (!(pass_
[[[cog t.category_name()]]]
[[[end]]] 
_filter(pptr, filter)))) {
			nptr = nptr->next;
			continue;
		} else {
			sptr = nptr->next;
			drop_
[[[cog t.category_name()]]]
[[[end]]] 
_node(nptr);
			nptr = sptr;
		}
	}
	autosave_
[[[cog t.category_name()]]]
[[[end]]] 
_nodes();
}

void  
[[[cog t.category_name()]]]
[[[end]]]
_init() {
	autoload_
[[[cog t.category_name()]]]
[[[end]]] 
_nodes();
}

void  
[[[cog t.category_name()]]]
[[[end]]]
_finalise() {}
