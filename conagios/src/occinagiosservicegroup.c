/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _occinagiosservicegroup_c_
#define _occinagiosservicegroup_c_

#include "nagiosservicegroup.h"

/*	--------------------------------------------------	*/
/*	o c c i _ n a g i o s _ s e r v i c e _ g r o u p 	*/
/*	--------------------------------------------------	*/

/*	--------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   m a n a g e m e n t   s t r u c t u r e 	*/
/*	--------------------------------------------------------------------	*/
struct nagios_service_group * allocate_nagios_service_group();
struct nagios_service_group * liberate_nagios_service_group(struct nagios_service_group * optr);
private pthread_mutex_t list_nagios_service_group_control=PTHREAD_MUTEX_INITIALIZER;
private struct occi_kind_node * nagios_service_group_first = (struct occi_kind_node *) 0;
private struct occi_kind_node * nagios_service_group_last  = (struct occi_kind_node *) 0;
public struct  occi_kind_node * occi_first_nagios_service_group_node() { return( nagios_service_group_first ); }
public struct  occi_kind_node * occi_last_nagios_service_group_node() { return( nagios_service_group_last ); }

/*	----------------------------------------------	*/
/*	o c c i   c a t e g o r y   d r o p   n o d e 	*/
/*	----------------------------------------------	*/
private struct occi_kind_node * ll_drop_nagios_service_group_node(struct occi_kind_node * nptr) {
	if ( nptr ) {
	if (!( nptr->previous ))
		nagios_service_group_first = nptr->next;
	else	nptr->previous->next = nptr->next;
	if (!( nptr->next ))
		nagios_service_group_last = nptr->previous;
	else	nptr->next->previous = nptr->previous;
		liberate_occi_kind_node( nptr );
		}
	return((struct occi_kind_node *)0);
}
private struct occi_kind_node * drop_nagios_service_group_node(struct occi_kind_node * nptr) {
	pthread_mutex_lock( &list_nagios_service_group_control );
	nptr = ll_drop_nagios_service_group_node( nptr );
	pthread_mutex_unlock( &list_nagios_service_group_control );
	return(nptr);
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   l o c a t e   n o d e 	*/
/*	--------------------------------------------------	*/
private struct occi_kind_node * ll_locate_nagios_service_group_node(char * id) {
	struct occi_kind_node * nptr;
	struct nagios_service_group * pptr;
	for ( nptr = nagios_service_group_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		else if (!( pptr->id )) continue;
		else if (!( strcmp(pptr->id,id) )) break;
		}
	return( nptr );
}
private struct occi_kind_node * locate_nagios_service_group_node(char * id) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_nagios_service_group_control );
	nptr = ll_locate_nagios_service_group_node(id);
	pthread_mutex_unlock( &list_nagios_service_group_control );
	return( nptr );
}

/*	--------------------------------------------	*/
/*	o c c i   c a t e g o r y   a d d   n o d e 	*/
/*	--------------------------------------------	*/
private struct occi_kind_node * ll_add_nagios_service_group_node(int mode) {
	struct occi_kind_node * nptr;
	struct nagios_service_group * pptr;
	if (!( nptr = allocate_occi_kind_node() ))
		return( nptr );
	else	{
		if (!( nptr->contents = allocate_nagios_service_group()))
			return( liberate_occi_kind_node(nptr) );
		if (!( pptr = nptr->contents ))
			return( liberate_occi_kind_node(nptr) );
		else if (( mode != 0 ) && (!( pptr->id = occi_allocate_uuid())))
			return( liberate_occi_kind_node(nptr) );
		else	{
			if (!( nptr->previous = nagios_service_group_last ))
				nagios_service_group_first = nptr;
			else	nptr->previous->next = nptr;
			nagios_service_group_last = nptr;
			return( nptr );
			}
		}
}
private struct occi_kind_node * add_nagios_service_group_node(int mode) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_nagios_service_group_control );
	nptr = ll_add_nagios_service_group_node( mode );
	pthread_mutex_unlock( &list_nagios_service_group_control );
	return(nptr);
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   l o a d 	*/
/*	------------------------------------------------------------------------------------------	*/
private char*autosave_nagios_service_group_name="nagios_service_group.xml";
private void autoload_nagios_service_group_nodes() {
	char * fn=autosave_nagios_service_group_name;	struct occi_kind_node * nptr;
	struct nagios_service_group * pptr;
	struct xml_element * document;
	struct xml_element * eptr;
	struct xml_element * vptr;
	struct xml_atribut  * aptr;
	if (!( document = document_parse_file(fn)))
		return;
	if ((eptr = document_element(document,"nagios_service_groups")) != (struct xml_element *) 0) {
		for (vptr=eptr->first; vptr != (struct xml_element *) 0; vptr=vptr->next) {
			if (!( vptr->name )) continue;
			else if ( strcmp( vptr->name, "nagios_service_group" ) ) continue;
			else if (!( nptr = add_nagios_service_group_node(0))) break;
			else if (!( pptr = nptr->contents )) break;
			if ((aptr = document_atribut( vptr, "id" )) != (struct xml_atribut *) 0)
				pptr->id = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "name" )) != (struct xml_atribut *) 0)
				pptr->name = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "alias" )) != (struct xml_atribut *) 0)
				pptr->alias = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "members" )) != (struct xml_atribut *) 0)
				pptr->members = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "servicegroup_members" )) != (struct xml_atribut *) 0)
				pptr->servicegroup_members = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "notes" )) != (struct xml_atribut *) 0)
				pptr->notes = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "notes_url" )) != (struct xml_atribut *) 0)
				pptr->notes_url = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "action_url" )) != (struct xml_atribut *) 0)
				pptr->action_url = document_atribut_string(aptr);
			}
		}
	document = document_drop( document );
	return;
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   s a v e 	*/
/*	------------------------------------------------------------------------------------------	*/
public  void set_autosave_nagios_service_group_name(char * fn) {
	autosave_nagios_service_group_name = fn;	return;
}
public  void autosave_nagios_service_group_nodes() {
	char * fn=autosave_nagios_service_group_name;	struct occi_kind_node * nptr;
	struct nagios_service_group * pptr;
	FILE * h;
	pthread_mutex_lock( &list_nagios_service_group_control );
	if (( h = fopen(fn,"w")) != (FILE *) 0) {
	fprintf(h,"<nagios_service_groups>\n");
	for ( nptr = nagios_service_group_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		fprintf(h,"<nagios_service_group\n");
		fprintf(h," id=%c",0x0022);
		fprintf(h,"%s",(pptr->id?pptr->id:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," name=%c",0x0022);
		fprintf(h,"%s",(pptr->name?pptr->name:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," alias=%c",0x0022);
		fprintf(h,"%s",(pptr->alias?pptr->alias:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," members=%c",0x0022);
		fprintf(h,"%u",pptr->members);
		fprintf(h,"%c",0x0022);
		fprintf(h," servicegroup_members=%c",0x0022);
		fprintf(h,"%u",pptr->servicegroup_members);
		fprintf(h,"%c",0x0022);
		fprintf(h," notes=%c",0x0022);
		fprintf(h,"%s",(pptr->notes?pptr->notes:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," notes_url=%c",0x0022);
		fprintf(h,"%s",(pptr->notes_url?pptr->notes_url:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," action_url=%c",0x0022);
		fprintf(h,"%s",(pptr->action_url?pptr->action_url:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," />\n");
		}
	fprintf(h,"</nagios_service_groups>\n");
	fclose(h);
	}
	pthread_mutex_unlock( &list_nagios_service_group_control );
	return;
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   s e t   f i e l d 	*/
/*	------------------------------------------------------------------------------------------	*/
private void set_nagios_service_group_field(
	struct occi_category * cptr,void * optr, char * nptr, char * vptr)
{
	struct nagios_service_group * pptr;
	char prefix[1024];
	if (!( pptr = optr )) return;
	sprintf(prefix,"%s.%s.",cptr->domain,cptr->id);
	if (!( strncmp( nptr, prefix, strlen(prefix) ) )) {
		nptr += strlen(prefix);
		if (!( strcmp( nptr, "name" ) ))
			pptr->name = allocate_string(vptr);
		if (!( strcmp( nptr, "alias" ) ))
			pptr->alias = allocate_string(vptr);
		if (!( strcmp( nptr, "members" ) ))
			pptr->members = atoi(vptr);
		if (!( strcmp( nptr, "servicegroup_members" ) ))
			pptr->servicegroup_members = atoi(vptr);
		if (!( strcmp( nptr, "notes" ) ))
			pptr->notes = allocate_string(vptr);
		if (!( strcmp( nptr, "notes_url" ) ))
			pptr->notes_url = allocate_string(vptr);
		if (!( strcmp( nptr, "action_url" ) ))
			pptr->action_url = allocate_string(vptr);
		}
	return;
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   i n f o 	*/
/*	--------------------------------------------------	*/
private struct nagios_service_group * filter_nagios_service_group_info(
	struct occi_category * optr,
	struct rest_request  * rptr,
	struct rest_response * aptr) {
	struct nagios_service_group * pptr;
		if (!( pptr = allocate_nagios_service_group()))
		return( pptr );
	else if (!( occi_process_atributs(optr, rptr, aptr, pptr, set_nagios_service_group_field) ))
		return( liberate_nagios_service_group(pptr));
	else	return( pptr );
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   p a s s 	*/
/*	--------------------------------------------------	*/
private int pass_nagios_service_group_filter(
	struct nagios_service_group * pptr,struct nagios_service_group * fptr) {
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
	if (( fptr->members ) && ( pptr->members != fptr->members )) return(0);
	if (( fptr->servicegroup_members ) && ( pptr->servicegroup_members != fptr->servicegroup_members )) return(0);
	if (( fptr->notes )
	&&  (strlen( fptr->notes ) != 0)) {
		if (!( pptr->notes ))
			return(0);
		else if ( strcmp(pptr->notes,fptr->notes) != 0)
			return(0);
		}
	if (( fptr->notes_url )
	&&  (strlen( fptr->notes_url ) != 0)) {
		if (!( pptr->notes_url ))
			return(0);
		else if ( strcmp(pptr->notes_url,fptr->notes_url) != 0)
			return(0);
		}
	if (( fptr->action_url )
	&&  (strlen( fptr->action_url ) != 0)) {
		if (!( pptr->action_url ))
			return(0);
		else if ( strcmp(pptr->action_url,fptr->action_url) != 0)
			return(0);
		}
	return(1);
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   r e s p o n s e 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_service_group_occi_response(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,
	struct nagios_service_group * pptr)
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
	sprintf(cptr->buffer,"%s.%s.members=%c%u%c",optr->domain,optr->id,0x0022,pptr->members,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.servicegroup_members=%c%u%c",optr->domain,optr->id,0x0022,pptr->servicegroup_members,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.notes=%c%s%c",optr->domain,optr->id,0x0022,pptr->notes,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.notes_url=%c%s%c",optr->domain,optr->id,0x0022,pptr->notes_url,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.action_url=%c%s%c",optr->domain,optr->id,0x0022,pptr->action_url,0x0022);
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
private struct rest_response * nagios_service_group_get_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_service_group * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_nagios_service_group_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->retrieve)) (*iptr->retrieve)(optr,nptr,rptr);
	autosave_nagios_service_group_nodes();
	return( nagios_service_group_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   l i n k 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_service_group_post_link(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_service_group * pptr;
	char * reqhost;
	if (!( nptr = locate_nagios_service_group_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   m i x i n 	*/
/*	--------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_service_group_post_mixin(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_service_group * pptr;
	char * reqhost;
	if (!( nptr = locate_nagios_service_group_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request"));
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   a c t i o n 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_service_group_post_action(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_action * fptr;
	struct occi_kind_node * nptr;
	struct nagios_service_group * pptr;
	char * reqhost;
	char * mptr;
	if (!( nptr = locate_nagios_service_group_node(id)))
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
private struct rest_response * nagios_service_group_post_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_service_group * pptr;
	char * reqhost;
	int    reqport=0;
	iptr = optr->callback;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else reqport = rptr->port;
	if (!( nptr = add_nagios_service_group_node(1)))
		return( rest_html_response( aptr, 500, "Server Failure") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (!( occi_process_atributs( optr, rptr,aptr, pptr, set_nagios_service_group_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->create)) (*iptr->create)(optr,nptr,rptr);
	autosave_nagios_service_group_nodes();
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
private struct rest_response * nagios_service_group_put_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_service_group * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_nagios_service_group_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (!( occi_process_atributs(optr,rptr,aptr, pptr, set_nagios_service_group_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->update)) (*iptr->update)(optr,nptr,rptr);
	autosave_nagios_service_group_nodes();
	return( nagios_service_group_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d   i t e m 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_service_group_head_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_kind_node * nptr;
	struct nagios_service_group * pptr;
	if (!( nptr = locate_nagios_service_group_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e   i t e m 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_service_group_delete_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_service_group * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_nagios_service_group_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->delete)) (*iptr->delete)(optr,nptr,rptr);
	drop_nagios_service_group_node( nptr );
	autosave_nagios_service_group_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t   l i s t 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_service_group_get_list(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_kind_node * sptr;
	struct nagios_service_group * pptr;
	struct nagios_service_group * fptr;
	char * reqhost;
	int reqport=0;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else reqport = rptr->port;
	if (!( fptr = filter_nagios_service_group_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	for ( sptr = nagios_service_group_first;
		sptr != (struct occi_kind_node *) 0;
		sptr = sptr->next ) {
		if (!( pptr = sptr->contents ))
			continue;
		if (!( pass_nagios_service_group_filter( pptr, fptr ) ))
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
private struct rest_response * nagios_service_group_delete_all(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct occi_kind_node * sptr;
	struct nagios_service_group * pptr;
	struct nagios_service_group * fptr;
	iptr = optr->callback;
	if (!( fptr = filter_nagios_service_group_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	nptr=nagios_service_group_first;
	while (nptr != (struct occi_kind_node *) 0) {
		if ((!( pptr = nptr->contents ))
		||  (!( pass_nagios_service_group_filter( pptr, fptr ) ))) {
			nptr = nptr->next;
			continue;
			}
		else	{
			if (( iptr ) && (iptr->delete)) { (*iptr->delete)(optr,nptr,rptr); }
			sptr = nptr->next;
			drop_nagios_service_group_node( nptr );
			nptr = sptr;
			}
		}
	autosave_nagios_service_group_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_service_group_get(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_service_group_get_list( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( nagios_service_group_get_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_service_group_head(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_service_group_head_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_service_group_post(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_service_group_post_item( optr, cptr, rptr, aptr ) );
	else if ( strncmp( rptr->object, optr->location,strlen(optr->location)) != 0)
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( rptr->parameters ))
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( strncmp( rptr->parameters, "action=", strlen("action=")) ))
		return( nagios_service_group_post_action( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "mixin=", strlen("mixin=")) ))
		return( nagios_service_group_post_mixin( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "link=", strlen("link=")) ))
		return( nagios_service_group_post_link( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p u t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_service_group_put(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_service_group_put_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e 	*/
/*	------------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_service_group_delete(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_service_group_delete_all( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( nagios_service_group_delete_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   r e d i r e c t i o n 	*/
/*	--------------------------------------------------------------------------------	*/
private void	redirect_occi_nagios_service_group_mt( struct rest_interface * iptr )
{
	iptr->get = occi_nagios_service_group_get;
	iptr->post = occi_nagios_service_group_post;
	iptr->put = occi_nagios_service_group_put;
	iptr->delete = occi_nagios_service_group_delete;
	iptr->head = occi_nagios_service_group_head;
	return;
}

/*	------------------------------------------	*/
/*	o c c i   c a t e g o r y   b u i l d e r 	*/
/*	------------------------------------------	*/
/* occi category rest instance builder for : occi_nagios_service_group */
public struct occi_category * occi_nagios_service_group_builder(char * a,char * b) {
	char * c="http://scheme.compatibleone.fr/scheme/compatible#";
	char * d="kind";
	char * e="http://scheme.ogf.org/occi/resource#";
	char * f="CompatibleOne OCCI resource nagios_service_group";
	struct occi_category * optr;
	if (!( optr = occi_create_category(a,b,c,d,e,f) )) { return(optr); }
	else {
		redirect_occi_nagios_service_group_mt(optr->interface);
		if (!( optr = occi_add_attribute(optr, "name",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "alias",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "members",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "servicegroup_members",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "notes",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "notes_url",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "action_url",0,0) ))
			return(optr);
		autoload_nagios_service_group_nodes();
		return(optr);
	}

}

/*	------------------------------------------------------------------	*/
/*	n a g i o s _ s e r v i c e _ g r o u p _ o c c i _ h e a d e r s 	*/
/*	------------------------------------------------------------------	*/
public struct rest_header *  nagios_service_group_occi_headers(struct nagios_service_group * sptr)
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
	sprintf(buffer,"nagios_service_group; scheme='http://scheme.compatibleone.fr/scheme/compatible#'; class='kind';\r\n");
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
	sprintf(buffer,"occi.nagios_service_group.name='%s'\r\n",(sptr->name?sptr->name:""));
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
	sprintf(buffer,"occi.nagios_service_group.alias='%s'\r\n",(sptr->alias?sptr->alias:""));
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
	sprintf(buffer,"occi.nagios_service_group.members='%u'\r\n",sptr->members);
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
	sprintf(buffer,"occi.nagios_service_group.servicegroup_members='%u'\r\n",sptr->servicegroup_members);
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
	sprintf(buffer,"occi.nagios_service_group.notes='%s'\r\n",(sptr->notes?sptr->notes:""));
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
	sprintf(buffer,"occi.nagios_service_group.notes_url='%s'\r\n",(sptr->notes_url?sptr->notes_url:""));
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
	sprintf(buffer,"occi.nagios_service_group.action_url='%s'\r\n",(sptr->action_url?sptr->action_url:""));
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	return(first);

}

#endif	/* _occinagiosservicegroup_c_ */
