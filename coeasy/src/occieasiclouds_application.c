/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>     */
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License"); 	*/
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at 				*/
/*  									*/
/*  http://www.apache.org/licenses/LICENSE-2.0 				*/
/*  									*/
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 	*/
/* implied. 								*/
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License. 					*/
/* -------------------------------------------------------------------- */

/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _occieasiclouds_application_c_
#define _occieasiclouds_application_c_

#include "easiclouds_application.h"

/*	------------------------------------------------------	*/
/*	o c c i _ e a s i c l o u d s _ a p p l i c a t i o n 	*/
/*	------------------------------------------------------	*/

/*	--------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   m a n a g e m e n t   s t r u c t u r e 	*/
/*	--------------------------------------------------------------------	*/
struct easiclouds_application * allocate_easiclouds_application();
struct easiclouds_application * liberate_easiclouds_application(struct easiclouds_application * optr);
private pthread_mutex_t list_easiclouds_application_control=PTHREAD_MUTEX_INITIALIZER;
private struct occi_kind_node * easiclouds_application_first = (struct occi_kind_node *) 0;
private struct occi_kind_node * easiclouds_application_last  = (struct occi_kind_node *) 0;
public struct  occi_kind_node * occi_first_easiclouds_application_node() { return( easiclouds_application_first ); }
public struct  occi_kind_node * occi_last_easiclouds_application_node() { return( easiclouds_application_last ); }

/*	----------------------------------------------	*/
/*	o c c i   c a t e g o r y   d r o p   n o d e 	*/
/*	----------------------------------------------	*/
private struct occi_kind_node * ll_drop_easiclouds_application_node(struct occi_kind_node * nptr) {
	if ( nptr ) {
	if (!( nptr->previous ))
		easiclouds_application_first = nptr->next;
	else	nptr->previous->next = nptr->next;
	if (!( nptr->next ))
		easiclouds_application_last = nptr->previous;
	else	nptr->next->previous = nptr->previous;
		liberate_occi_kind_node( nptr );
		}
	return((struct occi_kind_node *)0);
}
private struct occi_kind_node * drop_easiclouds_application_node(struct occi_kind_node * nptr) {
	pthread_mutex_lock( &list_easiclouds_application_control );
	nptr = ll_drop_easiclouds_application_node( nptr );
	pthread_mutex_unlock( &list_easiclouds_application_control );
	return(nptr);
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   l o c a t e   n o d e 	*/
/*	--------------------------------------------------	*/
private struct occi_kind_node * ll_locate_easiclouds_application_node(char * id) {
	struct occi_kind_node * nptr;
	struct easiclouds_application * pptr;
	for ( nptr = easiclouds_application_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		else if (!( pptr->id )) continue;
		else if (!( strcmp(pptr->id,id) )) break;
		}
	return( nptr );
}
private struct occi_kind_node * locate_easiclouds_application_node(char * id) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_easiclouds_application_control );
	nptr = ll_locate_easiclouds_application_node(id);
	pthread_mutex_unlock( &list_easiclouds_application_control );
	return( nptr );
}

/*	--------------------------------------------	*/
/*	o c c i   c a t e g o r y   a d d   n o d e 	*/
/*	--------------------------------------------	*/
private struct occi_kind_node * ll_add_easiclouds_application_node(int mode) {
	struct occi_kind_node * nptr;
	struct easiclouds_application * pptr;
	if (!( nptr = allocate_occi_kind_node() ))
		return( nptr );
	else	{
		if (!( nptr->contents = allocate_easiclouds_application()))
			return( liberate_occi_kind_node(nptr) );
		if (!( pptr = nptr->contents ))
			return( liberate_occi_kind_node(nptr) );
		else if (( mode != 0 ) && (!( pptr->id = occi_allocate_uuid())))
			return( liberate_occi_kind_node(nptr) );
		else	{
			if (!( nptr->previous = easiclouds_application_last ))
				easiclouds_application_first = nptr;
			else	nptr->previous->next = nptr;
			easiclouds_application_last = nptr;
			return( nptr );
			}
		}
}
private struct occi_kind_node * add_easiclouds_application_node(int mode) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_easiclouds_application_control );
	nptr = ll_add_easiclouds_application_node( mode );
	pthread_mutex_unlock( &list_easiclouds_application_control );
	return(nptr);
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   l o a d 	*/
/*	------------------------------------------------------------------------------------------	*/
private char*autosave_easiclouds_application_name="easiclouds_application.xml";
private void autoload_easiclouds_application_nodes() {
	char * fn=autosave_easiclouds_application_name;	struct occi_kind_node * nptr;
	struct easiclouds_application * pptr;
	struct xml_element * document;
	struct xml_element * eptr;
	struct xml_element * vptr;
	struct xml_atribut  * aptr;
	if (!( document = document_parse_file(fn)))
		return;
	if ((eptr = document_element(document,"easiclouds_applications")) != (struct xml_element *) 0) {
		for (vptr=eptr->first; vptr != (struct xml_element *) 0; vptr=vptr->next) {
			if (!( vptr->name )) continue;
			else if ( strcmp( vptr->name, "easiclouds_application" ) ) continue;
			else if (!( nptr = add_easiclouds_application_node(0))) break;
			else if (!( pptr = nptr->contents )) break;
			if ((aptr = document_atribut( vptr, "id" )) != (struct xml_atribut *) 0)
				pptr->id = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "name" )) != (struct xml_atribut *) 0)
				pptr->name = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "easiclouds_postconf" )) != (struct xml_atribut *) 0)
				pptr->easiclouds_postconf = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "state" )) != (struct xml_atribut *) 0)
				pptr->state = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "nodes" )) != (struct xml_atribut *) 0)
				pptr->nodes = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "links" )) != (struct xml_atribut *) 0)
				pptr->links = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "extras" )) != (struct xml_atribut *) 0)
				pptr->extras = document_atribut_value(aptr);
			}
		}
	document = document_drop( document );
	return;
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   s a v e 	*/
/*	------------------------------------------------------------------------------------------	*/
public  void set_autosave_easiclouds_application_name(char * fn) {
	autosave_easiclouds_application_name = fn;	return;
}
public  void autosave_easiclouds_application_nodes() {
	char * fn=autosave_easiclouds_application_name;	struct occi_kind_node * nptr;
	struct easiclouds_application * pptr;
	FILE * h;
	pthread_mutex_lock( &list_easiclouds_application_control );
	if (( h = fopen(fn,"w")) != (FILE *) 0) {
	fprintf(h,"<easiclouds_applications>\n");
	for ( nptr = easiclouds_application_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		fprintf(h,"<easiclouds_application\n");
		fprintf(h," id=%c",0x0022);
		fprintf(h,"%s",(pptr->id?pptr->id:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," name=%c",0x0022);
		fprintf(h,"%s",(pptr->name?pptr->name:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," easiclouds_postconf=%c",0x0022);
		fprintf(h,"%s",(pptr->easiclouds_postconf?pptr->easiclouds_postconf:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," state=%c",0x0022);
		fprintf(h,"%u",pptr->state);
		fprintf(h,"%c",0x0022);
		fprintf(h," nodes=%c",0x0022);
		fprintf(h,"%u",pptr->nodes);
		fprintf(h,"%c",0x0022);
		fprintf(h," links=%c",0x0022);
		fprintf(h,"%u",pptr->links);
		fprintf(h,"%c",0x0022);
		fprintf(h," extras=%c",0x0022);
		fprintf(h,"%u",pptr->extras);
		fprintf(h,"%c",0x0022);
		fprintf(h," />\n");
		}
	fprintf(h,"</easiclouds_applications>\n");
	fclose(h);
	}
	pthread_mutex_unlock( &list_easiclouds_application_control );
	return;
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   s e t   f i e l d 	*/
/*	------------------------------------------------------------------------------------------	*/
private void set_easiclouds_application_field(
	struct occi_category * cptr,void * optr, char * nptr, char * vptr)
{
	struct easiclouds_application * pptr;
	char prefix[1024];
	if (!( pptr = optr )) return;
	sprintf(prefix,"%s.%s.",cptr->domain,cptr->id);
	if (!( strncmp( nptr, prefix, strlen(prefix) ) )) {
		nptr += strlen(prefix);
		if (!( strcmp( nptr, "name" ) ))
			pptr->name = allocate_string(vptr);
		if (!( strcmp( nptr, "easiclouds_postconf" ) ))
			pptr->easiclouds_postconf = allocate_string(vptr);
		if (!( strcmp( nptr, "state" ) ))
			pptr->state = atoi(vptr);
		if (!( strcmp( nptr, "nodes" ) ))
			pptr->nodes = atoi(vptr);
		if (!( strcmp( nptr, "links" ) ))
			pptr->links = atoi(vptr);
		if (!( strcmp( nptr, "extras" ) ))
			pptr->extras = atoi(vptr);
		}
	return;
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   i n f o 	*/
/*	--------------------------------------------------	*/
private struct easiclouds_application * filter_easiclouds_application_info(
	struct occi_category * optr,
	struct rest_request  * rptr,
	struct rest_response * aptr) {
	struct easiclouds_application * pptr;
		if (!( pptr = allocate_easiclouds_application()))
		return( pptr );
	else if (!( occi_process_atributs(optr, rptr, aptr, pptr, set_easiclouds_application_field) ))
		return( liberate_easiclouds_application(pptr));
	else	return( pptr );
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   p a s s 	*/
/*	--------------------------------------------------	*/
private int pass_easiclouds_application_filter(
	struct easiclouds_application * pptr,struct easiclouds_application * fptr) {
	if (( fptr->id )
	&&  (strlen( fptr->id ) != 0)) {
		if (!( pptr->id ))
			return(0);
		else if ( strcmp(pptr->id,fptr->id) != 0)
			return(0);
		}
	if (( fptr->name )
	&&  (strlen( fptr->name ) != 0)) {
		if (!( pptr->name ))
			return(0);
		else if ( strcmp(pptr->name,fptr->name) != 0)
			return(0);
		}
	if (( fptr->easiclouds_postconf )
	&&  (strlen( fptr->easiclouds_postconf ) != 0)) {
		if (!( pptr->easiclouds_postconf ))
			return(0);
		else if ( strcmp(pptr->easiclouds_postconf,fptr->easiclouds_postconf) != 0)
			return(0);
		}
	if (( fptr->state ) && ( pptr->state != fptr->state )) return(0);
	if (( fptr->nodes ) && ( pptr->nodes != fptr->nodes )) return(0);
	if (( fptr->links ) && ( pptr->links != fptr->links )) return(0);
	if (( fptr->extras ) && ( pptr->extras != fptr->extras )) return(0);
	return(1);
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   r e s p o n s e 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * easiclouds_application_occi_response(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,
	struct easiclouds_application * pptr)
{
	struct rest_header * hptr;
	sprintf(cptr->buffer,"occi.core.id=%c%s%c",0x0022,pptr->id,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.name=%c%s%c",optr->domain,optr->id,0x0022,pptr->name,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.easiclouds_postconf=%c%s%c",optr->domain,optr->id,0x0022,pptr->easiclouds_postconf,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.state=%c%u%c",optr->domain,optr->id,0x0022,pptr->state,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.nodes=%c%u%c",optr->domain,optr->id,0x0022,pptr->nodes,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.links=%c%u%c",optr->domain,optr->id,0x0022,pptr->links,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.extras=%c%u%c",optr->domain,optr->id,0x0022,pptr->extras,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	if ( occi_render_links( aptr, pptr->id ) != 0)
		return( rest_html_response( aptr, 500, "Server Link Failure" ) );
	else	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t   i t e m 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * easiclouds_application_get_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct easiclouds_application * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_easiclouds_application_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->retrieve)) (*iptr->retrieve)(optr,nptr,rptr);
	autosave_easiclouds_application_nodes();
	return( easiclouds_application_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   l i n k 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * easiclouds_application_post_link(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct easiclouds_application * pptr;
	char * reqhost;
	if (!( nptr = locate_easiclouds_application_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   m i x i n 	*/
/*	--------------------------------------------------------------------------------------------	*/
private struct rest_response * easiclouds_application_post_mixin(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct easiclouds_application * pptr;
	char * reqhost;
	if (!( nptr = locate_easiclouds_application_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request"));
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   a c t i o n 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * easiclouds_application_post_action(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_action * fptr;
	struct occi_kind_node * nptr;
	struct easiclouds_application * pptr;
	char * reqhost;
	char * mptr;
	if (!( nptr = locate_easiclouds_application_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	mptr = (rptr->parameters+strlen("action="));
	for ( fptr=optr->firstact;
		fptr != (struct occi_action *) 0;
		fptr = fptr->next )
		if (!( strncmp( mptr, fptr->name, strlen( fptr->name )) ))
			return( occi_invoke_action(fptr,optr,cptr,rptr,aptr,pptr) );
	return( rest_html_response( aptr, 400, "Incorrect Action Request"));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   i t e m 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * easiclouds_application_post_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct easiclouds_application * pptr;
	char * reqhost;
	int    reqport=0;
	iptr = optr->callback;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else reqport = rptr->port;
	if (!( nptr = add_easiclouds_application_node(1)))
		return( rest_html_response( aptr, 500, "Server Failure") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (!( occi_process_atributs( optr, rptr,aptr, pptr, set_easiclouds_application_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->create)) (*iptr->create)(optr,nptr,rptr);
	autosave_easiclouds_application_nodes();
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
private struct rest_response * easiclouds_application_put_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct easiclouds_application * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_easiclouds_application_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (!( occi_process_atributs(optr,rptr,aptr, pptr, set_easiclouds_application_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->update)) (*iptr->update)(optr,nptr,rptr);
	autosave_easiclouds_application_nodes();
	return( easiclouds_application_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d   i t e m 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * easiclouds_application_head_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_kind_node * nptr;
	struct easiclouds_application * pptr;
	if (!( nptr = locate_easiclouds_application_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e   i t e m 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * easiclouds_application_delete_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct easiclouds_application * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_easiclouds_application_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->delete)) (*iptr->delete)(optr,nptr,rptr);
	drop_easiclouds_application_node( nptr );
	autosave_easiclouds_application_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t   l i s t 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * easiclouds_application_get_list(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_kind_node * sptr;
	struct easiclouds_application * pptr;
	struct easiclouds_application * fptr;
	char * reqhost;
	int reqport=0;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else reqport = rptr->port;
	if (!( fptr = filter_easiclouds_application_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	for ( sptr = easiclouds_application_first;
		sptr != (struct occi_kind_node *) 0;
		sptr = sptr->next ) {
		if (!( pptr = sptr->contents ))
			continue;
		if (!( pass_easiclouds_application_filter( pptr, fptr ) ))
			continue;
		sprintf(cptr->buffer,"%s:%u%s%s",reqhost,reqport,optr->location,pptr->id);
		if (!( hptr = rest_response_header( aptr, "X-OCCI-Location",cptr->buffer) ))
			return( rest_html_response( aptr, 500, "Server Failure" ) );
		}
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	--------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e   a l l 	*/
/*	--------------------------------------------------------------------------------------------	*/
private struct rest_response * easiclouds_application_delete_all(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct occi_kind_node * sptr;
	struct easiclouds_application * pptr;
	struct easiclouds_application * fptr;
	iptr = optr->callback;
	if (!( fptr = filter_easiclouds_application_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	nptr=easiclouds_application_first;
	while (nptr != (struct occi_kind_node *) 0) {
		if ((!( pptr = nptr->contents ))
		||  (!( pass_easiclouds_application_filter( pptr, fptr ) ))) {
			nptr = nptr->next;
			continue;
			}
		else	{
			if (( iptr ) && (iptr->delete)) { (*iptr->delete)(optr,nptr,rptr); }
			sptr = nptr->next;
			drop_easiclouds_application_node( nptr );
			nptr = sptr;
			}
		}
	autosave_easiclouds_application_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_easiclouds_application_get(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( easiclouds_application_get_list( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( easiclouds_application_get_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_easiclouds_application_head(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( easiclouds_application_head_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_easiclouds_application_post(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( easiclouds_application_post_item( optr, cptr, rptr, aptr ) );
	else if ( strncmp( rptr->object, optr->location,strlen(optr->location)) != 0)
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( rptr->parameters ))
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( strncmp( rptr->parameters, "action=", strlen("action=")) ))
		return( easiclouds_application_post_action( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "mixin=", strlen("mixin=")) ))
		return( easiclouds_application_post_mixin( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "link=", strlen("link=")) ))
		return( easiclouds_application_post_link( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p u t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_easiclouds_application_put(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( easiclouds_application_put_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e 	*/
/*	------------------------------------------------------------------------------------	*/
private struct rest_response * occi_easiclouds_application_delete(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( easiclouds_application_delete_all( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( easiclouds_application_delete_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   r e d i r e c t i o n 	*/
/*	--------------------------------------------------------------------------------	*/
private void	redirect_occi_easiclouds_application_mt( struct rest_interface * iptr )
{
	iptr->get = occi_easiclouds_application_get;
	iptr->post = occi_easiclouds_application_post;
	iptr->put = occi_easiclouds_application_put;
	iptr->delete = occi_easiclouds_application_delete;
	iptr->head = occi_easiclouds_application_head;
	return;
}

/*	------------------------------------	*/
/*	c r u d   d e l e t e   a c t i o n 	*/
/*	------------------------------------	*/
private struct rest_response * delete_action_easiclouds_application(struct occi_category * optr, 
struct rest_client * cptr,  
struct rest_request * rptr,  
struct rest_response * aptr,  
void * vptr )
{
	aptr = liberate_rest_response( aptr );
	return( occi_easiclouds_application_delete(optr,cptr,rptr));
}

/*	------------------------------------------	*/
/*	o c c i   c a t e g o r y   b u i l d e r 	*/
/*	------------------------------------------	*/
/* occi category rest instance builder for : occi_easiclouds_application */
public struct occi_category * occi_easiclouds_application_builder(char * a,char * b) {
	char * c="http://scheme.compatibleone.fr/scheme/compatible#";
	char * d="kind";
	char * e="http://scheme.ogf.org/occi/resource#";
	char * f="CompatibleOne OCCI resource easiclouds_application";
	struct occi_category * optr;
	if (!( optr = occi_create_category(a,b,c,d,e,f) )) { return(optr); }
	else {
		redirect_occi_easiclouds_application_mt(optr->interface);
		if (!( optr = occi_add_attribute(optr, "name",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "easiclouds_postconf",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "state",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "nodes",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "links",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "extras",0,0) ))
			return(optr);
		if (!( optr = occi_add_action( optr,"DELETE","",delete_action_easiclouds_application)))
			return( optr );
		autoload_easiclouds_application_nodes();
		return(optr);
	}

}

/*	----------------------------------------------------------------------	*/
/*	e a s i c l o u d s _ a p p l i c a t i o n _ o c c i _ h e a d e r s 	*/
/*	----------------------------------------------------------------------	*/
public struct rest_header *  easiclouds_application_occi_headers(struct easiclouds_application * sptr)
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
	sprintf(buffer,"easiclouds_application; scheme='http://scheme.compatibleone.fr/scheme/compatible#'; class='kind';\r\n");
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
	sprintf(buffer,"occi.easiclouds_application.name='%s'\r\n",(sptr->name?sptr->name:""));
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
	sprintf(buffer,"occi.easiclouds_application.easiclouds_postconf='%s'\r\n",(sptr->easiclouds_postconf?sptr->easiclouds_postconf:""));
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
	sprintf(buffer,"occi.easiclouds_application.state='%u'\r\n",sptr->state);
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
	sprintf(buffer,"occi.easiclouds_application.nodes='%u'\r\n",sptr->nodes);
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
	sprintf(buffer,"occi.easiclouds_application.links='%u'\r\n",sptr->links);
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
	sprintf(buffer,"occi.easiclouds_application.extras='%u'\r\n",sptr->extras);
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	return(first);

}

#endif	/* _occieasiclouds_application_c_ */
