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
#ifndef _occinagioscontact_c_
#define _occinagioscontact_c_

#include "nagioscontact.h"

/*	--------------------------------------	*/
/*	o c c i _ n a g i o s _ c o n t a c t 	*/
/*	--------------------------------------	*/

/*	--------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   m a n a g e m e n t   s t r u c t u r e 	*/
/*	--------------------------------------------------------------------	*/
struct nagios_contact * allocate_nagios_contact();
struct nagios_contact * liberate_nagios_contact(struct nagios_contact * optr);
private pthread_mutex_t list_nagios_contact_control=PTHREAD_MUTEX_INITIALIZER;
private struct occi_kind_node * nagios_contact_first = (struct occi_kind_node *) 0;
private struct occi_kind_node * nagios_contact_last  = (struct occi_kind_node *) 0;
public struct  occi_kind_node * occi_first_nagios_contact_node() { return( nagios_contact_first ); }
public struct  occi_kind_node * occi_last_nagios_contact_node() { return( nagios_contact_last ); }

/*	----------------------------------------------	*/
/*	o c c i   c a t e g o r y   d r o p   n o d e 	*/
/*	----------------------------------------------	*/
private struct occi_kind_node * ll_drop_nagios_contact_node(struct occi_kind_node * nptr) {
	if ( nptr ) {
	if (!( nptr->previous ))
		nagios_contact_first = nptr->next;
	else	nptr->previous->next = nptr->next;
	if (!( nptr->next ))
		nagios_contact_last = nptr->previous;
	else	nptr->next->previous = nptr->previous;
		liberate_occi_kind_node( nptr );
		}
	return((struct occi_kind_node *)0);
}
private struct occi_kind_node * drop_nagios_contact_node(struct occi_kind_node * nptr) {
	pthread_mutex_lock( &list_nagios_contact_control );
	nptr = ll_drop_nagios_contact_node( nptr );
	pthread_mutex_unlock( &list_nagios_contact_control );
	return(nptr);
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   l o c a t e   n o d e 	*/
/*	--------------------------------------------------	*/
private struct occi_kind_node * ll_locate_nagios_contact_node(char * id) {
	struct occi_kind_node * nptr;
	struct nagios_contact * pptr;
	for ( nptr = nagios_contact_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		else if (!( pptr->id )) continue;
		else if (!( strcmp(pptr->id,id) )) break;
		}
	return( nptr );
}
private struct occi_kind_node * locate_nagios_contact_node(char * id) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_nagios_contact_control );
	nptr = ll_locate_nagios_contact_node(id);
	pthread_mutex_unlock( &list_nagios_contact_control );
	return( nptr );
}

/*	--------------------------------------------	*/
/*	o c c i   c a t e g o r y   a d d   n o d e 	*/
/*	--------------------------------------------	*/
private struct occi_kind_node * ll_add_nagios_contact_node(int mode) {
	struct occi_kind_node * nptr;
	struct nagios_contact * pptr;
	if (!( nptr = allocate_occi_kind_node() ))
		return( nptr );
	else	{
		if (!( nptr->contents = allocate_nagios_contact()))
			return( liberate_occi_kind_node(nptr) );
		if (!( pptr = nptr->contents ))
			return( liberate_occi_kind_node(nptr) );
		else if (( mode != 0 ) && (!( pptr->id = occi_allocate_uuid())))
			return( liberate_occi_kind_node(nptr) );
		else	{
			if (!( nptr->previous = nagios_contact_last ))
				nagios_contact_first = nptr;
			else	nptr->previous->next = nptr;
			nagios_contact_last = nptr;
			return( nptr );
			}
		}
}
private struct occi_kind_node * add_nagios_contact_node(int mode) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_nagios_contact_control );
	nptr = ll_add_nagios_contact_node( mode );
	pthread_mutex_unlock( &list_nagios_contact_control );
	return(nptr);
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   l o a d 	*/
/*	------------------------------------------------------------------------------------------	*/
private char*autosave_nagios_contact_name="nagios_contact.xml";
private void autoload_nagios_contact_nodes() {
	char * fn=autosave_nagios_contact_name;	struct occi_kind_node * nptr;
	struct nagios_contact * pptr;
	struct xml_element * document;
	struct xml_element * eptr;
	struct xml_element * vptr;
	struct xml_atribut  * aptr;
	if (!( document = document_parse_file(fn)))
		return;
	if ((eptr = document_element(document,"nagios_contacts")) != (struct xml_element *) 0) {
		for (vptr=eptr->first; vptr != (struct xml_element *) 0; vptr=vptr->next) {
			if (!( vptr->name )) continue;
			else if ( strcmp( vptr->name, "nagios_contact" ) ) continue;
			else if (!( nptr = add_nagios_contact_node(0))) break;
			else if (!( pptr = nptr->contents )) break;
			if ((aptr = document_atribut( vptr, "id" )) != (struct xml_atribut *) 0)
				pptr->id = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "name" )) != (struct xml_atribut *) 0)
				pptr->name = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "alias" )) != (struct xml_atribut *) 0)
				pptr->alias = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "host_notification_period" )) != (struct xml_atribut *) 0)
				pptr->host_notification_period = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "service_notification_period" )) != (struct xml_atribut *) 0)
				pptr->service_notification_period = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "host_notification_options" )) != (struct xml_atribut *) 0)
				pptr->host_notification_options = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "service_notification_options" )) != (struct xml_atribut *) 0)
				pptr->service_notification_options = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "email" )) != (struct xml_atribut *) 0)
				pptr->email = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "pager" )) != (struct xml_atribut *) 0)
				pptr->pager = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "addressx" )) != (struct xml_atribut *) 0)
				pptr->addressx = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "host_notifications_enabled" )) != (struct xml_atribut *) 0)
				pptr->host_notifications_enabled = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "service_notifications_enabled" )) != (struct xml_atribut *) 0)
				pptr->service_notifications_enabled = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "can_submit_commands" )) != (struct xml_atribut *) 0)
				pptr->can_submit_commands = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "retain_status_information" )) != (struct xml_atribut *) 0)
				pptr->retain_status_information = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "retain_nonstatus_information" )) != (struct xml_atribut *) 0)
				pptr->retain_nonstatus_information = document_atribut_value(aptr);
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
public  void set_autosave_nagios_contact_name(char * fn) {
	autosave_nagios_contact_name = fn;	return;
}
public  void autosave_nagios_contact_nodes() {
	char * fn=autosave_nagios_contact_name;	struct occi_kind_node * nptr;
	struct nagios_contact * pptr;
	FILE * h;
	pthread_mutex_lock( &list_nagios_contact_control );
	if (( h = fopen(fn,"w")) != (FILE *) 0) {
	fprintf(h,"<nagios_contacts>\n");
	for ( nptr = nagios_contact_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		fprintf(h,"<nagios_contact\n");
		fprintf(h," id=%c",0x0022);
		fprintf(h,"%s",(pptr->id?pptr->id:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," name=%c",0x0022);
		fprintf(h,"%s",(pptr->name?pptr->name:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," alias=%c",0x0022);
		fprintf(h,"%s",(pptr->alias?pptr->alias:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," host_notification_period=%c",0x0022);
		fprintf(h,"%s",(pptr->host_notification_period?pptr->host_notification_period:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," service_notification_period=%c",0x0022);
		fprintf(h,"%s",(pptr->service_notification_period?pptr->service_notification_period:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," host_notification_options=%c",0x0022);
		fprintf(h,"%s",(pptr->host_notification_options?pptr->host_notification_options:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," service_notification_options=%c",0x0022);
		fprintf(h,"%s",(pptr->service_notification_options?pptr->service_notification_options:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," email=%c",0x0022);
		fprintf(h,"%s",(pptr->email?pptr->email:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," pager=%c",0x0022);
		fprintf(h,"%s",(pptr->pager?pptr->pager:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," addressx=%c",0x0022);
		fprintf(h,"%s",(pptr->addressx?pptr->addressx:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," host_notifications_enabled=%c",0x0022);
		fprintf(h,"%u",pptr->host_notifications_enabled);
		fprintf(h,"%c",0x0022);
		fprintf(h," service_notifications_enabled=%c",0x0022);
		fprintf(h,"%u",pptr->service_notifications_enabled);
		fprintf(h,"%c",0x0022);
		fprintf(h," can_submit_commands=%c",0x0022);
		fprintf(h,"%u",pptr->can_submit_commands);
		fprintf(h,"%c",0x0022);
		fprintf(h," retain_status_information=%c",0x0022);
		fprintf(h,"%u",pptr->retain_status_information);
		fprintf(h,"%c",0x0022);
		fprintf(h," retain_nonstatus_information=%c",0x0022);
		fprintf(h,"%u",pptr->retain_nonstatus_information);
		fprintf(h,"%c",0x0022);
		fprintf(h," status=%c",0x0022);
		fprintf(h,"%u",pptr->status);
		fprintf(h,"%c",0x0022);
		fprintf(h," />\n");
		}
	fprintf(h,"</nagios_contacts>\n");
	fclose(h);
	}
	pthread_mutex_unlock( &list_nagios_contact_control );
	return;
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   s e t   f i e l d 	*/
/*	------------------------------------------------------------------------------------------	*/
private void set_nagios_contact_field(
	struct occi_category * cptr,void * optr, char * nptr, char * vptr)
{
	struct nagios_contact * pptr;
	char prefix[1024];
	if (!( pptr = optr )) return;
	sprintf(prefix,"%s.%s.",cptr->domain,cptr->id);
	if (!( strncmp( nptr, prefix, strlen(prefix) ) )) {
		nptr += strlen(prefix);
		if (!( strcmp( nptr, "name" ) ))
			pptr->name = allocate_string(vptr);
		if (!( strcmp( nptr, "alias" ) ))
			pptr->alias = allocate_string(vptr);
		if (!( strcmp( nptr, "host_notification_period" ) ))
			pptr->host_notification_period = allocate_string(vptr);
		if (!( strcmp( nptr, "service_notification_period" ) ))
			pptr->service_notification_period = allocate_string(vptr);
		if (!( strcmp( nptr, "host_notification_options" ) ))
			pptr->host_notification_options = allocate_string(vptr);
		if (!( strcmp( nptr, "service_notification_options" ) ))
			pptr->service_notification_options = allocate_string(vptr);
		if (!( strcmp( nptr, "email" ) ))
			pptr->email = allocate_string(vptr);
		if (!( strcmp( nptr, "pager" ) ))
			pptr->pager = allocate_string(vptr);
		if (!( strcmp( nptr, "addressx" ) ))
			pptr->addressx = allocate_string(vptr);
		if (!( strcmp( nptr, "host_notifications_enabled" ) ))
			pptr->host_notifications_enabled = atoi(vptr);
		if (!( strcmp( nptr, "service_notifications_enabled" ) ))
			pptr->service_notifications_enabled = atoi(vptr);
		if (!( strcmp( nptr, "can_submit_commands" ) ))
			pptr->can_submit_commands = atoi(vptr);
		if (!( strcmp( nptr, "retain_status_information" ) ))
			pptr->retain_status_information = atoi(vptr);
		if (!( strcmp( nptr, "retain_nonstatus_information" ) ))
			pptr->retain_nonstatus_information = atoi(vptr);
		if (!( strcmp( nptr, "status" ) ))
			pptr->status = atoi(vptr);
		}
	return;
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   i n f o 	*/
/*	--------------------------------------------------	*/
private struct nagios_contact * filter_nagios_contact_info(
	struct occi_category * optr,
	struct rest_request  * rptr,
	struct rest_response * aptr) {
	struct nagios_contact * pptr;
		if (!( pptr = allocate_nagios_contact()))
		return( pptr );
	else if (!( occi_process_atributs(optr, rptr, aptr, pptr, set_nagios_contact_field) ))
		return( liberate_nagios_contact(pptr));
	else	return( pptr );
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   p a s s 	*/
/*	--------------------------------------------------	*/
private int pass_nagios_contact_filter(
	struct nagios_contact * pptr,struct nagios_contact * fptr) {
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
	if (( fptr->alias )
	&&  (strlen( fptr->alias ) != 0)) {
		if (!( pptr->alias ))
			return(0);
		else if ( strcmp(pptr->alias,fptr->alias) != 0)
			return(0);
		}
	if (( fptr->host_notification_period )
	&&  (strlen( fptr->host_notification_period ) != 0)) {
		if (!( pptr->host_notification_period ))
			return(0);
		else if ( strcmp(pptr->host_notification_period,fptr->host_notification_period) != 0)
			return(0);
		}
	if (( fptr->service_notification_period )
	&&  (strlen( fptr->service_notification_period ) != 0)) {
		if (!( pptr->service_notification_period ))
			return(0);
		else if ( strcmp(pptr->service_notification_period,fptr->service_notification_period) != 0)
			return(0);
		}
	if (( fptr->host_notification_options )
	&&  (strlen( fptr->host_notification_options ) != 0)) {
		if (!( pptr->host_notification_options ))
			return(0);
		else if ( strcmp(pptr->host_notification_options,fptr->host_notification_options) != 0)
			return(0);
		}
	if (( fptr->service_notification_options )
	&&  (strlen( fptr->service_notification_options ) != 0)) {
		if (!( pptr->service_notification_options ))
			return(0);
		else if ( strcmp(pptr->service_notification_options,fptr->service_notification_options) != 0)
			return(0);
		}
	if (( fptr->email )
	&&  (strlen( fptr->email ) != 0)) {
		if (!( pptr->email ))
			return(0);
		else if ( strcmp(pptr->email,fptr->email) != 0)
			return(0);
		}
	if (( fptr->pager )
	&&  (strlen( fptr->pager ) != 0)) {
		if (!( pptr->pager ))
			return(0);
		else if ( strcmp(pptr->pager,fptr->pager) != 0)
			return(0);
		}
	if (( fptr->addressx )
	&&  (strlen( fptr->addressx ) != 0)) {
		if (!( pptr->addressx ))
			return(0);
		else if ( strcmp(pptr->addressx,fptr->addressx) != 0)
			return(0);
		}
	if (( fptr->host_notifications_enabled ) && ( pptr->host_notifications_enabled != fptr->host_notifications_enabled )) return(0);
	if (( fptr->service_notifications_enabled ) && ( pptr->service_notifications_enabled != fptr->service_notifications_enabled )) return(0);
	if (( fptr->can_submit_commands ) && ( pptr->can_submit_commands != fptr->can_submit_commands )) return(0);
	if (( fptr->retain_status_information ) && ( pptr->retain_status_information != fptr->retain_status_information )) return(0);
	if (( fptr->retain_nonstatus_information ) && ( pptr->retain_nonstatus_information != fptr->retain_nonstatus_information )) return(0);
	if (( fptr->status ) && ( pptr->status != fptr->status )) return(0);
	return(1);
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   r e s p o n s e 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_contact_occi_response(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,
	struct nagios_contact * pptr)
{
	struct rest_header * hptr;
	sprintf(cptr->buffer,"occi.core.id=%c%s%c",0x0022,pptr->id,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.name=%c%s%c",optr->domain,optr->id,0x0022,pptr->name,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.alias=%c%s%c",optr->domain,optr->id,0x0022,pptr->alias,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.host_notification_period=%c%s%c",optr->domain,optr->id,0x0022,pptr->host_notification_period,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.service_notification_period=%c%s%c",optr->domain,optr->id,0x0022,pptr->service_notification_period,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.host_notification_options=%c%s%c",optr->domain,optr->id,0x0022,pptr->host_notification_options,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.service_notification_options=%c%s%c",optr->domain,optr->id,0x0022,pptr->service_notification_options,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.email=%c%s%c",optr->domain,optr->id,0x0022,pptr->email,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.pager=%c%s%c",optr->domain,optr->id,0x0022,pptr->pager,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.addressx=%c%s%c",optr->domain,optr->id,0x0022,pptr->addressx,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.host_notifications_enabled=%c%u%c",optr->domain,optr->id,0x0022,pptr->host_notifications_enabled,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.service_notifications_enabled=%c%u%c",optr->domain,optr->id,0x0022,pptr->service_notifications_enabled,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.can_submit_commands=%c%u%c",optr->domain,optr->id,0x0022,pptr->can_submit_commands,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.retain_status_information=%c%u%c",optr->domain,optr->id,0x0022,pptr->retain_status_information,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.retain_nonstatus_information=%c%u%c",optr->domain,optr->id,0x0022,pptr->retain_nonstatus_information,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.status=%c%u%c",optr->domain,optr->id,0x0022,pptr->status,0x0022);
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
private struct rest_response * nagios_contact_get_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_contact * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_nagios_contact_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->retrieve)) (*iptr->retrieve)(optr,nptr,rptr);
	autosave_nagios_contact_nodes();
	return( nagios_contact_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   l i n k 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_contact_post_link(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_contact * pptr;
	char * reqhost;
	if (!( nptr = locate_nagios_contact_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   m i x i n 	*/
/*	--------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_contact_post_mixin(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_contact * pptr;
	char * reqhost;
	if (!( nptr = locate_nagios_contact_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request"));
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   a c t i o n 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_contact_post_action(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_action * fptr;
	struct occi_kind_node * nptr;
	struct nagios_contact * pptr;
	char * reqhost;
	char * mptr;
	if (!( nptr = locate_nagios_contact_node(id)))
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
private struct rest_response * nagios_contact_post_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_contact * pptr;
	char * reqhost;
	iptr = optr->callback;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	if (!( nptr = add_nagios_contact_node(1)))
		return( rest_html_response( aptr, 500, "Server Failure") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (!( occi_process_atributs( optr, rptr,aptr, pptr, set_nagios_contact_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->create)) (*iptr->create)(optr,nptr,rptr);
	autosave_nagios_contact_nodes();
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
private struct rest_response * nagios_contact_put_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_contact * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_nagios_contact_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (!( occi_process_atributs(optr,rptr,aptr, pptr, set_nagios_contact_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->update)) (*iptr->update)(optr,nptr,rptr);
	autosave_nagios_contact_nodes();
	return( nagios_contact_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d   i t e m 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_contact_head_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_kind_node * nptr;
	struct nagios_contact * pptr;
	if (!( nptr = locate_nagios_contact_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e   i t e m 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_contact_delete_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_contact * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_nagios_contact_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->delete)) (*iptr->delete)(optr,nptr,rptr);
	drop_nagios_contact_node( nptr );
	autosave_nagios_contact_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t   l i s t 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_contact_get_list(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_kind_node * sptr;
	struct nagios_contact * pptr;
	struct nagios_contact * fptr;
	char * reqhost;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else if (!( fptr = filter_nagios_contact_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	for ( sptr = nagios_contact_first;
		sptr != (struct occi_kind_node *) 0;
		sptr = sptr->next ) {
		if (!( pptr = sptr->contents ))
			continue;
		if (!( pass_nagios_contact_filter( pptr, fptr ) ))
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
private struct rest_response * nagios_contact_delete_all(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct occi_kind_node * sptr;
	struct nagios_contact * pptr;
	struct nagios_contact * fptr;
	iptr = optr->callback;
	if (!( fptr = filter_nagios_contact_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	nptr=nagios_contact_first;
	while (nptr != (struct occi_kind_node *) 0) {
		if ((!( pptr = nptr->contents ))
		||  (!( pass_nagios_contact_filter( pptr, fptr ) ))) {
			nptr = nptr->next;
			continue;
			}
		else	{
			if (( iptr ) && (iptr->delete)) { (*iptr->delete)(optr,nptr,rptr); }
			sptr = nptr->next;
			drop_nagios_contact_node( nptr );
			nptr = sptr;
			}
		}
	autosave_nagios_contact_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_contact_get(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_contact_get_list( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( nagios_contact_get_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_contact_head(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_contact_head_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_contact_post(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_contact_post_item( optr, cptr, rptr, aptr ) );
	else if ( strncmp( rptr->object, optr->location,strlen(optr->location)) != 0)
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( rptr->parameters ))
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( strncmp( rptr->parameters, "action=", strlen("action=")) ))
		return( nagios_contact_post_action( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "mixin=", strlen("mixin=")) ))
		return( nagios_contact_post_mixin( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "link=", strlen("link=")) ))
		return( nagios_contact_post_link( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p u t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_contact_put(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_contact_put_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e 	*/
/*	------------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_contact_delete(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_contact_delete_all( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( nagios_contact_delete_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   r e d i r e c t i o n 	*/
/*	--------------------------------------------------------------------------------	*/
private void	redirect_occi_nagios_contact_mt( struct rest_interface * iptr )
{
	iptr->get = occi_nagios_contact_get;
	iptr->post = occi_nagios_contact_post;
	iptr->put = occi_nagios_contact_put;
	iptr->delete = occi_nagios_contact_delete;
	iptr->head = occi_nagios_contact_head;
	return;
}

/*	------------------------------------	*/
/*	c r u d   d e l e t e   a c t i o n 	*/
/*	------------------------------------	*/
private struct rest_response * delete_action_nagios_contact(struct occi_category * optr, 
struct rest_client * cptr,  
struct rest_request * rptr,  
struct rest_response * aptr,  
void * vptr )
{
	aptr = liberate_rest_response( aptr );
	return( occi_nagios_contact_delete(optr,cptr,rptr));
}

/*	------------------------------------------	*/
/*	o c c i   c a t e g o r y   b u i l d e r 	*/
/*	------------------------------------------	*/
/* occi category rest instance builder for : occi_nagios_contact */
public struct occi_category * occi_nagios_contact_builder(char * a,char * b) {
	char * c="http://scheme.compatibleone.fr/scheme/compatible#";
	char * d="kind";
	char * e="http://scheme.ogf.org/occi/resource#";
	char * f="CompatibleOne OCCI resource nagios_contact";
	struct occi_category * optr;
	if (!( optr = occi_create_category(a,b,c,d,e,f) )) { return(optr); }
	else {
		redirect_occi_nagios_contact_mt(optr->interface);
		if (!( optr = occi_add_attribute(optr, "name",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "alias",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "host_notification_period",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "service_notification_period",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "host_notification_options",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "service_notification_options",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "email",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "pager",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "addressx",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "host_notifications_enabled",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "service_notifications_enabled",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "can_submit_commands",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "retain_status_information",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "retain_nonstatus_information",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "status",0,0) ))
			return(optr);
		if (!( optr = occi_add_action( optr,"DELETE","",delete_action_nagios_contact)))
			return( optr );
		autoload_nagios_contact_nodes();
		return(optr);
	}

}

/*	------------------------------------------------------	*/
/*	n a g i o s _ c o n t a c t _ o c c i _ h e a d e r s 	*/
/*	------------------------------------------------------	*/
public struct rest_header *  nagios_contact_occi_headers(struct nagios_contact * sptr)
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
	sprintf(buffer,"nagios_contact; scheme='http://scheme.compatibleone.fr/scheme/compatible#'; class='kind';\r\n");
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
	sprintf(buffer,"occi.nagios_contact.name='%s'\r\n",(sptr->name?sptr->name:""));
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
	sprintf(buffer,"occi.nagios_contact.alias='%s'\r\n",(sptr->alias?sptr->alias:""));
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
	sprintf(buffer,"occi.nagios_contact.host_notification_period='%s'\r\n",(sptr->host_notification_period?sptr->host_notification_period:""));
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
	sprintf(buffer,"occi.nagios_contact.service_notification_period='%s'\r\n",(sptr->service_notification_period?sptr->service_notification_period:""));
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
	sprintf(buffer,"occi.nagios_contact.host_notification_options='%s'\r\n",(sptr->host_notification_options?sptr->host_notification_options:""));
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
	sprintf(buffer,"occi.nagios_contact.service_notification_options='%s'\r\n",(sptr->service_notification_options?sptr->service_notification_options:""));
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
	sprintf(buffer,"occi.nagios_contact.email='%s'\r\n",(sptr->email?sptr->email:""));
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
	sprintf(buffer,"occi.nagios_contact.pager='%s'\r\n",(sptr->pager?sptr->pager:""));
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
	sprintf(buffer,"occi.nagios_contact.addressx='%s'\r\n",(sptr->addressx?sptr->addressx:""));
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
	sprintf(buffer,"occi.nagios_contact.host_notifications_enabled='%u'\r\n",sptr->host_notifications_enabled);
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
	sprintf(buffer,"occi.nagios_contact.service_notifications_enabled='%u'\r\n",sptr->service_notifications_enabled);
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
	sprintf(buffer,"occi.nagios_contact.can_submit_commands='%u'\r\n",sptr->can_submit_commands);
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
	sprintf(buffer,"occi.nagios_contact.retain_status_information='%u'\r\n",sptr->retain_status_information);
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
	sprintf(buffer,"occi.nagios_contact.retain_nonstatus_information='%u'\r\n",sptr->retain_nonstatus_information);
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
	sprintf(buffer,"occi.nagios_contact.status='%u'\r\n",sptr->status);
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	return(first);

}

#endif	/* _occinagioscontact_c_ */
