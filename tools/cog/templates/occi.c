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

[[[cog import codegen.cog_common as t; t.init_models(model_dir, cog_category_file) ]]]
[[[end]]]
 
#ifndef _occi
[[[cog t.filename_root()]]]
[[[end]]] 
_c_
#define _occi
[[[cog t.filename_root()]]]
[[[end]]] 
_c_

#include "
[[[cog t.filename_root()]]]
[[[end]]] 
.h"
#include "
[[[cog t.filename_root()]]]
[[[end]]] 
_backend.h"
#include "
[[[cog t.filename_root()]]]
[[[end]]] 
_node_backend.h"


/*	--------------------------------------------	*/
/*	o c c i _ c o r d s _ p u b l i c a t i o n 	*/
/*	--------------------------------------------	*/

/*	--------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   m a n a g e m e n t   s t r u c t u r e 	*/
/*	--------------------------------------------------------------------	*/
struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * allocate_
[[[cog t.category_name()]]]
[[[end]]] 
();
struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * liberate_
[[[cog t.category_name()]]]
[[[end]]] 
(struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * optr);

private struct 
[[[cog t.filename_root()]]]
[[[end]]] 
_backend_interface *backend;

private struct rest_response *bad_request_html_response(struct rest_response *aptr) {
	return rest_html_response( aptr, 400, "Bad Request");
}

private struct rest_response *not_found_html_response(struct rest_response *aptr) {
	return rest_html_response( aptr, 404, "Not Found");
}

typedef int (*callback_func)(struct occi_category * optr, void * vptr,struct rest_request * rptr);

private void execute_callback(callback_func func, struct 
[[[cog t.category_name()]]]
[[[end]]] 
 *pub, struct occi_category * optr, struct rest_request * rptr) {
	if (func) {
		struct occi_kind_node dummy;
		dummy.contents = pub; // TODO Update callback functions to expect contents, not occi_kind_node
		(*func)(optr,&dummy,rptr);
	}
}


private struct rest_response *bad_request_response(struct rest_response * aptr, char *id) {
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 *pub;
	if (!(pub = backend->retrieve_from_id(id))) {
		return( not_found_html_response(aptr) );
	}
	else {
		liberate_
[[[cog t.category_name()]]]
[[[end]]] 
(pub);
		return( bad_request_html_response(aptr));
	}
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   s e t   f i e l d 	*/
/*	------------------------------------------------------------------------------------------	*/
private void set_
[[[cog t.category_name()]]]
[[[end]]] 
_field(
	struct occi_category * cptr,void * optr, char * nptr, char * vptr)
{
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * pptr;
	char prefix[1024];
	if (!( pptr = optr )) return;
	sprintf(prefix,"%s.%s.",cptr->domain,cptr->id);
	if (!( strncmp( nptr, prefix, strlen(prefix) ) )) {
		nptr += strlen(prefix);
		[[[cog t.set_field()]]]
		[[[end]]]
		}
	return;
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   i n f o 	*/
/*	--------------------------------------------------	*/
// Create a cords 
[[[cog t.filename_root()]]]
[[[end]]] 
 from a rest_request, for use in filtering
private struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * filter_
[[[cog t.category_name()]]]
[[[end]]] 
_info(
	struct occi_category * optr,
	struct rest_request  * rptr,
	struct rest_response * aptr) {
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * pptr;
		if (!( pptr = allocate_
[[[cog t.category_name()]]]
[[[end]]] 
()))
		return( pptr );
	else if (!( occi_process_atributs(optr, rptr, aptr, pptr, set_
[[[cog t.category_name()]]]
[[[end]]] 
_field) ))
		return( liberate_
[[[cog t.category_name()]]]
[[[end]]] 
(pptr));
	else	return( pptr );
}


/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   r e s p o n s e 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * 
[[[cog t.category_name()]]]
[[[end]]] 
_occi_response(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * pptr)
{
	struct rest_header * hptr;
	sprintf(cptr->buffer,"occi.core.id=%c%s%c",0x0022,pptr->id,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	[[[cog t.occi_response()]]]
	[[[end]]]
	if ( occi_render_links( aptr, pptr->id ) != 0)
		return( rest_html_response( aptr, 500, "Server Link Failure" ) );
	else	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

private struct rest_response* make_response_and_liberate_
[[[cog t.filename_root()]]]
[[[end]]] 
(
		struct occi_category* optr, struct rest_client* cptr,
		struct rest_request* rptr, struct rest_response* aptr,
		struct 
[[[cog t.category_name()]]]
[[[end]]] 
* pub) {
	struct rest_response* retVal = 
[[[cog t.category_name()]]]
[[[end]]] 
_occi_response(optr, cptr, rptr, aptr, pub);
	liberate_
[[[cog t.category_name()]]]
[[[end]]] 
(pub);
	return retVal;
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t   i t e m 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * 
[[[cog t.category_name()]]]
[[[end]]] 
_get_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	iptr = optr->callback;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * 
[[[cog t.filename_root()]]]
[[[end]]] 
;
	if (!( 
[[[cog t.filename_root()]]]
[[[end]]] 
 = backend->retrieve_from_id(id)))
		return( not_found_html_response(aptr) );
	if (iptr) {
		execute_callback(iptr->retrieve, 
[[[cog t.filename_root()]]]
[[[end]]] 
, optr, rptr);
		execute_callback(iptr->pre_retrieve, 
[[[cog t.filename_root()]]]
[[[end]]] 
, optr, rptr);
	}
	return make_response_and_liberate_
[[[cog t.filename_root()]]]
[[[end]]] 
(optr, cptr, rptr, aptr, 
[[[cog t.filename_root()]]]
[[[end]]] 
);
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   l i n k 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * 
[[[cog t.category_name()]]]
[[[end]]] 
_post_link(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	return (bad_request_response(aptr, id));
}

/*	--------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   m i x i n 	*/
/*	--------------------------------------------------------------------------------------------	*/
private struct rest_response * 
[[[cog t.category_name()]]]
[[[end]]] 
_post_mixin(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	return (bad_request_response(aptr, id));
}


/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   a c t i o n 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * 
[[[cog t.category_name()]]]
[[[end]]] 
_post_action(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_action * fptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * 
[[[cog t.filename_root()]]]
[[[end]]] 
;
	char * reqhost;
	char * mptr;
	if (!( 
[[[cog t.filename_root()]]]
[[[end]]] 
 = backend->retrieve_from_id(id) ))
		return( not_found_html_response(aptr) );
	mptr = (rptr->parameters+strlen("action="));
	for ( fptr=optr->firstact;
		fptr != (struct occi_action *) 0;
		fptr = fptr->next )
		if (!( strncmp( mptr, fptr->name, strlen( fptr->name )) )) {
			struct rest_response *retVal = occi_invoke_action(fptr,optr,cptr,rptr,aptr,
[[[cog t.filename_root()]]]
[[[end]]] 
);
			liberate_
[[[cog t.category_name()]]]
[[[end]]] 
(
[[[cog t.filename_root()]]]
[[[end]]] 
);
			return(retVal);
		}
	return( rest_html_response( aptr, 400, "Incorrect Action Request"));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   i t e m 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * 
[[[cog t.category_name()]]]
[[[end]]] 
_post_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * initial_
[[[cog t.filename_root()]]]
[[[end]]] 
, *new_
[[[cog t.filename_root()]]]
[[[end]]] 
;
	char * reqhost;
	int    reqport=0;
	iptr = optr->callback;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else reqport = rptr->port;
	if (!( initial_
[[[cog t.filename_root()]]]
[[[end]]] 
 = allocate_
[[[cog t.category_name()]]]
[[[end]]] 
()))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (!( occi_process_atributs( optr, rptr,aptr, initial_
[[[cog t.filename_root()]]]
[[[end]]] 
, set_
[[[cog t.category_name()]]]
[[[end]]] 
_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (iptr) {
		execute_callback(iptr->pre_create, initial_
[[[cog t.filename_root()]]]
[[[end]]] 
, optr, rptr);
		execute_callback(iptr->create, initial_
[[[cog t.filename_root()]]]
[[[end]]] 
, optr, rptr);
	}
	if (!( new_
[[[cog t.filename_root()]]]
[[[end]]] 
 = backend->create(1, initial_
[[[cog t.filename_root()]]]
[[[end]]] 
))) {
		liberate_
[[[cog t.category_name()]]]
[[[end]]] 
(initial_
[[[cog t.filename_root()]]]
[[[end]]] 
);
		return( rest_html_response( aptr, 500, "Server Failure") );
	}
	liberate_
[[[cog t.category_name()]]]
[[[end]]] 
(initial_
[[[cog t.filename_root()]]]
[[[end]]] 
);
	if (iptr) {	execute_callback(iptr->post_create, new_
[[[cog t.filename_root()]]]
[[[end]]] 
, optr, rptr); }
	sprintf(cptr->buffer,"%s:%u%s%s",reqhost,reqport,optr->location,new_
[[[cog t.filename_root()]]]
[[[end]]] 
->id);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Location",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	else if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p u t   i t e m 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * 
[[[cog t.category_name()]]]
[[[end]]] 
_put_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * 
[[[cog t.filename_root()]]]
[[[end]]] 
;
	iptr = optr->callback;
	if (!( 
[[[cog t.filename_root()]]]
[[[end]]] 
 = backend->retrieve_from_id(id) ))
		return( not_found_html_response(aptr) );
	if (!( occi_process_atributs(optr,rptr,aptr, 
[[[cog t.filename_root()]]]
[[[end]]] 
, set_
[[[cog t.category_name()]]]
[[[end]]] 
_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (iptr) {	execute_callback(iptr->pre_update, 
[[[cog t.filename_root()]]]
[[[end]]] 
, optr, rptr); }
	if (iptr) {
		execute_callback(iptr->update, 
[[[cog t.filename_root()]]]
[[[end]]] 
, optr, rptr);
	}
	backend->update(id, 
[[[cog t.filename_root()]]]
[[[end]]] 
);
	if (iptr) {	execute_callback(iptr->pre_update, 
[[[cog t.filename_root()]]]
[[[end]]] 
, optr, rptr); }
	return 
[[[cog t.category_name()]]]
[[[end]]] 
_occi_response(optr, cptr, rptr, aptr, 
[[[cog t.filename_root()]]]
[[[end]]] 
);
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d   i t e m 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * 
[[[cog t.category_name()]]]
[[[end]]] 
_head_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	bad_request_response(aptr, id);
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e   i t e m 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * 
[[[cog t.category_name()]]]
[[[end]]] 
_delete_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * pptr;
	iptr = optr->callback;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 *pub;
	if (!( pub = backend->retrieve_from_id(id)))
		return( not_found_html_response(aptr) );
	if (iptr) {	execute_callback(iptr->pre_delete, pub, optr, rptr); }
	if (iptr) {
		execute_callback(iptr->delete, pub, optr, rptr);
	}
	liberate_
[[[cog t.category_name()]]]
[[[end]]] 
(pub);
	backend->del(id);
	if (iptr) {	execute_callback(iptr->post_delete, NULL, optr, rptr); }
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t   l i s t 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * 
[[[cog t.category_name()]]]
[[[end]]] 
_get_list(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * filter;
	char * reqhost;
	int reqport=0;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else reqport = rptr->port;
	if (!( filter = filter_
[[[cog t.category_name()]]]
[[[end]]] 
_info( optr, rptr, aptr ) ))
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
private struct rest_response * 
[[[cog t.category_name()]]]
[[[end]]] 
_delete_all(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct occi_interface * iptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * filter;
	iptr = optr->callback;
	if (!( filter = filter_
[[[cog t.category_name()]]]
[[[end]]] 
_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	
[[[cog t.filename_root()]]]
[[[end]]] 
_list pub_list = backend->retrieve_from_filter(filter);
	int pub;
	for(pub = 0; pub < pub_list.count; pub++) {
		if (iptr) {
			execute_callback(iptr->delete, pub_list.
[[[cog t.filename_root()]]]
[[[end]]] 
s[pub], optr, rptr);
		}
		if (iptr) {	execute_callback(iptr->pre_delete, pub, optr, rptr); }
	}
	free_
[[[cog t.filename_root()]]]
[[[end]]] 
_list(&pub_list);
	backend->delete_all_matching_filter(filter);
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_
[[[cog t.category_name()]]]
[[[end]]] 
_get(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( 
[[[cog t.category_name()]]]
[[[end]]] 
_get_list( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( 
[[[cog t.category_name()]]]
[[[end]]] 
_get_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( bad_request_html_response(aptr) );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_
[[[cog t.category_name()]]]
[[[end]]] 
_head(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( 
[[[cog t.category_name()]]]
[[[end]]] 
_head_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( bad_request_html_response(aptr) );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_
[[[cog t.category_name()]]]
[[[end]]] 
_post(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( 
[[[cog t.category_name()]]]
[[[end]]] 
_post_item( optr, cptr, rptr, aptr ) );
	else if ( strncmp( rptr->object, optr->location,strlen(optr->location)) != 0)
		return( bad_request_html_response(aptr) );
	else if (!( rptr->parameters ))
		return( bad_request_html_response(aptr) );
	else if (!( strncmp( rptr->parameters, "action=", strlen("action=")) ))
		return( 
[[[cog t.category_name()]]]
[[[end]]] 
_post_action( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "mixin=", strlen("mixin=")) ))
		return( 
[[[cog t.category_name()]]]
[[[end]]] 
_post_mixin( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "link=", strlen("link=")) ))
		return( 
[[[cog t.category_name()]]]
[[[end]]] 
_post_link( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( bad_request_html_response(aptr) );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p u t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_
[[[cog t.category_name()]]]
[[[end]]] 
_put(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( 
[[[cog t.category_name()]]]
[[[end]]] 
_put_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( bad_request_html_response(aptr) );
}

/*	------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e 	*/
/*	------------------------------------------------------------------------------------	*/
private struct rest_response * occi_
[[[cog t.category_name()]]]
[[[end]]] 
_delete(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( 
[[[cog t.category_name()]]]
[[[end]]] 
_delete_all( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( 
[[[cog t.category_name()]]]
[[[end]]] 
_delete_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( bad_request_html_response(aptr) );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   r e d i r e c t i o n 	*/
/*	--------------------------------------------------------------------------------	*/
private void	redirect_occi_
[[[cog t.category_name()]]]
[[[end]]] 
_mt( struct rest_interface * iptr )
{
	iptr->get = occi_
[[[cog t.category_name()]]]
[[[end]]] 
_get;
	iptr->post = occi_
[[[cog t.category_name()]]]
[[[end]]] 
_post;
	iptr->put = occi_
[[[cog t.category_name()]]]
[[[end]]] 
_put;
	iptr->delete = occi_
[[[cog t.category_name()]]]
[[[end]]] 
_delete;
	iptr->head = occi_
[[[cog t.category_name()]]]
[[[end]]] 
_head;
	return;
}

/*	------------------------------------	*/
/*	c r u d   d e l e t e   a c t i o n 	*/
/*	------------------------------------	*/
private struct rest_response * delete_action_
[[[cog t.category_name()]]]
[[[end]]] 
(struct occi_category * optr, 
struct rest_client * cptr,  
struct rest_request * rptr,  
struct rest_response * aptr,  
void * vptr )
{
	aptr = liberate_rest_response( aptr );
	return( occi_
[[[cog t.category_name()]]]
[[[end]]] 
_delete(optr,cptr,rptr));
}

/*	------------------------------------------	*/
/*	o c c i   c a t e g o r y   b u i l d e r 	*/
/*	------------------------------------------	*/
/* occi category rest instance builder for : occi_
[[[cog t.category_name()]]]
[[[end]]] 
 */
public struct occi_category * occi_
[[[cog t.category_name()]]]
[[[end]]] 
_builder(char * a,char * b) {
	char * c="http://scheme.compatibleone.fr/scheme/compatible#";
	char * d="kind";
	char * e="http://scheme.ogf.org/occi/resource#";
	char * f="CompatibleOne OCCI resource 
[[[cog t.category_name()]]]
[[[end]]] 
";
	backend = node_interface_func();	// TODO There's no obvious place to delete this pointer on completion.  Find somewhere!
	struct occi_category * optr;
	if (!( optr = occi_create_category(a,b,c,d,e,f) )) { return(optr); }
	else {
		redirect_occi_
[[[cog t.category_name()]]]
[[[end]]] 
_mt(optr->interface);
		[[[cog t.occi_builder()]]]
		[[[end]]]
		if (!( optr = occi_add_action( optr,"DELETE","",delete_action_
[[[cog t.category_name()]]]
[[[end]]] 
)))
			return( optr );
		backend->init();
		return(optr);
	}

}

/*	------------------------------------------------------------	*/
/*	c o r d s _ p u b l i c a t i o n _ o c c i _ h e a d e r s 	*/
/*	------------------------------------------------------------	*/
public struct rest_header *  
[[[cog t.category_name()]]]
[[[end]]] 
_occi_headers(struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * sptr)
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
	sprintf(buffer,"
[[[cog t.category_name()]]]
[[[end]]] 
; scheme='http://scheme.compatibleone.fr/scheme/compatible#'; class='kind';\r\n");
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	[[[cog t.occi_headers()]]]
	[[[end]]]
	return(first);

}

#endif	/* _occi
[[[cog t.filename_root()]]]
[[[end]]] 
_c_ */
