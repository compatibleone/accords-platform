#include "publication.h"

#include "publication_backend.h"
#include "node_backend.h"

private pthread_mutex_t list_cords_publication_control=PTHREAD_MUTEX_INITIALIZER;
private struct occi_kind_node * cords_publication_first = (struct occi_kind_node *) 0;
private struct occi_kind_node * cords_publication_last  = (struct occi_kind_node *) 0;

private void init();
private void finalise();
private struct cords_publication *create  (int allocate_uuid, struct cords_publication *initial_publication);
private struct cords_publication *retrieve_from_id(char *id);
private publication_list retrieve_from_filter(struct cords_publication *filter);
private void update (char *id, struct cords_publication *updated_publication);
private void del    (char *id);
private void delete_all_matching_filter (struct cords_publication *filter);
private id_list list (struct cords_publication *filter);

struct publication_backend_interface *node_interface_func() {
	struct publication_backend_interface node_interface =
	   {init, finalise, create, retrieve_from_id, retrieve_from_filter, update, del, delete_all_matching_filter, list};
	struct publication_backend_interface *interface_ptr =
			(struct publication_backend_interface *) malloc (sizeof(struct publication_backend_interface));
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
private struct occi_kind_node * ll_drop_cords_publication_node(struct occi_kind_node * nptr) {
	if ( nptr ) {
	if (!( nptr->previous ))
		cords_publication_first = nptr->next;
	else	nptr->previous->next = nptr->next;
	if (!( nptr->next ))
		cords_publication_last = nptr->previous;
	else	nptr->next->previous = nptr->previous;
		liberate_occi_kind_node( nptr );
		}
	return((struct occi_kind_node *)0);
}
private struct occi_kind_node * drop_cords_publication_node(struct occi_kind_node * nptr) {
	pthread_mutex_lock( &list_cords_publication_control );
	nptr = ll_drop_cords_publication_node( nptr );
	pthread_mutex_unlock( &list_cords_publication_control );
	return(nptr);
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   l o c a t e   n o d e 	*/
/*	--------------------------------------------------	*/
private struct occi_kind_node * ll_locate_cords_publication_node(char * id) {
	struct occi_kind_node * nptr;
	struct cords_publication * pptr;
	for ( nptr = cords_publication_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		else if (!( pptr->id )) continue;
		else if (!( strcmp(pptr->id,id) )) break;
		}
	return( nptr );
}

private struct occi_kind_node * locate_cords_publication_node(char * id) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_cords_publication_control );
	nptr = ll_locate_cords_publication_node(id);
	pthread_mutex_unlock( &list_cords_publication_control );
	return( nptr );
}


/*	--------------------------------------------	*/
/*	o c c i   c a t e g o r y   a d d   n o d e 	*/
/*	--------------------------------------------	*/
private struct occi_kind_node * ll_add_cords_publication_node(int mode, struct cords_publication *publication) {
	struct occi_kind_node * nptr;
	if (!( nptr = allocate_occi_kind_node() ))
		return( nptr );
	else	{
		nptr->contents = publication;
		if (( mode != 0 ) && (!( publication->id = occi_allocate_uuid())))
			return( liberate_occi_kind_node(nptr) );
		else	{
			if (!( nptr->previous = cords_publication_last ))
				cords_publication_first = nptr;
			else	nptr->previous->next = nptr;
			cords_publication_last = nptr;
			return( nptr );
			}
		}
}
private struct occi_kind_node * add_cords_publication_node(int mode, struct cords_publication *publication) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_cords_publication_control );
	nptr = ll_add_cords_publication_node( mode, publication );
	pthread_mutex_unlock( &list_cords_publication_control );
	return(nptr);
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   p a s s 	*/
/*	--------------------------------------------------	*/
private int pass_cords_publication_filter(
	struct cords_publication * pptr,struct cords_publication * fptr) {
	if (( fptr->id )
	&&  (strlen( fptr->id ) != 0)) {
		if (!( pptr->id ))
			return(0);
		else if ( strcmp(pptr->id,fptr->id) != 0)
			return(0);
		}
	if (( fptr->rating )
	&&  (strlen( fptr->rating ) != 0)) {
		if (!( pptr->rating ))
			return(0);
		else if ( strcmp(pptr->rating,fptr->rating) != 0)
			return(0);
		}
	if (( fptr->what )
	&&  (strlen( fptr->what ) != 0)) {
		if (!( pptr->what ))
			return(0);
		else if ( strcmp(pptr->what,fptr->what) != 0)
			return(0);
		}
	if (( fptr->remote )
	&&  (strlen( fptr->remote ) != 0)) {
		if (!( pptr->remote ))
			return(0);
		else if ( strcmp(pptr->remote,fptr->remote) != 0)
			return(0);
		}
	if (( fptr->zone )
	&&  (strlen( fptr->zone ) != 0)) {
		if (!( pptr->zone ))
			return(0);
		else if ( strcmp(pptr->zone,fptr->zone) != 0)
			return(0);
		}
	if (( fptr->pid ) && ( pptr->pid != fptr->pid )) return(0);
	if (( fptr->who )
	&&  (strlen( fptr->who ) != 0)) {
		if (!( pptr->who ))
			return(0);
		else if ( strcmp(pptr->who,fptr->who) != 0)
			return(0);
		}
	if (( fptr->when ) && ( pptr->when != fptr->when )) return(0);
	if (( fptr->uptime ) && ( pptr->uptime != fptr->uptime )) return(0);
	if (( fptr->state ) && ( pptr->state != fptr->state )) return(0);
	if (( fptr->pass )
	&&  (strlen( fptr->pass ) != 0)) {
		if (!( pptr->pass ))
			return(0);
		else if ( strcmp(pptr->pass,fptr->pass) != 0)
			return(0);
		}
	if (( fptr->where )
	&&  (strlen( fptr->where ) != 0)) {
		if (!( pptr->where ))
			return(0);
		else if ( strcmp(pptr->where,fptr->where) != 0)
			return(0);
		}
	if (( fptr->why )
	&&  (strlen( fptr->why ) != 0)) {
		if (!( pptr->why ))
			return(0);
		else if ( strcmp(pptr->why,fptr->why) != 0)
			return(0);
		}
	if (( fptr->identity )
	&&  (strlen( fptr->identity ) != 0)) {
		if (!( pptr->identity ))
			return(0);
		else if ( strcmp(pptr->identity,fptr->identity) != 0)
			return(0);
		}
	if (( fptr->operator )
	&&  (strlen( fptr->operator ) != 0)) {
		if (!( pptr->operator ))
			return(0);
		else if ( strcmp(pptr->operator,fptr->operator) != 0)
			return(0);
		}
	if (( fptr->price )
	&&  (strlen( fptr->price ) != 0)) {
		if (!( pptr->price ))
			return(0);
		else if ( strcmp(pptr->price,fptr->price) != 0)
			return(0);
		}
	return(1);
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   l o a d 	*/
/*	------------------------------------------------------------------------------------------	*/
private char*autosave_cords_publication_name="cords_publication.xml";
private void autoload_cords_publication_nodes() {
	char * fn=autosave_cords_publication_name;
	struct cords_publication * publication;
	struct xml_element * document;
	struct xml_element * eptr;
	struct xml_element * vptr;
	struct xml_atribut  * aptr;
	if (!( document = document_parse_file(fn)))
		return;
	if ((eptr = document_element(document,"cords_publications")) != (struct xml_element *) 0) {
		for (vptr=eptr->first; vptr != (struct xml_element *) 0; vptr=vptr->next) {
			if (!( vptr->name )) continue;
			else if ( strcmp( vptr->name, "cords_publication" ) ) continue;
			if(!(publication = allocate_cords_publication())) { break; }
			if ((aptr = document_atribut( vptr, "id" )) != (struct xml_atribut *) 0)
				publication->id = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "rating" )) != (struct xml_atribut *) 0)
				publication->rating = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "what" )) != (struct xml_atribut *) 0)
				publication->what = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "remote" )) != (struct xml_atribut *) 0)
				publication->remote = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "zone" )) != (struct xml_atribut *) 0)
				publication->zone = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "pid" )) != (struct xml_atribut *) 0)
				publication->pid = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "who" )) != (struct xml_atribut *) 0)
				publication->who = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "when" )) != (struct xml_atribut *) 0)
				publication->when = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "uptime" )) != (struct xml_atribut *) 0)
				publication->uptime = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "state" )) != (struct xml_atribut *) 0)
				publication->state = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "pass" )) != (struct xml_atribut *) 0)
				publication->pass = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "where" )) != (struct xml_atribut *) 0)
				publication->where = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "why" )) != (struct xml_atribut *) 0)
				publication->why = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "identity" )) != (struct xml_atribut *) 0)
				publication->identity = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "operator" )) != (struct xml_atribut *) 0)
				publication->operator = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "price" )) != (struct xml_atribut *) 0)
				publication->price = document_atribut_string(aptr);
     		if (!( add_cords_publication_node(0, publication))) {
     			liberate_cords_publication(publication);
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
public  void autosave_cords_publication_nodes() {
	char * fn=autosave_cords_publication_name;	struct occi_kind_node * nptr;
	struct cords_publication * pptr;
	FILE * h;
	pthread_mutex_lock( &list_cords_publication_control );
	if (( h = fopen(fn,"w")) != (FILE *) 0) {
	fprintf(h,"<cords_publications>\n");
	for ( nptr = cords_publication_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		fprintf(h,"<cords_publication\n");
		fprintf(h," id=%c",0x0022);
		fprintf(h,"%s",(pptr->id?pptr->id:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," rating=%c",0x0022);
		fprintf(h,"%s",(pptr->rating?pptr->rating:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," what=%c",0x0022);
		fprintf(h,"%s",(pptr->what?pptr->what:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," remote=%c",0x0022);
		fprintf(h,"%s",(pptr->remote?pptr->remote:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," zone=%c",0x0022);
		fprintf(h,"%s",(pptr->zone?pptr->zone:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," pid=%c",0x0022);
		fprintf(h,"%u",pptr->pid);
		fprintf(h,"%c",0x0022);
		fprintf(h," who=%c",0x0022);
		fprintf(h,"%s",(pptr->who?pptr->who:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," when=%c",0x0022);
		fprintf(h,"%u",pptr->when);
		fprintf(h,"%c",0x0022);
		fprintf(h," uptime=%c",0x0022);
		fprintf(h,"%u",pptr->uptime);
		fprintf(h,"%c",0x0022);
		fprintf(h," state=%c",0x0022);
		fprintf(h,"%u",pptr->state);
		fprintf(h,"%c",0x0022);
		fprintf(h," pass=%c",0x0022);
		fprintf(h,"%s",(pptr->pass?pptr->pass:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," where=%c",0x0022);
		fprintf(h,"%s",(pptr->where?pptr->where:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," why=%c",0x0022);
		fprintf(h,"%s",(pptr->why?pptr->why:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," identity=%c",0x0022);
		fprintf(h,"%s",(pptr->identity?pptr->identity:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," operator=%c",0x0022);
		fprintf(h,"%s",(pptr->operator?pptr->operator:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," price=%c",0x0022);
		fprintf(h,"%s",(pptr->price?pptr->price:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," />\n");
		}
	fprintf(h,"</cords_publications>\n");
	fclose(h);
	}
	pthread_mutex_unlock( &list_cords_publication_control );
	return;
}


private struct cords_publication *clone_publication(struct cords_publication *original) {
	struct cords_publication *copy;
	if (!(copy = allocate_cords_publication())) { return NULL; }
	memset(copy, 0, sizeof(struct cords_publication));
	int success = 1;
	if (original->id) {
		success = success && (copy->id = allocate_string(original->id));
	}
	if (original->rating) {
		success = success && (copy->rating = allocate_string(original->rating));
	}
	if (original->what) {
		success = success && (copy->what = allocate_string(original->what));
	}
	if (original->remote) {
		success = success && (copy->remote = allocate_string(original->remote));
	}
	if (original->zone) {
		success = success && (copy->zone = allocate_string(original->zone));
	}
	if (original->pid) {
		copy->pid = original->pid;
	}
	if (original->who) {
		success = success && (copy->who = allocate_string(original->who));
	}
	if (original->when) {
		copy->when = original->when;
	}
	if (original->uptime) {
		copy->uptime = original->uptime;
	}
	if (original->state) {
		copy->state = original->state;
	}
	if (original->pass) {
		success = success && (copy->pass = allocate_string(original->pass));
	}
	if (original->where) {
		success = success && (copy->where = allocate_string(original->where));
	}
	if (original->why) {
		success = success && (copy->why = allocate_string(original->why));
	}
	if (original->identity) {
		success = success && (copy->identity = allocate_string(original->identity));
	}
	if (original->operator) {
		success = success && (copy->operator = allocate_string(original->operator));
	}
	if (original->price) {
		success = success && (copy->price = allocate_string(original->price));
	}
	if(!success) {
		liberate_cords_publication(copy);
		return NULL;
	}
	return copy;
}

private struct cords_publication *retrieve_from_id(char *id) {
	struct occi_kind_node *node = locate_cords_publication_node(id);
	struct cords_publication *publication_copy;
	if (node) {
		return clone_publication(node->contents);
	}
	return NULL;
}

private struct cords_publication *create(int allocate_uuid, struct cords_publication *initial_publication) {
	struct cords_publication *new_publication = clone_publication(initial_publication);
	add_cords_publication_node(1, new_publication);
	autosave_cords_publication_nodes();
	return new_publication;
}

private void del(char *id) {
	struct occi_kind_node *node = locate_cords_publication_node(id);
	if(node) {
		drop_cords_publication_node( node );
	}
	autosave_cords_publication_nodes();
}

void rebase_node_contents(struct occi_kind_node* node, struct cords_publication* updated_publication) {
	pthread_mutex_lock( &list_cords_publication_control );
	if (node->contents) {
		liberate_cords_publication(node->contents);
	}
	node->contents = updated_publication;
	pthread_mutex_unlock( &list_cords_publication_control );
}

private void update(char *id, struct cords_publication *updated_publication) {
	struct occi_kind_node *node = locate_cords_publication_node(id);
	if(node) {
		rebase_node_contents(node, updated_publication);
	}
	autosave_cords_publication_nodes();
}

int count_filter_matches(struct cords_publication* filter) {
	struct cords_publication * pptr;
	struct occi_kind_node * sptr;
	// First pass counts the size of the list
	int id_count = 0;
	for (sptr = cords_publication_first; sptr != (struct occi_kind_node*) 0;
			sptr = sptr->next) {
		if (!(pptr = sptr->contents))
			continue;

		if (pass_cords_publication_filter(pptr, filter)) {
			id_count++;
		}
	}
	return id_count;
}

void *allocate_pointer_array(int id_count) {
	return allocate(sizeof(char*) * id_count);
}

private id_list list(struct cords_publication *filter) {
	id_list ids;
	ids.count = 0;

	int id_count = count_filter_matches(filter);

	struct occi_kind_node * sptr;
	struct cords_publication * pptr;
	if (!(ids.ids = allocate_pointer_array(id_count))) {
		return ids;
	}
	for ( sptr = cords_publication_first;
			sptr != (struct occi_kind_node *) 0;
			sptr = sptr->next ) {
			if (!( pptr = sptr->contents ))
				continue;
			if ( pass_cords_publication_filter( pptr, filter ) ) {
				if(!(ids.ids[ids.count++] = allocate_string(pptr->id))) {
					free_id_list(&ids);
					return ids;
				}
			}
	}
	return ids;
}

private publication_list retrieve_from_filter(struct cords_publication *filter) {
	publication_list publications;
	publications.count = 0;

	int pub_count = count_filter_matches(filter);

	struct occi_kind_node * sptr;
	struct cords_publication * pptr;
	if (!(publications.publications = allocate_pointer_array(pub_count))) {
		return publications;
	}
	for ( sptr = cords_publication_first;
			sptr != (struct occi_kind_node *) 0;
			sptr = sptr->next ) {
			if (!( pptr = sptr->contents ))
				continue;
			if ( pass_cords_publication_filter( pptr, filter ) ) {
				if(!(publications.publications[publications.count++] = pptr)) {
					free_publication_list(&publications);
					return publications;
				}
			}
	}
	return publications;
}

void free_id_list(id_list *list) {
	int i;
	for(i = 0; i < list->count; i++) {
		liberate(list->ids[i]);
	}
	liberate(list->ids);
	list->count = 0;
}

void free_publication_list(publication_list *list) {
	liberate(list->publications);
	list->count = 0;
}

void delete_all_matching_filter(struct cords_publication* filter) {
	struct occi_kind_node* nptr;
	struct occi_kind_node* sptr;
	struct cords_publication* pptr;
	nptr = cords_publication_first;
	while (nptr != (struct occi_kind_node*) 0) {
		if ((!(pptr = nptr->contents))
				|| (!(pass_cords_publication_filter(pptr, filter)))) {
			nptr = nptr->next;
			continue;
		} else {
			sptr = nptr->next;
			drop_cords_publication_node(nptr);
			nptr = sptr;
		}
	}
	autosave_cords_publication_nodes();
}

void init() {
	autoload_cords_publication_nodes();
}

void finalise() {}
