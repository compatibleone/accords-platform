/* ---------------------------------------------------------------------------- */
/* Advanced Capabilities for Compatible One Resources Delivery System - ACCORDS	*/
/* (C) 2011 by Iain James Marshall <ijm667@hotmail.com>				*/
/* ---------------------------------------------------------------------------- */
/*										*/
/* This is free software; you can redistribute it and/or modify it		*/
/* under the terms of the GNU Lesser General Public License as			*/
/* published by the Free Software Foundation; either version 2.1 of		*/
/* the License, or (at your option) any later version.				*/
/*										*/
/* This software is distributed in the hope that it will be useful,		*/
/* but WITHOUT ANY WARRANTY; without even the implied warranty of		*/
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU		*/
/* Lesser General Public License for more details.				*/
/*										*/
/* You should have received a copy of the GNU Lesser General Public		*/
/* License along with this software; if not, write to the Free			*/
/* Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA		*/
/* 02110-1301 USA, or see the FSF site: http://www.fsf.org.			*/
/*										*/
/* ---------------------------------------------------------------------------- */

/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _occipublication_c_
#define _occipublication_c_

#include "../publication.h"
#include "publication_backend.h"
#include "node_backend.h"


/*	--------------------------------------------	*/
/*	o c c i _ c o r d s _ p u b l i c a t i o n 	*/
/*	--------------------------------------------	*/

/*	--------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   m a n a g e m e n t   s t r u c t u r e 	*/
/*	--------------------------------------------------------------------	*/
struct cords_publication * allocate_cords_publication();
struct cords_publication * liberate_cords_publication(struct cords_publication * optr);

private struct publication_backend_interface *backend;

private struct rest_response *bad_request_html_response(struct rest_response *aptr) {
	return rest_html_response( aptr, 400, "Bad Request");
}

private struct rest_response *not_found_html_response(struct rest_response *aptr) {
	return rest_html_response( aptr, 404, "Not Found");
}

typedef int (*callback_func)(struct occi_category * optr, void * vptr,struct rest_request * rptr);

private void execute_callback(callback_func func, struct cords_publication *pub, struct occi_category * optr, struct rest_request * rptr) {
	if (func) {
		struct occi_kind_node dummy;
		dummy.contents = pub; // TODO Update callback functions to expect contents, not occi_kind_node
		(*func)(optr,&dummy,rptr);
	}
}


private struct rest_response *bad_request_response(struct rest_response * aptr, char *id) {
	struct cords_publication *pub;
	if (!(pub = backend->retrieve_from_id(id))) {
		return( not_found_html_response(aptr) );
	}
	else {
		liberate_cords_publication(pub);
		return( bad_request_html_response(aptr));
	}
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   s e t   f i e l d 	*/
/*	------------------------------------------------------------------------------------------	*/
private void set_cords_publication_field(
	struct occi_category * cptr,void * optr, char * nptr, char * vptr)
{
	struct cords_publication * pptr;
	char prefix[1024];
	if (!( pptr = optr )) return;
	sprintf(prefix,"%s.%s.",cptr->domain,cptr->id);
	if (!( strncmp( nptr, prefix, strlen(prefix) ) )) {
		nptr += strlen(prefix);
		if (!( strcmp( nptr, "rating" ) ))
			pptr->rating = allocate_string(vptr);
		if (!( strcmp( nptr, "what" ) ))
			pptr->what = allocate_string(vptr);
		if (!( strcmp( nptr, "remote" ) ))
			pptr->remote = allocate_string(vptr);
		if (!( strcmp( nptr, "zone" ) ))
			pptr->zone = allocate_string(vptr);
		if (!( strcmp( nptr, "pid" ) ))
			pptr->pid = atoi(vptr);
		if (!( strcmp( nptr, "who" ) ))
			pptr->who = allocate_string(vptr);
		if (!( strcmp( nptr, "when" ) ))
			pptr->when = atoi(vptr);
		if (!( strcmp( nptr, "uptime" ) ))
			pptr->uptime = atoi(vptr);
		if (!( strcmp( nptr, "state" ) ))
			pptr->state = atoi(vptr);
		if (!( strcmp( nptr, "pass" ) ))
			pptr->pass = allocate_string(vptr);
		if (!( strcmp( nptr, "where" ) ))
			pptr->where = allocate_string(vptr);
		if (!( strcmp( nptr, "why" ) ))
			pptr->why = allocate_string(vptr);
		if (!( strcmp( nptr, "identity" ) ))
			pptr->identity = allocate_string(vptr);
		if (!( strcmp( nptr, "operator" ) ))
			pptr->operator = allocate_string(vptr);
		if (!( strcmp( nptr, "price" ) ))
			pptr->price = allocate_string(vptr);
		}
	return;
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   i n f o 	*/
/*	--------------------------------------------------	*/
// Create a cords publication from a rest_request, for use in filtering
private struct cords_publication * filter_cords_publication_info(
	struct occi_category * optr,
	struct rest_request  * rptr,
	struct rest_response * aptr) {
	struct cords_publication * pptr;
		if (!( pptr = allocate_cords_publication()))
		return( pptr );
	else if (!( occi_process_atributs(optr, rptr, aptr, pptr, set_cords_publication_field) ))
		return( liberate_cords_publication(pptr));
	else	return( pptr );
}


/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   r e s p o n s e 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * cords_publication_occi_response(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,
	struct cords_publication * pptr)
{
	struct rest_header * hptr;
	sprintf(cptr->buffer,"occi.core.id=%c%s%c",0x0022,pptr->id,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.rating=%c%s%c",optr->domain,optr->id,0x0022,pptr->rating,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.what=%c%s%c",optr->domain,optr->id,0x0022,pptr->what,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.remote=%c%s%c",optr->domain,optr->id,0x0022,pptr->remote,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.zone=%c%s%c",optr->domain,optr->id,0x0022,pptr->zone,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.pid=%c%u%c",optr->domain,optr->id,0x0022,pptr->pid,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.who=%c%s%c",optr->domain,optr->id,0x0022,pptr->who,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.when=%c%u%c",optr->domain,optr->id,0x0022,pptr->when,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.uptime=%c%u%c",optr->domain,optr->id,0x0022,pptr->uptime,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.state=%c%u%c",optr->domain,optr->id,0x0022,pptr->state,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.pass=%c%s%c",optr->domain,optr->id,0x0022,pptr->pass,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.where=%c%s%c",optr->domain,optr->id,0x0022,pptr->where,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.why=%c%s%c",optr->domain,optr->id,0x0022,pptr->why,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.identity=%c%s%c",optr->domain,optr->id,0x0022,pptr->identity,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.operator=%c%s%c",optr->domain,optr->id,0x0022,pptr->operator,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.price=%c%s%c",optr->domain,optr->id,0x0022,pptr->price,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	if ( occi_render_links( aptr, pptr->id ) != 0)
		return( rest_html_response( aptr, 500, "Server Link Failure" ) );
	else	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

private struct rest_response* make_response_and_liberate_publication(
		struct occi_category* optr, struct rest_client* cptr,
		struct rest_request* rptr, struct rest_response* aptr,
		struct cords_publication* pub) {
	struct rest_response* retVal = cords_publication_occi_response(optr, cptr, rptr, aptr, pub);
	liberate_cords_publication(pub);
	return retVal;
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t   i t e m 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * cords_publication_get_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	iptr = optr->callback;
	struct cords_publication * publication;
	if (!( publication = backend->retrieve_from_id(id)))
		return( not_found_html_response(aptr) );
	if (iptr) {
		execute_callback(iptr->retrieve, publication, optr, rptr);
		execute_callback(iptr->pre_retrieve, publication, optr, rptr);
	}
	return make_response_and_liberate_publication(optr, cptr, rptr, aptr, publication);
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   l i n k 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * cords_publication_post_link(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	return (bad_request_response(aptr, id));
}

/*	--------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   m i x i n 	*/
/*	--------------------------------------------------------------------------------------------	*/
private struct rest_response * cords_publication_post_mixin(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	return (bad_request_response(aptr, id));
}


/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   a c t i o n 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * cords_publication_post_action(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_action * fptr;
	struct cords_publication * publication;
	char * reqhost;
	char * mptr;
	if (!( publication = backend->retrieve_from_id(id) ))
		return( not_found_html_response(aptr) );
	mptr = (rptr->parameters+strlen("action="));
	for ( fptr=optr->firstact;
		fptr != (struct occi_action *) 0;
		fptr = fptr->next )
		if (!( strncmp( mptr, fptr->name, strlen( fptr->name )) )) {
			struct rest_response *retVal = occi_invoke_action(fptr,optr,cptr,rptr,aptr,publication);
			liberate_cords_publication(publication);
			return(retVal);
		}
	return( rest_html_response( aptr, 400, "Incorrect Action Request"));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   i t e m 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * cords_publication_post_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct cords_publication * initial_publication, *new_publication;
	char * reqhost;
	int    reqport=0;
	iptr = optr->callback;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else reqport = rptr->port;
	if (!( initial_publication = allocate_cords_publication()))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (!( occi_process_atributs( optr, rptr,aptr, initial_publication, set_cords_publication_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (iptr) {
		execute_callback(iptr->pre_create, initial_publication, optr, rptr);
		execute_callback(iptr->create, initial_publication, optr, rptr);
	}
	if (!( new_publication = backend->create(1, initial_publication))) {
		liberate_cords_publication(initial_publication);
		return( rest_html_response( aptr, 500, "Server Failure") );
	}
	liberate_cords_publication(initial_publication);
	if (iptr) {	execute_callback(iptr->post_create, new_publication, optr, rptr); }
	sprintf(cptr->buffer,"%s:%u%s%s",reqhost,reqport,optr->location,new_publication->id);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Location",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	else if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p u t   i t e m 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * cords_publication_put_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct cords_publication * publication;
	iptr = optr->callback;
	if (!( publication = backend->retrieve_from_id(id) ))
		return( not_found_html_response(aptr) );
	if (!( occi_process_atributs(optr,rptr,aptr, publication, set_cords_publication_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (iptr) {	execute_callback(iptr->pre_update, publication, optr, rptr); }
	if (iptr) {
		execute_callback(iptr->update, publication, optr, rptr);
	}
	backend->update(id, publication);
	if (iptr) {	execute_callback(iptr->pre_update, publication, optr, rptr); }
	return cords_publication_occi_response(optr, cptr, rptr, aptr, publication);
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d   i t e m 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * cords_publication_head_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	bad_request_response(aptr, id);
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e   i t e m 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * cords_publication_delete_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct cords_publication * pptr;
	iptr = optr->callback;
	struct cords_publication *pub;
	if (!( pub = backend->retrieve_from_id(id)))
		return( not_found_html_response(aptr) );
	if (iptr) {	execute_callback(iptr->pre_delete, pub, optr, rptr); }
	if (iptr) {
		execute_callback(iptr->delete, pub, optr, rptr);
	}
	liberate_cords_publication(pub);
	backend->del(id);
	if (iptr) {	execute_callback(iptr->post_delete, NULL, optr, rptr); }
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t   l i s t 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * cords_publication_get_list(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct cords_publication * filter;
	char * reqhost;
	int reqport=0;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else reqport = rptr->port;
	if (!( filter = filter_cords_publication_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	id_list ids = backend->list(filter);
	int id;
	int failed = 0;
	for(id = 0; id < ids.count; id++) {
		sprintf(cptr->buffer,"%s:%u%s%s",reqhost,reqport,optr->location,ids.ids[id]);
		if (!( hptr = rest_response_header( aptr, "X-OCCI-Location",cptr->buffer) ))
			failed = 1;
	}
	free_id_list(&ids);
	if(failed) {
		return (rest_html_response( aptr, 500, "Server Failure" ));
	}
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}


/*	--------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e   a l l 	*/
/*	--------------------------------------------------------------------------------------------	*/
private struct rest_response * cords_publication_delete_all(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct occi_interface * iptr;
	struct cords_publication * filter;
	iptr = optr->callback;
	if (!( filter = filter_cords_publication_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	publication_list pub_list = backend->retrieve_from_filter(filter);
	int pub;
	for(pub = 0; pub < pub_list.count; pub++) {
		if (iptr) {
			execute_callback(iptr->delete, pub_list.publications[pub], optr, rptr);
		}
		if (iptr) {	execute_callback(iptr->pre_delete, pub, optr, rptr); }
	}
	free_publication_list(&pub_list);
	backend->delete_all_matching_filter(filter);
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_cords_publication_get(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
{
	struct rest_response * aptr;
	struct rest_header   * hptr;
	struct occi_category * optr;
	char * ctptr;
	char * mptr;
	if (!( hptr = rest_resolve_header( rptr->first, "Content-Type" ) ))
		ctptr = "text/occi";
	else	ctptr = hptr->value;
	if (!( optr = vptr )) 
		return( rest_bad_request(vptr,cptr,rptr) );
	if(!(aptr = rest_allocate_response( cptr )))
		return( aptr );
	else if (!(strcmp( rptr->object, optr->location ) ))
		return( cords_publication_get_list( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( cords_publication_get_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( bad_request_html_response(aptr) );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_cords_publication_head(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
{
	struct rest_response * aptr;
	struct rest_header   * hptr;
	struct occi_category * optr;
	char * ctptr;
	char * mptr;
	if (!( hptr = rest_resolve_header( rptr->first, "Content-Type" ) ))
		ctptr = "text/occi";
	else	ctptr = hptr->value;
	if (!( optr = vptr )) 
		return( rest_bad_request(vptr,cptr,rptr) );
	if(!(aptr = rest_allocate_response( cptr )))
		return( aptr );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( cords_publication_head_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( bad_request_html_response(aptr) );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_cords_publication_post(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
{
	struct rest_response * aptr;
	struct rest_header   * hptr;
	struct occi_category * optr;
	char * ctptr;
	char * mptr;
	if (!( hptr = rest_resolve_header( rptr->first, "Content-Type" ) ))
		ctptr = "text/occi";
	else	ctptr = hptr->value;
	if (!( optr = vptr )) 
		return( rest_bad_request(vptr,cptr,rptr) );
	if(!(aptr = rest_allocate_response( cptr )))
		return( aptr );
	else if (!( strcmp( rptr->object, optr->location ) ))
		return( cords_publication_post_item( optr, cptr, rptr, aptr ) );
	else if ( strncmp( rptr->object, optr->location,strlen(optr->location)) != 0)
		return( bad_request_html_response(aptr) );
	else if (!( rptr->parameters ))
		return( bad_request_html_response(aptr) );
	else if (!( strncmp( rptr->parameters, "action=", strlen("action=")) ))
		return( cords_publication_post_action( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "mixin=", strlen("mixin=")) ))
		return( cords_publication_post_mixin( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "link=", strlen("link=")) ))
		return( cords_publication_post_link( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( bad_request_html_response(aptr) );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p u t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_cords_publication_put(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
{
	struct rest_response * aptr;
	struct rest_header   * hptr;
	struct occi_category * optr;
	char * ctptr;
	char * mptr;
	if (!( hptr = rest_resolve_header( rptr->first, "Content-Type" ) ))
		ctptr = "text/occi";
	else	ctptr = hptr->value;
	if (!( optr = vptr )) 
		return( rest_bad_request(vptr,cptr,rptr) );
	if(!(aptr = rest_allocate_response( cptr )))
		return( aptr );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( cords_publication_put_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( bad_request_html_response(aptr) );
}

/*	------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e 	*/
/*	------------------------------------------------------------------------------------	*/
private struct rest_response * occi_cords_publication_delete(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
{
	struct rest_response * aptr;
	struct rest_header   * hptr;
	struct occi_category * optr;
	char * ctptr;
	char * mptr;
	if (!( hptr = rest_resolve_header( rptr->first, "Content-Type" ) ))
		ctptr = "text/occi";
	else	ctptr = hptr->value;
	if (!( optr = vptr )) 
		return( rest_bad_request(vptr,cptr,rptr) );
	if(!(aptr = rest_allocate_response( cptr )))
		return( aptr );
	else if (!(strcmp( rptr->object, optr->location ) ))
		return( cords_publication_delete_all( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( cords_publication_delete_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( bad_request_html_response(aptr) );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   r e d i r e c t i o n 	*/
/*	--------------------------------------------------------------------------------	*/
private void	redirect_occi_cords_publication_mt( struct rest_interface * iptr )
{
	iptr->get = occi_cords_publication_get;
	iptr->post = occi_cords_publication_post;
	iptr->put = occi_cords_publication_put;
	iptr->delete = occi_cords_publication_delete;
	iptr->head = occi_cords_publication_head;
	return;
}

/*	------------------------------------	*/
/*	c r u d   d e l e t e   a c t i o n 	*/
/*	------------------------------------	*/
private struct rest_response * delete_action_cords_publication(struct occi_category * optr, 
struct rest_client * cptr,  
struct rest_request * rptr,  
struct rest_response * aptr,  
void * vptr )
{
	aptr = liberate_rest_response( aptr );
	return( occi_cords_publication_delete(optr,cptr,rptr));
}

/*	------------------------------------------	*/
/*	o c c i   c a t e g o r y   b u i l d e r 	*/
/*	------------------------------------------	*/
/* occi category rest instance builder for : occi_cords_publication */
public struct occi_category * occi_cords_publication_builder(char * a,char * b) {
	char * c="http://scheme.compatibleone.fr/scheme/compatible#";
	char * d="kind";
	char * e="http://scheme.ogf.org/occi/resource#";
	char * f="CompatibleOne OCCI resource cords_publication";
	backend = node_interface_func();	// TODO There's no obvious place to delete this pointer on completion.  Find somewhere!
	struct occi_category * optr;
	if (!( optr = occi_create_category(a,b,c,d,e,f) )) { return(optr); }
	else {
		redirect_occi_cords_publication_mt(optr->interface);
		if (!( optr = occi_add_attribute(optr, "rating",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "what",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "remote",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "zone",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "pid",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "who",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "when",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "uptime",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "state",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "pass",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "where",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "why",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "identity",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "operator",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "price",0,0) ))
			return(optr);
		if (!( optr = occi_add_action( optr,"DELETE","",delete_action_cords_publication)))
			return( optr );
		backend->init();
		return(optr);
	}

}

/*	------------------------------------------------------------	*/
/*	c o r d s _ p u b l i c a t i o n _ o c c i _ h e a d e r s 	*/
/*	------------------------------------------------------------	*/
public struct rest_header *  cords_publication_occi_headers(struct cords_publication * sptr)
{
	struct rest_header * first=(struct rest_header *) 0;
	struct rest_header * last=(struct rest_header *) 0;
	struct rest_header * hptr=(struct rest_header *) 0;
	char buffer[8192];
	if (!( sptr )) return(0);
	if (!( hptr = allocate_rest_header()))
		return(hptr);
		else	if (!( hptr->previous = last))
			first = hptr;
		else	hptr->previous->next = hptr;
		last = hptr;
	if (!( hptr->name = allocate_string("Category")))
		return(first);
	sprintf(buffer,"cords_publication; scheme='http://scheme.compatibleone.fr/scheme/compatible#'; class='kind';\r\n");
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	if (!( hptr = allocate_rest_header()))
		return(first);
		else	if (!( hptr->previous = last))
			first = hptr;
		else	hptr->previous->next = hptr;
		last = hptr;
	if (!( hptr->name = allocate_string("X-OCCI-Attribute")))
		return(first);
	sprintf(buffer,"occi.cords_publication.rating='%s'\r\n",(sptr->rating?sptr->rating:""));
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	if (!( hptr = allocate_rest_header()))
		return(first);
		else	if (!( hptr->previous = last))
			first = hptr;
		else	hptr->previous->next = hptr;
		last = hptr;
	if (!( hptr->name = allocate_string("X-OCCI-Attribute")))
		return(first);
	sprintf(buffer,"occi.cords_publication.what='%s'\r\n",(sptr->what?sptr->what:""));
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	if (!( hptr = allocate_rest_header()))
		return(first);
		else	if (!( hptr->previous = last))
			first = hptr;
		else	hptr->previous->next = hptr;
		last = hptr;
	if (!( hptr->name = allocate_string("X-OCCI-Attribute")))
		return(first);
	sprintf(buffer,"occi.cords_publication.remote='%s'\r\n",(sptr->remote?sptr->remote:""));
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	if (!( hptr = allocate_rest_header()))
		return(first);
		else	if (!( hptr->previous = last))
			first = hptr;
		else	hptr->previous->next = hptr;
		last = hptr;
	if (!( hptr->name = allocate_string("X-OCCI-Attribute")))
		return(first);
	sprintf(buffer,"occi.cords_publication.zone='%s'\r\n",(sptr->zone?sptr->zone:""));
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	if (!( hptr = allocate_rest_header()))
		return(first);
		else	if (!( hptr->previous = last))
			first = hptr;
		else	hptr->previous->next = hptr;
		last = hptr;
	if (!( hptr->name = allocate_string("X-OCCI-Attribute")))
		return(first);
	sprintf(buffer,"occi.cords_publication.pid='%u'\r\n",sptr->pid);
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	if (!( hptr = allocate_rest_header()))
		return(first);
		else	if (!( hptr->previous = last))
			first = hptr;
		else	hptr->previous->next = hptr;
		last = hptr;
	if (!( hptr->name = allocate_string("X-OCCI-Attribute")))
		return(first);
	sprintf(buffer,"occi.cords_publication.who='%s'\r\n",(sptr->who?sptr->who:""));
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	if (!( hptr = allocate_rest_header()))
		return(first);
		else	if (!( hptr->previous = last))
			first = hptr;
		else	hptr->previous->next = hptr;
		last = hptr;
	if (!( hptr->name = allocate_string("X-OCCI-Attribute")))
		return(first);
	sprintf(buffer,"occi.cords_publication.when='%u'\r\n",sptr->when);
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	if (!( hptr = allocate_rest_header()))
		return(first);
		else	if (!( hptr->previous = last))
			first = hptr;
		else	hptr->previous->next = hptr;
		last = hptr;
	if (!( hptr->name = allocate_string("X-OCCI-Attribute")))
		return(first);
	sprintf(buffer,"occi.cords_publication.uptime='%u'\r\n",sptr->uptime);
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	if (!( hptr = allocate_rest_header()))
		return(first);
		else	if (!( hptr->previous = last))
			first = hptr;
		else	hptr->previous->next = hptr;
		last = hptr;
	if (!( hptr->name = allocate_string("X-OCCI-Attribute")))
		return(first);
	sprintf(buffer,"occi.cords_publication.state='%u'\r\n",sptr->state);
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	if (!( hptr = allocate_rest_header()))
		return(first);
		else	if (!( hptr->previous = last))
			first = hptr;
		else	hptr->previous->next = hptr;
		last = hptr;
	if (!( hptr->name = allocate_string("X-OCCI-Attribute")))
		return(first);
	sprintf(buffer,"occi.cords_publication.pass='%s'\r\n",(sptr->pass?sptr->pass:""));
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	if (!( hptr = allocate_rest_header()))
		return(first);
		else	if (!( hptr->previous = last))
			first = hptr;
		else	hptr->previous->next = hptr;
		last = hptr;
	if (!( hptr->name = allocate_string("X-OCCI-Attribute")))
		return(first);
	sprintf(buffer,"occi.cords_publication.where='%s'\r\n",(sptr->where?sptr->where:""));
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	if (!( hptr = allocate_rest_header()))
		return(first);
		else	if (!( hptr->previous = last))
			first = hptr;
		else	hptr->previous->next = hptr;
		last = hptr;
	if (!( hptr->name = allocate_string("X-OCCI-Attribute")))
		return(first);
	sprintf(buffer,"occi.cords_publication.why='%s'\r\n",(sptr->why?sptr->why:""));
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	if (!( hptr = allocate_rest_header()))
		return(first);
		else	if (!( hptr->previous = last))
			first = hptr;
		else	hptr->previous->next = hptr;
		last = hptr;
	if (!( hptr->name = allocate_string("X-OCCI-Attribute")))
		return(first);
	sprintf(buffer,"occi.cords_publication.identity='%s'\r\n",(sptr->identity?sptr->identity:""));
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	if (!( hptr = allocate_rest_header()))
		return(first);
		else	if (!( hptr->previous = last))
			first = hptr;
		else	hptr->previous->next = hptr;
		last = hptr;
	if (!( hptr->name = allocate_string("X-OCCI-Attribute")))
		return(first);
	sprintf(buffer,"occi.cords_publication.operator='%s'\r\n",(sptr->operator?sptr->operator:""));
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	if (!( hptr = allocate_rest_header()))
		return(first);
		else	if (!( hptr->previous = last))
			first = hptr;
		else	hptr->previous->next = hptr;
		last = hptr;
	if (!( hptr->name = allocate_string("X-OCCI-Attribute")))
		return(first);
	sprintf(buffer,"occi.cords_publication.price='%s'\r\n",(sptr->price?sptr->price:""));
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	return(first);

}

#endif	/* _occipublication_c_ */
