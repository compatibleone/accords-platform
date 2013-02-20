/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _occinagioshostextinfo_c_
#define _occinagioshostextinfo_c_

#include "nagioshostextinfo.h"

/*	--------------------------------------------------	*/
/*	o c c i _ n a g i o s _ h o s t _ e x t _ i n f o 	*/
/*	--------------------------------------------------	*/

/*	--------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   m a n a g e m e n t   s t r u c t u r e 	*/
/*	--------------------------------------------------------------------	*/
struct nagios_host_ext_info * allocate_nagios_host_ext_info();
struct nagios_host_ext_info * liberate_nagios_host_ext_info(struct nagios_host_ext_info * optr);
private pthread_mutex_t list_nagios_host_ext_info_control=PTHREAD_MUTEX_INITIALIZER;
private struct occi_kind_node * nagios_host_ext_info_first = (struct occi_kind_node *) 0;
private struct occi_kind_node * nagios_host_ext_info_last  = (struct occi_kind_node *) 0;
public struct  occi_kind_node * occi_first_nagios_host_ext_info_node() { return( nagios_host_ext_info_first ); }
public struct  occi_kind_node * occi_last_nagios_host_ext_info_node() { return( nagios_host_ext_info_last ); }

/*	----------------------------------------------	*/
/*	o c c i   c a t e g o r y   d r o p   n o d e 	*/
/*	----------------------------------------------	*/
private struct occi_kind_node * ll_drop_nagios_host_ext_info_node(struct occi_kind_node * nptr) {
	if ( nptr ) {
	if (!( nptr->previous ))
		nagios_host_ext_info_first = nptr->next;
	else	nptr->previous->next = nptr->next;
	if (!( nptr->next ))
		nagios_host_ext_info_last = nptr->previous;
	else	nptr->next->previous = nptr->previous;
		liberate_occi_kind_node( nptr );
		}
	return((struct occi_kind_node *)0);
}
private struct occi_kind_node * drop_nagios_host_ext_info_node(struct occi_kind_node * nptr) {
	pthread_mutex_lock( &list_nagios_host_ext_info_control );
	nptr = ll_drop_nagios_host_ext_info_node( nptr );
	pthread_mutex_unlock( &list_nagios_host_ext_info_control );
	return(nptr);
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   l o c a t e   n o d e 	*/
/*	--------------------------------------------------	*/
private struct occi_kind_node * ll_locate_nagios_host_ext_info_node(char * id) {
	struct occi_kind_node * nptr;
	struct nagios_host_ext_info * pptr;
	for ( nptr = nagios_host_ext_info_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		else if (!( pptr->id )) continue;
		else if (!( strcmp(pptr->id,id) )) break;
		}
	return( nptr );
}
private struct occi_kind_node * locate_nagios_host_ext_info_node(char * id) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_nagios_host_ext_info_control );
	nptr = ll_locate_nagios_host_ext_info_node(id);
	pthread_mutex_unlock( &list_nagios_host_ext_info_control );
	return( nptr );
}

/*	--------------------------------------------	*/
/*	o c c i   c a t e g o r y   a d d   n o d e 	*/
/*	--------------------------------------------	*/
private struct occi_kind_node * ll_add_nagios_host_ext_info_node(int mode) {
	struct occi_kind_node * nptr;
	struct nagios_host_ext_info * pptr;
	if (!( nptr = allocate_occi_kind_node() ))
		return( nptr );
	else	{
		if (!( nptr->contents = allocate_nagios_host_ext_info()))
			return( liberate_occi_kind_node(nptr) );
		if (!( pptr = nptr->contents ))
			return( liberate_occi_kind_node(nptr) );
		else if (( mode != 0 ) && (!( pptr->id = occi_allocate_uuid())))
			return( liberate_occi_kind_node(nptr) );
		else	{
			if (!( nptr->previous = nagios_host_ext_info_last ))
				nagios_host_ext_info_first = nptr;
			else	nptr->previous->next = nptr;
			nagios_host_ext_info_last = nptr;
			return( nptr );
			}
		}
}
private struct occi_kind_node * add_nagios_host_ext_info_node(int mode) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_nagios_host_ext_info_control );
	nptr = ll_add_nagios_host_ext_info_node( mode );
	pthread_mutex_unlock( &list_nagios_host_ext_info_control );
	return(nptr);
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   l o a d 	*/
/*	------------------------------------------------------------------------------------------	*/
private char*autosave_nagios_host_ext_info_name="nagios_host_ext_info.xml";
private void autoload_nagios_host_ext_info_nodes() {
	char * fn=autosave_nagios_host_ext_info_name;	struct occi_kind_node * nptr;
	struct nagios_host_ext_info * pptr;
	struct xml_element * document;
	struct xml_element * eptr;
	struct xml_element * vptr;
	struct xml_atribut  * aptr;
	if (!( document = document_parse_file(fn)))
		return;
	if ((eptr = document_element(document,"nagios_host_ext_infos")) != (struct xml_element *) 0) {
		for (vptr=eptr->first; vptr != (struct xml_element *) 0; vptr=vptr->next) {
			if (!( vptr->name )) continue;
			else if ( strcmp( vptr->name, "nagios_host_ext_info" ) ) continue;
			else if (!( nptr = add_nagios_host_ext_info_node(0))) break;
			else if (!( pptr = nptr->contents )) break;
			if ((aptr = document_atribut( vptr, "id" )) != (struct xml_atribut *) 0)
				pptr->id = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "name" )) != (struct xml_atribut *) 0)
				pptr->name = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "host_name" )) != (struct xml_atribut *) 0)
				pptr->host_name = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "notes" )) != (struct xml_atribut *) 0)
				pptr->notes = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "notes_url" )) != (struct xml_atribut *) 0)
				pptr->notes_url = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "action_url" )) != (struct xml_atribut *) 0)
				pptr->action_url = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "icon_image" )) != (struct xml_atribut *) 0)
				pptr->icon_image = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "icon_image_alt" )) != (struct xml_atribut *) 0)
				pptr->icon_image_alt = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "vrml_image" )) != (struct xml_atribut *) 0)
				pptr->vrml_image = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "statusmap_image" )) != (struct xml_atribut *) 0)
				pptr->statusmap_image = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "twod_coords" )) != (struct xml_atribut *) 0)
				pptr->twod_coords = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "threed_coords" )) != (struct xml_atribut *) 0)
				pptr->threed_coords = document_atribut_string(aptr);
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
public  void set_autosave_nagios_host_ext_info_name(char * fn) {
	autosave_nagios_host_ext_info_name = fn;	return;
}
public  void autosave_nagios_host_ext_info_nodes() {
	char * fn=autosave_nagios_host_ext_info_name;	struct occi_kind_node * nptr;
	struct nagios_host_ext_info * pptr;
	FILE * h;
	pthread_mutex_lock( &list_nagios_host_ext_info_control );
	if (( h = fopen(fn,"w")) != (FILE *) 0) {
	fprintf(h,"<nagios_host_ext_infos>\n");
	for ( nptr = nagios_host_ext_info_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		fprintf(h,"<nagios_host_ext_info\n");
		fprintf(h," id=%c",0x0022);
		fprintf(h,"%s",(pptr->id?pptr->id:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," name=%c",0x0022);
		fprintf(h,"%s",(pptr->name?pptr->name:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," host_name=%c",0x0022);
		fprintf(h,"%s",(pptr->host_name?pptr->host_name:""));
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
		fprintf(h," icon_image=%c",0x0022);
		fprintf(h,"%s",(pptr->icon_image?pptr->icon_image:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," icon_image_alt=%c",0x0022);
		fprintf(h,"%s",(pptr->icon_image_alt?pptr->icon_image_alt:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," vrml_image=%c",0x0022);
		fprintf(h,"%s",(pptr->vrml_image?pptr->vrml_image:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," statusmap_image=%c",0x0022);
		fprintf(h,"%s",(pptr->statusmap_image?pptr->statusmap_image:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," twod_coords=%c",0x0022);
		fprintf(h,"%s",(pptr->twod_coords?pptr->twod_coords:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," threed_coords=%c",0x0022);
		fprintf(h,"%s",(pptr->threed_coords?pptr->threed_coords:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," status=%c",0x0022);
		fprintf(h,"%u",pptr->status);
		fprintf(h,"%c",0x0022);
		fprintf(h," />\n");
		}
	fprintf(h,"</nagios_host_ext_infos>\n");
	fclose(h);
	}
	pthread_mutex_unlock( &list_nagios_host_ext_info_control );
	return;
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   s e t   f i e l d 	*/
/*	------------------------------------------------------------------------------------------	*/
private void set_nagios_host_ext_info_field(
	struct occi_category * cptr,void * optr, char * nptr, char * vptr)
{
	struct nagios_host_ext_info * pptr;
	char prefix[1024];
	if (!( pptr = optr )) return;
	sprintf(prefix,"%s.%s.",cptr->domain,cptr->id);
	if (!( strncmp( nptr, prefix, strlen(prefix) ) )) {
		nptr += strlen(prefix);
		if (!( strcmp( nptr, "name" ) ))
			pptr->name = allocate_string(vptr);
		if (!( strcmp( nptr, "host_name" ) ))
			pptr->host_name = allocate_string(vptr);
		if (!( strcmp( nptr, "notes" ) ))
			pptr->notes = allocate_string(vptr);
		if (!( strcmp( nptr, "notes_url" ) ))
			pptr->notes_url = allocate_string(vptr);
		if (!( strcmp( nptr, "action_url" ) ))
			pptr->action_url = allocate_string(vptr);
		if (!( strcmp( nptr, "icon_image" ) ))
			pptr->icon_image = allocate_string(vptr);
		if (!( strcmp( nptr, "icon_image_alt" ) ))
			pptr->icon_image_alt = allocate_string(vptr);
		if (!( strcmp( nptr, "vrml_image" ) ))
			pptr->vrml_image = allocate_string(vptr);
		if (!( strcmp( nptr, "statusmap_image" ) ))
			pptr->statusmap_image = allocate_string(vptr);
		if (!( strcmp( nptr, "twod_coords" ) ))
			pptr->twod_coords = allocate_string(vptr);
		if (!( strcmp( nptr, "threed_coords" ) ))
			pptr->threed_coords = allocate_string(vptr);
		if (!( strcmp( nptr, "status" ) ))
			pptr->status = atoi(vptr);
		}
	return;
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   i n f o 	*/
/*	--------------------------------------------------	*/
private struct nagios_host_ext_info * filter_nagios_host_ext_info_info(
	struct occi_category * optr,
	struct rest_request  * rptr,
	struct rest_response * aptr) {
	struct nagios_host_ext_info * pptr;
		if (!( pptr = allocate_nagios_host_ext_info()))
		return( pptr );
	else if (!( occi_process_atributs(optr, rptr, aptr, pptr, set_nagios_host_ext_info_field) ))
		return( liberate_nagios_host_ext_info(pptr));
	else	return( pptr );
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   p a s s 	*/
/*	--------------------------------------------------	*/
private int pass_nagios_host_ext_info_filter(
	struct nagios_host_ext_info * pptr,struct nagios_host_ext_info * fptr) {
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
	if (( fptr->host_name )
	&&  (strlen( fptr->host_name ) != 0)) {
		if (!( pptr->host_name ))
			return(0);
		else if ( strcmp(pptr->host_name,fptr->host_name) != 0)
			return(0);
		}
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
	if (( fptr->icon_image )
	&&  (strlen( fptr->icon_image ) != 0)) {
		if (!( pptr->icon_image ))
			return(0);
		else if ( strcmp(pptr->icon_image,fptr->icon_image) != 0)
			return(0);
		}
	if (( fptr->icon_image_alt )
	&&  (strlen( fptr->icon_image_alt ) != 0)) {
		if (!( pptr->icon_image_alt ))
			return(0);
		else if ( strcmp(pptr->icon_image_alt,fptr->icon_image_alt) != 0)
			return(0);
		}
	if (( fptr->vrml_image )
	&&  (strlen( fptr->vrml_image ) != 0)) {
		if (!( pptr->vrml_image ))
			return(0);
		else if ( strcmp(pptr->vrml_image,fptr->vrml_image) != 0)
			return(0);
		}
	if (( fptr->statusmap_image )
	&&  (strlen( fptr->statusmap_image ) != 0)) {
		if (!( pptr->statusmap_image ))
			return(0);
		else if ( strcmp(pptr->statusmap_image,fptr->statusmap_image) != 0)
			return(0);
		}
	if (( fptr->twod_coords )
	&&  (strlen( fptr->twod_coords ) != 0)) {
		if (!( pptr->twod_coords ))
			return(0);
		else if ( strcmp(pptr->twod_coords,fptr->twod_coords) != 0)
			return(0);
		}
	if (( fptr->threed_coords )
	&&  (strlen( fptr->threed_coords ) != 0)) {
		if (!( pptr->threed_coords ))
			return(0);
		else if ( strcmp(pptr->threed_coords,fptr->threed_coords) != 0)
			return(0);
		}
	if (( fptr->status ) && ( pptr->status != fptr->status )) return(0);
	return(1);
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   r e s p o n s e 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_host_ext_info_occi_response(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,
	struct nagios_host_ext_info * pptr)
{
	struct rest_header * hptr;
	sprintf(cptr->buffer,"occi.core.id=%c%s%c",0x0022,pptr->id,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.name=%c%s%c",optr->domain,optr->id,0x0022,pptr->name,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.host_name=%c%s%c",optr->domain,optr->id,0x0022,pptr->host_name,0x0022);
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
	sprintf(cptr->buffer,"%s.%s.icon_image=%c%s%c",optr->domain,optr->id,0x0022,pptr->icon_image,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.icon_image_alt=%c%s%c",optr->domain,optr->id,0x0022,pptr->icon_image_alt,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.vrml_image=%c%s%c",optr->domain,optr->id,0x0022,pptr->vrml_image,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.statusmap_image=%c%s%c",optr->domain,optr->id,0x0022,pptr->statusmap_image,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.twod_coords=%c%s%c",optr->domain,optr->id,0x0022,pptr->twod_coords,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.threed_coords=%c%s%c",optr->domain,optr->id,0x0022,pptr->threed_coords,0x0022);
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
private struct rest_response * nagios_host_ext_info_get_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_host_ext_info * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_nagios_host_ext_info_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->retrieve)) (*iptr->retrieve)(optr,nptr,rptr);
	autosave_nagios_host_ext_info_nodes();
	return( nagios_host_ext_info_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   l i n k 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_host_ext_info_post_link(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_host_ext_info * pptr;
	char * reqhost;
	if (!( nptr = locate_nagios_host_ext_info_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   m i x i n 	*/
/*	--------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_host_ext_info_post_mixin(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_host_ext_info * pptr;
	char * reqhost;
	if (!( nptr = locate_nagios_host_ext_info_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request"));
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   a c t i o n 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_host_ext_info_post_action(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_action * fptr;
	struct occi_kind_node * nptr;
	struct nagios_host_ext_info * pptr;
	char * reqhost;
	char * mptr;
	if (!( nptr = locate_nagios_host_ext_info_node(id)))
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
private struct rest_response * nagios_host_ext_info_post_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_host_ext_info * pptr;
	char * reqhost;
	int    reqport=0;
	iptr = optr->callback;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else reqport = rptr->port;
	if (!( nptr = add_nagios_host_ext_info_node(1)))
		return( rest_html_response( aptr, 500, "Server Failure") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (!( occi_process_atributs( optr, rptr,aptr, pptr, set_nagios_host_ext_info_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->create)) (*iptr->create)(optr,nptr,rptr);
	autosave_nagios_host_ext_info_nodes();
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
private struct rest_response * nagios_host_ext_info_put_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_host_ext_info * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_nagios_host_ext_info_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (!( occi_process_atributs(optr,rptr,aptr, pptr, set_nagios_host_ext_info_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->update)) (*iptr->update)(optr,nptr,rptr);
	autosave_nagios_host_ext_info_nodes();
	return( nagios_host_ext_info_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d   i t e m 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_host_ext_info_head_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_kind_node * nptr;
	struct nagios_host_ext_info * pptr;
	if (!( nptr = locate_nagios_host_ext_info_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e   i t e m 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_host_ext_info_delete_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_host_ext_info * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_nagios_host_ext_info_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->delete)) (*iptr->delete)(optr,nptr,rptr);
	drop_nagios_host_ext_info_node( nptr );
	autosave_nagios_host_ext_info_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t   l i s t 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_host_ext_info_get_list(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_kind_node * sptr;
	struct nagios_host_ext_info * pptr;
	struct nagios_host_ext_info * fptr;
	char * reqhost;
	int reqport=0;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else reqport = rptr->port;
	if (!( fptr = filter_nagios_host_ext_info_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	for ( sptr = nagios_host_ext_info_first;
		sptr != (struct occi_kind_node *) 0;
		sptr = sptr->next ) {
		if (!( pptr = sptr->contents ))
			continue;
		if (!( pass_nagios_host_ext_info_filter( pptr, fptr ) ))
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
private struct rest_response * nagios_host_ext_info_delete_all(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct occi_kind_node * sptr;
	struct nagios_host_ext_info * pptr;
	struct nagios_host_ext_info * fptr;
	iptr = optr->callback;
	if (!( fptr = filter_nagios_host_ext_info_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	nptr=nagios_host_ext_info_first;
	while (nptr != (struct occi_kind_node *) 0) {
		if ((!( pptr = nptr->contents ))
		||  (!( pass_nagios_host_ext_info_filter( pptr, fptr ) ))) {
			nptr = nptr->next;
			continue;
			}
		else	{
			if (( iptr ) && (iptr->delete)) { (*iptr->delete)(optr,nptr,rptr); }
			sptr = nptr->next;
			drop_nagios_host_ext_info_node( nptr );
			nptr = sptr;
			}
		}
	autosave_nagios_host_ext_info_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_host_ext_info_get(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_host_ext_info_get_list( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( nagios_host_ext_info_get_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_host_ext_info_head(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_host_ext_info_head_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_host_ext_info_post(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_host_ext_info_post_item( optr, cptr, rptr, aptr ) );
	else if ( strncmp( rptr->object, optr->location,strlen(optr->location)) != 0)
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( rptr->parameters ))
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( strncmp( rptr->parameters, "action=", strlen("action=")) ))
		return( nagios_host_ext_info_post_action( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "mixin=", strlen("mixin=")) ))
		return( nagios_host_ext_info_post_mixin( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "link=", strlen("link=")) ))
		return( nagios_host_ext_info_post_link( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p u t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_host_ext_info_put(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_host_ext_info_put_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e 	*/
/*	------------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_host_ext_info_delete(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_host_ext_info_delete_all( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( nagios_host_ext_info_delete_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   r e d i r e c t i o n 	*/
/*	--------------------------------------------------------------------------------	*/
private void	redirect_occi_nagios_host_ext_info_mt( struct rest_interface * iptr )
{
	iptr->get = occi_nagios_host_ext_info_get;
	iptr->post = occi_nagios_host_ext_info_post;
	iptr->put = occi_nagios_host_ext_info_put;
	iptr->delete = occi_nagios_host_ext_info_delete;
	iptr->head = occi_nagios_host_ext_info_head;
	return;
}

/*	------------------------------------------	*/
/*	o c c i   c a t e g o r y   b u i l d e r 	*/
/*	------------------------------------------	*/
/* occi category rest instance builder for : occi_nagios_host_ext_info */
public struct occi_category * occi_nagios_host_ext_info_builder(char * a,char * b) {
	char * c="http://scheme.compatibleone.fr/scheme/compatible#";
	char * d="kind";
	char * e="http://scheme.ogf.org/occi/resource#";
	char * f="CompatibleOne OCCI resource nagios_host_ext_info";
	struct occi_category * optr;
	if (!( optr = occi_create_category(a,b,c,d,e,f) )) { return(optr); }
	else {
		redirect_occi_nagios_host_ext_info_mt(optr->interface);
		if (!( optr = occi_add_attribute(optr, "name",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "host_name",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "notes",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "notes_url",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "action_url",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "icon_image",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "icon_image_alt",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "vrml_image",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "statusmap_image",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "twod_coords",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "threed_coords",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "status",0,0) ))
			return(optr);
		autoload_nagios_host_ext_info_nodes();
		return(optr);
	}

}

/*	------------------------------------------------------------------	*/
/*	n a g i o s _ h o s t _ e x t _ i n f o _ o c c i _ h e a d e r s 	*/
/*	------------------------------------------------------------------	*/
public struct rest_header *  nagios_host_ext_info_occi_headers(struct nagios_host_ext_info * sptr)
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
	sprintf(buffer,"nagios_host_ext_info; scheme='http://scheme.compatibleone.fr/scheme/compatible#'; class='kind';\r\n");
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
	sprintf(buffer,"occi.nagios_host_ext_info.name='%s'\r\n",(sptr->name?sptr->name:""));
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
	sprintf(buffer,"occi.nagios_host_ext_info.host_name='%s'\r\n",(sptr->host_name?sptr->host_name:""));
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
	sprintf(buffer,"occi.nagios_host_ext_info.notes='%s'\r\n",(sptr->notes?sptr->notes:""));
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
	sprintf(buffer,"occi.nagios_host_ext_info.notes_url='%s'\r\n",(sptr->notes_url?sptr->notes_url:""));
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
	sprintf(buffer,"occi.nagios_host_ext_info.action_url='%s'\r\n",(sptr->action_url?sptr->action_url:""));
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
	sprintf(buffer,"occi.nagios_host_ext_info.icon_image='%s'\r\n",(sptr->icon_image?sptr->icon_image:""));
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
	sprintf(buffer,"occi.nagios_host_ext_info.icon_image_alt='%s'\r\n",(sptr->icon_image_alt?sptr->icon_image_alt:""));
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
	sprintf(buffer,"occi.nagios_host_ext_info.vrml_image='%s'\r\n",(sptr->vrml_image?sptr->vrml_image:""));
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
	sprintf(buffer,"occi.nagios_host_ext_info.statusmap_image='%s'\r\n",(sptr->statusmap_image?sptr->statusmap_image:""));
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
	sprintf(buffer,"occi.nagios_host_ext_info.twod_coords='%s'\r\n",(sptr->twod_coords?sptr->twod_coords:""));
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
	sprintf(buffer,"occi.nagios_host_ext_info.threed_coords='%s'\r\n",(sptr->threed_coords?sptr->threed_coords:""));
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
	sprintf(buffer,"occi.nagios_host_ext_info.status='%u'\r\n",sptr->status);
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	return(first);

}

#endif	/* _occinagioshostextinfo_c_ */
