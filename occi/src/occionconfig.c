/* ------------------------------------------------------------------------------------	*/
/*				 CompatibleOne Cloudware				*/
/* ------------------------------------------------------------------------------------ */
/*											*/
/* Ce fichier fait partie de ce(tte) oeuvre de Iain James Marshall et est mise a 	*/
/* disposition selon les termes de la licence Creative Commons Paternit‚ : 		*/
/*											*/
/*			 	Pas d'Utilisation Commerciale 				*/
/*				Pas de Modification 					*/
/*				3.0 non transcrit.					*/
/*											*/
/* ------------------------------------------------------------------------------------ */
/* 			Copyright (c) 2011 Iain James Marshall for Prologue 		*/
/*				   All rights reserved					*/
/* ------------------------------------------------------------------------------------ */
#ifndef _onconfig_c_
#define _onconfig_c_

#include "onconfig.h"

/*	----------------------------	*/
/*	o c c i _ o n _ c o n f i g 	*/
/*	----------------------------	*/

/*	--------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   m a n a g e m e n t   s t r u c t u r e 	*/
/*	--------------------------------------------------------------------	*/
struct on_config * allocate_on_config();
struct on_config * liberate_on_config(struct on_config * optr);
private pthread_mutex_t list_on_config_control=PTHREAD_MUTEX_INITIALIZER;
private struct occi_kind_node * on_config_first = (struct occi_kind_node *) 0;
private struct occi_kind_node * on_config_last  = (struct occi_kind_node *) 0;
public struct  occi_kind_node * occi_first_on_config_node() { return( on_config_first ); }

/*	----------------------------------------------	*/
/*	o c c i   c a t e g o r y   d r o p   n o d e 	*/
/*	----------------------------------------------	*/
private struct occi_kind_node * ll_drop_on_config_node(struct occi_kind_node * nptr) {
	if ( nptr ) {
	if (!( nptr->previous ))
		on_config_first = nptr->next;
	else	nptr->previous->next = nptr->next;
	if (!( nptr->next ))
		on_config_last = nptr->previous;
	else	nptr->next->previous = nptr->previous;
		liberate_occi_kind_node( nptr );
		}
	return((struct occi_kind_node *)0);
}
private struct occi_kind_node * drop_on_config_node(struct occi_kind_node * nptr) {
	pthread_mutex_lock( &list_on_config_control );
	nptr = ll_drop_on_config_node( nptr );
	pthread_mutex_unlock( &list_on_config_control );
	return(nptr);
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   l o c a t e   n o d e 	*/
/*	--------------------------------------------------	*/
private struct occi_kind_node * ll_locate_on_config_node(char * id) {
	struct occi_kind_node * nptr;
	struct on_config * pptr;
	for ( nptr = on_config_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		else if (!( pptr->id )) continue;
		else if (!( strcmp(pptr->id,id) )) break;
		}
	return( nptr );
}
private struct occi_kind_node * locate_on_config_node(char * id) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_on_config_control );
	nptr = ll_locate_on_config_node(id);
	pthread_mutex_unlock( &list_on_config_control );
	return( nptr );
}

/*	--------------------------------------------	*/
/*	o c c i   c a t e g o r y   a d d   n o d e 	*/
/*	--------------------------------------------	*/
private struct occi_kind_node * ll_add_on_config_node(int mode) {
	struct occi_kind_node * nptr;
	struct on_config * pptr;
	if (!( nptr = allocate_occi_kind_node() ))
		return( nptr );
	else	{
		if (!( nptr->contents = allocate_on_config()))
			return( liberate_occi_kind_node(nptr) );
		if (!( pptr = nptr->contents ))
			return( liberate_occi_kind_node(nptr) );
		else if (( mode != 0 ) && (!( pptr->id = occi_allocate_uuid())))
			return( liberate_occi_kind_node(nptr) );
		else	{
			if (!( nptr->previous = on_config_last ))
				on_config_first = nptr;
			else	nptr->previous->next = nptr;
			on_config_last = nptr;
			return( nptr );
			}
		}
}
private struct occi_kind_node * add_on_config_node(int mode) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_on_config_control );
	nptr = ll_add_on_config_node( mode );
	pthread_mutex_unlock( &list_on_config_control );
	return(nptr);
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   l o a d 	*/
/*	------------------------------------------------------------------------------------------	*/
private char*autosave_on_config_name="on_config.xml";
private void autoload_on_config_nodes() {
	char * fn=autosave_on_config_name;	struct occi_kind_node * nptr;
	struct on_config * pptr;
	struct xml_element * document;
	struct xml_element * eptr;
	struct xml_element * vptr;
	struct xml_atribut  * aptr;
	if (!( document = document_parse_file(fn)))
		return;
	if ((eptr = document_element(document,"on_configs")) != (struct xml_element *) 0) {
		for (vptr=eptr->first; vptr != (struct xml_element *) 0; vptr=vptr->next) {
			if (!( vptr->name )) continue;
			else if ( strcmp( vptr->name, "on_config" ) ) continue;
			else if (!( nptr = add_on_config_node(0))) break;
			else if (!( pptr = nptr->contents )) break;
			if ((aptr = document_atribut( vptr, "id" )) != (struct xml_atribut *) 0)
				pptr->id = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "description" )) != (struct xml_atribut *) 0)
				pptr->description = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "user" )) != (struct xml_atribut *) 0)
				pptr->user = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "password" )) != (struct xml_atribut *) 0)
				pptr->password = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "authenticate" )) != (struct xml_atribut *) 0)
				pptr->authenticate = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "agent" )) != (struct xml_atribut *) 0)
				pptr->agent = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "host" )) != (struct xml_atribut *) 0)
				pptr->host = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "version" )) != (struct xml_atribut *) 0)
				pptr->version = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "namespace" )) != (struct xml_atribut *) 0)
				pptr->namespace = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "base" )) != (struct xml_atribut *) 0)
				pptr->base = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "tls" )) != (struct xml_atribut *) 0)
				pptr->tls = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "current" )) != (struct xml_atribut *) 0)
				pptr->current = document_atribut_value(aptr);
			}
		}
	document = document_drop( document );
	return;
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   s a v e 	*/
/*	------------------------------------------------------------------------------------------	*/
public  void set_autosave_on_config_name(char * fn) {
	autosave_on_config_name = fn;	return;
}
public  void autosave_on_config_nodes() {
	char * fn=autosave_on_config_name;	struct occi_kind_node * nptr;
	struct on_config * pptr;
	FILE * h;
	pthread_mutex_lock( &list_on_config_control );
	if (( h = fopen(fn,"w")) != (FILE *) 0) {
	fprintf(h,"<on_configs>\n");
	for ( nptr = on_config_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		fprintf(h,"<on_config\n");
		fprintf(h," id=%c",0x0022);
		fprintf(h,"%s",(pptr->id?pptr->id:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," description=%c",0x0022);
		fprintf(h,"%s",(pptr->description?pptr->description:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," user=%c",0x0022);
		fprintf(h,"%s",(pptr->user?pptr->user:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," password=%c",0x0022);
		fprintf(h,"%s",(pptr->password?pptr->password:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," authenticate=%c",0x0022);
		fprintf(h,"%s",(pptr->authenticate?pptr->authenticate:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," agent=%c",0x0022);
		fprintf(h,"%s",(pptr->agent?pptr->agent:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," host=%c",0x0022);
		fprintf(h,"%s",(pptr->host?pptr->host:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," version=%c",0x0022);
		fprintf(h,"%s",(pptr->version?pptr->version:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," namespace=%c",0x0022);
		fprintf(h,"%s",(pptr->namespace?pptr->namespace:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," base=%c",0x0022);
		fprintf(h,"%s",(pptr->base?pptr->base:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," tls=%c",0x0022);
		fprintf(h,"%s",(pptr->tls?pptr->tls:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," current=%c",0x0022);
		fprintf(h,"%u",pptr->current);
		fprintf(h,"%c",0x0022);
		fprintf(h," />\n");
		}
	fprintf(h,"</on_configs>\n");
	fclose(h);
	}
	pthread_mutex_unlock( &list_on_config_control );
	return;
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   s e t   f i e l d 	*/
/*	------------------------------------------------------------------------------------------	*/
private void set_on_config_field(
	struct occi_category * cptr,void * optr, char * nptr, char * vptr)
{
	struct on_config * pptr;
	char prefix[1024];
	if (!( pptr = optr )) return;
	sprintf(prefix,"%s.%s.",cptr->domain,cptr->id);
	if (!( strncmp( nptr, prefix, strlen(prefix) ) )) {
		nptr += strlen(prefix);
		if (!( strcmp( nptr, "description" ) ))
			pptr->description = allocate_string(vptr);
		if (!( strcmp( nptr, "user" ) ))
			pptr->user = allocate_string(vptr);
		if (!( strcmp( nptr, "password" ) ))
			pptr->password = allocate_string(vptr);
		if (!( strcmp( nptr, "authenticate" ) ))
			pptr->authenticate = allocate_string(vptr);
		if (!( strcmp( nptr, "agent" ) ))
			pptr->agent = allocate_string(vptr);
		if (!( strcmp( nptr, "host" ) ))
			pptr->host = allocate_string(vptr);
		if (!( strcmp( nptr, "version" ) ))
			pptr->version = allocate_string(vptr);
		if (!( strcmp( nptr, "namespace" ) ))
			pptr->namespace = allocate_string(vptr);
		if (!( strcmp( nptr, "base" ) ))
			pptr->base = allocate_string(vptr);
		if (!( strcmp( nptr, "tls" ) ))
			pptr->tls = allocate_string(vptr);
		if (!( strcmp( nptr, "current" ) ))
			pptr->current = atoi(vptr);
		}
	return;
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   i n f o 	*/
/*	--------------------------------------------------	*/
private struct on_config * filter_on_config_info(
	struct occi_category * optr,
	struct rest_request  * rptr,
	struct rest_response * aptr) {
	struct on_config * pptr;
		if (!( pptr = allocate_on_config()))
		return( pptr );
	else if (!( occi_process_atributs(optr, rptr, aptr, pptr, set_on_config_field) ))
		return( liberate_on_config(pptr));
	else	return( pptr );
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   p a s s 	*/
/*	--------------------------------------------------	*/
private int pass_on_config_filter(
	struct on_config * pptr,struct on_config * fptr) {
	if (( fptr->id )
	&&  (strlen( fptr->id ) != 0)) {
		if (!( pptr->id ))
			return(0);
		else if ( strcmp(pptr->id,fptr->id) != 0)
			return(0);
		}
	if (( fptr->description )
	&&  (strlen( fptr->description ) != 0)) {
		if (!( pptr->description ))
			return(0);
		else if ( strcmp(pptr->description,fptr->description) != 0)
			return(0);
		}
	if (( fptr->user )
	&&  (strlen( fptr->user ) != 0)) {
		if (!( pptr->user ))
			return(0);
		else if ( strcmp(pptr->user,fptr->user) != 0)
			return(0);
		}
	if (( fptr->password )
	&&  (strlen( fptr->password ) != 0)) {
		if (!( pptr->password ))
			return(0);
		else if ( strcmp(pptr->password,fptr->password) != 0)
			return(0);
		}
	if (( fptr->authenticate )
	&&  (strlen( fptr->authenticate ) != 0)) {
		if (!( pptr->authenticate ))
			return(0);
		else if ( strcmp(pptr->authenticate,fptr->authenticate) != 0)
			return(0);
		}
	if (( fptr->agent )
	&&  (strlen( fptr->agent ) != 0)) {
		if (!( pptr->agent ))
			return(0);
		else if ( strcmp(pptr->agent,fptr->agent) != 0)
			return(0);
		}
	if (( fptr->host )
	&&  (strlen( fptr->host ) != 0)) {
		if (!( pptr->host ))
			return(0);
		else if ( strcmp(pptr->host,fptr->host) != 0)
			return(0);
		}
	if (( fptr->version )
	&&  (strlen( fptr->version ) != 0)) {
		if (!( pptr->version ))
			return(0);
		else if ( strcmp(pptr->version,fptr->version) != 0)
			return(0);
		}
	if (( fptr->namespace )
	&&  (strlen( fptr->namespace ) != 0)) {
		if (!( pptr->namespace ))
			return(0);
		else if ( strcmp(pptr->namespace,fptr->namespace) != 0)
			return(0);
		}
	if (( fptr->base )
	&&  (strlen( fptr->base ) != 0)) {
		if (!( pptr->base ))
			return(0);
		else if ( strcmp(pptr->base,fptr->base) != 0)
			return(0);
		}
	if (( fptr->tls )
	&&  (strlen( fptr->tls ) != 0)) {
		if (!( pptr->tls ))
			return(0);
		else if ( strcmp(pptr->tls,fptr->tls) != 0)
			return(0);
		}
	if (( fptr->current ) && ( pptr->current != fptr->current )) return(0);
	return(1);
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   r e s p o n s e 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * on_config_occi_response(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,
	struct on_config * pptr)
{
	struct rest_header * hptr;
	sprintf(cptr->buffer,"occi.core.id=%s",pptr->id);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.description=%s",optr->domain,optr->id,pptr->description);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.user=%s",optr->domain,optr->id,pptr->user);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.password=%s",optr->domain,optr->id,pptr->password);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.authenticate=%s",optr->domain,optr->id,pptr->authenticate);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.agent=%s",optr->domain,optr->id,pptr->agent);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.host=%s",optr->domain,optr->id,pptr->host);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.version=%s",optr->domain,optr->id,pptr->version);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.namespace=%s",optr->domain,optr->id,pptr->namespace);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.base=%s",optr->domain,optr->id,pptr->base);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.tls=%s",optr->domain,optr->id,pptr->tls);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.current=%u",optr->domain,optr->id,pptr->current);
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
private struct rest_response * on_config_get_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct on_config * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_on_config_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->retrieve)) (*iptr->retrieve)(optr,nptr);
	autosave_on_config_nodes();
	return( on_config_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   l i n k 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * on_config_post_link(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct on_config * pptr;
	char * reqhost;
	if (!( nptr = locate_on_config_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   m i x i n 	*/
/*	--------------------------------------------------------------------------------------------	*/
private struct rest_response * on_config_post_mixin(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct on_config * pptr;
	char * reqhost;
	if (!( nptr = locate_on_config_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request"));
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   a c t i o n 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * on_config_post_action(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_action * fptr;
	struct occi_kind_node * nptr;
	struct on_config * pptr;
	char * reqhost;
	char * mptr;
	if (!( nptr = locate_on_config_node(id)))
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
private struct rest_response * on_config_post_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct on_config * pptr;
	char * reqhost;
	iptr = optr->callback;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	if (!( nptr = add_on_config_node(1)))
		return( rest_html_response( aptr, 500, "Server Failure") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (!( occi_process_atributs( optr, rptr,aptr, pptr, set_on_config_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->create)) (*iptr->create)(optr,nptr);
	autosave_on_config_nodes();
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
private struct rest_response * on_config_put_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct on_config * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_on_config_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (!( occi_process_atributs(optr,rptr,aptr, pptr, set_on_config_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->update)) (*iptr->update)(optr,nptr);
	autosave_on_config_nodes();
	return( on_config_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d   i t e m 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * on_config_head_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_kind_node * nptr;
	struct on_config * pptr;
	if (!( nptr = locate_on_config_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e   i t e m 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * on_config_delete_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct on_config * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_on_config_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->delete)) (*iptr->delete)(optr,nptr);
	drop_on_config_node( nptr );
	autosave_on_config_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t   l i s t 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * on_config_get_list(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_kind_node * sptr;
	struct on_config * pptr;
	struct on_config * fptr;
	char * reqhost;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else if (!( fptr = filter_on_config_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	for ( sptr = on_config_first;
		sptr != (struct occi_kind_node *) 0;
		sptr = sptr->next ) {
		if (!( pptr = sptr->contents ))
			continue;
		if (!( pass_on_config_filter( pptr, fptr ) ))
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
private struct rest_response * on_config_delete_all(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct occi_kind_node * sptr;
	struct on_config * pptr;
	struct on_config * fptr;
	iptr = optr->callback;
	if (!( fptr = filter_on_config_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	nptr=on_config_first;
	while (nptr != (struct occi_kind_node *) 0) {
		if ((!( pptr = nptr->contents ))
		||  (!( pass_on_config_filter( pptr, fptr ) ))) {
			nptr = nptr->next;
			continue;
			}
		else	{
			if (( iptr ) && (iptr->delete)) { (*iptr->delete)(optr,nptr); }
			sptr = nptr->next;
			drop_on_config_node( nptr );
			nptr = sptr;
			}
		}
	autosave_on_config_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_on_config_get(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( on_config_get_list( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( on_config_get_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_on_config_head(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( on_config_head_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_on_config_post(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( on_config_post_item( optr, cptr, rptr, aptr ) );
	else if ( strncmp( rptr->object, optr->location,strlen(optr->location)) != 0)
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( rptr->parameters ))
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( strncmp( rptr->parameters, "action=", strlen("action=")) ))
		return( on_config_post_action( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "mixin=", strlen("mixin=")) ))
		return( on_config_post_mixin( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "link=", strlen("link=")) ))
		return( on_config_post_link( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p u t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_on_config_put(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( on_config_put_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e 	*/
/*	------------------------------------------------------------------------------------	*/
private struct rest_response * occi_on_config_delete(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( on_config_delete_all( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( on_config_delete_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   t a b l e 	*/
/*	--------------------------------------------------------------------	*/
private struct rest_interface occi_on_config_mt = {
	(void*) 0,
	(void*) 0,
	(void*) 0,
	occi_on_config_get,
	occi_on_config_post,
	occi_on_config_put,
	occi_on_config_delete,
	occi_on_config_head,
	(void*) 0
	};

/*	------------------------------------------	*/
/*	o c c i   c a t e g o r y   b u i l d e r 	*/
/*	------------------------------------------	*/
/* occi category rest instance builder for : occi_on_config */
public struct occi_category * occi_on_config_builder(char * a,char * b) {
	char * c="http://scheme.compatibleone.fr/scheme/compatible#";
	char * d="kind";
	char * e="http://scheme.ogf.org/occi/resource#";
	char * f="CompatibleOne OCCI resource on_config";
	struct occi_category * optr;
	if (!( optr = occi_create_category(a,b,c,d,e,f) )) { return(optr); }
	else {
		optr->interface = &occi_on_config_mt;
		if (!( optr = occi_add_attribute(optr, "description",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "user",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "password",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "authenticate",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "agent",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "host",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "version",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "namespace",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "base",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "tls",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "current",0,0) ))
			return(optr);
		autoload_on_config_nodes();
		return(optr);
	}

}

/*	--------------------------------------------	*/
/*	o n _ c o n f i g _ o c c i _ h e a d e r s 	*/
/*	--------------------------------------------	*/
public struct rest_header *  on_config_occi_headers(struct on_config * sptr)
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
	sprintf(buffer,"on_config; scheme='http://scheme.compatibleone.fr/scheme/compatible#'; class='kind';\r\n");
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
	sprintf(buffer,"occi.on_config.description='%s'\r\n",(sptr->description?sptr->description:""));
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
	sprintf(buffer,"occi.on_config.user='%s'\r\n",(sptr->user?sptr->user:""));
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
	sprintf(buffer,"occi.on_config.password='%s'\r\n",(sptr->password?sptr->password:""));
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
	sprintf(buffer,"occi.on_config.authenticate='%s'\r\n",(sptr->authenticate?sptr->authenticate:""));
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
	sprintf(buffer,"occi.on_config.agent='%s'\r\n",(sptr->agent?sptr->agent:""));
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
	sprintf(buffer,"occi.on_config.host='%s'\r\n",(sptr->host?sptr->host:""));
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
	sprintf(buffer,"occi.on_config.version='%s'\r\n",(sptr->version?sptr->version:""));
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
	sprintf(buffer,"occi.on_config.namespace='%s'\r\n",(sptr->namespace?sptr->namespace:""));
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
	sprintf(buffer,"occi.on_config.base='%s'\r\n",(sptr->base?sptr->base:""));
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
	sprintf(buffer,"occi.on_config.tls='%s'\r\n",(sptr->tls?sptr->tls:""));
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
	sprintf(buffer,"occi.on_config.current='%u'\r\n",sptr->current);
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	return(first);

}

#endif	/* _onconfig_c_ */
