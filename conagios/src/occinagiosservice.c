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
#ifndef _occinagiosservice_c_
#define _occinagiosservice_c_

#include "nagiosservice.h"

/*	--------------------------------------	*/
/*	o c c i _ n a g i o s _ s e r v i c e 	*/
/*	--------------------------------------	*/

/*	--------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   m a n a g e m e n t   s t r u c t u r e 	*/
/*	--------------------------------------------------------------------	*/
struct nagios_service * allocate_nagios_service();
struct nagios_service * liberate_nagios_service(struct nagios_service * optr);
private pthread_mutex_t list_nagios_service_control=PTHREAD_MUTEX_INITIALIZER;
private struct occi_kind_node * nagios_service_first = (struct occi_kind_node *) 0;
private struct occi_kind_node * nagios_service_last  = (struct occi_kind_node *) 0;
public struct  occi_kind_node * occi_first_nagios_service_node() { return( nagios_service_first ); }
public struct  occi_kind_node * occi_last_nagios_service_node() { return( nagios_service_last ); }

/*	----------------------------------------------	*/
/*	o c c i   c a t e g o r y   d r o p   n o d e 	*/
/*	----------------------------------------------	*/
private struct occi_kind_node * ll_drop_nagios_service_node(struct occi_kind_node * nptr) {
	if ( nptr ) {
	if (!( nptr->previous ))
		nagios_service_first = nptr->next;
	else	nptr->previous->next = nptr->next;
	if (!( nptr->next ))
		nagios_service_last = nptr->previous;
	else	nptr->next->previous = nptr->previous;
		liberate_occi_kind_node( nptr );
		}
	return((struct occi_kind_node *)0);
}
private struct occi_kind_node * drop_nagios_service_node(struct occi_kind_node * nptr) {
	pthread_mutex_lock( &list_nagios_service_control );
	nptr = ll_drop_nagios_service_node( nptr );
	pthread_mutex_unlock( &list_nagios_service_control );
	return(nptr);
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   l o c a t e   n o d e 	*/
/*	--------------------------------------------------	*/
private struct occi_kind_node * ll_locate_nagios_service_node(char * id) {
	struct occi_kind_node * nptr;
	struct nagios_service * pptr;
	for ( nptr = nagios_service_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		else if (!( pptr->id )) continue;
		else if (!( strcmp(pptr->id,id) )) break;
		}
	return( nptr );
}
private struct occi_kind_node * locate_nagios_service_node(char * id) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_nagios_service_control );
	nptr = ll_locate_nagios_service_node(id);
	pthread_mutex_unlock( &list_nagios_service_control );
	return( nptr );
}

/*	--------------------------------------------	*/
/*	o c c i   c a t e g o r y   a d d   n o d e 	*/
/*	--------------------------------------------	*/
private struct occi_kind_node * ll_add_nagios_service_node(int mode) {
	struct occi_kind_node * nptr;
	struct nagios_service * pptr;
	if (!( nptr = allocate_occi_kind_node() ))
		return( nptr );
	else	{
		if (!( nptr->contents = allocate_nagios_service()))
			return( liberate_occi_kind_node(nptr) );
		if (!( pptr = nptr->contents ))
			return( liberate_occi_kind_node(nptr) );
		else if (( mode != 0 ) && (!( pptr->id = occi_allocate_uuid())))
			return( liberate_occi_kind_node(nptr) );
		else	{
			if (!( nptr->previous = nagios_service_last ))
				nagios_service_first = nptr;
			else	nptr->previous->next = nptr;
			nagios_service_last = nptr;
			return( nptr );
			}
		}
}
private struct occi_kind_node * add_nagios_service_node(int mode) {
	struct occi_kind_node * nptr;
	pthread_mutex_lock( &list_nagios_service_control );
	nptr = ll_add_nagios_service_node( mode );
	pthread_mutex_unlock( &list_nagios_service_control );
	return(nptr);
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   a u t o   l o a d 	*/
/*	------------------------------------------------------------------------------------------	*/
private char*autosave_nagios_service_name="nagios_service.xml";
private void autoload_nagios_service_nodes() {
	char * fn=autosave_nagios_service_name;	struct occi_kind_node * nptr;
	struct nagios_service * pptr;
	struct xml_element * document;
	struct xml_element * eptr;
	struct xml_element * vptr;
	struct xml_atribut  * aptr;
	if (!( document = document_parse_file(fn)))
		return;
	if ((eptr = document_element(document,"nagios_services")) != (struct xml_element *) 0) {
		for (vptr=eptr->first; vptr != (struct xml_element *) 0; vptr=vptr->next) {
			if (!( vptr->name )) continue;
			else if ( strcmp( vptr->name, "nagios_service" ) ) continue;
			else if (!( nptr = add_nagios_service_node(0))) break;
			else if (!( pptr = nptr->contents )) break;
			if ((aptr = document_atribut( vptr, "id" )) != (struct xml_atribut *) 0)
				pptr->id = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "name" )) != (struct xml_atribut *) 0)
				pptr->name = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "host_name" )) != (struct xml_atribut *) 0)
				pptr->host_name = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "hostgroup_name" )) != (struct xml_atribut *) 0)
				pptr->hostgroup_name = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "service_description" )) != (struct xml_atribut *) 0)
				pptr->service_description = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "display_name" )) != (struct xml_atribut *) 0)
				pptr->display_name = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "servicegroups" )) != (struct xml_atribut *) 0)
				pptr->servicegroups = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "is_volatile" )) != (struct xml_atribut *) 0)
				pptr->is_volatile = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "check_command" )) != (struct xml_atribut *) 0)
				pptr->check_command = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "initial_state" )) != (struct xml_atribut *) 0)
				pptr->initial_state = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "max_check_attempts" )) != (struct xml_atribut *) 0)
				pptr->max_check_attempts = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "check_interval" )) != (struct xml_atribut *) 0)
				pptr->check_interval = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "retry_interval" )) != (struct xml_atribut *) 0)
				pptr->retry_interval = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "active_checks_enabled" )) != (struct xml_atribut *) 0)
				pptr->active_checks_enabled = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "passive_checks_enabled" )) != (struct xml_atribut *) 0)
				pptr->passive_checks_enabled = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "check_period" )) != (struct xml_atribut *) 0)
				pptr->check_period = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "obsess_over_service" )) != (struct xml_atribut *) 0)
				pptr->obsess_over_service = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "check_freshness" )) != (struct xml_atribut *) 0)
				pptr->check_freshness = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "freshness_threshold" )) != (struct xml_atribut *) 0)
				pptr->freshness_threshold = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "event_handler" )) != (struct xml_atribut *) 0)
				pptr->event_handler = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "event_handler_enabled" )) != (struct xml_atribut *) 0)
				pptr->event_handler_enabled = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "low_flap_threshold" )) != (struct xml_atribut *) 0)
				pptr->low_flap_threshold = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "high_flap_threshold" )) != (struct xml_atribut *) 0)
				pptr->high_flap_threshold = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "flap_detection_enabled" )) != (struct xml_atribut *) 0)
				pptr->flap_detection_enabled = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "flap_detection_options" )) != (struct xml_atribut *) 0)
				pptr->flap_detection_options = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "process_perf_data" )) != (struct xml_atribut *) 0)
				pptr->process_perf_data = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "retain_status_information" )) != (struct xml_atribut *) 0)
				pptr->retain_status_information = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "retain_nonstatus_information" )) != (struct xml_atribut *) 0)
				pptr->retain_nonstatus_information = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "notification_interval" )) != (struct xml_atribut *) 0)
				pptr->notification_interval = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "notification_period" )) != (struct xml_atribut *) 0)
				pptr->notification_period = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "notification_options" )) != (struct xml_atribut *) 0)
				pptr->notification_options = document_atribut_string(aptr);
			if ((aptr = document_atribut( vptr, "notifications_enabled" )) != (struct xml_atribut *) 0)
				pptr->notifications_enabled = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "contacts" )) != (struct xml_atribut *) 0)
				pptr->contacts = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "contact_groups" )) != (struct xml_atribut *) 0)
				pptr->contact_groups = document_atribut_value(aptr);
			if ((aptr = document_atribut( vptr, "stalking_options" )) != (struct xml_atribut *) 0)
				pptr->stalking_options = document_atribut_string(aptr);
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
public  void set_autosave_nagios_service_name(char * fn) {
	autosave_nagios_service_name = fn;	return;
}
public  void autosave_nagios_service_nodes() {
	char * fn=autosave_nagios_service_name;	struct occi_kind_node * nptr;
	struct nagios_service * pptr;
	FILE * h;
	pthread_mutex_lock( &list_nagios_service_control );
	if (( h = fopen(fn,"w")) != (FILE *) 0) {
	fprintf(h,"<nagios_services>\n");
	for ( nptr = nagios_service_first;
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next ) {
		if (!( pptr = nptr->contents )) continue;
		fprintf(h,"<nagios_service\n");
		fprintf(h," id=%c",0x0022);
		fprintf(h,"%s",(pptr->id?pptr->id:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," name=%c",0x0022);
		fprintf(h,"%s",(pptr->name?pptr->name:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," host_name=%c",0x0022);
		fprintf(h,"%s",(pptr->host_name?pptr->host_name:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," hostgroup_name=%c",0x0022);
		fprintf(h,"%s",(pptr->hostgroup_name?pptr->hostgroup_name:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," service_description=%c",0x0022);
		fprintf(h,"%s",(pptr->service_description?pptr->service_description:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," display_name=%c",0x0022);
		fprintf(h,"%s",(pptr->display_name?pptr->display_name:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," servicegroups=%c",0x0022);
		fprintf(h,"%u",pptr->servicegroups);
		fprintf(h,"%c",0x0022);
		fprintf(h," is_volatile=%c",0x0022);
		fprintf(h,"%u",pptr->is_volatile);
		fprintf(h,"%c",0x0022);
		fprintf(h," check_command=%c",0x0022);
		fprintf(h,"%s",(pptr->check_command?pptr->check_command:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," initial_state=%c",0x0022);
		fprintf(h,"%s",(pptr->initial_state?pptr->initial_state:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," max_check_attempts=%c",0x0022);
		fprintf(h,"%u",pptr->max_check_attempts);
		fprintf(h,"%c",0x0022);
		fprintf(h," check_interval=%c",0x0022);
		fprintf(h,"%u",pptr->check_interval);
		fprintf(h,"%c",0x0022);
		fprintf(h," retry_interval=%c",0x0022);
		fprintf(h,"%u",pptr->retry_interval);
		fprintf(h,"%c",0x0022);
		fprintf(h," active_checks_enabled=%c",0x0022);
		fprintf(h,"%u",pptr->active_checks_enabled);
		fprintf(h,"%c",0x0022);
		fprintf(h," passive_checks_enabled=%c",0x0022);
		fprintf(h,"%u",pptr->passive_checks_enabled);
		fprintf(h,"%c",0x0022);
		fprintf(h," check_period=%c",0x0022);
		fprintf(h,"%s",(pptr->check_period?pptr->check_period:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," obsess_over_service=%c",0x0022);
		fprintf(h,"%u",pptr->obsess_over_service);
		fprintf(h,"%c",0x0022);
		fprintf(h," check_freshness=%c",0x0022);
		fprintf(h,"%u",pptr->check_freshness);
		fprintf(h,"%c",0x0022);
		fprintf(h," freshness_threshold=%c",0x0022);
		fprintf(h,"%u",pptr->freshness_threshold);
		fprintf(h,"%c",0x0022);
		fprintf(h," event_handler=%c",0x0022);
		fprintf(h,"%s",(pptr->event_handler?pptr->event_handler:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," event_handler_enabled=%c",0x0022);
		fprintf(h,"%u",pptr->event_handler_enabled);
		fprintf(h,"%c",0x0022);
		fprintf(h," low_flap_threshold=%c",0x0022);
		fprintf(h,"%u",pptr->low_flap_threshold);
		fprintf(h,"%c",0x0022);
		fprintf(h," high_flap_threshold=%c",0x0022);
		fprintf(h,"%u",pptr->high_flap_threshold);
		fprintf(h,"%c",0x0022);
		fprintf(h," flap_detection_enabled=%c",0x0022);
		fprintf(h,"%u",pptr->flap_detection_enabled);
		fprintf(h,"%c",0x0022);
		fprintf(h," flap_detection_options=%c",0x0022);
		fprintf(h,"%s",(pptr->flap_detection_options?pptr->flap_detection_options:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," process_perf_data=%c",0x0022);
		fprintf(h,"%u",pptr->process_perf_data);
		fprintf(h,"%c",0x0022);
		fprintf(h," retain_status_information=%c",0x0022);
		fprintf(h,"%u",pptr->retain_status_information);
		fprintf(h,"%c",0x0022);
		fprintf(h," retain_nonstatus_information=%c",0x0022);
		fprintf(h,"%u",pptr->retain_nonstatus_information);
		fprintf(h,"%c",0x0022);
		fprintf(h," notification_interval=%c",0x0022);
		fprintf(h,"%u",pptr->notification_interval);
		fprintf(h,"%c",0x0022);
		fprintf(h," notification_period=%c",0x0022);
		fprintf(h,"%s",(pptr->notification_period?pptr->notification_period:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," notification_options=%c",0x0022);
		fprintf(h,"%s",(pptr->notification_options?pptr->notification_options:""));
		fprintf(h,"%c",0x0022);
		fprintf(h," notifications_enabled=%c",0x0022);
		fprintf(h,"%u",pptr->notifications_enabled);
		fprintf(h,"%c",0x0022);
		fprintf(h," contacts=%c",0x0022);
		fprintf(h,"%u",pptr->contacts);
		fprintf(h,"%c",0x0022);
		fprintf(h," contact_groups=%c",0x0022);
		fprintf(h,"%u",pptr->contact_groups);
		fprintf(h,"%c",0x0022);
		fprintf(h," stalking_options=%c",0x0022);
		fprintf(h,"%s",(pptr->stalking_options?pptr->stalking_options:""));
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
		fprintf(h," status=%c",0x0022);
		fprintf(h,"%u",pptr->status);
		fprintf(h,"%c",0x0022);
		fprintf(h," />\n");
		}
	fprintf(h,"</nagios_services>\n");
	fclose(h);
	}
	pthread_mutex_unlock( &list_nagios_service_control );
	return;
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   s e t   f i e l d 	*/
/*	------------------------------------------------------------------------------------------	*/
private void set_nagios_service_field(
	struct occi_category * cptr,void * optr, char * nptr, char * vptr)
{
	struct nagios_service * pptr;
	char prefix[1024];
	if (!( pptr = optr )) return;
	sprintf(prefix,"%s.%s.",cptr->domain,cptr->id);
	if (!( strncmp( nptr, prefix, strlen(prefix) ) )) {
		nptr += strlen(prefix);
		if (!( strcmp( nptr, "name" ) ))
			pptr->name = allocate_string(vptr);
		if (!( strcmp( nptr, "host_name" ) ))
			pptr->host_name = allocate_string(vptr);
		if (!( strcmp( nptr, "hostgroup_name" ) ))
			pptr->hostgroup_name = allocate_string(vptr);
		if (!( strcmp( nptr, "service_description" ) ))
			pptr->service_description = allocate_string(vptr);
		if (!( strcmp( nptr, "display_name" ) ))
			pptr->display_name = allocate_string(vptr);
		if (!( strcmp( nptr, "servicegroups" ) ))
			pptr->servicegroups = atoi(vptr);
		if (!( strcmp( nptr, "is_volatile" ) ))
			pptr->is_volatile = atoi(vptr);
		if (!( strcmp( nptr, "check_command" ) ))
			pptr->check_command = allocate_string(vptr);
		if (!( strcmp( nptr, "initial_state" ) ))
			pptr->initial_state = allocate_string(vptr);
		if (!( strcmp( nptr, "max_check_attempts" ) ))
			pptr->max_check_attempts = atoi(vptr);
		if (!( strcmp( nptr, "check_interval" ) ))
			pptr->check_interval = atoi(vptr);
		if (!( strcmp( nptr, "retry_interval" ) ))
			pptr->retry_interval = atoi(vptr);
		if (!( strcmp( nptr, "active_checks_enabled" ) ))
			pptr->active_checks_enabled = atoi(vptr);
		if (!( strcmp( nptr, "passive_checks_enabled" ) ))
			pptr->passive_checks_enabled = atoi(vptr);
		if (!( strcmp( nptr, "check_period" ) ))
			pptr->check_period = allocate_string(vptr);
		if (!( strcmp( nptr, "obsess_over_service" ) ))
			pptr->obsess_over_service = atoi(vptr);
		if (!( strcmp( nptr, "check_freshness" ) ))
			pptr->check_freshness = atoi(vptr);
		if (!( strcmp( nptr, "freshness_threshold" ) ))
			pptr->freshness_threshold = atoi(vptr);
		if (!( strcmp( nptr, "event_handler" ) ))
			pptr->event_handler = allocate_string(vptr);
		if (!( strcmp( nptr, "event_handler_enabled" ) ))
			pptr->event_handler_enabled = atoi(vptr);
		if (!( strcmp( nptr, "low_flap_threshold" ) ))
			pptr->low_flap_threshold = atoi(vptr);
		if (!( strcmp( nptr, "high_flap_threshold" ) ))
			pptr->high_flap_threshold = atoi(vptr);
		if (!( strcmp( nptr, "flap_detection_enabled" ) ))
			pptr->flap_detection_enabled = atoi(vptr);
		if (!( strcmp( nptr, "flap_detection_options" ) ))
			pptr->flap_detection_options = allocate_string(vptr);
		if (!( strcmp( nptr, "process_perf_data" ) ))
			pptr->process_perf_data = atoi(vptr);
		if (!( strcmp( nptr, "retain_status_information" ) ))
			pptr->retain_status_information = atoi(vptr);
		if (!( strcmp( nptr, "retain_nonstatus_information" ) ))
			pptr->retain_nonstatus_information = atoi(vptr);
		if (!( strcmp( nptr, "notification_interval" ) ))
			pptr->notification_interval = atoi(vptr);
		if (!( strcmp( nptr, "notification_period" ) ))
			pptr->notification_period = allocate_string(vptr);
		if (!( strcmp( nptr, "notification_options" ) ))
			pptr->notification_options = allocate_string(vptr);
		if (!( strcmp( nptr, "notifications_enabled" ) ))
			pptr->notifications_enabled = atoi(vptr);
		if (!( strcmp( nptr, "contacts" ) ))
			pptr->contacts = atoi(vptr);
		if (!( strcmp( nptr, "contact_groups" ) ))
			pptr->contact_groups = atoi(vptr);
		if (!( strcmp( nptr, "stalking_options" ) ))
			pptr->stalking_options = allocate_string(vptr);
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
		if (!( strcmp( nptr, "status" ) ))
			pptr->status = atoi(vptr);
		}
	return;
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   i n f o 	*/
/*	--------------------------------------------------	*/
private struct nagios_service * filter_nagios_service_info(
	struct occi_category * optr,
	struct rest_request  * rptr,
	struct rest_response * aptr) {
	struct nagios_service * pptr;
		if (!( pptr = allocate_nagios_service()))
		return( pptr );
	else if (!( occi_process_atributs(optr, rptr, aptr, pptr, set_nagios_service_field) ))
		return( liberate_nagios_service(pptr));
	else	return( pptr );
}

/*	--------------------------------------------------	*/
/*	o c c i   c a t e g o r y   f i l t e r   p a s s 	*/
/*	--------------------------------------------------	*/
private int pass_nagios_service_filter(
	struct nagios_service * pptr,struct nagios_service * fptr) {
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
	if (( fptr->hostgroup_name )
	&&  (strlen( fptr->hostgroup_name ) != 0)) {
		if (!( pptr->hostgroup_name ))
			return(0);
		else if ( strcmp(pptr->hostgroup_name,fptr->hostgroup_name) != 0)
			return(0);
		}
	if (( fptr->service_description )
	&&  (strlen( fptr->service_description ) != 0)) {
		if (!( pptr->service_description ))
			return(0);
		else if ( strcmp(pptr->service_description,fptr->service_description) != 0)
			return(0);
		}
	if (( fptr->display_name )
	&&  (strlen( fptr->display_name ) != 0)) {
		if (!( pptr->display_name ))
			return(0);
		else if ( strcmp(pptr->display_name,fptr->display_name) != 0)
			return(0);
		}
	if (( fptr->servicegroups ) && ( pptr->servicegroups != fptr->servicegroups )) return(0);
	if (( fptr->is_volatile ) && ( pptr->is_volatile != fptr->is_volatile )) return(0);
	if (( fptr->check_command )
	&&  (strlen( fptr->check_command ) != 0)) {
		if (!( pptr->check_command ))
			return(0);
		else if ( strcmp(pptr->check_command,fptr->check_command) != 0)
			return(0);
		}
	if (( fptr->initial_state )
	&&  (strlen( fptr->initial_state ) != 0)) {
		if (!( pptr->initial_state ))
			return(0);
		else if ( strcmp(pptr->initial_state,fptr->initial_state) != 0)
			return(0);
		}
	if (( fptr->max_check_attempts ) && ( pptr->max_check_attempts != fptr->max_check_attempts )) return(0);
	if (( fptr->check_interval ) && ( pptr->check_interval != fptr->check_interval )) return(0);
	if (( fptr->retry_interval ) && ( pptr->retry_interval != fptr->retry_interval )) return(0);
	if (( fptr->active_checks_enabled ) && ( pptr->active_checks_enabled != fptr->active_checks_enabled )) return(0);
	if (( fptr->passive_checks_enabled ) && ( pptr->passive_checks_enabled != fptr->passive_checks_enabled )) return(0);
	if (( fptr->check_period )
	&&  (strlen( fptr->check_period ) != 0)) {
		if (!( pptr->check_period ))
			return(0);
		else if ( strcmp(pptr->check_period,fptr->check_period) != 0)
			return(0);
		}
	if (( fptr->obsess_over_service ) && ( pptr->obsess_over_service != fptr->obsess_over_service )) return(0);
	if (( fptr->check_freshness ) && ( pptr->check_freshness != fptr->check_freshness )) return(0);
	if (( fptr->freshness_threshold ) && ( pptr->freshness_threshold != fptr->freshness_threshold )) return(0);
	if (( fptr->event_handler )
	&&  (strlen( fptr->event_handler ) != 0)) {
		if (!( pptr->event_handler ))
			return(0);
		else if ( strcmp(pptr->event_handler,fptr->event_handler) != 0)
			return(0);
		}
	if (( fptr->event_handler_enabled ) && ( pptr->event_handler_enabled != fptr->event_handler_enabled )) return(0);
	if (( fptr->low_flap_threshold ) && ( pptr->low_flap_threshold != fptr->low_flap_threshold )) return(0);
	if (( fptr->high_flap_threshold ) && ( pptr->high_flap_threshold != fptr->high_flap_threshold )) return(0);
	if (( fptr->flap_detection_enabled ) && ( pptr->flap_detection_enabled != fptr->flap_detection_enabled )) return(0);
	if (( fptr->flap_detection_options )
	&&  (strlen( fptr->flap_detection_options ) != 0)) {
		if (!( pptr->flap_detection_options ))
			return(0);
		else if ( strcmp(pptr->flap_detection_options,fptr->flap_detection_options) != 0)
			return(0);
		}
	if (( fptr->process_perf_data ) && ( pptr->process_perf_data != fptr->process_perf_data )) return(0);
	if (( fptr->retain_status_information ) && ( pptr->retain_status_information != fptr->retain_status_information )) return(0);
	if (( fptr->retain_nonstatus_information ) && ( pptr->retain_nonstatus_information != fptr->retain_nonstatus_information )) return(0);
	if (( fptr->notification_interval ) && ( pptr->notification_interval != fptr->notification_interval )) return(0);
	if (( fptr->notification_period )
	&&  (strlen( fptr->notification_period ) != 0)) {
		if (!( pptr->notification_period ))
			return(0);
		else if ( strcmp(pptr->notification_period,fptr->notification_period) != 0)
			return(0);
		}
	if (( fptr->notification_options )
	&&  (strlen( fptr->notification_options ) != 0)) {
		if (!( pptr->notification_options ))
			return(0);
		else if ( strcmp(pptr->notification_options,fptr->notification_options) != 0)
			return(0);
		}
	if (( fptr->notifications_enabled ) && ( pptr->notifications_enabled != fptr->notifications_enabled )) return(0);
	if (( fptr->contacts ) && ( pptr->contacts != fptr->contacts )) return(0);
	if (( fptr->contact_groups ) && ( pptr->contact_groups != fptr->contact_groups )) return(0);
	if (( fptr->stalking_options )
	&&  (strlen( fptr->stalking_options ) != 0)) {
		if (!( pptr->stalking_options ))
			return(0);
		else if ( strcmp(pptr->stalking_options,fptr->stalking_options) != 0)
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
	if (( fptr->status ) && ( pptr->status != fptr->status )) return(0);
	return(1);
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   r e s p o n s e 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_service_occi_response(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,
	struct nagios_service * pptr)
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
	sprintf(cptr->buffer,"%s.%s.hostgroup_name=%c%s%c",optr->domain,optr->id,0x0022,pptr->hostgroup_name,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.service_description=%c%s%c",optr->domain,optr->id,0x0022,pptr->service_description,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.display_name=%c%s%c",optr->domain,optr->id,0x0022,pptr->display_name,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.servicegroups=%c%u%c",optr->domain,optr->id,0x0022,pptr->servicegroups,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.is_volatile=%c%u%c",optr->domain,optr->id,0x0022,pptr->is_volatile,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.check_command=%c%s%c",optr->domain,optr->id,0x0022,pptr->check_command,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.initial_state=%c%s%c",optr->domain,optr->id,0x0022,pptr->initial_state,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.max_check_attempts=%c%u%c",optr->domain,optr->id,0x0022,pptr->max_check_attempts,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.check_interval=%c%u%c",optr->domain,optr->id,0x0022,pptr->check_interval,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.retry_interval=%c%u%c",optr->domain,optr->id,0x0022,pptr->retry_interval,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.active_checks_enabled=%c%u%c",optr->domain,optr->id,0x0022,pptr->active_checks_enabled,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.passive_checks_enabled=%c%u%c",optr->domain,optr->id,0x0022,pptr->passive_checks_enabled,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.check_period=%c%s%c",optr->domain,optr->id,0x0022,pptr->check_period,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.obsess_over_service=%c%u%c",optr->domain,optr->id,0x0022,pptr->obsess_over_service,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.check_freshness=%c%u%c",optr->domain,optr->id,0x0022,pptr->check_freshness,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.freshness_threshold=%c%u%c",optr->domain,optr->id,0x0022,pptr->freshness_threshold,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.event_handler=%c%s%c",optr->domain,optr->id,0x0022,pptr->event_handler,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.event_handler_enabled=%c%u%c",optr->domain,optr->id,0x0022,pptr->event_handler_enabled,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.low_flap_threshold=%c%u%c",optr->domain,optr->id,0x0022,pptr->low_flap_threshold,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.high_flap_threshold=%c%u%c",optr->domain,optr->id,0x0022,pptr->high_flap_threshold,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.flap_detection_enabled=%c%u%c",optr->domain,optr->id,0x0022,pptr->flap_detection_enabled,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.flap_detection_options=%c%s%c",optr->domain,optr->id,0x0022,pptr->flap_detection_options,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.process_perf_data=%c%u%c",optr->domain,optr->id,0x0022,pptr->process_perf_data,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.retain_status_information=%c%u%c",optr->domain,optr->id,0x0022,pptr->retain_status_information,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.retain_nonstatus_information=%c%u%c",optr->domain,optr->id,0x0022,pptr->retain_nonstatus_information,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.notification_interval=%c%u%c",optr->domain,optr->id,0x0022,pptr->notification_interval,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.notification_period=%c%s%c",optr->domain,optr->id,0x0022,pptr->notification_period,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.notification_options=%c%s%c",optr->domain,optr->id,0x0022,pptr->notification_options,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.notifications_enabled=%c%u%c",optr->domain,optr->id,0x0022,pptr->notifications_enabled,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.contacts=%c%u%c",optr->domain,optr->id,0x0022,pptr->contacts,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.contact_groups=%c%u%c",optr->domain,optr->id,0x0022,pptr->contact_groups,0x0022);
	if (!( hptr = rest_response_header( aptr, "X-OCCI-Attribute",cptr->buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure" ) );
	sprintf(cptr->buffer,"%s.%s.stalking_options=%c%s%c",optr->domain,optr->id,0x0022,pptr->stalking_options,0x0022);
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
private struct rest_response * nagios_service_get_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_service * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_nagios_service_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->retrieve)) (*iptr->retrieve)(optr,nptr,rptr);
	autosave_nagios_service_nodes();
	return( nagios_service_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   l i n k 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_service_post_link(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_service * pptr;
	char * reqhost;
	if (!( nptr = locate_nagios_service_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   m i x i n 	*/
/*	--------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_service_post_mixin(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_service * pptr;
	char * reqhost;
	if (!( nptr = locate_nagios_service_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request"));
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t   a c t i o n 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_service_post_action(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_action * fptr;
	struct occi_kind_node * nptr;
	struct nagios_service * pptr;
	char * reqhost;
	char * mptr;
	if (!( nptr = locate_nagios_service_node(id)))
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
private struct rest_response * nagios_service_post_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_service * pptr;
	char * reqhost;
	iptr = optr->callback;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	if (!( nptr = add_nagios_service_node(1)))
		return( rest_html_response( aptr, 500, "Server Failure") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (!( occi_process_atributs( optr, rptr,aptr, pptr, set_nagios_service_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->create)) (*iptr->create)(optr,nptr,rptr);
	autosave_nagios_service_nodes();
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
private struct rest_response * nagios_service_put_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_service * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_nagios_service_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (!( occi_process_atributs(optr,rptr,aptr, pptr, set_nagios_service_field ) ))
		return( rest_html_response( aptr, 500, "Server Failure") );
	if (( iptr ) && (iptr->update)) (*iptr->update)(optr,nptr,rptr);
	autosave_nagios_service_nodes();
	return( nagios_service_occi_response(optr,cptr,rptr,aptr,pptr));
}

/*	------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d   i t e m 	*/
/*	------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_service_head_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr,char * id)
{
	struct rest_header * hptr;
	struct occi_kind_node * nptr;
	struct nagios_service * pptr;
	if (!( nptr = locate_nagios_service_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	else if (!( pptr = nptr->contents ))
		return( rest_html_response( aptr, 404, "Not Found") );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	----------------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e   i t e m 	*/
/*	----------------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_service_delete_item(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr, char * id)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct nagios_service * pptr;
	iptr = optr->callback;
	if (!( nptr = locate_nagios_service_node(id)))
		return( rest_html_response( aptr, 404, "Not Found") );
	if (( iptr ) && (iptr->delete)) (*iptr->delete)(optr,nptr,rptr);
	drop_nagios_service_node( nptr );
	autosave_nagios_service_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	----------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t   l i s t 	*/
/*	----------------------------------------------------------------------------------------	*/
private struct rest_response * nagios_service_get_list(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_kind_node * sptr;
	struct nagios_service * pptr;
	struct nagios_service * fptr;
	char * reqhost;
	if (!( reqhost = rest_request_host( rptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	else if (!( fptr = filter_nagios_service_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	for ( sptr = nagios_service_first;
		sptr != (struct occi_kind_node *) 0;
		sptr = sptr->next ) {
		if (!( pptr = sptr->contents ))
			continue;
		if (!( pass_nagios_service_filter( pptr, fptr ) ))
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
private struct rest_response * nagios_service_delete_all(
	struct occi_category * optr, struct rest_client * cptr,
	struct rest_request * rptr, struct rest_response * aptr)
{
	struct rest_header * hptr;
	struct occi_interface * iptr;
	struct occi_kind_node * nptr;
	struct occi_kind_node * sptr;
	struct nagios_service * pptr;
	struct nagios_service * fptr;
	iptr = optr->callback;
	if (!( fptr = filter_nagios_service_info( optr, rptr, aptr ) ))
		return( rest_html_response( aptr, 400, "Bad Request" ) );
	nptr=nagios_service_first;
	while (nptr != (struct occi_kind_node *) 0) {
		if ((!( pptr = nptr->contents ))
		||  (!( pass_nagios_service_filter( pptr, fptr ) ))) {
			nptr = nptr->next;
			continue;
			}
		else	{
			if (( iptr ) && (iptr->delete)) { (*iptr->delete)(optr,nptr,rptr); }
			sptr = nptr->next;
			drop_nagios_service_node( nptr );
			nptr = sptr;
			}
		}
	autosave_nagios_service_nodes();
	if (!( occi_success( aptr ) ))
		return( rest_response_status( aptr, 500, "Server Failure" ) );
	else	return( rest_response_status( aptr, 200, "OK" ) );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   g e t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_service_get(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_service_get_list( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( nagios_service_get_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   h e a d 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_service_head(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_service_head_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p o s t 	*/
/*	--------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_service_post(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_service_post_item( optr, cptr, rptr, aptr ) );
	else if ( strncmp( rptr->object, optr->location,strlen(optr->location)) != 0)
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( rptr->parameters ))
		return( rest_html_response( aptr, 400, "Bad Request") );
	else if (!( strncmp( rptr->parameters, "action=", strlen("action=")) ))
		return( nagios_service_post_action( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "mixin=", strlen("mixin=")) ))
		return( nagios_service_post_mixin( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else if (!( strncmp( rptr->parameters, "link=", strlen("link=")) ))
		return( nagios_service_post_link( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   p u t 	*/
/*	------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_service_put(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_service_put_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	------------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   m e t h o d   d e l e t e 	*/
/*	------------------------------------------------------------------------------------	*/
private struct rest_response * occi_nagios_service_delete(void * vptr, struct rest_client * cptr, struct rest_request * rptr)
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
		return( nagios_service_delete_all( optr, cptr, rptr, aptr ) );
	else if (!(strncmp( rptr->object, optr->location, strlen(optr->location) ) ))
		return( nagios_service_delete_item( optr, cptr, rptr, aptr,rptr->object+strlen(optr->location) ) );
	else	return( rest_html_response( aptr, 400, "Bad Request") );
}

/*	--------------------------------------------------------------------------------	*/
/*	o c c i   c a t e g o r y   r e s t   i n t e r f a c e   r e d i r e c t i o n 	*/
/*	--------------------------------------------------------------------------------	*/
private void	redirect_occi_nagios_service_mt( struct rest_interface * iptr )
{
	iptr->get = occi_nagios_service_get;
	iptr->post = occi_nagios_service_post;
	iptr->put = occi_nagios_service_put;
	iptr->delete = occi_nagios_service_delete;
	iptr->head = occi_nagios_service_head;
	return;
}

/*	------------------------------------	*/
/*	c r u d   d e l e t e   a c t i o n 	*/
/*	------------------------------------	*/
private struct rest_response * delete_action_nagios_service(struct occi_category * optr, 
struct rest_client * cptr,  
struct rest_request * rptr,  
struct rest_response * aptr,  
void * vptr )
{
	aptr = liberate_rest_response( aptr );
	return( occi_nagios_service_delete(optr,cptr,rptr));
}

/*	------------------------------------------	*/
/*	o c c i   c a t e g o r y   b u i l d e r 	*/
/*	------------------------------------------	*/
/* occi category rest instance builder for : occi_nagios_service */
public struct occi_category * occi_nagios_service_builder(char * a,char * b) {
	char * c="http://scheme.compatibleone.fr/scheme/compatible#";
	char * d="kind";
	char * e="http://scheme.ogf.org/occi/resource#";
	char * f="CompatibleOne OCCI resource nagios_service";
	struct occi_category * optr;
	if (!( optr = occi_create_category(a,b,c,d,e,f) )) { return(optr); }
	else {
		redirect_occi_nagios_service_mt(optr->interface);
		if (!( optr = occi_add_attribute(optr, "name",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "host_name",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "hostgroup_name",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "service_description",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "display_name",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "servicegroups",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "is_volatile",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "check_command",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "initial_state",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "max_check_attempts",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "check_interval",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "retry_interval",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "active_checks_enabled",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "passive_checks_enabled",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "check_period",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "obsess_over_service",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "check_freshness",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "freshness_threshold",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "event_handler",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "event_handler_enabled",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "low_flap_threshold",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "high_flap_threshold",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "flap_detection_enabled",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "flap_detection_options",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "process_perf_data",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "retain_status_information",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "retain_nonstatus_information",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "notification_interval",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "notification_period",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "notification_options",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "notifications_enabled",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "contacts",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "contact_groups",0,0) ))
			return(optr);
		if (!( optr = occi_add_attribute(optr, "stalking_options",0,0) ))
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
		if (!( optr = occi_add_attribute(optr, "status",0,0) ))
			return(optr);
		if (!( optr = occi_add_action( optr,"DELETE","",delete_action_nagios_service)))
			return( optr );
		autoload_nagios_service_nodes();
		return(optr);
	}

}

/*	------------------------------------------------------	*/
/*	n a g i o s _ s e r v i c e _ o c c i _ h e a d e r s 	*/
/*	------------------------------------------------------	*/
public struct rest_header *  nagios_service_occi_headers(struct nagios_service * sptr)
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
	sprintf(buffer,"nagios_service; scheme='http://scheme.compatibleone.fr/scheme/compatible#'; class='kind';\r\n");
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
	sprintf(buffer,"occi.nagios_service.name='%s'\r\n",(sptr->name?sptr->name:""));
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
	sprintf(buffer,"occi.nagios_service.host_name='%s'\r\n",(sptr->host_name?sptr->host_name:""));
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
	sprintf(buffer,"occi.nagios_service.hostgroup_name='%s'\r\n",(sptr->hostgroup_name?sptr->hostgroup_name:""));
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
	sprintf(buffer,"occi.nagios_service.service_description='%s'\r\n",(sptr->service_description?sptr->service_description:""));
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
	sprintf(buffer,"occi.nagios_service.display_name='%s'\r\n",(sptr->display_name?sptr->display_name:""));
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
	sprintf(buffer,"occi.nagios_service.servicegroups='%u'\r\n",sptr->servicegroups);
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
	sprintf(buffer,"occi.nagios_service.is_volatile='%u'\r\n",sptr->is_volatile);
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
	sprintf(buffer,"occi.nagios_service.check_command='%s'\r\n",(sptr->check_command?sptr->check_command:""));
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
	sprintf(buffer,"occi.nagios_service.initial_state='%s'\r\n",(sptr->initial_state?sptr->initial_state:""));
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
	sprintf(buffer,"occi.nagios_service.max_check_attempts='%u'\r\n",sptr->max_check_attempts);
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
	sprintf(buffer,"occi.nagios_service.check_interval='%u'\r\n",sptr->check_interval);
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
	sprintf(buffer,"occi.nagios_service.retry_interval='%u'\r\n",sptr->retry_interval);
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
	sprintf(buffer,"occi.nagios_service.active_checks_enabled='%u'\r\n",sptr->active_checks_enabled);
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
	sprintf(buffer,"occi.nagios_service.passive_checks_enabled='%u'\r\n",sptr->passive_checks_enabled);
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
	sprintf(buffer,"occi.nagios_service.check_period='%s'\r\n",(sptr->check_period?sptr->check_period:""));
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
	sprintf(buffer,"occi.nagios_service.obsess_over_service='%u'\r\n",sptr->obsess_over_service);
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
	sprintf(buffer,"occi.nagios_service.check_freshness='%u'\r\n",sptr->check_freshness);
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
	sprintf(buffer,"occi.nagios_service.freshness_threshold='%u'\r\n",sptr->freshness_threshold);
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
	sprintf(buffer,"occi.nagios_service.event_handler='%s'\r\n",(sptr->event_handler?sptr->event_handler:""));
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
	sprintf(buffer,"occi.nagios_service.event_handler_enabled='%u'\r\n",sptr->event_handler_enabled);
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
	sprintf(buffer,"occi.nagios_service.low_flap_threshold='%u'\r\n",sptr->low_flap_threshold);
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
	sprintf(buffer,"occi.nagios_service.high_flap_threshold='%u'\r\n",sptr->high_flap_threshold);
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
	sprintf(buffer,"occi.nagios_service.flap_detection_enabled='%u'\r\n",sptr->flap_detection_enabled);
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
	sprintf(buffer,"occi.nagios_service.flap_detection_options='%s'\r\n",(sptr->flap_detection_options?sptr->flap_detection_options:""));
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
	sprintf(buffer,"occi.nagios_service.process_perf_data='%u'\r\n",sptr->process_perf_data);
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
	sprintf(buffer,"occi.nagios_service.retain_status_information='%u'\r\n",sptr->retain_status_information);
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
	sprintf(buffer,"occi.nagios_service.retain_nonstatus_information='%u'\r\n",sptr->retain_nonstatus_information);
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
	sprintf(buffer,"occi.nagios_service.notification_interval='%u'\r\n",sptr->notification_interval);
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
	sprintf(buffer,"occi.nagios_service.notification_period='%s'\r\n",(sptr->notification_period?sptr->notification_period:""));
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
	sprintf(buffer,"occi.nagios_service.notification_options='%s'\r\n",(sptr->notification_options?sptr->notification_options:""));
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
	sprintf(buffer,"occi.nagios_service.notifications_enabled='%u'\r\n",sptr->notifications_enabled);
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
	sprintf(buffer,"occi.nagios_service.contacts='%u'\r\n",sptr->contacts);
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
	sprintf(buffer,"occi.nagios_service.contact_groups='%u'\r\n",sptr->contact_groups);
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
	sprintf(buffer,"occi.nagios_service.stalking_options='%s'\r\n",(sptr->stalking_options?sptr->stalking_options:""));
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
	sprintf(buffer,"occi.nagios_service.notes='%s'\r\n",(sptr->notes?sptr->notes:""));
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
	sprintf(buffer,"occi.nagios_service.notes_url='%s'\r\n",(sptr->notes_url?sptr->notes_url:""));
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
	sprintf(buffer,"occi.nagios_service.action_url='%s'\r\n",(sptr->action_url?sptr->action_url:""));
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
	sprintf(buffer,"occi.nagios_service.icon_image='%s'\r\n",(sptr->icon_image?sptr->icon_image:""));
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
	sprintf(buffer,"occi.nagios_service.icon_image_alt='%s'\r\n",(sptr->icon_image_alt?sptr->icon_image_alt:""));
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
	sprintf(buffer,"occi.nagios_service.status='%u'\r\n",sptr->status);
	if (!( hptr->value = allocate_string(buffer)))
		return(first);
	return(first);

}

#endif	/* _occinagiosservice_c_ */
