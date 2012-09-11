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
#ifndef _occicontract_c_
#define _occicontract_c_

#include "contract.h"

/*	--------------------------------------	*/
/*	o c c i _ c o r d s _ c o n t r a c t 	*/
/*	--------------------------------------	*/

/*	--------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   m a n a g e m e n t   s t r u c t u r e 	*/
/*	--------------------------------------------------------------------	*/
struct cords_contract * allocate_cords_contract();
struct cords_contract * liberate_cords_contract(struct cords_contract * optr);
private pthread_mutex_t list_cords_contract_control=PTHREAD_MUTEX_INITIALIZER;
private struct occi_kind_node * cords_contract_first = (struct occi_kind_node *) 0;
private struct occi_kind_node * cords_contract_last  = (struct occi_kind_node *) 0;
public struct  occi_kind_node * occi_first_cords_contract_node() { return( cords_contract_first ); }
public struct  occi_kind_node * occi_last_cords_contract_node() { return( cords_contract_last ); }

/*	----------------------------------------------	*/
/*	o c c i   c a t e g o r y   d r o p   n o d e 	*/
/*	----------------------------------------------	*/
private struct occi_kind_node * ll_drop_cords_contract_node(struct occi_kind_node * nptr) {
	if ( nptr ) {
	if (!( nptr->previous ))
		cords_contract_first = nptr->next;
	else	nptr->previous->next = nptr->next;
	if (!( nptr->next ))
		cords_contract_last = nptr->previous;
	else	nptr->next->previous = nptr->previous;
		liberate_occi_kind_node( nptr );
		}
	return((struct occi_kind_node *)0);
}
private struct occi_kind_node * drop_cords_contract_node(struct occi_kind_node * nptr) {
	pthread_mutex_lock( &list_cords_contract_control );
	nptr = ll_drop_cords_contract_node( nptr );
	pthread_mutex_unlock( &list_cords_contract_control );
	return(nptr);
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   l o c a t e   n o d e 	*/
/*	--------------------------------------------------	*/
private struct occi_kind_node * ll_locate_cords_contract_node(char * id) {
	struct occi_kind_node * nptr;
	struct cords_contract * pptr;
	for ( nptr = cords_contract_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		else if (!( pptr->id )) continue;
		else if (!( strcmp(pptr->id,id) )) break;
		}
	return( nptr );
}
private struct occi_kind_node * locate_cords_contract_node(char * id) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_cords_contract_control );
	nptr = ll_locate_cords_contract_node(id);
	pthread_mutex_unlock( &list_cords_contract_control );
	return( nptr );
}

/*	--------------------------------------------	*/
/*	o c c i   c a t e g o r y   a d d   n o d e 	*/
/*	--------------------------------------------	*/
private struct occi_kind_node * ll_add_cords_contract_node(int mode) {
	struct occi_kind_node * nptr;
	struct cords_contract * pptr;
	if (!( nptr = allocate_occi_kind_node() ))
		return( nptr );
	else	{
		if (!( nptr->contents = allocate_cords_contract()))
			return( liberate_occi_kind_node(nptr) );
		if (!( pptr = nptr->contents ))
			return( liberate_occi_kind_node(nptr) );
		else if (( mode != 0 ) && (!( pptr->id = occi_allocate_uuid())))
			return( liberate_occi_kind_node(nptr) );
		else	{
			if (!( nptr->previous = cords_contract_last ))
				cords_contract_first = nptr;
			else	nptr->previous->next = nptr;
			cords_contract_last = nptr;
			return( nptr );
			}
		}
}
private struct occi_kind_node * add_cords_contract_node(int mode) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_cords_contract_control );
	nptr = ll_add_cords_contract_node( mode );
	pthread_mutex_unlock( &list_cords_contract_control );
	return(nptr);
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   l o a d 	*/
/*	------------------------------------------------------------------------------------------	*/
private char*autosave_cords_contract_name="cords_contract.xml";
private void autoload_cords_contract_nodes() {
	char * fn=autosave_cords_contract_name;	struct occi_kind_node * nptr;
	struct cords_contract * pptr;
	struct xml_element * document;
	struct xml_element * eptr;
	struct xml_element * vptr;
	struct xml_atribut  * aptr;
	if (!( document = document_parse_file(fn)))
		return;
	if ((eptr = document_element(document,"cords_contracts")) != (struct xml_element *) 0) {
		for (vptr=eptr->first; vptr != (struct xml_element *) 0; vptr=vptr->next) {
			if (!( vptr->name )) continue;
			else if ( strcmp( vptr->name, "cords_contract" ) ) continue;
			else if (!( nptr = add_cords_contract_node(0))) break;
			else if (!( pptr = nptr->contents )) break;
			if ((aptr = document_atribut( vptr, "id" )) != (struct xml_atribut *) 0)
				pptr->id = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "name" )) != (struct xml_atribut *) 0)
				pptr->name = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "node" )) != (struct xml_atribut *) 0)
				pptr->node = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "provider" )) != (struct xml_atribut *) 0)
				pptr->provider = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "profile" )) != (struct xml_atribut *) 0)
				pptr->profile = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "reference" )) != (struct xml_atribut *) 0)
				pptr->reference = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "hostname" )) != (struct xml_atribut *) 0)
				pptr->hostname = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "rootpass" )) != (struct xml_atribut *) 0)
				pptr->rootpass = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "workload" )) != (struct xml_atribut *) 0)
				pptr->workload = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "price" )) != (struct xml_atribut *) 0)
				pptr->price = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "access" )) != (struct xml_atribut *) 0)
				pptr->access = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "common" )) != (struct xml_atribut *) 0)
				pptr->common = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "session" )) != (struct xml_atribut *) 0)
				pptr->session = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "scope" )) != (struct xml_atribut *) 0)
				pptr->scope = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "type" )) != (struct xml_atribut *) 0)
				pptr->type = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "service" )) != (struct xml_atribut *) 0)
				pptr->service = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "firewall" )) != (struct xml_atribut *) 0)
				pptr->firewall = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "placement" )) != (struct xml_atribut *) 0)
				pptr->placement = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "parentservice" )) != (struct xml_atribut *) 0)
				pptr->parentservice = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "agreement" )) != (struct xml_atribut *) 0)
				pptr->agreement = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "flags" )) != (struct xml_atribut *) 0)
				pptr->flags = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "startduration" )) != (struct xml_atribut *) 0)
				pptr->startduration = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "stopduration" )) != (struct xml_atribut *) 0)
				pptr->stopduration = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "saveduration" )) != (struct xml_atribut *) 0)
				pptr->saveduration = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "instructions" )) != (struct xml_atribut *) 0)
				pptr->instructions = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "commons" )) != (struct xml_atribut *) 0)
				pptr->commons = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "when" )) != (struct xml_atribut *) 0)
				pptr->when = document_atribut_value(aptr);
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
public  void set_autosave_cords_contract_name(char * fn) {
	autosave_cords_contract_name = fn;	return;
}
public  void autosave_cords_contract_nodes() {
	char * fn=autosave_cords_contract_name;	struct occi_kind_node * nptr;
	struct cords_contract * pptr;
	FILE * h;
	pthread_mutex_lock( &list_cords_contract_control );
	if (( h = fopen(fn,"w")) != (FILE *) 0) {
	fprintf(h,"<cords_contracts>\n");
	for ( nptr = cords_contract_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		fprintf(h,"<cords_contract\n");
		fprintf(h," id=%c",0x0022);
		fprintf(h,"%s",(pptr->id?pptr->id:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," name=%c",0x0022);
		fprintf(h,"%s",(pptr->name?pptr->name:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," node=%c",0x0022);
		fprintf(h,"%s",(pptr->node?pptr->node:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," provider=%c",0x0022);
		fprintf(h,"%s",(pptr->provider?pptr->provider:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," profile=%c",0x0022);
		fprintf(h,"%s",(pptr->profile?pptr->profile:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," reference=%c",0x0022);
		fprintf(h,"%s",(pptr->reference?pptr->reference:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," hostname=%c",0x0022);
		fprintf(h,"%s",(pptr->hostname?pptr->hostname:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," rootpass=%c",0x0022);
		fprintf(h,"%s",(pptr->rootpass?pptr->rootpass:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," workload=%c",0x0022);
		fprintf(h,"%s",(pptr->workload?pptr->workload:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," price=%c",0x0022);
		fprintf(h,"%s",(pptr->price?pptr->price:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," access=%c",0x0022);
		fprintf(h,"%s",(pptr->access?pptr->access:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," common=%c",0x0022);
		fprintf(h,"%s",(pptr->common?pptr->common:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," session=%c",0x0022);
		fprintf(h,"%s",(pptr->session?pptr->session:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," scope=%c",0x0022);
		fprintf(h,"%s",(pptr->scope?pptr->scope:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," type=%c",0x0022);
		fprintf(h,"%s",(pptr->type?pptr->type:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," service=%c",0x0022);
		fprintf(h,"%s",(pptr->service?pptr->service:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," firewall=%c",0x0022);
		fprintf(h,"%s",(pptr->firewall?pptr->firewall:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," placement=%c",0x0022);
		fprintf(h,"%s",(pptr->placement?pptr->placement:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," parentservice=%c",0x0022);
		fprintf(h,"%s",(pptr->parentservice?pptr->parentservice:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," agreement=%c",0x0022);
		fprintf(h,"%s",(pptr->agreement?pptr->agreement:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," flags=%c",0x0022);
		fprintf(h,"%u",pptr->flags);
		fprintf(h,"%c",0x0022);
		fprintf(h," startduration=%c",0x0022);
		fprintf(h,"%u",pptr->startduration);
		fprintf(h,"%c",0x0022);
		fprintf(h," stopduration=%c",0x0022);
		fprintf(h,"%u",pptr->stopduration);
		fprintf(h,"%c",0x0022);
		fprintf(h," saveduration=%c",0x0022);
		fprintf(h,"%u",pptr->saveduration);
		fprintf(h,"%c",0x0022);
		fprintf(h," instructions=%c",0x0022);
		fprintf(h,"%u",pptr->instructions);
		fprintf(h,"%c",0x0022);
		fprintf(h," commons=%c",0x0022);
		fprintf(h,"%u",pptr->commons);
		fprintf(h,"%c",0x0022);
		fprintf(h," when=%c",0x0022);
		fprintf(h,"%u",pptr->when);
		fprintf(h,"%c",0x0022);
		fprintf(h," state=%c",0x0022);
		fprintf(h,"%u",pptr->state);
		fprintf(h,"%c",0x0022);
		fprintf(h," />\n");
		}
	fprintf(h,"</cords_contracts>\n");
	fclose(h);
	}
	pthread_mutex_unlock( &list_cords_contract_control );
	return;
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   s e t   f i e l d 	*/
/*	------------------------------------------------------------------------------------------	*/
private void set_cords_contract_field(
	struct occi_category * cptr,void * optr, char * nptr, char * vptr)
{
	struct cords_contract * pptr;
	char prefix[1024];
	if (!( pptr = optr )) return;
	sprintf(prefix,"%s.%s.",cptr->domain,cptr->id);
	if (!( strncmp( nptr, prefix, strlen(prefix) ) )) {
		nptr += strlen(prefix);
		if (!( strcmp( nptr, "name" ) ))
			pptr->name = allocate_string(vptr);
		if (!( strcmp( nptr, "node" ) ))
			pptr->node = allocate_string(vptr);
		if (!( strcmp( nptr, "provider" ) ))
			pptr->provider = allocate_string(vptr);
		if (!( strcmp( nptr, "profile" ) ))
			pptr->profile = allocate_string(vptr);
		if (!( strcmp( nptr, "reference" ) ))
			pptr->reference = allocate_string(vptr);
		if (!( strcmp( nptr, "hostname" ) ))
			pptr->hostname = allocate_string(vptr);
		if (!( strcmp( nptr, "rootpass" ) ))
			pptr->rootpass = allocate_string(vptr);
		if (!( strcmp( nptr, "workload" ) ))
			pptr->workload = allocate_string(vptr);
		if (!( strcmp( nptr, "price" ) ))
			pptr->price = allocate_string(vptr);
		if (!( strcmp( nptr, "access" ) ))
			pptr->access = allocate_string(vptr);
		if (!( strcmp( nptr, "common" ) ))
			pptr->common = allocate_string(vptr);
		if (!( strcmp( nptr, "session" ) ))
			pptr->session = allocate_string(vptr);
		if (!( strcmp( nptr, "scope" ) ))
			pptr->scope = allocate_string(vptr);
		if (!( strcmp( nptr, "type" ) ))
			pptr->type = allocate_string(vptr);
		if (!( strcmp( nptr, "service" ) ))
			pptr->service = allocate_string(vptr);
		if (!( strcmp( nptr, "firewall" ) ))
			pptr->firewall = allocate_string(vptr);
		if (!( strcmp( nptr, "placement" ) ))
			pptr->placement = allocate_string(vptr);
		if (!( strcmp( nptr, "parentservice" ) ))
			pptr->parentservice = allocate_string(vptr);
		if (!( strcmp( nptr, "agreement" ) ))
			pptr->agreement = allocate_string(vptr);
		if (!( strcmp( nptr, "flags" ) ))
			pptr->flags = atoi(vptr);
		if (!( strcmp( nptr, "startduration" ) ))
			pptr->startduration = atoi(vptr);
		if (!( strcmp( nptr, "stopduration" ) ))
			pptr->stopduration = atoi(vptr);
		if (!( strcmp( nptr, "saveduration" ) ))
			pptr->saveduration = atoi(vptr);
		if (!( strcmp( nptr, "instructions" ) ))
			pptr->instructions = atoi(vptr);
		if (!( strcmp( nptr, "commons" ) ))
			pptr->commons = atoi(vptr);
		if (!( strcmp( nptr, "when" ) ))
			pptr->when = atoi(vptr);
		if (!( strcmp( nptr, "state" ) ))
			pptr->state = atoi(vptr);
		}
	return;
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   i n f o 	*/
/*	--------------------------------------------------	*/
private struct cords_contract * filter_cords_contract_info(
	struct occi_category * optr,
	struct rest_request  * rptr,
	struct rest_response * aptr) {
	struct cords_contract * pptr;
		if (!( pptr = allocate_cords_contract()))
		return( pptr );
	else if (!( occi_process_atributs(optr, rptr, aptr, pptr, set_cords_contract_field) ))
		return( liberate_cords_contract(pptr));
	else	return( pptr );
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   p a s s 	*/
/*	--------------------------------------------------	*/
private int pass_cords_contract_filter(
	struct cords_contract * pptr,struct cords_contract * fptr) {
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
	if (( fptr->node )
	&&  (strlen( fptr->node ) != 0)) {
		if (!( pptr->node ))
			return(0);
		else if ( strcmp(pptr->node,fptr->node) != 0)
			return(0);
		}
	if (( fptr->provider )
	&&  (strlen( fptr->provider ) != 0)) {
		if (!( pptr->provider ))
			return(0);
		else if ( strcmp(pptr->provider,fptr->provider) != 0)
			return(0);
		}
	if (( fptr->profile )
	&&  (strlen( fptr->profile ) != 0)) {
		if (!( pptr->profile ))
			return(0);
		else if ( strcmp(pptr->profile,fptr->profile) != 0)
			return(0);
		}
	if (( fptr->reference )
	&&  (strlen( fptr->reference ) != 0)) {
		if (!( pptr->reference ))
			return(0);
		else if ( strcmp(pptr->reference,fptr->reference) != 0)
			return(0);
		}
	if (( fptr->hostname )
	&&  (strlen( fptr->hostname ) != 0)) {
		if (!( pptr->hostname ))
			return(0);
		else if ( strcmp(pptr->hostname,fptr->hostname) != 0)
			return(0);
		}
	if (( fptr->rootpass )
	&&  (strlen( fptr->rootpass ) != 0)) {
		if (!( pptr->rootpass ))
			return(0);
		else if ( strcmp(pptr->rootpass,fptr->rootpass) != 0)
			return(0);
		}
	if (( fptr->workload )
	&&  (strlen( fptr->workload ) != 0)) {
		if (!( pptr->workload ))
			return(0);
		else if ( strcmp(pptr->workload,fptr->workload) != 0)
			return(0);
		}
	if (( fptr->price )
	&&  (strlen( fptr->price ) != 0)) {
		if (!( pptr->price ))
			return(0);
		else if ( strcmp(pptr->price,fptr->price) != 0)
			return(0);
		}
	if (( fptr->access )
	&&  (strlen( fptr->access ) != 0)) {
		if (!( pptr->access ))
			return(0);
		else if ( strcmp(pptr->access,fptr->access) != 0)
			return(0);
		}
	if (( fptr->common )
	&&  (strlen( fptr->common ) != 0)) {
		if (!( pptr->common ))
			return(0);
		else if ( strcmp(pptr->common,fptr->common) != 0)
			return(0);
		}
	if (( fptr->session )
	&&  (strlen( fptr->session ) != 0)) {
		if (!( pptr->session ))
			return(0);
		else if ( strcmp(pptr->session,fptr->session) != 0)
			return(0);
		}
	if (( fptr->scope )
	&&  (strlen( fptr->scope ) != 0)) {
		if (!( pptr->scope ))
			return(0);
		else if ( strcmp(pptr->scope,fptr->scope) != 0)
			return(0);
		}
	if (( fptr->type )
	&&  (strlen( fptr->type ) != 0)) {
		if (!( pptr->type ))
			return(0);
		else if ( strcmp(pptr->type,fptr->type) != 0)
			return(0);
		}
	if (( fptr->service )
	&&  (strlen( fptr->service ) != 0)) {
		if (!( pptr->service ))
			return(0);
		else if ( strcmp(pptr->service,fptr->service) != 0)
			return(0);
		}
	if (( fptr->firewall )
	&&  (strlen( fptr->firewall ) != 0)) {
		if (!( pptr->firewall ))
			return(0);
		else if ( strcmp(pptr->firewall,fptr->firewall) != 0)
			return(0);
		}
	if (( fptr->placement )
	&&  (strlen( fptr->placement ) != 0)) {
		if (!( pptr->placement ))
			return(0);
		else if ( strcmp(pptr->placement,fptr->placement) != 0)
			return(0);
		}
	if (( fptr->parentservice )
	&&  (strlen( fptr->parentservice ) != 0)) {
		if (!( pptr->parentservice ))
			return(0);
		else if ( strcmp(pptr->parentservice,fptr->parentservice) != 0)
			return(0);
		}
	if (( fptr->agreement )
	&&  (strlen( fptr->agreement ) != 0)) {
		if (!( pptr->agreement ))
			return(0);
		else if ( strcmp(pptr->agreement,fptr->agreement) != 0)
			return(0);
		}
	if (( fptr->flags ) && ( pptr->flags != fptr->flags )) return(0);
	if (( fptr->startduration ) && ( pptr->startduration != fptr->startduration )) return(0);
	if (( fptr->stopduration ) && ( pptr->stopduration != fptr->stopduration )) return(0);
	if (( fptr->saveduration ) && ( pptr->saveduration != fptr->saveduration )) return(0);
	if (( fptr->instructions ) && ( pptr->instructions != fptr->instructions )) return(0);
	if (( fptr->commons ) && ( pptr->commons != fptr->commons )) return(0);
	if (( fptr->when ) && ( pptr->when != fptr->when )) return(0);
	if (( fptr->state ) && ( pptr->state != fptr->state )) return(0);
	return(1);
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   r e s p o n s e 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * cords_contract_occi_response(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,
	struct cords_contract * pptr)
{
	struct rest_header * hptr;
	sprintf(cptr->buffer,"occi.core.id=%s",pptr->id);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.name=%s",optr->domain,optr->id,pptr->name);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.node=%s",optr->domain,optr->id,pptr->node);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.provider=%s",optr->domain,optr->id,pptr->provider);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.profile=%s",optr->domain,optr->id,pptr->profile);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.reference=%s",optr->domain,optr->id,pptr->reference);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.hostname=%s",optr->domain,optr->id,pptr->hostname);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.rootpass=%s",optr->domain,optr->id,pptr->rootpass);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.workload=%s",optr->domain,optr->id,pptr->workload);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.price=%s",optr->domain,optr->id,pptr->price);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.access=%s",optr->domain,optr->id,pptr->access);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.common=%s",optr->domain,optr->id,pptr->common);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.session=%s",optr->domain,optr->id,pptr->session);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.scope=%s",optr->domain,optr->id,pptr->scope);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.type=%s",optr->domain,optr->id,pptr->type);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.service=%s",optr->domain,optr->id,pptr->service);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.firewall=%s",optr->domain,optr->id,pptr->firewall);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.placement=%s",optr->domain,optr->id,pptr->placement);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.parentservice=%s",optr->domain,optr->id,pptr->parentservice);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.agreement=%s",optr->domain,optr->id,pptr->agreement);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.flags=%u",optr->domain,optr->id,pptr->flags);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.startduration=%u",optr->domain,optr->id,pptr->startduration);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.stopduration=%u",optr->domain,optr->id,pptr->stopduration);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.saveduration=%u",optr->domain,optr->id,pptr->saveduration);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.instructions=%u",optr->domain,optr->id,pptr->instructions);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.commons=%u",optr->domain,optr->id,pptr->commons);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.when=%u",optr->domain,optr->id,pptr->when);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.state=%u",optr->domain,optr->id,pptr->state);
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
private struct rest_response * cords_contract_get_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct cords_contract * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_cords_contract_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->retrieve)) (*iptr->retrieve)(optr,nptr);
	autosave_cords_contract_nodes();
	return( cords_contract_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   l i n k 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * cords_contract_post_link(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct cords_contract * pptr;
	char * reqhost;
	if (!( nptr = locate_cords_contract_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   m i x i n 	*/
/*	--------------------------------------------------------------------------------------------	*/
private struct rest_response * cords_contract_post_mixin(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct cords_contract * pptr;
	char * reqhost;
	if (!( nptr = locate_cords_contract_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request"));
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   a c t i o n 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * cords_contract_post_action(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_action * fptr;
	struct occi_kind_node * nptr;
	struct cords_contract * pptr;
	char * reqhost;
	char * mptr;
	if (!( nptr = locate_cords_contract_node(id)))
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
private struct rest_response * cords_contract_post_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct cords_contract * pptr;
	char * reqhost;
	iptr = optr->callback;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	if (!( nptr = add_cords_contract_node(1)))
		return( rest_html_response( aptr, 500, "Server Failure") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (!( occi_process_atributs( optr, rptr,aptr, pptr, set_cords_contract_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->create)) (*iptr->create)(optr,nptr);
	autosave_cords_contract_nodes();
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
private struct rest_response * cords_contract_put_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct cords_contract * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_cords_contract_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (!( occi_process_atributs(optr,rptr,aptr, pptr, set_cords_contract_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->update)) (*iptr->update)(optr,nptr);
	autosave_cords_contract_nodes();
	return( cords_contract_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d   i t e m 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * cords_contract_head_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_kind_node * nptr;
	struct cords_contract * pptr;
	if (!( nptr = locate_cords_contract_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e   i t e m 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * cords_contract_delete_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct cords_contract * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_cords_contract_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->delete)) (*iptr->delete)(optr,nptr);
	drop_cords_contract_node( nptr );
	autosave_cords_contract_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t   l i s t 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * cords_contract_get_list(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_kind_node * sptr;
	struct cords_contract * pptr;
	struct cords_contract * fptr;
	char * reqhost;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else if (!( fptr = filter_cords_contract_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	for ( sptr = cords_contract_first;
		sptr != (struct occi_kind_node *) 0;
		sptr = sptr->next ) {
		if (!( pptr = sptr->contents ))
			continue;
		if (!( pass_cords_contract_filter( pptr, fptr ) ))
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
private struct rest_response * cords_contract_delete_all(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct occi_kind_node * sptr;
	struct cords_contract * pptr;
	struct cords_contract * fptr;
	iptr = optr->callback;
	if (!( fptr = filter_cords_contract_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	nptr=cords_contract_first;
	while (nptr != (struct occi_kind_node *) 0) {
		if ((!( pptr = nptr->contents ))
		||  (!( pass_cords_contract_filter( pptr, fptr ) ))) {
			nptr = nptr->next;
			continue;
			}
		else	{
			if (( iptr ) && (iptr->delete)) { (*iptr->delete)(optr,nptr); }
			sptr = nptr->next;
			drop_cords_contract_node( nptr );
			nptr = sptr;
			}
		}
	autosave_cords_contract_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_cords_contract_get(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( cords_contract_get_list( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( cords_contract_get_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_cords_contract_head(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( cords_contract_head_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_cords_contract_post(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( cords_contract_post_item( optr, cptr, rptr, aptr ) );
	else if ( strncmp( rptr->object, optr->location,strlen(optr->location)) != 0)
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( rptr->parameters ))
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( strncmp( rptr->parameters, "action=", strlen("action=")) ))
		return( cords_contract_post_action( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "mixin=", strlen("mixin=")) ))
		return( cords_contract_post_mixin( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "link=", strlen("link=")) ))
		return( cords_contract_post_link( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p u t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_cords_contract_put(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( cords_contract_put_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e 	*/
/*	------------------------------------------------------------------------------------	*/
private struct rest_response * occi_cords_contract_delete(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( cords_contract_delete_all( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( cords_contract_delete_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   r e d i r e c t i o n 	*/
/*	--------------------------------------------------------------------------------	*/
private void	redirect_occi_cords_contract_mt( struct rest_interface * iptr )
{
	iptr->get = occi_cords_contract_get;
	iptr->post = occi_cords_contract_post;
	iptr->put = occi_cords_contract_put;
	iptr->delete = occi_cords_contract_delete;
	iptr->head = occi_cords_contract_head;
	return;
}

/*	------------------------------------------	*/
/*	o c c i   c a t e g o r y   b u i l d e r 	*/
/*	------------------------------------------	*/
/* occi category rest instance builder for : occi_cords_contract */
public struct occi_category * occi_cords_contract_builder(char * a,char * b) {
	char * c="http://scheme.compatibleone.fr/scheme/compatible#";
	char * d="kind";
	char * e="http://scheme.ogf.org/occi/resource#";
	char * f="CompatibleOne OCCI resource cords_contract";
	struct occi_category * optr;
	if (!( optr = occi_create_category(a,b,c,d,e,f) )) { return(optr); }
	else {
		redirect_occi_cords_contract_mt(optr->interface);
		if (!( optr = occi_add_attribute(optr, "name",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "node",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "provider",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "profile",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "reference",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "hostname",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "rootpass",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "workload",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "price",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "access",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "common",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "session",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "scope",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "type",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "service",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "firewall",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "placement",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "parentservice",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "agreement",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "flags",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "startduration",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "stopduration",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "saveduration",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "instructions",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "commons",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "when",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "state",0,0) ))
			return(optr);
		autoload_cords_contract_nodes();
		return(optr);
	}

}

/*	------------------------------------------------------	*/
/*	c o r d s _ c o n t r a c t _ o c c i _ h e a d e r s 	*/
/*	------------------------------------------------------	*/
public struct rest_header *  cords_contract_occi_headers(struct cords_contract * sptr)
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
	sprintf(buffer,"cords_contract; scheme='http://scheme.compatibleone.fr/scheme/compatible#'; class='kind';\r\n");
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
	sprintf(buffer,"occi.cords_contract.name='%s'\r\n",(sptr->name?sptr->name:""));
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
	sprintf(buffer,"occi.cords_contract.node='%s'\r\n",(sptr->node?sptr->node:""));
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
	sprintf(buffer,"occi.cords_contract.provider='%s'\r\n",(sptr->provider?sptr->provider:""));
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
	sprintf(buffer,"occi.cords_contract.profile='%s'\r\n",(sptr->profile?sptr->profile:""));
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
	sprintf(buffer,"occi.cords_contract.reference='%s'\r\n",(sptr->reference?sptr->reference:""));
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
	sprintf(buffer,"occi.cords_contract.hostname='%s'\r\n",(sptr->hostname?sptr->hostname:""));
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
	sprintf(buffer,"occi.cords_contract.rootpass='%s'\r\n",(sptr->rootpass?sptr->rootpass:""));
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
	sprintf(buffer,"occi.cords_contract.workload='%s'\r\n",(sptr->workload?sptr->workload:""));
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
	sprintf(buffer,"occi.cords_contract.price='%s'\r\n",(sptr->price?sptr->price:""));
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
	sprintf(buffer,"occi.cords_contract.access='%s'\r\n",(sptr->access?sptr->access:""));
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
	sprintf(buffer,"occi.cords_contract.common='%s'\r\n",(sptr->common?sptr->common:""));
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
	sprintf(buffer,"occi.cords_contract.session='%s'\r\n",(sptr->session?sptr->session:""));
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
	sprintf(buffer,"occi.cords_contract.scope='%s'\r\n",(sptr->scope?sptr->scope:""));
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
	sprintf(buffer,"occi.cords_contract.type='%s'\r\n",(sptr->type?sptr->type:""));
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
	sprintf(buffer,"occi.cords_contract.service='%s'\r\n",(sptr->service?sptr->service:""));
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
	sprintf(buffer,"occi.cords_contract.firewall='%s'\r\n",(sptr->firewall?sptr->firewall:""));
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
	sprintf(buffer,"occi.cords_contract.placement='%s'\r\n",(sptr->placement?sptr->placement:""));
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
	sprintf(buffer,"occi.cords_contract.parentservice='%s'\r\n",(sptr->parentservice?sptr->parentservice:""));
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
	sprintf(buffer,"occi.cords_contract.agreement='%s'\r\n",(sptr->agreement?sptr->agreement:""));
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
	sprintf(buffer,"occi.cords_contract.flags='%u'\r\n",sptr->flags);
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
	sprintf(buffer,"occi.cords_contract.startduration='%u'\r\n",sptr->startduration);
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
	sprintf(buffer,"occi.cords_contract.stopduration='%u'\r\n",sptr->stopduration);
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
	sprintf(buffer,"occi.cords_contract.saveduration='%u'\r\n",sptr->saveduration);
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
	sprintf(buffer,"occi.cords_contract.instructions='%u'\r\n",sptr->instructions);
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
	sprintf(buffer,"occi.cords_contract.commons='%u'\r\n",sptr->commons);
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
	sprintf(buffer,"occi.cords_contract.when='%u'\r\n",sptr->when);
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
	sprintf(buffer,"occi.cords_contract.state='%u'\r\n",sptr->state);
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	return(first);

}

#endif	/* _occicontract_c_ */
