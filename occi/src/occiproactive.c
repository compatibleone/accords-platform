/* ------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                   */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>    */
/* --------------------------------------------------------------------*/
/*  This is free software; you can redistribute it and/or modify it    */
/*  under the terms of the GNU Lesser General Public License as        */
/*  published by the Free Software Foundation; either version 2.1 of   */
/*  the License, or (at your option) any later version.                */
/*                                                                     */
/*  This software is distributed in the hope that it will be useful,   */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of     */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   */
/*  Lesser General Public License for more details.                    */
/*                                                                     */
/*  You should have received a copy of the GNU Lesser General Public   */
/*  License along with this software; if not, write to the Free        */
/*  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA */
/*  02110-1301 USA, or see the FSF site: http://www.fsf.org.           */
/* --------------------------------------------------------------------*/

/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _occiproactive_c_
#define _occiproactive_c_

#include "proactive.h"

/*	----------------------------	*/
/*	o c c i _ p r o a c t i v e 	*/
/*	----------------------------	*/

/*	--------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   m a n a g e m e n t   s t r u c t u r e 	*/
/*	--------------------------------------------------------------------	*/
struct proactive * allocate_proactive();
struct proactive * liberate_proactive(struct proactive * optr);
private pthread_mutex_t list_proactive_control=PTHREAD_MUTEX_INITIALIZER;
private struct occi_kind_node * proactive_first = (struct occi_kind_node *) 0;
private struct occi_kind_node * proactive_last  = (struct occi_kind_node *) 0;
public struct  occi_kind_node * occi_first_proactive_node() { return( proactive_first ); }

/*	----------------------------------------------	*/
/*	o c c i   c a t e g o r y   d r o p   n o d e 	*/
/*	----------------------------------------------	*/
private struct occi_kind_node * ll_drop_proactive_node(struct occi_kind_node * nptr) {
	if ( nptr ) {
	if (!( nptr->previous ))
		proactive_first = nptr->next;
	else	nptr->previous->next = nptr->next;
	if (!( nptr->next ))
		proactive_last = nptr->previous;
	else	nptr->next->previous = nptr->previous;
		liberate_occi_kind_node( nptr );
		}
	return((struct occi_kind_node *)0);
}
private struct occi_kind_node * drop_proactive_node(struct occi_kind_node * nptr) {
	pthread_mutex_lock( &list_proactive_control );
	nptr = ll_drop_proactive_node( nptr );
	pthread_mutex_unlock( &list_proactive_control );
	return(nptr);
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   l o c a t e   n o d e 	*/
/*	--------------------------------------------------	*/
private struct occi_kind_node * ll_locate_proactive_node(char * id) {
	struct occi_kind_node * nptr;
	struct proactive * pptr;
	for ( nptr = proactive_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		else if (!( pptr->id )) continue;
		else if (!( strcmp(pptr->id,id) )) break;
		}
	return( nptr );
}
private struct occi_kind_node * locate_proactive_node(char * id) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_proactive_control );
	nptr = ll_locate_proactive_node(id);
	pthread_mutex_unlock( &list_proactive_control );
	return( nptr );
}

/*	--------------------------------------------	*/
/*	o c c i   c a t e g o r y   a d d   n o d e 	*/
/*	--------------------------------------------	*/
private struct occi_kind_node * ll_add_proactive_node(int mode) {
	struct occi_kind_node * nptr;
	struct proactive * pptr;
	if (!( nptr = allocate_occi_kind_node() ))
		return( nptr );
	else	{
		if (!( nptr->contents = allocate_proactive()))
			return( liberate_occi_kind_node(nptr) );
		if (!( pptr = nptr->contents ))
			return( liberate_occi_kind_node(nptr) );
		else if (( mode != 0 ) && (!( pptr->id = occi_allocate_uuid())))
			return( liberate_occi_kind_node(nptr) );
		else	{
			if (!( nptr->previous = proactive_last ))
				proactive_first = nptr;
			else	nptr->previous->next = nptr;
			proactive_last = nptr;
			return( nptr );
			}
		}
}
private struct occi_kind_node * add_proactive_node(int mode) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_proactive_control );
	nptr = ll_add_proactive_node( mode );
	pthread_mutex_unlock( &list_proactive_control );
	return(nptr);
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   l o a d 	*/
/*	------------------------------------------------------------------------------------------	*/
private char*autosave_proactive_name="proactive.xml";
private void autoload_proactive_nodes() {
	char * fn=autosave_proactive_name;	struct occi_kind_node * nptr;
	struct proactive * pptr;
	struct xml_element * document;
	struct xml_element * eptr;
	struct xml_element * vptr;
	struct xml_atribut  * aptr;
	if (!( document = document_parse_file(fn)))
		return;
	if ((eptr = document_element(document,"proactives")) != (struct xml_element *) 0) {
		for (vptr=eptr->first; vptr != (struct xml_element *) 0; vptr=vptr->next) {
			if (!( vptr->name )) continue;
			else if ( strcmp( vptr->name, "proactive" ) ) continue;
			else if (!( nptr = add_proactive_node(0))) break;
			else if (!( pptr = nptr->contents )) break;
			if ((aptr = document_atribut( vptr, "id" )) != (struct xml_atribut *) 0)
				pptr->id = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "name" )) != (struct xml_atribut *) 0)
				pptr->name = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "flavor" )) != (struct xml_atribut *) 0)
				pptr->flavor = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "image" )) != (struct xml_atribut *) 0)
				pptr->image = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "profile" )) != (struct xml_atribut *) 0)
				pptr->profile = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "number" )) != (struct xml_atribut *) 0)
				pptr->number = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "access" )) != (struct xml_atribut *) 0)
				pptr->access = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "rootpass" )) != (struct xml_atribut *) 0)
				pptr->rootpass = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "reference" )) != (struct xml_atribut *) 0)
				pptr->reference = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "publicaddr" )) != (struct xml_atribut *) 0)
				pptr->publicaddr = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "privateaddr" )) != (struct xml_atribut *) 0)
				pptr->privateaddr = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "hostname" )) != (struct xml_atribut *) 0)
				pptr->hostname = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "workload" )) != (struct xml_atribut *) 0)
				pptr->workload = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "when" )) != (struct xml_atribut *) 0)
				pptr->when = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "status" )) != (struct xml_atribut *) 0)
				pptr->status = document_atribut_value(aptr);
			}
		}
	document = document_drop( document );
	return;
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   s a v e 	*/
/*	------------------------------------------------------------------------------------------	*/
public  void set_autosave_proactive_name(char * fn) {
	autosave_proactive_name = fn;	return;
}
public  void autosave_proactive_nodes() {
	char * fn=autosave_proactive_name;	struct occi_kind_node * nptr;
	struct proactive * pptr;
	FILE * h;
	pthread_mutex_lock( &list_proactive_control );
	if (( h = fopen(fn,"w")) != (FILE *) 0) {
	fprintf(h,"<proactives>\n");
	for ( nptr = proactive_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		fprintf(h,"<proactive\n");
		fprintf(h," id=%c",0x0022);
		fprintf(h,"%s",(pptr->id?pptr->id:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," name=%c",0x0022);
		fprintf(h,"%s",(pptr->name?pptr->name:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," flavor=%c",0x0022);
		fprintf(h,"%s",(pptr->flavor?pptr->flavor:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," image=%c",0x0022);
		fprintf(h,"%s",(pptr->image?pptr->image:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," profile=%c",0x0022);
		fprintf(h,"%s",(pptr->profile?pptr->profile:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," number=%c",0x0022);
		fprintf(h,"%s",(pptr->number?pptr->number:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," access=%c",0x0022);
		fprintf(h,"%s",(pptr->access?pptr->access:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," rootpass=%c",0x0022);
		fprintf(h,"%s",(pptr->rootpass?pptr->rootpass:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," reference=%c",0x0022);
		fprintf(h,"%s",(pptr->reference?pptr->reference:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," publicaddr=%c",0x0022);
		fprintf(h,"%s",(pptr->publicaddr?pptr->publicaddr:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," privateaddr=%c",0x0022);
		fprintf(h,"%s",(pptr->privateaddr?pptr->privateaddr:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," hostname=%c",0x0022);
		fprintf(h,"%s",(pptr->hostname?pptr->hostname:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," workload=%c",0x0022);
		fprintf(h,"%s",(pptr->workload?pptr->workload:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," when=%c",0x0022);
		fprintf(h,"%u",pptr->when);
		fprintf(h,"%c",0x0022);
		fprintf(h," status=%c",0x0022);
		fprintf(h,"%u",pptr->status);
		fprintf(h,"%c",0x0022);
		fprintf(h," />\n");
		}
	fprintf(h,"</proactives>\n");
	fclose(h);
	}
	pthread_mutex_unlock( &list_proactive_control );
	return;
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   s e t   f i e l d 	*/
/*	------------------------------------------------------------------------------------------	*/
private void set_proactive_field(
	struct occi_category * cptr,void * optr, char * nptr, char * vptr)
{
	struct proactive * pptr;
	char prefix[1024];
	if (!( pptr = optr )) return;
	sprintf(prefix,"%s.%s.",cptr->domain,cptr->id);
	if (!( strncmp( nptr, prefix, strlen(prefix) ) )) {
		nptr += strlen(prefix);
		if (!( strcmp( nptr, "name" ) ))
			pptr->name = allocate_string(vptr);
		if (!( strcmp( nptr, "flavor" ) ))
			pptr->flavor = allocate_string(vptr);
		if (!( strcmp( nptr, "image" ) ))
			pptr->image = allocate_string(vptr);
		if (!( strcmp( nptr, "profile" ) ))
			pptr->profile = allocate_string(vptr);
		if (!( strcmp( nptr, "number" ) ))
			pptr->number = allocate_string(vptr);
		if (!( strcmp( nptr, "access" ) ))
			pptr->access = allocate_string(vptr);
		if (!( strcmp( nptr, "rootpass" ) ))
			pptr->rootpass = allocate_string(vptr);
		if (!( strcmp( nptr, "reference" ) ))
			pptr->reference = allocate_string(vptr);
		if (!( strcmp( nptr, "publicaddr" ) ))
			pptr->publicaddr = allocate_string(vptr);
		if (!( strcmp( nptr, "privateaddr" ) ))
			pptr->privateaddr = allocate_string(vptr);
		if (!( strcmp( nptr, "hostname" ) ))
			pptr->hostname = allocate_string(vptr);
		if (!( strcmp( nptr, "workload" ) ))
			pptr->workload = allocate_string(vptr);
		if (!( strcmp( nptr, "when" ) ))
			pptr->when = atoi(vptr);
		if (!( strcmp( nptr, "status" ) ))
			pptr->status = atoi(vptr);
		}
	return;
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   i n f o 	*/
/*	--------------------------------------------------	*/
private struct proactive * filter_proactive_info(
	struct occi_category * optr,
	struct rest_request  * rptr,
	struct rest_response * aptr) {
	struct proactive * pptr;
		if (!( pptr = allocate_proactive()))
		return( pptr );
	else if (!( occi_process_atributs(optr, rptr, aptr, pptr, set_proactive_field) ))
		return( liberate_proactive(pptr));
	else	return( pptr );
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   p a s s 	*/
/*	--------------------------------------------------	*/
private int pass_proactive_filter(
	struct proactive * pptr,struct proactive * fptr) {
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
	if (( fptr->flavor )
	&&  (strlen( fptr->flavor ) != 0)) {
		if (!( pptr->flavor ))
			return(0);
		else if ( strcmp(pptr->flavor,fptr->flavor) != 0)
			return(0);
		}
	if (( fptr->image )
	&&  (strlen( fptr->image ) != 0)) {
		if (!( pptr->image ))
			return(0);
		else if ( strcmp(pptr->image,fptr->image) != 0)
			return(0);
		}
	if (( fptr->profile )
	&&  (strlen( fptr->profile ) != 0)) {
		if (!( pptr->profile ))
			return(0);
		else if ( strcmp(pptr->profile,fptr->profile) != 0)
			return(0);
		}
	if (( fptr->number )
	&&  (strlen( fptr->number ) != 0)) {
		if (!( pptr->number ))
			return(0);
		else if ( strcmp(pptr->number,fptr->number) != 0)
			return(0);
		}
	if (( fptr->access )
	&&  (strlen( fptr->access ) != 0)) {
		if (!( pptr->access ))
			return(0);
		else if ( strcmp(pptr->access,fptr->access) != 0)
			return(0);
		}
	if (( fptr->rootpass )
	&&  (strlen( fptr->rootpass ) != 0)) {
		if (!( pptr->rootpass ))
			return(0);
		else if ( strcmp(pptr->rootpass,fptr->rootpass) != 0)
			return(0);
		}
	if (( fptr->reference )
	&&  (strlen( fptr->reference ) != 0)) {
		if (!( pptr->reference ))
			return(0);
		else if ( strcmp(pptr->reference,fptr->reference) != 0)
			return(0);
		}
	if (( fptr->publicaddr )
	&&  (strlen( fptr->publicaddr ) != 0)) {
		if (!( pptr->publicaddr ))
			return(0);
		else if ( strcmp(pptr->publicaddr,fptr->publicaddr) != 0)
			return(0);
		}
	if (( fptr->privateaddr )
	&&  (strlen( fptr->privateaddr ) != 0)) {
		if (!( pptr->privateaddr ))
			return(0);
		else if ( strcmp(pptr->privateaddr,fptr->privateaddr) != 0)
			return(0);
		}
	if (( fptr->hostname )
	&&  (strlen( fptr->hostname ) != 0)) {
		if (!( pptr->hostname ))
			return(0);
		else if ( strcmp(pptr->hostname,fptr->hostname) != 0)
			return(0);
		}
	if (( fptr->workload )
	&&  (strlen( fptr->workload ) != 0)) {
		if (!( pptr->workload ))
			return(0);
		else if ( strcmp(pptr->workload,fptr->workload) != 0)
			return(0);
		}
	if (( fptr->when ) && ( pptr->when != fptr->when )) return(0);
	if (( fptr->status ) && ( pptr->status != fptr->status )) return(0);
	return(1);
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   r e s p o n s e 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * proactive_occi_response(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,
	struct proactive * pptr)
{
	struct rest_header * hptr;
	sprintf(cptr->buffer,"occi.core.id=%s",pptr->id);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.name=%s",optr->domain,optr->id,pptr->name);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.flavor=%s",optr->domain,optr->id,pptr->flavor);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.image=%s",optr->domain,optr->id,pptr->image);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.profile=%s",optr->domain,optr->id,pptr->profile);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.number=%s",optr->domain,optr->id,pptr->number);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.access=%s",optr->domain,optr->id,pptr->access);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.rootpass=%s",optr->domain,optr->id,pptr->rootpass);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.reference=%s",optr->domain,optr->id,pptr->reference);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.publicaddr=%s",optr->domain,optr->id,pptr->publicaddr);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.privateaddr=%s",optr->domain,optr->id,pptr->privateaddr);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.hostname=%s",optr->domain,optr->id,pptr->hostname);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.workload=%s",optr->domain,optr->id,pptr->workload);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.when=%u",optr->domain,optr->id,pptr->when);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.status=%u",optr->domain,optr->id,pptr->status);
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
private struct rest_response * proactive_get_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct proactive * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_proactive_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->retrieve)) (*iptr->retrieve)(optr,nptr);
	autosave_proactive_nodes();
	return( proactive_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   l i n k 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * proactive_post_link(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct proactive * pptr;
	char * reqhost;
	if (!( nptr = locate_proactive_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   m i x i n 	*/
/*	--------------------------------------------------------------------------------------------	*/
private struct rest_response * proactive_post_mixin(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct proactive * pptr;
	char * reqhost;
	if (!( nptr = locate_proactive_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request"));
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   a c t i o n 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * proactive_post_action(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_action * fptr;
	struct occi_kind_node * nptr;
	struct proactive * pptr;
	char * reqhost;
	char * mptr;
	if (!( nptr = locate_proactive_node(id)))
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
private struct rest_response * proactive_post_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct proactive * pptr;
	char * reqhost;
	iptr = optr->callback;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	if (!( nptr = add_proactive_node(1)))
		return( rest_html_response( aptr, 500, "Server Failure") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (!( occi_process_atributs( optr, rptr,aptr, pptr, set_proactive_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->create)) (*iptr->create)(optr,nptr);
	autosave_proactive_nodes();
	sprintf(cptr->buffer,"%s%s%s",reqhost,optr->location,pptr->id);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Location",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	else if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p u t   i t e m 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * proactive_put_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct proactive * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_proactive_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (!( occi_process_atributs(optr,rptr,aptr, pptr, set_proactive_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->update)) (*iptr->update)(optr,nptr);
	autosave_proactive_nodes();
	return( proactive_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d   i t e m 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * proactive_head_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_kind_node * nptr;
	struct proactive * pptr;
	if (!( nptr = locate_proactive_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e   i t e m 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * proactive_delete_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct proactive * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_proactive_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->delete)) (*iptr->delete)(optr,nptr);
	drop_proactive_node( nptr );
	autosave_proactive_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t   l i s t 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * proactive_get_list(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_kind_node * sptr;
	struct proactive * pptr;
	struct proactive * fptr;
	char * reqhost;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else if (!( fptr = filter_proactive_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	for ( sptr = proactive_first;
		sptr != (struct occi_kind_node *) 0;
		sptr = sptr->next ) {
		if (!( pptr = sptr->contents ))
			continue;
		if (!( pass_proactive_filter( pptr, fptr ) ))
			continue;
		sprintf(cptr->buffer,"%s%s%s",reqhost,optr->location,pptr->id);
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
private struct rest_response * proactive_delete_all(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct occi_kind_node * sptr;
	struct proactive * pptr;
	struct proactive * fptr;
	iptr = optr->callback;
	if (!( fptr = filter_proactive_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	nptr=proactive_first;
	while (nptr != (struct occi_kind_node *) 0) {
		if ((!( pptr = nptr->contents ))
		||  (!( pass_proactive_filter( pptr, fptr ) ))) {
			nptr = nptr->next;
			continue;
			}
		else	{
			if (( iptr ) && (iptr->delete)) { (*iptr->delete)(optr,nptr); }
			sptr = nptr->next;
			drop_proactive_node( nptr );
			nptr = sptr;
			}
		}
	autosave_proactive_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_proactive_get(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( proactive_get_list( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( proactive_get_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_proactive_head(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( proactive_head_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_proactive_post(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( proactive_post_item( optr, cptr, rptr, aptr ) );
	else if ( strncmp( rptr->object, optr->location,strlen(optr->location)) != 0)
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( rptr->parameters ))
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( strncmp( rptr->parameters, "action=", strlen("action=")) ))
		return( proactive_post_action( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "mixin=", strlen("mixin=")) ))
		return( proactive_post_mixin( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "link=", strlen("link=")) ))
		return( proactive_post_link( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p u t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_proactive_put(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( proactive_put_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e 	*/
/*	------------------------------------------------------------------------------------	*/
private struct rest_response * occi_proactive_delete(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( proactive_delete_all( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( proactive_delete_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   r e d i r e c t i o n 	*/
/*	--------------------------------------------------------------------------------	*/
private void	redirect_occi_proactive_mt( struct rest_interface * iptr )
{
	iptr->get = occi_proactive_get;
	iptr->post = occi_proactive_post;
	iptr->put = occi_proactive_put;
	iptr->delete = occi_proactive_delete;
	iptr->head = occi_proactive_head;
	return;
}

/*	------------------------------------------	*/
/*	o c c i   c a t e g o r y   b u i l d e r 	*/
/*	------------------------------------------	*/
/* occi category rest instance builder for : occi_proactive */
public struct occi_category * occi_proactive_builder(char * a,char * b) {
	char * c="http://scheme.compatibleone.fr/scheme/compatible#";
	char * d="kind";
	char * e="http://scheme.ogf.org/occi/resource#";
	char * f="CompatibleOne OCCI resource proactive";
	struct occi_category * optr;
	if (!( optr = occi_create_category(a,b,c,d,e,f) )) { return(optr); }
	else {
		redirect_occi_proactive_mt(optr->interface);
		if (!( optr = occi_add_attribute(optr, "name",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "flavor",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "image",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "profile",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "number",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "access",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "rootpass",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "reference",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "publicaddr",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "privateaddr",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "hostname",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "workload",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "when",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "status",0,0) ))
			return(optr);
		autoload_proactive_nodes();
		return(optr);
	}

}

/*	--------------------------------------------	*/
/*	p r o a c t i v e _ o c c i _ h e a d e r s 	*/
/*	--------------------------------------------	*/
public struct rest_header *  proactive_occi_headers(struct proactive * sptr)
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
	sprintf(buffer,"proactive; scheme='http://scheme.compatibleone.fr/scheme/compatible#'; class='kind';\r\n");
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
	sprintf(buffer,"occi.proactive.name='%s'\r\n",(sptr->name?sptr->name:""));
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
	sprintf(buffer,"occi.proactive.flavor='%s'\r\n",(sptr->flavor?sptr->flavor:""));
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
	sprintf(buffer,"occi.proactive.image='%s'\r\n",(sptr->image?sptr->image:""));
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
	sprintf(buffer,"occi.proactive.profile='%s'\r\n",(sptr->profile?sptr->profile:""));
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
	sprintf(buffer,"occi.proactive.number='%s'\r\n",(sptr->number?sptr->number:""));
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
	sprintf(buffer,"occi.proactive.access='%s'\r\n",(sptr->access?sptr->access:""));
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
	sprintf(buffer,"occi.proactive.rootpass='%s'\r\n",(sptr->rootpass?sptr->rootpass:""));
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
	sprintf(buffer,"occi.proactive.reference='%s'\r\n",(sptr->reference?sptr->reference:""));
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
	sprintf(buffer,"occi.proactive.publicaddr='%s'\r\n",(sptr->publicaddr?sptr->publicaddr:""));
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
	sprintf(buffer,"occi.proactive.privateaddr='%s'\r\n",(sptr->privateaddr?sptr->privateaddr:""));
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
	sprintf(buffer,"occi.proactive.hostname='%s'\r\n",(sptr->hostname?sptr->hostname:""));
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
	sprintf(buffer,"occi.proactive.workload='%s'\r\n",(sptr->workload?sptr->workload:""));
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
	sprintf(buffer,"occi.proactive.when='%u'\r\n",sptr->when);
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
	sprintf(buffer,"occi.proactive.status='%u'\r\n",sptr->status);
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	return(first);

}

#endif	/* _occiproactive_c_ */
