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
#ifndef _occinagioshostdependancy_c_
#define _occinagioshostdependancy_c_

#include "nagioshostdependancy.h"

/*	------------------------------------------------------	*/
/*	o c c i _ n a g i o s _ h o s t _ d e p e n d e n c y 	*/
/*	------------------------------------------------------	*/

/*	--------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   m a n a g e m e n t   s t r u c t u r e 	*/
/*	--------------------------------------------------------------------	*/
struct nagios_host_dependency * allocate_nagios_host_dependency();
struct nagios_host_dependency * liberate_nagios_host_dependency(struct nagios_host_dependency * optr);
private pthread_mutex_t list_nagios_host_dependency_control=PTHREAD_MUTEX_INITIALIZER;
private struct occi_kind_node * nagios_host_dependency_first = (struct occi_kind_node *) 0;
private struct occi_kind_node * nagios_host_dependency_last  = (struct occi_kind_node *) 0;
public struct  occi_kind_node * occi_first_nagios_host_dependency_node() { return( nagios_host_dependency_first ); }
public struct  occi_kind_node * occi_last_nagios_host_dependency_node() { return( nagios_host_dependency_last ); }

/*	----------------------------------------------	*/
/*	o c c i   c a t e g o r y   d r o p   n o d e 	*/
/*	----------------------------------------------	*/
private struct occi_kind_node * ll_drop_nagios_host_dependency_node(struct occi_kind_node * nptr) {
	if ( nptr ) {
	if (!( nptr->previous ))
		nagios_host_dependency_first = nptr->next;
	else	nptr->previous->next = nptr->next;
	if (!( nptr->next ))
		nagios_host_dependency_last = nptr->previous;
	else	nptr->next->previous = nptr->previous;
		liberate_occi_kind_node( nptr );
		}
	return((struct occi_kind_node *)0);
}
private struct occi_kind_node * drop_nagios_host_dependency_node(struct occi_kind_node * nptr) {
	pthread_mutex_lock( &list_nagios_host_dependency_control );
	nptr = ll_drop_nagios_host_dependency_node( nptr );
	pthread_mutex_unlock( &list_nagios_host_dependency_control );
	return(nptr);
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   l o c a t e   n o d e 	*/
/*	--------------------------------------------------	*/
private struct occi_kind_node * ll_locate_nagios_host_dependency_node(char * id) {
	struct occi_kind_node * nptr;
	struct nagios_host_dependency * pptr;
	for ( nptr = nagios_host_dependency_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		else if (!( pptr->id )) continue;
		else if (!( strcmp(pptr->id,id) )) break;
		}
	return( nptr );
}
private struct occi_kind_node * locate_nagios_host_dependency_node(char * id) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_nagios_host_dependency_control );
	nptr = ll_locate_nagios_host_dependency_node(id);
	pthread_mutex_unlock( &list_nagios_host_dependency_control );
	return( nptr );
}

/*	--------------------------------------------	*/
/*	o c c i   c a t e g o r y   a d d   n o d e 	*/
/*	--------------------------------------------	*/
private struct occi_kind_node * ll_add_nagios_host_dependency_node(int mode) {
	struct occi_kind_node * nptr;
	struct nagios_host_dependency * pptr;
	if (!( nptr = allocate_occi_kind_node() ))
		return( nptr );
	else	{
		if (!( nptr->contents = allocate_nagios_host_dependency()))
			return( liberate_occi_kind_node(nptr) );
		if (!( pptr = nptr->contents ))
			return( liberate_occi_kind_node(nptr) );
		else if (( mode != 0 ) && (!( pptr->id = occi_allocate_uuid())))
			return( liberate_occi_kind_node(nptr) );
		else	{
			if (!( nptr->previous = nagios_host_dependency_last ))
				nagios_host_dependency_first = nptr;
			else	nptr->previous->next = nptr;
			nagios_host_dependency_last = nptr;
			return( nptr );
			}
		}
}
private struct occi_kind_node * add_nagios_host_dependency_node(int mode) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_nagios_host_dependency_control );
	nptr = ll_add_nagios_host_dependency_node( mode );
	pthread_mutex_unlock( &list_nagios_host_dependency_control );
	return(nptr);
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   l o a d 	*/
/*	------------------------------------------------------------------------------------------	*/
private char*autosave_nagios_host_dependency_name="nagios_host_dependency.xml";
private void autoload_nagios_host_dependency_nodes() {
	char * fn=autosave_nagios_host_dependency_name;	struct occi_kind_node * nptr;
	struct nagios_host_dependency * pptr;
	struct xml_element * document;
	struct xml_element * eptr;
	struct xml_element * vptr;
	struct xml_atribut  * aptr;
	if (!( document = document_parse_file(fn)))
		return;
	if ((eptr = document_element(document,"nagios_host_dependencys")) != (struct xml_element *) 0) {
		for (vptr=eptr->first; vptr != (struct xml_element *) 0; vptr=vptr->next) {
			if (!( vptr->name )) continue;
			else if ( strcmp( vptr->name, "nagios_host_dependency" ) ) continue;
			else if (!( nptr = add_nagios_host_dependency_node(0))) break;
			else if (!( pptr = nptr->contents )) break;
			if ((aptr = document_atribut( vptr, "id" )) != (struct xml_atribut *) 0)
				pptr->id = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "name" )) != (struct xml_atribut *) 0)
				pptr->name = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "dependent_host_name" )) != (struct xml_atribut *) 0)
				pptr->dependent_host_name = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "dependent_hostgroup_name" )) != (struct xml_atribut *) 0)
				pptr->dependent_hostgroup_name = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "host_name" )) != (struct xml_atribut *) 0)
				pptr->host_name = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "hostgroup_name" )) != (struct xml_atribut *) 0)
				pptr->hostgroup_name = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "inherits_parent" )) != (struct xml_atribut *) 0)
				pptr->inherits_parent = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "execution_failure_criteria" )) != (struct xml_atribut *) 0)
				pptr->execution_failure_criteria = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "notification_failure_criteria" )) != (struct xml_atribut *) 0)
				pptr->notification_failure_criteria = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "dependency_period" )) != (struct xml_atribut *) 0)
				pptr->dependency_period = document_atribut_string(aptr);
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
public  void set_autosave_nagios_host_dependency_name(char * fn) {
	autosave_nagios_host_dependency_name = fn;	return;
}
public  void autosave_nagios_host_dependency_nodes() {
	char * fn=autosave_nagios_host_dependency_name;	struct occi_kind_node * nptr;
	struct nagios_host_dependency * pptr;
	FILE * h;
	pthread_mutex_lock( &list_nagios_host_dependency_control );
	if (( h = fopen(fn,"w")) != (FILE *) 0) {
	fprintf(h,"<nagios_host_dependencys>\n");
	for ( nptr = nagios_host_dependency_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		fprintf(h,"<nagios_host_dependency\n");
		fprintf(h," id=%c",0x0022);
		fprintf(h,"%s",(pptr->id?pptr->id:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," name=%c",0x0022);
		fprintf(h,"%s",(pptr->name?pptr->name:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," dependent_host_name=%c",0x0022);
		fprintf(h,"%s",(pptr->dependent_host_name?pptr->dependent_host_name:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," dependent_hostgroup_name=%c",0x0022);
		fprintf(h,"%s",(pptr->dependent_hostgroup_name?pptr->dependent_hostgroup_name:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," host_name=%c",0x0022);
		fprintf(h,"%s",(pptr->host_name?pptr->host_name:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," hostgroup_name=%c",0x0022);
		fprintf(h,"%s",(pptr->hostgroup_name?pptr->hostgroup_name:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," inherits_parent=%c",0x0022);
		fprintf(h,"%u",pptr->inherits_parent);
		fprintf(h,"%c",0x0022);
		fprintf(h," execution_failure_criteria=%c",0x0022);
		fprintf(h,"%s",(pptr->execution_failure_criteria?pptr->execution_failure_criteria:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," notification_failure_criteria=%c",0x0022);
		fprintf(h,"%s",(pptr->notification_failure_criteria?pptr->notification_failure_criteria:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," dependency_period=%c",0x0022);
		fprintf(h,"%s",(pptr->dependency_period?pptr->dependency_period:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," status=%c",0x0022);
		fprintf(h,"%u",pptr->status);
		fprintf(h,"%c",0x0022);
		fprintf(h," />\n");
		}
	fprintf(h,"</nagios_host_dependencys>\n");
	fclose(h);
	}
	pthread_mutex_unlock( &list_nagios_host_dependency_control );
	return;
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   s e t   f i e l d 	*/
/*	------------------------------------------------------------------------------------------	*/
private void set_nagios_host_dependency_field(
	struct occi_category * cptr,void * optr, char * nptr, char * vptr)
{
	struct nagios_host_dependency * pptr;
	char prefix[1024];
	if (!( pptr = optr )) return;
	sprintf(prefix,"%s.%s.",cptr->domain,cptr->id);
	if (!( strncmp( nptr, prefix, strlen(prefix) ) )) {
		nptr += strlen(prefix);
		if (!( strcmp( nptr, "name" ) ))
			pptr->name = allocate_string(vptr);
		if (!( strcmp( nptr, "dependent_host_name" ) ))
			pptr->dependent_host_name = allocate_string(vptr);
		if (!( strcmp( nptr, "dependent_hostgroup_name" ) ))
			pptr->dependent_hostgroup_name = allocate_string(vptr);
		if (!( strcmp( nptr, "host_name" ) ))
			pptr->host_name = allocate_string(vptr);
		if (!( strcmp( nptr, "hostgroup_name" ) ))
			pptr->hostgroup_name = allocate_string(vptr);
		if (!( strcmp( nptr, "inherits_parent" ) ))
			pptr->inherits_parent = atoi(vptr);
		if (!( strcmp( nptr, "execution_failure_criteria" ) ))
			pptr->execution_failure_criteria = allocate_string(vptr);
		if (!( strcmp( nptr, "notification_failure_criteria" ) ))
			pptr->notification_failure_criteria = allocate_string(vptr);
		if (!( strcmp( nptr, "dependency_period" ) ))
			pptr->dependency_period = allocate_string(vptr);
		if (!( strcmp( nptr, "status" ) ))
			pptr->status = atoi(vptr);
		}
	return;
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   i n f o 	*/
/*	--------------------------------------------------	*/
private struct nagios_host_dependency * filter_nagios_host_dependency_info(
	struct occi_category * optr,
	struct rest_request  * rptr,
	struct rest_response * aptr) {
	struct nagios_host_dependency * pptr;
		if (!( pptr = allocate_nagios_host_dependency()))
		return( pptr );
	else if (!( occi_process_atributs(optr, rptr, aptr, pptr, set_nagios_host_dependency_field) ))
		return( liberate_nagios_host_dependency(pptr));
	else	return( pptr );
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   p a s s 	*/
/*	--------------------------------------------------	*/
private int pass_nagios_host_dependency_filter(
	struct nagios_host_dependency * pptr,struct nagios_host_dependency * fptr) {
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
	if (( fptr->dependent_host_name )
	&&  (strlen( fptr->dependent_host_name ) != 0)) {
		if (!( pptr->dependent_host_name ))
			return(0);
		else if ( strcmp(pptr->dependent_host_name,fptr->dependent_host_name) != 0)
			return(0);
		}
	if (( fptr->dependent_hostgroup_name )
	&&  (strlen( fptr->dependent_hostgroup_name ) != 0)) {
		if (!( pptr->dependent_hostgroup_name ))
			return(0);
		else if ( strcmp(pptr->dependent_hostgroup_name,fptr->dependent_hostgroup_name) != 0)
			return(0);
		}
	if (( fptr->host_name )
	&&  (strlen( fptr->host_name ) != 0)) {
		if (!( pptr->host_name ))
			return(0);
		else if ( strcmp(pptr->host_name,fptr->host_name) != 0)
			return(0);
		}
	if (( fptr->hostgroup_name )
	&&  (strlen( fptr->hostgroup_name ) != 0)) {
		if (!( pptr->hostgroup_name ))
			return(0);
		else if ( strcmp(pptr->hostgroup_name,fptr->hostgroup_name) != 0)
			return(0);
		}
	if (( fptr->inherits_parent ) && ( pptr->inherits_parent != fptr->inherits_parent )) return(0);
	if (( fptr->execution_failure_criteria )
	&&  (strlen( fptr->execution_failure_criteria ) != 0)) {
		if (!( pptr->execution_failure_criteria ))
			return(0);
		else if ( strcmp(pptr->execution_failure_criteria,fptr->execution_failure_criteria) != 0)
			return(0);
		}
	if (( fptr->notification_failure_criteria )
	&&  (strlen( fptr->notification_failure_criteria ) != 0)) {
		if (!( pptr->notification_failure_criteria ))
			return(0);
		else if ( strcmp(pptr->notification_failure_criteria,fptr->notification_failure_criteria) != 0)
			return(0);
		}
	if (( fptr->dependency_period )
	&&  (strlen( fptr->dependency_period ) != 0)) {
		if (!( pptr->dependency_period ))
			return(0);
		else if ( strcmp(pptr->dependency_period,fptr->dependency_period) != 0)
			return(0);
		}
	if (( fptr->status ) && ( pptr->status != fptr->status )) return(0);
	return(1);
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   r e s p o n s e 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_host_dependency_occi_response(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,
	struct nagios_host_dependency * pptr)
{
	struct rest_header * hptr;
	sprintf(cptr->buffer,"occi.core.id=%c%s%c",0x0022,pptr->id,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.name=%c%s%c",optr->domain,optr->id,0x0022,pptr->name,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.dependent_host_name=%c%s%c",optr->domain,optr->id,0x0022,pptr->dependent_host_name,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.dependent_hostgroup_name=%c%s%c",optr->domain,optr->id,0x0022,pptr->dependent_hostgroup_name,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.host_name=%c%s%c",optr->domain,optr->id,0x0022,pptr->host_name,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.hostgroup_name=%c%s%c",optr->domain,optr->id,0x0022,pptr->hostgroup_name,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.inherits_parent=%c%u%c",optr->domain,optr->id,0x0022,pptr->inherits_parent,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.execution_failure_criteria=%c%s%c",optr->domain,optr->id,0x0022,pptr->execution_failure_criteria,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.notification_failure_criteria=%c%s%c",optr->domain,optr->id,0x0022,pptr->notification_failure_criteria,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.dependency_period=%c%s%c",optr->domain,optr->id,0x0022,pptr->dependency_period,0x0022);
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
private struct rest_response * nagios_host_dependency_get_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_host_dependency * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_nagios_host_dependency_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->retrieve)) (*iptr->retrieve)(optr,nptr,rptr);
	autosave_nagios_host_dependency_nodes();
	return( nagios_host_dependency_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   l i n k 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_host_dependency_post_link(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_host_dependency * pptr;
	char * reqhost;
	if (!( nptr = locate_nagios_host_dependency_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   m i x i n 	*/
/*	--------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_host_dependency_post_mixin(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_host_dependency * pptr;
	char * reqhost;
	if (!( nptr = locate_nagios_host_dependency_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request"));
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   a c t i o n 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_host_dependency_post_action(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_action * fptr;
	struct occi_kind_node * nptr;
	struct nagios_host_dependency * pptr;
	char * reqhost;
	char * mptr;
	if (!( nptr = locate_nagios_host_dependency_node(id)))
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
private struct rest_response * nagios_host_dependency_post_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_host_dependency * pptr;
	char * reqhost;
	iptr = optr->callback;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	if (!( nptr = add_nagios_host_dependency_node(1)))
		return( rest_html_response( aptr, 500, "Server Failure") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (!( occi_process_atributs( optr, rptr,aptr, pptr, set_nagios_host_dependency_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->create)) (*iptr->create)(optr,nptr,rptr);
	autosave_nagios_host_dependency_nodes();
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
private struct rest_response * nagios_host_dependency_put_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_host_dependency * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_nagios_host_dependency_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (!( occi_process_atributs(optr,rptr,aptr, pptr, set_nagios_host_dependency_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->update)) (*iptr->update)(optr,nptr,rptr);
	autosave_nagios_host_dependency_nodes();
	return( nagios_host_dependency_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d   i t e m 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_host_dependency_head_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_kind_node * nptr;
	struct nagios_host_dependency * pptr;
	if (!( nptr = locate_nagios_host_dependency_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e   i t e m 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_host_dependency_delete_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_host_dependency * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_nagios_host_dependency_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->delete)) (*iptr->delete)(optr,nptr,rptr);
	drop_nagios_host_dependency_node( nptr );
	autosave_nagios_host_dependency_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t   l i s t 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_host_dependency_get_list(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_kind_node * sptr;
	struct nagios_host_dependency * pptr;
	struct nagios_host_dependency * fptr;
	char * reqhost;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else if (!( fptr = filter_nagios_host_dependency_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	for ( sptr = nagios_host_dependency_first;
		sptr != (struct occi_kind_node *) 0;
		sptr = sptr->next ) {
		if (!( pptr = sptr->contents ))
			continue;
		if (!( pass_nagios_host_dependency_filter( pptr, fptr ) ))
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
private struct rest_response * nagios_host_dependency_delete_all(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct occi_kind_node * sptr;
	struct nagios_host_dependency * pptr;
	struct nagios_host_dependency * fptr;
	iptr = optr->callback;
	if (!( fptr = filter_nagios_host_dependency_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	nptr=nagios_host_dependency_first;
	while (nptr != (struct occi_kind_node *) 0) {
		if ((!( pptr = nptr->contents ))
		||  (!( pass_nagios_host_dependency_filter( pptr, fptr ) ))) {
			nptr = nptr->next;
			continue;
			}
		else	{
			if (( iptr ) && (iptr->delete)) { (*iptr->delete)(optr,nptr,rptr); }
			sptr = nptr->next;
			drop_nagios_host_dependency_node( nptr );
			nptr = sptr;
			}
		}
	autosave_nagios_host_dependency_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_host_dependency_get(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_host_dependency_get_list( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( nagios_host_dependency_get_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_host_dependency_head(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_host_dependency_head_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_host_dependency_post(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_host_dependency_post_item( optr, cptr, rptr, aptr ) );
	else if ( strncmp( rptr->object, optr->location,strlen(optr->location)) != 0)
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( rptr->parameters ))
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( strncmp( rptr->parameters, "action=", strlen("action=")) ))
		return( nagios_host_dependency_post_action( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "mixin=", strlen("mixin=")) ))
		return( nagios_host_dependency_post_mixin( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "link=", strlen("link=")) ))
		return( nagios_host_dependency_post_link( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p u t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_host_dependency_put(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_host_dependency_put_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e 	*/
/*	------------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_host_dependency_delete(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_host_dependency_delete_all( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( nagios_host_dependency_delete_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   r e d i r e c t i o n 	*/
/*	--------------------------------------------------------------------------------	*/
private void	redirect_occi_nagios_host_dependency_mt( struct rest_interface * iptr )
{
	iptr->get = occi_nagios_host_dependency_get;
	iptr->post = occi_nagios_host_dependency_post;
	iptr->put = occi_nagios_host_dependency_put;
	iptr->delete = occi_nagios_host_dependency_delete;
	iptr->head = occi_nagios_host_dependency_head;
	return;
}

/*	------------------------------------	*/
/*	c r u d   d e l e t e   a c t i o n 	*/
/*	------------------------------------	*/
private struct rest_response * delete_action_nagios_host_dependency(struct occi_category * optr, 
struct rest_client * cptr,  
struct rest_request * rptr,  
struct rest_response * aptr,  
void * vptr )
{
	aptr = liberate_rest_response( aptr );
	return( occi_nagios_host_dependency_delete(optr,cptr,rptr));
}

/*	------------------------------------------	*/
/*	o c c i   c a t e g o r y   b u i l d e r 	*/
/*	------------------------------------------	*/
/* occi category rest instance builder for : occi_nagios_host_dependency */
public struct occi_category * occi_nagios_host_dependency_builder(char * a,char * b) {
	char * c="http://scheme.compatibleone.fr/scheme/compatible#";
	char * d="kind";
	char * e="http://scheme.ogf.org/occi/resource#";
	char * f="CompatibleOne OCCI resource nagios_host_dependency";
	struct occi_category * optr;
	if (!( optr = occi_create_category(a,b,c,d,e,f) )) { return(optr); }
	else {
		redirect_occi_nagios_host_dependency_mt(optr->interface);
		if (!( optr = occi_add_attribute(optr, "name",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "dependent_host_name",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "dependent_hostgroup_name",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "host_name",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "hostgroup_name",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "inherits_parent",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "execution_failure_criteria",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "notification_failure_criteria",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "dependency_period",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "status",0,0) ))
			return(optr);
		if (!( optr = occi_add_action( optr,"DELETE","",delete_action_nagios_host_dependency)))
			return( optr );
		autoload_nagios_host_dependency_nodes();
		return(optr);
	}

}

/*	----------------------------------------------------------------------	*/
/*	n a g i o s _ h o s t _ d e p e n d e n c y _ o c c i _ h e a d e r s 	*/
/*	----------------------------------------------------------------------	*/
public struct rest_header *  nagios_host_dependency_occi_headers(struct nagios_host_dependency * sptr)
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
	sprintf(buffer,"nagios_host_dependency; scheme='http://scheme.compatibleone.fr/scheme/compatible#'; class='kind';\r\n");
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
	sprintf(buffer,"occi.nagios_host_dependency.name='%s'\r\n",(sptr->name?sptr->name:""));
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
	sprintf(buffer,"occi.nagios_host_dependency.dependent_host_name='%s'\r\n",(sptr->dependent_host_name?sptr->dependent_host_name:""));
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
	sprintf(buffer,"occi.nagios_host_dependency.dependent_hostgroup_name='%s'\r\n",(sptr->dependent_hostgroup_name?sptr->dependent_hostgroup_name:""));
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
	sprintf(buffer,"occi.nagios_host_dependency.host_name='%s'\r\n",(sptr->host_name?sptr->host_name:""));
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
	sprintf(buffer,"occi.nagios_host_dependency.hostgroup_name='%s'\r\n",(sptr->hostgroup_name?sptr->hostgroup_name:""));
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
	sprintf(buffer,"occi.nagios_host_dependency.inherits_parent='%u'\r\n",sptr->inherits_parent);
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
	sprintf(buffer,"occi.nagios_host_dependency.execution_failure_criteria='%s'\r\n",(sptr->execution_failure_criteria?sptr->execution_failure_criteria:""));
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
	sprintf(buffer,"occi.nagios_host_dependency.notification_failure_criteria='%s'\r\n",(sptr->notification_failure_criteria?sptr->notification_failure_criteria:""));
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
	sprintf(buffer,"occi.nagios_host_dependency.dependency_period='%s'\r\n",(sptr->dependency_period?sptr->dependency_period:""));
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
	sprintf(buffer,"occi.nagios_host_dependency.status='%u'\r\n",sptr->status);
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	return(first);

}

#endif	/* _occinagioshostdependancy_c_ */
