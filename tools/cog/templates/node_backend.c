[[[cog import codegen.cog_common as t; t.init_models(model_dir, cog_category_file) ]]]
[[[end]]]
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "standard.h"
#include "occi.h"
#include "document.h"

#include "FILENAME_ROOT.h"
#include "FILENAME_ROOT_occi_filter.h"
#include "FILENAME_ROOT_backend_interface.h"
#include "FILENAME_ROOT_node_backend.h"
#include "backend_common.h"

#include "backend_profiling.h"

#ifdef BACKEND_PROFILING
static struct backend_profile FILENAME_ROOT_backend_profile;
static void CATEGORY_NAME_count_filters(struct CATEGORY_NAME_occi_filter *filter, filter_count *counts);
#endif

/*	--------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   m a n a g e m e n t   s t r u c t u r e 	*/
/*	--------------------------------------------------------------------	*/
private pthread_mutex_t list_CATEGORY_NAME_control=PTHREAD_MUTEX_INITIALIZER;
private struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * CATEGORY_NAME_first = (struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node *) 0;
private struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * CATEGORY_NAME_last  = (struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node *) 0;
public struct  occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * occi_first_
[[[cog t.node_category()]]]
[[[end]]] 
_node() { return( CATEGORY_NAME_first ); }
public struct  occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * occi_last_
[[[cog t.node_category()]]]
[[[end]]] 
_node() { return( CATEGORY_NAME_last ); }

private void  CATEGORY_NAME_init();
private void CATEGORY_NAME_finalise();
private struct CATEGORY_NAME * 
 [[[cog t.category_name()]]]
 [[[end]]] 
_create  (int allocate_uuid, struct CATEGORY_NAME *initial_FILENAME_ROOT);
private struct CATEGORY_NAME * 
 [[[cog t.category_name()]]]
 [[[end]]] 
_retrieve_from_id(char *id);
private FILENAME_ROOT_list  CATEGORY_NAME_retrieve_from_filter(struct CATEGORY_NAME_occi_filter *filter);
private void  CATEGORY_NAME_update (char *id, struct CATEGORY_NAME *updated_FILENAME_ROOT);
private void  CATEGORY_NAME_del    (char *id);
private void  CATEGORY_NAME_delete_all_matching_filter (struct CATEGORY_NAME_occi_filter *filter);
private  CATEGORY_NAME_id_list  CATEGORY_NAME_list_ids (struct CATEGORY_NAME_occi_filter *filter);
[[[cog t.retrieve_from_name_declaration()]]]
[[[end]]]

struct FILENAME_ROOT_backend_interface *  CATEGORY_NAME_node_interface_func() {
	struct FILENAME_ROOT_backend_interface node_interface =
	   { CATEGORY_NAME_init,
         CATEGORY_NAME_finalise,
         CATEGORY_NAME_create,
         CATEGORY_NAME_retrieve_from_id,
         CATEGORY_NAME_retrieve_from_filter,
         CATEGORY_NAME_update,
         CATEGORY_NAME_del,
         CATEGORY_NAME_delete_all_matching_filter,
         CATEGORY_NAME_list_ids,
         [[[cog t.retrieve_from_name_add_to_struct()]]]
         [[[end]]]
	struct FILENAME_ROOT_backend_interface *interface_ptr =
			(struct FILENAME_ROOT_backend_interface *) malloc (sizeof(struct FILENAME_ROOT_backend_interface));
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
_node * ll_drop_CATEGORY_NAME_node(struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr) {
	if ( nptr ) {
	if (!( nptr->previous ))
		CATEGORY_NAME_first = nptr->next;
	else	nptr->previous->next = nptr->next;
	if (!( nptr->next ))
		CATEGORY_NAME_last = nptr->previous;
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

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   l o c a t e   n o d e 	*/
/*	--------------------------------------------------	*/
private struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * ll_locate_CATEGORY_NAME_node(char * id) {
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	struct CATEGORY_NAME * pptr;
	for ( nptr = CATEGORY_NAME_first;
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

[[[cog t.retrieve_from_name_locate_funcs()]]]
[[[end]]]
/*	--------------------------------------------	*/
/*	o c c i   c a t e g o r y   a d d   n o d e 	*/
/*	--------------------------------------------	*/
private struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * ll_add_CATEGORY_NAME_node(int mode, struct CATEGORY_NAME *FILENAME_ROOT) {
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
		nptr->contents = FILENAME_ROOT;
		if (( mode != 0 ) && (!( FILENAME_ROOT->id = occi_allocate_uuid())))
			return( liberate_occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node(nptr) );
		else	{
			if (!( nptr->previous = CATEGORY_NAME_last ))
				CATEGORY_NAME_first = nptr;
			else	nptr->previous->next = nptr;
			CATEGORY_NAME_last = nptr;
			return( nptr );
			}
		}
}
private struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * add_CATEGORY_NAME_node(int mode, struct CATEGORY_NAME *FILENAME_ROOT) {
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	pthread_mutex_lock( &list_CATEGORY_NAME_control );
	nptr = ll_add_CATEGORY_NAME_node( mode, FILENAME_ROOT );
	pthread_mutex_unlock( &list_CATEGORY_NAME_control );
	return(nptr);
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   p a s s 	*/
/*	--------------------------------------------------	*/
private int pass_CATEGORY_NAME_filter(
	struct CATEGORY_NAME * pptr, struct CATEGORY_NAME_occi_filter *filter) {
	[[[cog t.pass_category_filter()]]]
	[[[end]]]
	return(1);
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   l o a d 	*/
/*	------------------------------------------------------------------------------------------	*/
private char*autosave_CATEGORY_NAME_name="CATEGORY_NAME.xml";
private void autoload_CATEGORY_NAME_nodes() {
	char * fn=autosave_CATEGORY_NAME_name;
	struct CATEGORY_NAME * FILENAME_ROOT;
	struct xml_element * document;
	struct xml_element * eptr;
	struct xml_element * vptr;
	struct xml_atribut  * aptr;
	if (!( document = document_parse_file(fn)))
		return;
	if ((eptr = document_element(document,"CATEGORY_NAMEs")) != (struct xml_element *) 0) {
		for (vptr=eptr->first; vptr != (struct xml_element *) 0; vptr=vptr->next) {
			if (!( vptr->name )) continue;
			else if ( strcmp( vptr->name, "CATEGORY_NAME" ) ) continue;
			if(!(FILENAME_ROOT = allocate_CATEGORY_NAME())) { break; }
			[[[cog t.load_attributes()]]]
			[[[end]]]
     		if (!( add_CATEGORY_NAME_node(0, FILENAME_ROOT))) {
     			liberate_CATEGORY_NAME(FILENAME_ROOT);
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
public  void set_autosave_CATEGORY_NAME_name(char * fn) {
	autosave_CATEGORY_NAME_name = fn;	return;
}
public  void autosave_CATEGORY_NAME_nodes() {
	char * fn=autosave_CATEGORY_NAME_name;	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	struct CATEGORY_NAME * pptr;
	FILE * h;
	pthread_mutex_lock( &list_CATEGORY_NAME_control );
	if (( h = fopen(fn,"w")) != (FILE *) 0) {
	fprintf(h,"<CATEGORY_NAMEs>\n");
	for ( nptr = CATEGORY_NAME_first;
		nptr != (struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		fprintf(h,"<CATEGORY_NAME\n");
		[[[cog t.save_attributes()]]]
		[[[end]]]
		fprintf(h," />\n");
		}
	fprintf(h,"</CATEGORY_NAMEs>\n");
	fclose(h);
	}
	pthread_mutex_unlock( &list_CATEGORY_NAME_control );
	return;
}


private struct CATEGORY_NAME *clone_CATEGORY_NAME(struct CATEGORY_NAME *original) {
	struct CATEGORY_NAME *copy;
	if (!(copy = allocate_CATEGORY_NAME())) { return NULL; }
	memset(copy, 0, sizeof(struct CATEGORY_NAME));
	int success = 1;
	[[[cog t.clone()]]]
	[[[end]]]
	if(!success) {
		liberate_CATEGORY_NAME(copy);
		return NULL;
	}
	return copy;
}

private struct CATEGORY_NAME *  CATEGORY_NAME_retrieve_from_id(char *id) {
[[[cog t.profile('retrieve_from_ids')]]]
[[[end]]]
    struct CATEGORY_NAME*retVal = NULL;
    pthread_mutex_lock( &list_CATEGORY_NAME_control );
    struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node *node = ll_locate_CATEGORY_NAME_node(id);
    if (node) {
        retVal = clone_CATEGORY_NAME(node->contents);
    }
    pthread_mutex_unlock( &list_CATEGORY_NAME_control );
    return retVal;
}

private struct CATEGORY_NAME *  CATEGORY_NAME_create(int allocate_uuid, struct CATEGORY_NAME *initial_FILENAME_ROOT) {
[[[cog t.profile('creates')]]]
[[[end]]]
	struct CATEGORY_NAME *new_FILENAME_ROOT = clone_CATEGORY_NAME(initial_FILENAME_ROOT);
	add_CATEGORY_NAME_node(1, new_FILENAME_ROOT);
	autosave_CATEGORY_NAME_nodes();
	return new_FILENAME_ROOT;
}

private void   CATEGORY_NAME_del(char *id) {
[[[cog t.profile('deletes')]]]
[[[end]]]
    pthread_mutex_lock( &list_CATEGORY_NAME_control );
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node *node = ll_locate_CATEGORY_NAME_node(id);
	if(node) {
		ll_drop_CATEGORY_NAME_node( node );
	}
    pthread_mutex_unlock( &list_CATEGORY_NAME_control );
	autosave_CATEGORY_NAME_nodes();
}

void ll_CATEGORY_NAME_rebase_node_contents(struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node* node, struct CATEGORY_NAME* updated_FILENAME_ROOT) {
	if (node->contents) {
		liberate_CATEGORY_NAME(node->contents);
	}
	node->contents = updated_FILENAME_ROOT;
}

private void   CATEGORY_NAME_update(char *id, struct CATEGORY_NAME *updated_FILENAME_ROOT) {
[[[cog t.profile('updates')]]]
[[[end]]]
    pthread_mutex_lock( &list_CATEGORY_NAME_control );
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node *node = ll_locate_CATEGORY_NAME_node(id);
	if(node) {
		ll_CATEGORY_NAME_rebase_node_contents(node, clone_CATEGORY_NAME(updated_FILENAME_ROOT));
	}
    pthread_mutex_unlock( &list_CATEGORY_NAME_control );
	autosave_CATEGORY_NAME_nodes();
}

int   CATEGORY_NAME_count_filter_matches(struct CATEGORY_NAME_occi_filter* filter) {
	struct CATEGORY_NAME * pptr;
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * sptr;
	// First pass counts the size of the list
	int id_count = 0;
	for (sptr = CATEGORY_NAME_first; sptr != (struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node*) 0;
			sptr = sptr->next) {
		if (!(pptr = sptr->contents))
			continue;

		if (pass_CATEGORY_NAME_filter(pptr, filter)) {
			id_count++;
		}
	}
	return id_count;
}

private CATEGORY_NAME_id_list   CATEGORY_NAME_list_ids(struct CATEGORY_NAME_occi_filter *filter) {
[[[cog t.profile('lists', 'list_counts')]]]
[[[end]]]
    pthread_mutex_lock( &list_CATEGORY_NAME_control );
	CATEGORY_NAME_id_list ids;
	ids.count = 0;

	int id_count = CATEGORY_NAME_count_filter_matches(filter);

	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * sptr;
	struct CATEGORY_NAME * pptr;
    ids.ids = allocate_pointer_array(id_count);
    if(ids.ids) {
        for ( sptr = CATEGORY_NAME_first;
            sptr != (struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node *) 0;
            sptr = sptr->next ) {
			if (!( pptr = sptr->contents ))
				continue;
			if ( pass_CATEGORY_NAME_filter( pptr, filter ) ) {
				if(!(ids.ids[ids.count++] = allocate_string(pptr->id))) {
					CATEGORY_NAME_free_id_list(&ids);
					break;
				}
			}
        }
	}
    pthread_mutex_unlock( &list_CATEGORY_NAME_control );
	return ids;
}

private FILENAME_ROOT_list CATEGORY_NAME_retrieve_from_filter(struct CATEGORY_NAME_occi_filter *filter) {
[[[cog t.profile('retrieve_from_filters', 'retrieve_from_counts')]]]
[[[end]]]
    pthread_mutex_lock( &list_CATEGORY_NAME_control );
	FILENAME_ROOT_list FILENAME_ROOTs;
	FILENAME_ROOTs.count = 0;

	int pub_count = CATEGORY_NAME_count_filter_matches(filter);

	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * sptr;
	struct CATEGORY_NAME * pptr;
	FILENAME_ROOTs.FILENAME_ROOTs = allocate_pointer_array(pub_count);
    if (FILENAME_ROOTs.FILENAME_ROOTs) {
        for ( sptr = CATEGORY_NAME_first;
			sptr != (struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node *) 0;
			sptr = sptr->next ) {
			if (!( pptr = sptr->contents ))
				continue;
			if ( pass_CATEGORY_NAME_filter( pptr, filter ) ) {
				FILENAME_ROOTs.FILENAME_ROOTs[FILENAME_ROOTs.count++] = pptr;
            }
        }
	}
    pthread_mutex_unlock( &list_CATEGORY_NAME_control );
	return FILENAME_ROOTs;
}

void CATEGORY_NAME_free_id_list(CATEGORY_NAME_id_list *list) {
	int i;
	for(i = 0; i < list->count; i++) {
		liberate(list->ids[i]);
	}
	liberate(list->ids);
	list->count = 0;
}

void free_FILENAME_ROOT_list(FILENAME_ROOT_list *list) {
	liberate(list->FILENAME_ROOTs);
	list->count = 0;
}

void  CATEGORY_NAME_delete_all_matching_filter(struct CATEGORY_NAME_occi_filter *filter) {
[[[cog t.profile('delete_from_filters', 'delete_from_counts')]]]
[[[end]]]
    pthread_mutex_lock( &list_CATEGORY_NAME_control );
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node* nptr;
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node* sptr;
	struct CATEGORY_NAME* pptr;
	nptr = CATEGORY_NAME_first;
	while (nptr != (struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node*) 0) {
		if ((!(pptr = nptr->contents))
				|| (!(pass_CATEGORY_NAME_filter(pptr, filter)))) {
			nptr = nptr->next;
			continue;
		} else {
			sptr = nptr->next;
			ll_drop_CATEGORY_NAME_node(nptr);
			nptr = sptr;
		}
	}
    pthread_mutex_unlock( &list_CATEGORY_NAME_control );
	autosave_CATEGORY_NAME_nodes();
}

void  CATEGORY_NAME_init() {
	autoload_CATEGORY_NAME_nodes();
}

void CATEGORY_NAME_finalise() {}

#ifdef BACKEND_PROFILING
[[[cog t.count_filters()]]]
[[[end]]]
    switch (count) {
    case 0:
        counts->zeros++;
        break;
    case 1:
        counts->ones++;
        break;
    case 2:
        counts->twos++;
        break;
    default:
        counts->mores++;
        break;
    }
}
#endif
       
