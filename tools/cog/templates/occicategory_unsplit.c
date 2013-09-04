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
#ifndef _occiFILENAME_ROOT_c_
#define _occiFILENAME_ROOT_c_

#include "FILENAME_ROOT.h"
#include "FILENAME_ROOT_occi_filter.h"

/*	--------------------------------------------	*/
/*	o c c i _ 
[[[cog t.split_category_name()]]]
[[[end]]] 
	*/
/*	--------------------------------------------	*/

/*	--------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   m a n a g e m e n t   s t r u c t u r e 	*/
/*	--------------------------------------------------------------------	*/
struct CATEGORY_NAME * allocate_CATEGORY_NAME();
struct CATEGORY_NAME * liberate_CATEGORY_NAME(struct CATEGORY_NAME * optr);
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
private struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * drop_CATEGORY_NAME_node(struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr) {
	pthread_mutex_lock( &list_CATEGORY_NAME_control );
	nptr = ll_drop_CATEGORY_NAME_node( nptr );
	pthread_mutex_unlock( &list_CATEGORY_NAME_control );
	return(nptr);
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
private struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * locate_CATEGORY_NAME_node(char * id) {
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	pthread_mutex_lock( &list_CATEGORY_NAME_control );
	nptr = ll_locate_CATEGORY_NAME_node(id);
	pthread_mutex_unlock( &list_CATEGORY_NAME_control );
	return( nptr );
}

/*	--------------------------------------------	*/
/*	o c c i   c a t e g o r y   a d d   n o d e 	*/
/*	--------------------------------------------	*/
private struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * ll_add_CATEGORY_NAME_node(int mode) {
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	struct CATEGORY_NAME * pptr;
	if (!( nptr = allocate_occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node() ))
		return( nptr );
	else	{
		if (!( nptr->contents = allocate_CATEGORY_NAME()))
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
_node * add_CATEGORY_NAME_node(int mode) {
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	pthread_mutex_lock( &list_CATEGORY_NAME_control );
	nptr = ll_add_CATEGORY_NAME_node( mode );
	pthread_mutex_unlock( &list_CATEGORY_NAME_control );
	return(nptr);
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   l o a d 	*/
/*	------------------------------------------------------------------------------------------	*/
private char*autosave_CATEGORY_NAME_name="CATEGORY_NAME.xml";
private void autoload_CATEGORY_NAME_nodes() {
	char * fn=autosave_CATEGORY_NAME_name;	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	struct CATEGORY_NAME * pptr;
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
			else if (!( nptr = add_CATEGORY_NAME_node(0))) break;
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

/*	------------------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a c t i v a t e   f i l t e r 	*/
/*	------------------------------------------------------------------------------------------------------	*/
private void activate_CATEGORY_NAME_filter_on_field(
	struct occi_category * cptr,void * target_void, char * field_name)
{
	struct CATEGORY_NAME_occi_filter * target;
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
private void set_CATEGORY_NAME_field(
	struct occi_category * cptr,void * target_void, char * field_name, char * value)
{
	struct CATEGORY_NAME * pptr;
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
private void set_CATEGORY_NAME_filter_on_field(
	struct occi_category * category, void * target_void, char * field_name, char * value)
{
	struct CATEGORY_NAME_occi_filter *filter = target_void;
	set_CATEGORY_NAME_field(category, filter->attributes, field_name, value);
	activate_CATEGORY_NAME_filter_on_field(category, filter, field_name);
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   i n f o 	*/
/*	--------------------------------------------------	*/
private int filter_CATEGORY_NAME_info(
	struct CATEGORY_NAME_occi_filter *filter,
	struct occi_category * optr,
	struct rest_request  * rptr,
	struct rest_response * aptr) {
	*filter = (const struct CATEGORY_NAME_occi_filter) {0};
	if (!( filter->attributes = allocate_CATEGORY_NAME()))
		return 0;
	else if (!( occi_process_atributs(optr, rptr, aptr, filter, set_CATEGORY_NAME_filter_on_field) )) {
		liberate_CATEGORY_NAME(filter->attributes);
		return 0;
	}
	else	return( 1 );
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

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   r e s p o n s e 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * CATEGORY_NAME_occi_response(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,
	struct CATEGORY_NAME * pptr)
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
private struct rest_response * CATEGORY_NAME_get_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	struct CATEGORY_NAME * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_CATEGORY_NAME_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->retrieve)) (*iptr->retrieve)(optr,nptr,rptr);
	autosave_CATEGORY_NAME_nodes();
	return( CATEGORY_NAME_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   l i n k 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * CATEGORY_NAME_post_link(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	struct CATEGORY_NAME * pptr;
	char * reqhost;
	if (!( nptr = locate_CATEGORY_NAME_node(id)))
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
private struct rest_response * CATEGORY_NAME_post_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	struct CATEGORY_NAME * pptr;
	char * reqhost;
	int    reqport=0;
	iptr = optr->callback;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else reqport = rptr->port;
	if (!( nptr = add_CATEGORY_NAME_node(1)))
		return( rest_html_response( aptr, 500, "Server Failure") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (!( occi_process_atributs( optr, rptr,aptr, pptr, set_CATEGORY_NAME_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->create)) (*iptr->create)(optr,nptr,rptr);
	autosave_CATEGORY_NAME_nodes();
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
private struct rest_response * CATEGORY_NAME_put_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	struct CATEGORY_NAME * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_CATEGORY_NAME_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (!( occi_process_atributs(optr,rptr,aptr, pptr, set_CATEGORY_NAME_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->update)) (*iptr->update)(optr,nptr,rptr);
	autosave_CATEGORY_NAME_nodes();
	return( CATEGORY_NAME_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d   i t e m 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * CATEGORY_NAME_head_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	struct CATEGORY_NAME * pptr;
	if (!( nptr = locate_CATEGORY_NAME_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e   i t e m 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * CATEGORY_NAME_delete_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * nptr;
	struct CATEGORY_NAME * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_CATEGORY_NAME_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->delete)) (*iptr->delete)(optr,nptr,rptr);
	drop_CATEGORY_NAME_node( nptr );
	autosave_CATEGORY_NAME_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t   l i s t 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * CATEGORY_NAME_get_list(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node * sptr;
	struct CATEGORY_NAME * pptr;
	struct CATEGORY_NAME_occi_filter filter;
	char * reqhost;
	int reqport=0;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else reqport = rptr->port;
	if (!( filter_CATEGORY_NAME_info(&filter, optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	for ( sptr = CATEGORY_NAME_first;
		sptr != (struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node *) 0;
		sptr = sptr->next ) {
		if (!( pptr = sptr->contents ))
			continue;
		if (!( pass_CATEGORY_NAME_filter( pptr, &filter) ))
			continue;
		sprintf(cptr->buffer,"%s:%u%s%s",reqhost,reqport,optr->location,pptr->id);
		if (!( hptr = rest_response_header( aptr, "X-OCCI-Location",cptr->buffer) )) {
			liberate_CATEGORY_NAME(filter.attributes);
			return( rest_html_response( aptr, 500, "Server Failure" ) );
		}
	}
	liberate_CATEGORY_NAME(filter.attributes);
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	--------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e   a l l 	*/
/*	--------------------------------------------------------------------------------------------	*/
private struct rest_response * CATEGORY_NAME_delete_all(
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
	struct CATEGORY_NAME * pptr;
	struct CATEGORY_NAME_occi_filter filter;
	iptr = optr->callback;
	if (!( filter_CATEGORY_NAME_info(&filter, optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	nptr=CATEGORY_NAME_first;
	while (nptr != (struct occi_
[[[cog t.node_type()]]]
[[[end]]] 
_node *) 0) {
		if ((!( pptr = nptr->contents ))
		||  (!( pass_CATEGORY_NAME_filter( pptr, &filter) ))) {
			nptr = nptr->next;
			continue;
			}
		else	{
			if (( iptr ) && (iptr->delete)) { (*iptr->delete)(optr,nptr,rptr); }
			sptr = nptr->next;
			drop_CATEGORY_NAME_node( nptr );
			nptr = sptr;
		}
	}
	liberate_CATEGORY_NAME(filter.attributes);
	autosave_CATEGORY_NAME_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_CATEGORY_NAME_get(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( CATEGORY_NAME_get_list( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( CATEGORY_NAME_get_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_CATEGORY_NAME_head(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( CATEGORY_NAME_head_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_CATEGORY_NAME_post(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( CATEGORY_NAME_post_item( optr, cptr, rptr, aptr ) );
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
private struct rest_response * occi_CATEGORY_NAME_put(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( CATEGORY_NAME_put_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e 	*/
/*	------------------------------------------------------------------------------------	*/
private struct rest_response * occi_CATEGORY_NAME_delete(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( CATEGORY_NAME_delete_all( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( CATEGORY_NAME_delete_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   r e d i r e c t i o n 	*/
/*	--------------------------------------------------------------------------------	*/
private void	redirect_occi_CATEGORY_NAME_mt( struct rest_interface * iptr )
{
	iptr->get = occi_CATEGORY_NAME_get;
	iptr->post = occi_CATEGORY_NAME_post;
	iptr->put = occi_CATEGORY_NAME_put;
	iptr->delete = occi_CATEGORY_NAME_delete;
	iptr->head = occi_CATEGORY_NAME_head;
	return;
}

[[[cog t.crud_delete_action()]]]
[[[end]]]

/*	------------------------------------------	*/
/*	o c c i   c a t e g o r y   b u i l d e r 	*/
/*	------------------------------------------	*/
/* occi category rest instance builder for : occi_CATEGORY_NAME */
public struct occi_category * occi_CATEGORY_NAME_builder(char * a,char * b) {
	char * c="http://scheme.compatibleone.fr/scheme/compatible#";
	char * d="
[[[cog t.node_type()]]]
[[[end]]] 
";
	char * e="http://scheme.ogf.org/occi/
[[[cog t.scheme_kind()]]]
[[[end]]]
#";
	char * f="CompatibleOne OCCI resource CATEGORY_NAME";
	struct occi_category * optr;
	if (!( optr = occi_create_category(a,b,c,d,e,f) )) { return(optr); }
	else {
		redirect_occi_CATEGORY_NAME_mt(optr->interface);
		[[[cog t.occi_builder()]]]
		[[[end]]]		
		autoload_CATEGORY_NAME_nodes();
		return(optr);
	}

}

/*	------------------------------------------------------------	*/
/*	
[[[cog t.split_category_name()]]]
[[[end]]] 
_ o c c i _ h e a d e r s 	*/
/*	------------------------------------------------------------	*/
public struct rest_header *  CATEGORY_NAME_occi_headers(struct CATEGORY_NAME * sptr)
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
	sprintf(buffer,"CATEGORY_NAME; scheme='http://scheme.compatibleone.fr/scheme/compatible#'; class='
[[[cog t.node_type()]]]
[[[end]]] 
';\r\n");
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	[[[cog t.occi_headers()]]]
	[[[end]]]
	return(first);

}

#endif	/* _occiFILENAME_ROOT_c_ */
