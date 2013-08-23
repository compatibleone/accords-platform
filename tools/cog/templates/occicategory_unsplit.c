[[[cog import codegen.cog_common as t; t.init_models(model_dir, cog_category_file) ]]]
[[[end]]]
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
_occi_filter.h"

/*	--------------------------------------------	*/
/*	o c c i _ 
[[[cog t.split_category_name()]]]
[[[end]]] 
	*/
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

/*	--------------------------------------------	*/
/*	o c c i   c a t e g o r y   a d d   n o d e 	*/
/*	--------------------------------------------	*/
private struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * ll_add_
[[[cog t.category_name()]]]
[[[end]]] 
_node(int mode) {
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * pptr;
	if (!( nptr = allocate_occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node() ))
		return( nptr );
	else	{
		if (!( nptr->contents = allocate_
[[[cog t.category_name()]]]
[[[end]]] 
()))
			return( liberate_occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node(nptr) );
		if (!( pptr = nptr->contents ))
			return( liberate_occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node(nptr) );
		else if (( mode != 0 ) && (!( pptr->id = occi_allocate_uuid())))
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
_node(int mode) {
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
_node( mode );
	pthread_mutex_unlock( &list_
[[[cog t.category_name()]]]
[[[end]]] 
_control );
	return(nptr);
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
_name;	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * pptr;
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
			else if (!( nptr = add_
[[[cog t.category_name()]]]
[[[end]]] 					
_node(0))) break;
			else if (!( pptr = nptr->contents )) break;
			[[[cog t.load_attributes()]]]
			[[[end]]]
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

/*	------------------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a c t i v a t e   f i l t e r 	*/
/*	------------------------------------------------------------------------------------------------------	*/
private void activate_
[[[cog t.category_name()]]]
[[[end]]] 
_filter_on_field(
	struct occi_category * cptr,void * target_void, char * field_name)
{
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
_occi_filter * target;
	char prefix[1024];
	if (!( target = target_void )) return;
	sprintf(prefix,"%s.%s.",cptr->domain,cptr->id);
	if (!( strncmp( field_name, prefix, strlen(prefix) ) )) {
		field_name += strlen(prefix);
		[[[cog t.activate_filter()]]]
		[[[end]]]
		}
	return;
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   s e t   f i e l d 	*/
/*	------------------------------------------------------------------------------------------	*/
private void set_
[[[cog t.category_name()]]]
[[[end]]] 
_field(
	struct occi_category * cptr,void * target_void, char * field_name, char * value)
{
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * pptr;
	char prefix[1024];
	if (!( pptr = target_void )) return;
	sprintf(prefix,"%s.%s.",cptr->domain,cptr->id);
	if (!( strncmp( field_name, prefix, strlen(prefix) ) )) {
		field_name += strlen(prefix);
		[[[cog t.set_field()]]]
		[[[end]]]
		}
	return;
}

/*	--------------------------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   s e t   f i l t e r   o n   f i e l d 	*/
/*	--------------------------------------------------------------------------------------------------------------	*/
private void set_
[[[cog t.category_name()]]]
[[[end]]] 
_filter_on_field(
	struct occi_category * category, void * target_void, char * field_name, char * value)
{
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
_occi_filter *filter = target_void;
	set_
[[[cog t.category_name()]]]
[[[end]]] 
_field(category, filter->attributes, field_name, value);
	activate_
[[[cog t.category_name()]]]
[[[end]]] 
_filter_on_field(category, filter, field_name);
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   i n f o 	*/
/*	--------------------------------------------------	*/
private int filter_
[[[cog t.category_name()]]]
[[[end]]] 
_info(
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
_occi_filter *filter,
	struct occi_category * optr,
	struct rest_request  * rptr,
	struct rest_response * aptr) {
	*filter = (const struct 
[[[cog t.category_name()]]]
[[[end]]] 
_occi_filter) {0};
	if (!( filter->attributes = allocate_
[[[cog t.category_name()]]]
[[[end]]] 
()))
		return 0;
	else if (!( occi_process_atributs(optr, rptr, aptr, filter, set_
[[[cog t.category_name()]]]
[[[end]]] 
_filter_on_field) )) {
		liberate_
[[[cog t.category_name()]]]
[[[end]]] 
(filter->attributes);
		return 0;
	}
	else	return( 1 );
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
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_
[[[cog t.category_name()]]]
[[[end]]] 
_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->retrieve)) (*iptr->retrieve)(optr,nptr,rptr);
	autosave_
[[[cog t.category_name()]]]
[[[end]]] 
_nodes();
	return( 
[[[cog t.category_name()]]]
[[[end]]] 
_occi_response(optr,cptr,rptr,aptr,pptr));
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
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * pptr;
	char * reqhost;
	if (!( nptr = locate_
[[[cog t.category_name()]]]
[[[end]]] 
_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

[[[cog t.post_mixin_and_action()]]]
[[[end]]]
 
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
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * pptr;
	char * reqhost;
	int    reqport=0;
	iptr = optr->callback;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else reqport = rptr->port;
	if (!( nptr = add_
[[[cog t.category_name()]]]
[[[end]]] 
_node(1)))
		return( rest_html_response( aptr, 500, "Server Failure") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (!( occi_process_atributs( optr, rptr,aptr, pptr, set_
[[[cog t.category_name()]]]
[[[end]]] 
_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->create)) (*iptr->create)(optr,nptr,rptr);
	autosave_
[[[cog t.category_name()]]]
[[[end]]] 
_nodes();
	sprintf(cptr->buffer,"%s:%u%s%s",reqhost,reqport,optr->location,pptr->id);
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
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_
[[[cog t.category_name()]]]
[[[end]]] 
_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (!( occi_process_atributs(optr,rptr,aptr, pptr, set_
[[[cog t.category_name()]]]
[[[end]]] 
_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->update)) (*iptr->update)(optr,nptr,rptr);
	autosave_
[[[cog t.category_name()]]]
[[[end]]] 
_nodes();
	return( 
[[[cog t.category_name()]]]
[[[end]]] 
_occi_response(optr,cptr,rptr,aptr,pptr));
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
	struct rest_header * hptr;
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * pptr;
	if (!( nptr = locate_
[[[cog t.category_name()]]]
[[[end]]] 
_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
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
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_
[[[cog t.category_name()]]]
[[[end]]] 
_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->delete)) (*iptr->delete)(optr,nptr,rptr);
	drop_
[[[cog t.category_name()]]]
[[[end]]] 
_node( nptr );
	autosave_
[[[cog t.category_name()]]]
[[[end]]] 
_nodes();
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
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * sptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * pptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
_occi_filter filter;
	char * reqhost;
	int reqport=0;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else reqport = rptr->port;
	if (!( filter_
[[[cog t.category_name()]]]
[[[end]]] 
_info(&filter, optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
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
		if (!( pass_
[[[cog t.category_name()]]]
[[[end]]] 
_filter( pptr, &filter) ))
			continue;
		sprintf(cptr->buffer,"%s:%u%s%s",reqhost,reqport,optr->location,pptr->id);
		if (!( hptr = rest_response_header( aptr, "X-OCCI-Location",cptr->buffer) )) {
			liberate_
[[[cog t.category_name()]]]
[[[end]]] 
(filter.attributes);
			return( rest_html_response( aptr, 500, "Server Failure" ) );
		}
	}
	liberate_
[[[cog t.category_name()]]]
[[[end]]] 
(filter.attributes);
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
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * sptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
 * pptr;
	struct 
[[[cog t.category_name()]]]
[[[end]]] 
_occi_filter filter;
	iptr = optr->callback;
	if (!( filter_
[[[cog t.category_name()]]]
[[[end]]] 
_info(&filter, optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	nptr=
[[[cog t.category_name()]]]
[[[end]]] 
_first;
	while (nptr != (struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node *) 0) {
		if ((!( pptr = nptr->contents ))
		||  (!( pass_
[[[cog t.category_name()]]]
[[[end]]] 
_filter( pptr, &filter) ))) {
			nptr = nptr->next;
			continue;
			}
		else	{
			if (( iptr ) && (iptr->delete)) { (*iptr->delete)(optr,nptr,rptr); }
			sptr = nptr->next;
			drop_
[[[cog t.category_name()]]]
[[[end]]] 
_node( nptr );
			nptr = sptr;
		}
	}
	liberate_
[[[cog t.category_name()]]]
[[[end]]] 
(filter.attributes);
	autosave_
[[[cog t.category_name()]]]
[[[end]]] 
_nodes();
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
	else	return( rest_html_response( aptr, 400, "Bad Request") );
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
	else	return( rest_html_response( aptr, 400, "Bad Request") );
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
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( rptr->parameters ))
		return( rest_html_response( aptr, 400, "Bad Request") );
	[[[cog t.post_actions()]]]
	[[[end]]]
	else	return( rest_html_response( aptr, 400, "Bad Request") );
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
	else	return( rest_html_response( aptr, 400, "Bad Request") );
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
	else	return( rest_html_response( aptr, 400, "Bad Request") );
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

[[[cog t.crud_delete_action()]]]
[[[end]]]

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
	char * d="
[[[cog t.node_type()]]]
[[[end]]] 
";
	char * e="http://scheme.ogf.org/occi/
[[[cog t.scheme_kind()]]]
[[[end]]]
#";
	char * f="CompatibleOne OCCI resource 
[[[cog t.category_name()]]]
[[[end]]] 
";
	struct occi_category * optr;
	if (!( optr = occi_create_category(a,b,c,d,e,f) )) { return(optr); }
	else {
		redirect_occi_
[[[cog t.category_name()]]]
[[[end]]] 
_mt(optr->interface);
		[[[cog t.occi_builder()]]]
		[[[end]]]		
		autoload_
[[[cog t.category_name()]]]
[[[end]]] 
_nodes();
		return(optr);
	}

}

/*	------------------------------------------------------------	*/
/*	
[[[cog t.split_category_name()]]]
[[[end]]] 
_ o c c i _ h e a d e r s 	*/
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
; scheme='http://scheme.compatibleone.fr/scheme/compatible#'; class='
[[[cog t.node_type()]]]
[[[end]]] 
';\r\n");
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
