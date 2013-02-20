/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _occipaas_environment_template_c_
#define _occipaas_environment_template_c_

#include "paas_environment_template.h"

/*	------------------------------------------------------------	*/
/*	o c c i _ p a a s _ e n v i r o n m e n t _ t e m p l a t e 	*/
/*	------------------------------------------------------------	*/

/*	--------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   m a n a g e m e n t   s t r u c t u r e 	*/
/*	--------------------------------------------------------------------	*/
struct paas_environment_template * allocate_paas_environment_template();
struct paas_environment_template * liberate_paas_environment_template(struct paas_environment_template * optr);
private pthread_mutex_t list_paas_environment_template_control=PTHREAD_MUTEX_INITIALIZER;
private struct occi_kind_node * paas_environment_template_first = (struct occi_kind_node *) 0;
private struct occi_kind_node * paas_environment_template_last  = (struct occi_kind_node *) 0;
public struct  occi_kind_node * occi_first_paas_environment_template_node() { return( paas_environment_template_first ); }
public struct  occi_kind_node * occi_last_paas_environment_template_node() { return( paas_environment_template_last ); }

/*	----------------------------------------------	*/
/*	o c c i   c a t e g o r y   d r o p   n o d e 	*/
/*	----------------------------------------------	*/
private struct occi_kind_node * ll_drop_paas_environment_template_node(struct occi_kind_node * nptr) {
	if ( nptr ) {
	if (!( nptr->previous ))
		paas_environment_template_first = nptr->next;
	else	nptr->previous->next = nptr->next;
	if (!( nptr->next ))
		paas_environment_template_last = nptr->previous;
	else	nptr->next->previous = nptr->previous;
		liberate_occi_kind_node( nptr );
		}
	return((struct occi_kind_node *)0);
}
private struct occi_kind_node * drop_paas_environment_template_node(struct occi_kind_node * nptr) {
	pthread_mutex_lock( &list_paas_environment_template_control );
	nptr = ll_drop_paas_environment_template_node( nptr );
	pthread_mutex_unlock( &list_paas_environment_template_control );
	return(nptr);
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   l o c a t e   n o d e 	*/
/*	--------------------------------------------------	*/
private struct occi_kind_node * ll_locate_paas_environment_template_node(char * id) {
	struct occi_kind_node * nptr;
	struct paas_environment_template * pptr;
	for ( nptr = paas_environment_template_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		else if (!( pptr->id )) continue;
		else if (!( strcmp(pptr->id,id) )) break;
		}
	return( nptr );
}
private struct occi_kind_node * locate_paas_environment_template_node(char * id) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_paas_environment_template_control );
	nptr = ll_locate_paas_environment_template_node(id);
	pthread_mutex_unlock( &list_paas_environment_template_control );
	return( nptr );
}

/*	--------------------------------------------	*/
/*	o c c i   c a t e g o r y   a d d   n o d e 	*/
/*	--------------------------------------------	*/
private struct occi_kind_node * ll_add_paas_environment_template_node(int mode) {
	struct occi_kind_node * nptr;
	struct paas_environment_template * pptr;
	if (!( nptr = allocate_occi_kind_node() ))
		return( nptr );
	else	{
		if (!( nptr->contents = allocate_paas_environment_template()))
			return( liberate_occi_kind_node(nptr) );
		if (!( pptr = nptr->contents ))
			return( liberate_occi_kind_node(nptr) );
		else if (( mode != 0 ) && (!( pptr->id = occi_allocate_uuid())))
			return( liberate_occi_kind_node(nptr) );
		else	{
			if (!( nptr->previous = paas_environment_template_last ))
				paas_environment_template_first = nptr;
			else	nptr->previous->next = nptr;
			paas_environment_template_last = nptr;
			return( nptr );
			}
		}
}
private struct occi_kind_node * add_paas_environment_template_node(int mode) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_paas_environment_template_control );
	nptr = ll_add_paas_environment_template_node( mode );
	pthread_mutex_unlock( &list_paas_environment_template_control );
	return(nptr);
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   l o a d 	*/
/*	------------------------------------------------------------------------------------------	*/
private char*autosave_paas_environment_template_name="paas_environment_template.xml";
private void autoload_paas_environment_template_nodes() {
	char * fn=autosave_paas_environment_template_name;	struct occi_kind_node * nptr;
	struct paas_environment_template * pptr;
	struct xml_element * document;
	struct xml_element * eptr;
	struct xml_element * vptr;
	struct xml_atribut  * aptr;
	if (!( document = document_parse_file(fn)))
		return;
	if ((eptr = document_element(document,"paas_environment_templates")) != (struct xml_element *) 0) {
		for (vptr=eptr->first; vptr != (struct xml_element *) 0; vptr=vptr->next) {
			if (!( vptr->name )) continue;
			else if ( strcmp( vptr->name, "paas_environment_template" ) ) continue;
			else if (!( nptr = add_paas_environment_template_node(0))) break;
			else if (!( pptr = nptr->contents )) break;
			if ((aptr = document_atribut( vptr, "id" )) != (struct xml_atribut *) 0)
				pptr->id = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "name" )) != (struct xml_atribut *) 0)
				pptr->name = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "memory" )) != (struct xml_atribut *) 0)
				pptr->memory = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "disk" )) != (struct xml_atribut *) 0)
				pptr->disk = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "description" )) != (struct xml_atribut *) 0)
				pptr->description = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "paas_environment_relation" )) != (struct xml_atribut *) 0)
				pptr->paas_environment_relation = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "paas_environment_configuration" )) != (struct xml_atribut *) 0)
				pptr->paas_environment_configuration = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "state" )) != (struct xml_atribut *) 0)
				pptr->state = document_atribut_value(aptr);
			}
		}
	document = document_drop( document );
	return;
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   s a v e 	*/
/*	------------------------------------------------------------------------------------------	*/
public  void set_autosave_paas_environment_template_name(char * fn) {
	autosave_paas_environment_template_name = fn;	return;
}
public  void autosave_paas_environment_template_nodes() {
	char * fn=autosave_paas_environment_template_name;	struct occi_kind_node * nptr;
	struct paas_environment_template * pptr;
	FILE * h;
	pthread_mutex_lock( &list_paas_environment_template_control );
	if (( h = fopen(fn,"w")) != (FILE *) 0) {
	fprintf(h,"<paas_environment_templates>\n");
	for ( nptr = paas_environment_template_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		fprintf(h,"<paas_environment_template\n");
		fprintf(h," id=%c",0x0022);
		fprintf(h,"%s",(pptr->id?pptr->id:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," name=%c",0x0022);
		fprintf(h,"%s",(pptr->name?pptr->name:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," memory=%c",0x0022);
		fprintf(h,"%s",(pptr->memory?pptr->memory:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," disk=%c",0x0022);
		fprintf(h,"%s",(pptr->disk?pptr->disk:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," description=%c",0x0022);
		fprintf(h,"%s",(pptr->description?pptr->description:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," paas_environment_relation=%c",0x0022);
		fprintf(h,"%s",(pptr->paas_environment_relation?pptr->paas_environment_relation:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," paas_environment_configuration=%c",0x0022);
		fprintf(h,"%s",(pptr->paas_environment_configuration?pptr->paas_environment_configuration:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," state=%c",0x0022);
		fprintf(h,"%u",pptr->state);
		fprintf(h,"%c",0x0022);
		fprintf(h," />\n");
		}
	fprintf(h,"</paas_environment_templates>\n");
	fclose(h);
	}
	pthread_mutex_unlock( &list_paas_environment_template_control );
	return;
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   s e t   f i e l d 	*/
/*	------------------------------------------------------------------------------------------	*/
private void set_paas_environment_template_field(
	struct occi_category * cptr,void * optr, char * nptr, char * vptr)
{
	struct paas_environment_template * pptr;
	char prefix[1024];
	if (!( pptr = optr )) return;
	sprintf(prefix,"%s.%s.",cptr->domain,cptr->id);
	if (!( strncmp( nptr, prefix, strlen(prefix) ) )) {
		nptr += strlen(prefix);
		if (!( strcmp( nptr, "name" ) ))
			pptr->name = allocate_string(vptr);
		if (!( strcmp( nptr, "memory" ) ))
			pptr->memory = allocate_string(vptr);
		if (!( strcmp( nptr, "disk" ) ))
			pptr->disk = allocate_string(vptr);
		if (!( strcmp( nptr, "description" ) ))
			pptr->description = allocate_string(vptr);
		if (!( strcmp( nptr, "paas_environment_relation" ) ))
			pptr->paas_environment_relation = allocate_string(vptr);
		if (!( strcmp( nptr, "paas_environment_configuration" ) ))
			pptr->paas_environment_configuration = allocate_string(vptr);
		if (!( strcmp( nptr, "state" ) ))
			pptr->state = atoi(vptr);
		}
	return;
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   i n f o 	*/
/*	--------------------------------------------------	*/
private struct paas_environment_template * filter_paas_environment_template_info(
	struct occi_category * optr,
	struct rest_request  * rptr,
	struct rest_response * aptr) {
	struct paas_environment_template * pptr;
		if (!( pptr = allocate_paas_environment_template()))
		return( pptr );
	else if (!( occi_process_atributs(optr, rptr, aptr, pptr, set_paas_environment_template_field) ))
		return( liberate_paas_environment_template(pptr));
	else	return( pptr );
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   p a s s 	*/
/*	--------------------------------------------------	*/
private int pass_paas_environment_template_filter(
	struct paas_environment_template * pptr,struct paas_environment_template * fptr) {
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
	if (( fptr->memory )
	&&  (strlen( fptr->memory ) != 0)) {
		if (!( pptr->memory ))
			return(0);
		else if ( strcmp(pptr->memory,fptr->memory) != 0)
			return(0);
		}
	if (( fptr->disk )
	&&  (strlen( fptr->disk ) != 0)) {
		if (!( pptr->disk ))
			return(0);
		else if ( strcmp(pptr->disk,fptr->disk) != 0)
			return(0);
		}
	if (( fptr->description )
	&&  (strlen( fptr->description ) != 0)) {
		if (!( pptr->description ))
			return(0);
		else if ( strcmp(pptr->description,fptr->description) != 0)
			return(0);
		}
	if (( fptr->paas_environment_relation )
	&&  (strlen( fptr->paas_environment_relation ) != 0)) {
		if (!( pptr->paas_environment_relation ))
			return(0);
		else if ( strcmp(pptr->paas_environment_relation,fptr->paas_environment_relation) != 0)
			return(0);
		}
	if (( fptr->paas_environment_configuration )
	&&  (strlen( fptr->paas_environment_configuration ) != 0)) {
		if (!( pptr->paas_environment_configuration ))
			return(0);
		else if ( strcmp(pptr->paas_environment_configuration,fptr->paas_environment_configuration) != 0)
			return(0);
		}
	if (( fptr->state ) && ( pptr->state != fptr->state )) return(0);
	return(1);
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   r e s p o n s e 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * paas_environment_template_occi_response(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,
	struct paas_environment_template * pptr)
{
	struct rest_header * hptr;
	sprintf(cptr->buffer,"occi.core.id=%c%s%c",0x0022,pptr->id,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.name=%c%s%c",optr->domain,optr->id,0x0022,pptr->name,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.memory=%c%s%c",optr->domain,optr->id,0x0022,pptr->memory,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.disk=%c%s%c",optr->domain,optr->id,0x0022,pptr->disk,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.description=%c%s%c",optr->domain,optr->id,0x0022,pptr->description,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.paas_environment_relation=%c%s%c",optr->domain,optr->id,0x0022,pptr->paas_environment_relation,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.paas_environment_configuration=%c%s%c",optr->domain,optr->id,0x0022,pptr->paas_environment_configuration,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.state=%c%u%c",optr->domain,optr->id,0x0022,pptr->state,0x0022);
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
private struct rest_response * paas_environment_template_get_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct paas_environment_template * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_paas_environment_template_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->retrieve)) (*iptr->retrieve)(optr,nptr,rptr);
	autosave_paas_environment_template_nodes();
	return( paas_environment_template_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   l i n k 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * paas_environment_template_post_link(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct paas_environment_template * pptr;
	char * reqhost;
	if (!( nptr = locate_paas_environment_template_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   m i x i n 	*/
/*	--------------------------------------------------------------------------------------------	*/
private struct rest_response * paas_environment_template_post_mixin(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct paas_environment_template * pptr;
	char * reqhost;
	if (!( nptr = locate_paas_environment_template_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request"));
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   a c t i o n 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * paas_environment_template_post_action(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_action * fptr;
	struct occi_kind_node * nptr;
	struct paas_environment_template * pptr;
	char * reqhost;
	char * mptr;
	if (!( nptr = locate_paas_environment_template_node(id)))
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
private struct rest_response * paas_environment_template_post_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct paas_environment_template * pptr;
	char * reqhost;
	int    reqport=0;
	iptr = optr->callback;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else reqport = rptr->port;
	if (!( nptr = add_paas_environment_template_node(1)))
		return( rest_html_response( aptr, 500, "Server Failure") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (!( occi_process_atributs( optr, rptr,aptr, pptr, set_paas_environment_template_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->create)) (*iptr->create)(optr,nptr,rptr);
	autosave_paas_environment_template_nodes();
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
private struct rest_response * paas_environment_template_put_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct paas_environment_template * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_paas_environment_template_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (!( occi_process_atributs(optr,rptr,aptr, pptr, set_paas_environment_template_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->update)) (*iptr->update)(optr,nptr,rptr);
	autosave_paas_environment_template_nodes();
	return( paas_environment_template_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d   i t e m 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * paas_environment_template_head_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_kind_node * nptr;
	struct paas_environment_template * pptr;
	if (!( nptr = locate_paas_environment_template_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e   i t e m 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * paas_environment_template_delete_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct paas_environment_template * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_paas_environment_template_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->delete)) (*iptr->delete)(optr,nptr,rptr);
	drop_paas_environment_template_node( nptr );
	autosave_paas_environment_template_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t   l i s t 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * paas_environment_template_get_list(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_kind_node * sptr;
	struct paas_environment_template * pptr;
	struct paas_environment_template * fptr;
	char * reqhost;
	int reqport=0;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else reqport = rptr->port;
	if (!( fptr = filter_paas_environment_template_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	for ( sptr = paas_environment_template_first;
		sptr != (struct occi_kind_node *) 0;
		sptr = sptr->next ) {
		if (!( pptr = sptr->contents ))
			continue;
		if (!( pass_paas_environment_template_filter( pptr, fptr ) ))
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
private struct rest_response * paas_environment_template_delete_all(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct occi_kind_node * sptr;
	struct paas_environment_template * pptr;
	struct paas_environment_template * fptr;
	iptr = optr->callback;
	if (!( fptr = filter_paas_environment_template_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	nptr=paas_environment_template_first;
	while (nptr != (struct occi_kind_node *) 0) {
		if ((!( pptr = nptr->contents ))
		||  (!( pass_paas_environment_template_filter( pptr, fptr ) ))) {
			nptr = nptr->next;
			continue;
			}
		else	{
			if (( iptr ) && (iptr->delete)) { (*iptr->delete)(optr,nptr,rptr); }
			sptr = nptr->next;
			drop_paas_environment_template_node( nptr );
			nptr = sptr;
			}
		}
	autosave_paas_environment_template_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_paas_environment_template_get(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( paas_environment_template_get_list( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( paas_environment_template_get_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_paas_environment_template_head(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( paas_environment_template_head_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_paas_environment_template_post(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( paas_environment_template_post_item( optr, cptr, rptr, aptr ) );
	else if ( strncmp( rptr->object, optr->location,strlen(optr->location)) != 0)
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( rptr->parameters ))
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( strncmp( rptr->parameters, "action=", strlen("action=")) ))
		return( paas_environment_template_post_action( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "mixin=", strlen("mixin=")) ))
		return( paas_environment_template_post_mixin( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "link=", strlen("link=")) ))
		return( paas_environment_template_post_link( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p u t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_paas_environment_template_put(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( paas_environment_template_put_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e 	*/
/*	------------------------------------------------------------------------------------	*/
private struct rest_response * occi_paas_environment_template_delete(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( paas_environment_template_delete_all( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( paas_environment_template_delete_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   r e d i r e c t i o n 	*/
/*	--------------------------------------------------------------------------------	*/
private void	redirect_occi_paas_environment_template_mt( struct rest_interface * iptr )
{
	iptr->get = occi_paas_environment_template_get;
	iptr->post = occi_paas_environment_template_post;
	iptr->put = occi_paas_environment_template_put;
	iptr->delete = occi_paas_environment_template_delete;
	iptr->head = occi_paas_environment_template_head;
	return;
}

/*	------------------------------------------	*/
/*	o c c i   c a t e g o r y   b u i l d e r 	*/
/*	------------------------------------------	*/
/* occi category rest instance builder for : occi_paas_environment_template */
public struct occi_category * occi_paas_environment_template_builder(char * a,char * b) {
	char * c="http://scheme.compatibleone.fr/scheme/compatible#";
	char * d="kind";
	char * e="http://scheme.ogf.org/occi/resource#";
	char * f="CompatibleOne OCCI resource paas_environment_template";
	struct occi_category * optr;
	if (!( optr = occi_create_category(a,b,c,d,e,f) )) { return(optr); }
	else {
		redirect_occi_paas_environment_template_mt(optr->interface);
		if (!( optr = occi_add_attribute(optr, "name",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "memory",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "disk",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "description",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "paas_environment_relation",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "paas_environment_configuration",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "state",0,0) ))
			return(optr);
		autoload_paas_environment_template_nodes();
		return(optr);
	}

}

/*	----------------------------------------------------------------------------	*/
/*	p a a s _ e n v i r o n m e n t _ t e m p l a t e _ o c c i _ h e a d e r s 	*/
/*	----------------------------------------------------------------------------	*/
public struct rest_header *  paas_environment_template_occi_headers(struct paas_environment_template * sptr)
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
	sprintf(buffer,"paas_environment_template; scheme='http://scheme.compatibleone.fr/scheme/compatible#'; class='kind';\r\n");
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
	sprintf(buffer,"occi.paas_environment_template.name='%s'\r\n",(sptr->name?sptr->name:""));
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
	sprintf(buffer,"occi.paas_environment_template.memory='%s'\r\n",(sptr->memory?sptr->memory:""));
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
	sprintf(buffer,"occi.paas_environment_template.disk='%s'\r\n",(sptr->disk?sptr->disk:""));
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
	sprintf(buffer,"occi.paas_environment_template.description='%s'\r\n",(sptr->description?sptr->description:""));
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
	sprintf(buffer,"occi.paas_environment_template.paas_environment_relation='%s'\r\n",(sptr->paas_environment_relation?sptr->paas_environment_relation:""));
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
	sprintf(buffer,"occi.paas_environment_template.paas_environment_configuration='%s'\r\n",(sptr->paas_environment_configuration?sptr->paas_environment_configuration:""));
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
	sprintf(buffer,"occi.paas_environment_template.state='%u'\r\n",sptr->state);
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	return(first);

}

#endif	/* _occipaas_environment_template_c_ */
