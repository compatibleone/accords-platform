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
#ifndef _occilinkgw_c_
#define _occilinkgw_c_

#include "linkgw.h"

/*	----------------------	*/
/*	o c c i _ l i n k g w 	*/
/*	----------------------	*/

/*	--------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   m a n a g e m e n t   s t r u c t u r e 	*/
/*	--------------------------------------------------------------------	*/
struct linkgw * allocate_linkgw();
struct linkgw * liberate_linkgw(struct linkgw * optr);
private pthread_mutex_t list_linkgw_control=PTHREAD_MUTEX_INITIALIZER;
private struct occi_kind_node * linkgw_first = (struct occi_kind_node *) 0;
private struct occi_kind_node * linkgw_last  = (struct occi_kind_node *) 0;
public struct  occi_kind_node * occi_first_linkgw_node() { return( linkgw_first ); }
public struct  occi_kind_node * occi_last_linkgw_node() { return( linkgw_last ); }

/*	----------------------------------------------	*/
/*	o c c i   c a t e g o r y   d r o p   n o d e 	*/
/*	----------------------------------------------	*/
private struct occi_kind_node * ll_drop_linkgw_node(struct occi_kind_node * nptr) {
	if ( nptr ) {
	if (!( nptr->previous ))
		linkgw_first = nptr->next;
	else	nptr->previous->next = nptr->next;
	if (!( nptr->next ))
		linkgw_last = nptr->previous;
	else	nptr->next->previous = nptr->previous;
		liberate_occi_kind_node( nptr );
		}
	return((struct occi_kind_node *)0);
}
private struct occi_kind_node * drop_linkgw_node(struct occi_kind_node * nptr) {
	pthread_mutex_lock( &list_linkgw_control );
	nptr = ll_drop_linkgw_node( nptr );
	pthread_mutex_unlock( &list_linkgw_control );
	return(nptr);
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   l o c a t e   n o d e 	*/
/*	--------------------------------------------------	*/
private struct occi_kind_node * ll_locate_linkgw_node(char * id) {
	struct occi_kind_node * nptr;
	struct linkgw * pptr;
	for ( nptr = linkgw_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		else if (!( pptr->id )) continue;
		else if (!( strcmp(pptr->id,id) )) break;
		}
	return( nptr );
}
private struct occi_kind_node * locate_linkgw_node(char * id) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_linkgw_control );
	nptr = ll_locate_linkgw_node(id);
	pthread_mutex_unlock( &list_linkgw_control );
	return( nptr );
}

/*	--------------------------------------------	*/
/*	o c c i   c a t e g o r y   a d d   n o d e 	*/
/*	--------------------------------------------	*/
private struct occi_kind_node * ll_add_linkgw_node(int mode) {
	struct occi_kind_node * nptr;
	struct linkgw * pptr;
	if (!( nptr = allocate_occi_kind_node() ))
		return( nptr );
	else	{
		if (!( nptr->contents = allocate_linkgw()))
			return( liberate_occi_kind_node(nptr) );
		if (!( pptr = nptr->contents ))
			return( liberate_occi_kind_node(nptr) );
		else if (( mode != 0 ) && (!( pptr->id = occi_allocate_uuid())))
			return( liberate_occi_kind_node(nptr) );
		else	{
			if (!( nptr->previous = linkgw_last ))
				linkgw_first = nptr;
			else	nptr->previous->next = nptr;
			linkgw_last = nptr;
			return( nptr );
			}
		}
}
private struct occi_kind_node * add_linkgw_node(int mode) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_linkgw_control );
	nptr = ll_add_linkgw_node( mode );
	pthread_mutex_unlock( &list_linkgw_control );
	return(nptr);
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   l o a d 	*/
/*	------------------------------------------------------------------------------------------	*/
private char*autosave_linkgw_name="linkgw.xml";
private void autoload_linkgw_nodes() {
	char * fn=autosave_linkgw_name;	struct occi_kind_node * nptr;
	struct linkgw * pptr;
	struct xml_element * document;
	struct xml_element * eptr;
	struct xml_element * vptr;
	struct xml_atribut  * aptr;
	if (!( document = document_parse_file(fn)))
		return;
	if ((eptr = document_element(document,"linkgws")) != (struct xml_element *) 0) {
		for (vptr=eptr->first; vptr != (struct xml_element *) 0; vptr=vptr->next) {
			if (!( vptr->name )) continue;
			else if ( strcmp( vptr->name, "linkgw" ) ) continue;
			else if (!( nptr = add_linkgw_node(0))) break;
			else if (!( pptr = nptr->contents )) break;
			if ((aptr = document_atribut( vptr, "id" )) != (struct xml_atribut *) 0)
				pptr->id = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "name" )) != (struct xml_atribut *) 0)
				pptr->name = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "intercloudGW" )) != (struct xml_atribut *) 0)
				pptr->intercloudGW = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "account" )) != (struct xml_atribut *) 0)
				pptr->account = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "gwsrc" )) != (struct xml_atribut *) 0)
				pptr->gwsrc = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "gwdst" )) != (struct xml_atribut *) 0)
				pptr->gwdst = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "tunnelproto" )) != (struct xml_atribut *) 0)
				pptr->tunnelproto = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "addressgresrc" )) != (struct xml_atribut *) 0)
				pptr->addressgresrc = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "addressgredst" )) != (struct xml_atribut *) 0)
				pptr->addressgredst = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "prefix" )) != (struct xml_atribut *) 0)
				pptr->prefix = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "authenticationkey" )) != (struct xml_atribut *) 0)
				pptr->authenticationkey = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "endpointsrc" )) != (struct xml_atribut *) 0)
				pptr->endpointsrc = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "endpointdst" )) != (struct xml_atribut *) 0)
				pptr->endpointdst = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "state" )) != (struct xml_atribut *) 0)
				pptr->state = document_atribut_string(aptr);
			}
		}
	document = document_drop( document );
	return;
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   s a v e 	*/
/*	------------------------------------------------------------------------------------------	*/
public  void set_autosave_linkgw_name(char * fn) {
	autosave_linkgw_name = fn;	return;
}
public  void autosave_linkgw_nodes() {
	char * fn=autosave_linkgw_name;	struct occi_kind_node * nptr;
	struct linkgw * pptr;
	FILE * h;
	pthread_mutex_lock( &list_linkgw_control );
	if (( h = fopen(fn,"w")) != (FILE *) 0) {
	fprintf(h,"<linkgws>\n");
	for ( nptr = linkgw_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		fprintf(h,"<linkgw\n");
		fprintf(h," id=%c",0x0022);
		fprintf(h,"%s",(pptr->id?pptr->id:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," name=%c",0x0022);
		fprintf(h,"%s",(pptr->name?pptr->name:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," intercloudGW=%c",0x0022);
		fprintf(h,"%s",(pptr->intercloudGW?pptr->intercloudGW:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," account=%c",0x0022);
		fprintf(h,"%s",(pptr->account?pptr->account:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," gwsrc=%c",0x0022);
		fprintf(h,"%s",(pptr->gwsrc?pptr->gwsrc:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," gwdst=%c",0x0022);
		fprintf(h,"%s",(pptr->gwdst?pptr->gwdst:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," tunnelproto=%c",0x0022);
		fprintf(h,"%s",(pptr->tunnelproto?pptr->tunnelproto:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," addressgresrc=%c",0x0022);
		fprintf(h,"%s",(pptr->addressgresrc?pptr->addressgresrc:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," addressgredst=%c",0x0022);
		fprintf(h,"%s",(pptr->addressgredst?pptr->addressgredst:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," prefix=%c",0x0022);
		fprintf(h,"%s",(pptr->prefix?pptr->prefix:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," authenticationkey=%c",0x0022);
		fprintf(h,"%s",(pptr->authenticationkey?pptr->authenticationkey:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," endpointsrc=%c",0x0022);
		fprintf(h,"%s",(pptr->endpointsrc?pptr->endpointsrc:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," endpointdst=%c",0x0022);
		fprintf(h,"%s",(pptr->endpointdst?pptr->endpointdst:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," state=%c",0x0022);
		fprintf(h,"%s",(pptr->state?pptr->state:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," />\n");
		}
	fprintf(h,"</linkgws>\n");
	fclose(h);
	}
	pthread_mutex_unlock( &list_linkgw_control );
	return;
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   s e t   f i e l d 	*/
/*	------------------------------------------------------------------------------------------	*/
private void set_linkgw_field(
	struct occi_category * cptr,void * optr, char * nptr, char * vptr)
{
	struct linkgw * pptr;
	char prefix[1024];
	if (!( pptr = optr )) return;
	sprintf(prefix,"%s.%s.",cptr->domain,cptr->id);
	if (!( strncmp( nptr, prefix, strlen(prefix) ) )) {
		nptr += strlen(prefix);
		if (!( strcmp( nptr, "name" ) ))
			pptr->name = allocate_string(vptr);
		if (!( strcmp( nptr, "intercloudGW" ) ))
			pptr->intercloudGW = allocate_string(vptr);
		if (!( strcmp( nptr, "account" ) ))
			pptr->account = allocate_string(vptr);
		if (!( strcmp( nptr, "gwsrc" ) ))
			pptr->gwsrc = allocate_string(vptr);
		if (!( strcmp( nptr, "gwdst" ) ))
			pptr->gwdst = allocate_string(vptr);
		if (!( strcmp( nptr, "tunnelproto" ) ))
			pptr->tunnelproto = allocate_string(vptr);
		if (!( strcmp( nptr, "addressgresrc" ) ))
			pptr->addressgresrc = allocate_string(vptr);
		if (!( strcmp( nptr, "addressgredst" ) ))
			pptr->addressgredst = allocate_string(vptr);
		if (!( strcmp( nptr, "prefix" ) ))
			pptr->prefix = allocate_string(vptr);
		if (!( strcmp( nptr, "authenticationkey" ) ))
			pptr->authenticationkey = allocate_string(vptr);
		if (!( strcmp( nptr, "endpointsrc" ) ))
			pptr->endpointsrc = allocate_string(vptr);
		if (!( strcmp( nptr, "endpointdst" ) ))
			pptr->endpointdst = allocate_string(vptr);
		if (!( strcmp( nptr, "state" ) ))
			pptr->state = allocate_string(vptr);
		}
	return;
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   i n f o 	*/
/*	--------------------------------------------------	*/
private struct linkgw * filter_linkgw_info(
	struct occi_category * optr,
	struct rest_request  * rptr,
	struct rest_response * aptr) {
	struct linkgw * pptr;
		if (!( pptr = allocate_linkgw()))
		return( pptr );
	else if (!( occi_process_atributs(optr, rptr, aptr, pptr, set_linkgw_field) ))
		return( liberate_linkgw(pptr));
	else	return( pptr );
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   p a s s 	*/
/*	--------------------------------------------------	*/
private int pass_linkgw_filter(
	struct linkgw * pptr,struct linkgw * fptr) {
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
	if (( fptr->intercloudGW )
	&&  (strlen( fptr->intercloudGW ) != 0)) {
		if (!( pptr->intercloudGW ))
			return(0);
		else if ( strcmp(pptr->intercloudGW,fptr->intercloudGW) != 0)
			return(0);
		}
	if (( fptr->account )
	&&  (strlen( fptr->account ) != 0)) {
		if (!( pptr->account ))
			return(0);
		else if ( strcmp(pptr->account,fptr->account) != 0)
			return(0);
		}
	if (( fptr->gwsrc )
	&&  (strlen( fptr->gwsrc ) != 0)) {
		if (!( pptr->gwsrc ))
			return(0);
		else if ( strcmp(pptr->gwsrc,fptr->gwsrc) != 0)
			return(0);
		}
	if (( fptr->gwdst )
	&&  (strlen( fptr->gwdst ) != 0)) {
		if (!( pptr->gwdst ))
			return(0);
		else if ( strcmp(pptr->gwdst,fptr->gwdst) != 0)
			return(0);
		}
	if (( fptr->tunnelproto )
	&&  (strlen( fptr->tunnelproto ) != 0)) {
		if (!( pptr->tunnelproto ))
			return(0);
		else if ( strcmp(pptr->tunnelproto,fptr->tunnelproto) != 0)
			return(0);
		}
	if (( fptr->addressgresrc )
	&&  (strlen( fptr->addressgresrc ) != 0)) {
		if (!( pptr->addressgresrc ))
			return(0);
		else if ( strcmp(pptr->addressgresrc,fptr->addressgresrc) != 0)
			return(0);
		}
	if (( fptr->addressgredst )
	&&  (strlen( fptr->addressgredst ) != 0)) {
		if (!( pptr->addressgredst ))
			return(0);
		else if ( strcmp(pptr->addressgredst,fptr->addressgredst) != 0)
			return(0);
		}
	if (( fptr->prefix )
	&&  (strlen( fptr->prefix ) != 0)) {
		if (!( pptr->prefix ))
			return(0);
		else if ( strcmp(pptr->prefix,fptr->prefix) != 0)
			return(0);
		}
	if (( fptr->authenticationkey )
	&&  (strlen( fptr->authenticationkey ) != 0)) {
		if (!( pptr->authenticationkey ))
			return(0);
		else if ( strcmp(pptr->authenticationkey,fptr->authenticationkey) != 0)
			return(0);
		}
	if (( fptr->endpointsrc )
	&&  (strlen( fptr->endpointsrc ) != 0)) {
		if (!( pptr->endpointsrc ))
			return(0);
		else if ( strcmp(pptr->endpointsrc,fptr->endpointsrc) != 0)
			return(0);
		}
	if (( fptr->endpointdst )
	&&  (strlen( fptr->endpointdst ) != 0)) {
		if (!( pptr->endpointdst ))
			return(0);
		else if ( strcmp(pptr->endpointdst,fptr->endpointdst) != 0)
			return(0);
		}
	if (( fptr->state )
	&&  (strlen( fptr->state ) != 0)) {
		if (!( pptr->state ))
			return(0);
		else if ( strcmp(pptr->state,fptr->state) != 0)
			return(0);
		}
	return(1);
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   r e s p o n s e 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * linkgw_occi_response(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,
	struct linkgw * pptr)
{
	struct rest_header * hptr;
	sprintf(cptr->buffer,"occi.core.id=%c%s%c",0x0022,pptr->id,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.name=%c%s%c",optr->domain,optr->id,0x0022,pptr->name,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.intercloudGW=%c%s%c",optr->domain,optr->id,0x0022,pptr->intercloudGW,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.account=%c%s%c",optr->domain,optr->id,0x0022,pptr->account,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.gwsrc=%c%s%c",optr->domain,optr->id,0x0022,pptr->gwsrc,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.gwdst=%c%s%c",optr->domain,optr->id,0x0022,pptr->gwdst,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.tunnelproto=%c%s%c",optr->domain,optr->id,0x0022,pptr->tunnelproto,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.addressgresrc=%c%s%c",optr->domain,optr->id,0x0022,pptr->addressgresrc,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.addressgredst=%c%s%c",optr->domain,optr->id,0x0022,pptr->addressgredst,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.prefix=%c%s%c",optr->domain,optr->id,0x0022,pptr->prefix,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.authenticationkey=%c%s%c",optr->domain,optr->id,0x0022,pptr->authenticationkey,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.endpointsrc=%c%s%c",optr->domain,optr->id,0x0022,pptr->endpointsrc,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.endpointdst=%c%s%c",optr->domain,optr->id,0x0022,pptr->endpointdst,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.state=%c%s%c",optr->domain,optr->id,0x0022,pptr->state,0x0022);
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
private struct rest_response * linkgw_get_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct linkgw * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_linkgw_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->retrieve)) (*iptr->retrieve)(optr,nptr,rptr);
	autosave_linkgw_nodes();
	return( linkgw_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   l i n k 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * linkgw_post_link(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct linkgw * pptr;
	char * reqhost;
	if (!( nptr = locate_linkgw_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   m i x i n 	*/
/*	--------------------------------------------------------------------------------------------	*/
private struct rest_response * linkgw_post_mixin(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct linkgw * pptr;
	char * reqhost;
	if (!( nptr = locate_linkgw_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request"));
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   a c t i o n 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * linkgw_post_action(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_action * fptr;
	struct occi_kind_node * nptr;
	struct linkgw * pptr;
	char * reqhost;
	char * mptr;
	if (!( nptr = locate_linkgw_node(id)))
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
private struct rest_response * linkgw_post_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct linkgw * pptr;
	char * reqhost;
	int    reqport=0;
	iptr = optr->callback;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else reqport = rptr->port;
	if (!( nptr = add_linkgw_node(1)))
		return( rest_html_response( aptr, 500, "Server Failure") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (!( occi_process_atributs( optr, rptr,aptr, pptr, set_linkgw_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->create)) (*iptr->create)(optr,nptr,rptr);
	autosave_linkgw_nodes();
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
private struct rest_response * linkgw_put_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct linkgw * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_linkgw_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (!( occi_process_atributs(optr,rptr,aptr, pptr, set_linkgw_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->update)) (*iptr->update)(optr,nptr,rptr);
	autosave_linkgw_nodes();
	return( linkgw_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d   i t e m 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * linkgw_head_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_kind_node * nptr;
	struct linkgw * pptr;
	if (!( nptr = locate_linkgw_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e   i t e m 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * linkgw_delete_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct linkgw * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_linkgw_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->delete)) (*iptr->delete)(optr,nptr,rptr);
	drop_linkgw_node( nptr );
	autosave_linkgw_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t   l i s t 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * linkgw_get_list(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_kind_node * sptr;
	struct linkgw * pptr;
	struct linkgw * fptr;
	char * reqhost;
	int reqport=0;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else reqport = rptr->port;
	if (!( fptr = filter_linkgw_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	for ( sptr = linkgw_first;
		sptr != (struct occi_kind_node *) 0;
		sptr = sptr->next ) {
		if (!( pptr = sptr->contents ))
			continue;
		if (!( pass_linkgw_filter( pptr, fptr ) ))
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
private struct rest_response * linkgw_delete_all(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct occi_kind_node * sptr;
	struct linkgw * pptr;
	struct linkgw * fptr;
	iptr = optr->callback;
	if (!( fptr = filter_linkgw_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	nptr=linkgw_first;
	while (nptr != (struct occi_kind_node *) 0) {
		if ((!( pptr = nptr->contents ))
		||  (!( pass_linkgw_filter( pptr, fptr ) ))) {
			nptr = nptr->next;
			continue;
			}
		else	{
			if (( iptr ) && (iptr->delete)) { (*iptr->delete)(optr,nptr,rptr); }
			sptr = nptr->next;
			drop_linkgw_node( nptr );
			nptr = sptr;
			}
		}
	autosave_linkgw_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_linkgw_get(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( linkgw_get_list( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( linkgw_get_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_linkgw_head(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( linkgw_head_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_linkgw_post(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( linkgw_post_item( optr, cptr, rptr, aptr ) );
	else if ( strncmp( rptr->object, optr->location,strlen(optr->location)) != 0)
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( rptr->parameters ))
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( strncmp( rptr->parameters, "action=", strlen("action=")) ))
		return( linkgw_post_action( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "mixin=", strlen("mixin=")) ))
		return( linkgw_post_mixin( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "link=", strlen("link=")) ))
		return( linkgw_post_link( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p u t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_linkgw_put(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( linkgw_put_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e 	*/
/*	------------------------------------------------------------------------------------	*/
private struct rest_response * occi_linkgw_delete(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( linkgw_delete_all( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( linkgw_delete_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   r e d i r e c t i o n 	*/
/*	--------------------------------------------------------------------------------	*/
private void	redirect_occi_linkgw_mt( struct rest_interface * iptr )
{
	iptr->get = occi_linkgw_get;
	iptr->post = occi_linkgw_post;
	iptr->put = occi_linkgw_put;
	iptr->delete = occi_linkgw_delete;
	iptr->head = occi_linkgw_head;
	return;
}

/*	------------------------------------	*/
/*	c r u d   d e l e t e   a c t i o n 	*/
/*	------------------------------------	*/
private struct rest_response * delete_action_linkgw(struct occi_category * optr, 
struct rest_client * cptr,  
struct rest_request * rptr,  
struct rest_response * aptr,  
void * vptr )
{
	aptr = liberate_rest_response( aptr );
	return( occi_linkgw_delete(optr,cptr,rptr));
}

/*	------------------------------------------	*/
/*	o c c i   c a t e g o r y   b u i l d e r 	*/
/*	------------------------------------------	*/
/* occi category rest instance builder for : occi_linkgw */
public struct occi_category * occi_linkgw_builder(char * a,char * b) {
	char * c="http://scheme.compatibleone.fr/scheme/compatible#";
	char * d="kind";
	char * e="http://scheme.ogf.org/occi/resource#";
	char * f="CompatibleOne OCCI resource linkgw";
	struct occi_category * optr;
	if (!( optr = occi_create_category(a,b,c,d,e,f) )) { return(optr); }
	else {
		redirect_occi_linkgw_mt(optr->interface);
		if (!( optr = occi_add_attribute(optr, "name",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "intercloudGW",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "account",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "gwsrc",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "gwdst",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "tunnelproto",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "addressgresrc",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "addressgredst",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "prefix",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "authenticationkey",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "endpointsrc",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "endpointdst",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "state",0,0) ))
			return(optr);
		if (!( optr = occi_add_action( optr,"DELETE","",delete_action_linkgw)))
			return( optr );
		autoload_linkgw_nodes();
		return(optr);
	}

}

/*	--------------------------------------	*/
/*	l i n k g w _ o c c i _ h e a d e r s 	*/
/*	--------------------------------------	*/
public struct rest_header *  linkgw_occi_headers(struct linkgw * sptr)
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
	sprintf(buffer,"linkgw; scheme='http://scheme.compatibleone.fr/scheme/compatible#'; class='kind';\r\n");
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
	sprintf(buffer,"occi.linkgw.name='%s'\r\n",(sptr->name?sptr->name:""));
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
	sprintf(buffer,"occi.linkgw.intercloudGW='%s'\r\n",(sptr->intercloudGW?sptr->intercloudGW:""));
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
	sprintf(buffer,"occi.linkgw.account='%s'\r\n",(sptr->account?sptr->account:""));
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
	sprintf(buffer,"occi.linkgw.gwsrc='%s'\r\n",(sptr->gwsrc?sptr->gwsrc:""));
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
	sprintf(buffer,"occi.linkgw.gwdst='%s'\r\n",(sptr->gwdst?sptr->gwdst:""));
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
	sprintf(buffer,"occi.linkgw.tunnelproto='%s'\r\n",(sptr->tunnelproto?sptr->tunnelproto:""));
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
	sprintf(buffer,"occi.linkgw.addressgresrc='%s'\r\n",(sptr->addressgresrc?sptr->addressgresrc:""));
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
	sprintf(buffer,"occi.linkgw.addressgredst='%s'\r\n",(sptr->addressgredst?sptr->addressgredst:""));
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
	sprintf(buffer,"occi.linkgw.prefix='%s'\r\n",(sptr->prefix?sptr->prefix:""));
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
	sprintf(buffer,"occi.linkgw.authenticationkey='%s'\r\n",(sptr->authenticationkey?sptr->authenticationkey:""));
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
	sprintf(buffer,"occi.linkgw.endpointsrc='%s'\r\n",(sptr->endpointsrc?sptr->endpointsrc:""));
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
	sprintf(buffer,"occi.linkgw.endpointdst='%s'\r\n",(sptr->endpointdst?sptr->endpointdst:""));
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
	sprintf(buffer,"occi.linkgw.state='%s'\r\n",(sptr->state?sptr->state:""));
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	return(first);

}

#endif	/* _occilinkgw_c_ */
