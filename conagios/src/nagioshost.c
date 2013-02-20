/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _nagioshost_c_
#define _nagioshost_c_

#include "element.h"

#include "nagioshost.h"

/*	----------------------------------------	*/
/*	l i b e r a t e _ n a g i o s _ h o s t 	*/
/*	----------------------------------------	*/
public struct nagios_host * liberate_nagios_host(struct nagios_host * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->alias )
			 sptr->alias = liberate(sptr->alias);
		if ( sptr->display_name )
			 sptr->display_name = liberate(sptr->display_name);
		if ( sptr->address )
			 sptr->address = liberate(sptr->address);
		if ( sptr->check_command )
			 sptr->check_command = liberate(sptr->check_command);
		if ( sptr->initial_state )
			 sptr->initial_state = liberate(sptr->initial_state);
		if ( sptr->check_period )
			 sptr->check_period = liberate(sptr->check_period);
		if ( sptr->event_handler )
			 sptr->event_handler = liberate(sptr->event_handler);
		if ( sptr->flap_detection_options )
			 sptr->flap_detection_options = liberate(sptr->flap_detection_options);
		if ( sptr->notification_period )
			 sptr->notification_period = liberate(sptr->notification_period);
		if ( sptr->notification_options )
			 sptr->notification_options = liberate(sptr->notification_options);
		if ( sptr->stalking_options )
			 sptr->stalking_options = liberate(sptr->stalking_options);
		if ( sptr->notes )
			 sptr->notes = liberate(sptr->notes);
		if ( sptr->notes_url )
			 sptr->notes_url = liberate(sptr->notes_url);
		if ( sptr->action_url )
			 sptr->action_url = liberate(sptr->action_url);
		if ( sptr->icon_image )
			 sptr->icon_image = liberate(sptr->icon_image);
		if ( sptr->icon_image_alt )
			 sptr->icon_image_alt = liberate(sptr->icon_image_alt);
		if ( sptr->vrml_image )
			 sptr->vrml_image = liberate(sptr->vrml_image);
		if ( sptr->statusmap_image )
			 sptr->statusmap_image = liberate(sptr->statusmap_image);
		if ( sptr->twod_coords )
			 sptr->twod_coords = liberate(sptr->twod_coords);
		if ( sptr->threed_coords )
			 sptr->threed_coords = liberate(sptr->threed_coords);
		sptr = liberate( sptr );
	}
	return((struct nagios_host *) 0);

}

/*	----------------------------------	*/
/*	r e s e t _ n a g i o s _ h o s t 	*/
/*	----------------------------------	*/
public struct nagios_host * reset_nagios_host(struct nagios_host * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->alias = (char*) 0;
		sptr->display_name = (char*) 0;
		sptr->address = (char*) 0;
		sptr->parents =  0;
		sptr->hostgroups =  0;
		sptr->check_command = (char*) 0;
		sptr->initial_state = (char*) 0;
		sptr->max_check_attempts =  0;
		sptr->check_interval =  0;
		sptr->retry_interval =  0;
		sptr->active_checks_enabled =  0;
		sptr->passive_checks_enabled =  0;
		sptr->check_period = (char*) 0;
		sptr->obsess_over_host =  0;
		sptr->check_freshness =  0;
		sptr->freshness_threshold =  0;
		sptr->event_handler = (char*) 0;
		sptr->event_handler_enabled =  0;
		sptr->low_flap_threshold =  0;
		sptr->high_flap_threshold =  0;
		sptr->flap_detection_enabled =  0;
		sptr->flap_detection_options = (char*) 0;
		sptr->process_perf_data =  0;
		sptr->retain_status_information =  0;
		sptr->retain_nonstatus_information =  0;
		sptr->contacts =  0;
		sptr->contact_groups =  0;
		sptr->notification_interval =  0;
		sptr->first_notification_delay =  0;
		sptr->notification_period = (char*) 0;
		sptr->notification_options = (char*) 0;
		sptr->notifications_enabled =  0;
		sptr->stalking_options = (char*) 0;
		sptr->notes = (char*) 0;
		sptr->notes_url = (char*) 0;
		sptr->action_url = (char*) 0;
		sptr->icon_image = (char*) 0;
		sptr->icon_image_alt = (char*) 0;
		sptr->vrml_image = (char*) 0;
		sptr->statusmap_image = (char*) 0;
		sptr->twod_coords = (char*) 0;
		sptr->threed_coords = (char*) 0;
		sptr->status =  0;
	}
	return(sptr);

}

/*	----------------------------------------	*/
/*	a l l o c a t e _ n a g i o s _ h o s t 	*/
/*	----------------------------------------	*/
public struct nagios_host * allocate_nagios_host()
{
	struct nagios_host * sptr;
	if (!( sptr = allocate( sizeof( struct nagios_host ) ) ))
		return( sptr );
	else	return( reset_nagios_host(sptr) );
}

/*	----------------------------------	*/
/*	x m l i n _ n a g i o s _ h o s t 	*/
/*	----------------------------------	*/
public int xmlin_nagios_host(struct nagios_host * sptr,struct xml_element * eptr)
{
	struct xml_element * wptr;
	if (!( eptr )) return(0);
	if (!( sptr )) return(0);
	for ( wptr=eptr->first; wptr != (struct xml_element *) 0; wptr=wptr->next)
	{
		if (!( strcmp(wptr->name,"id") ))
		{
			if ( wptr->value ) { sptr->id = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"name") ))
		{
			if ( wptr->value ) { sptr->name = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"alias") ))
		{
			if ( wptr->value ) { sptr->alias = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"display_name") ))
		{
			if ( wptr->value ) { sptr->display_name = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"address") ))
		{
			if ( wptr->value ) { sptr->address = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"parents") ))
		{
			if ( wptr->value ) { sptr->parents = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"hostgroups") ))
		{
			if ( wptr->value ) { sptr->hostgroups = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"check_command") ))
		{
			if ( wptr->value ) { sptr->check_command = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"initial_state") ))
		{
			if ( wptr->value ) { sptr->initial_state = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"max_check_attempts") ))
		{
			if ( wptr->value ) { sptr->max_check_attempts = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"check_interval") ))
		{
			if ( wptr->value ) { sptr->check_interval = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"retry_interval") ))
		{
			if ( wptr->value ) { sptr->retry_interval = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"active_checks_enabled") ))
		{
			if ( wptr->value ) { sptr->active_checks_enabled = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"passive_checks_enabled") ))
		{
			if ( wptr->value ) { sptr->passive_checks_enabled = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"check_period") ))
		{
			if ( wptr->value ) { sptr->check_period = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"obsess_over_host") ))
		{
			if ( wptr->value ) { sptr->obsess_over_host = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"check_freshness") ))
		{
			if ( wptr->value ) { sptr->check_freshness = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"freshness_threshold") ))
		{
			if ( wptr->value ) { sptr->freshness_threshold = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"event_handler") ))
		{
			if ( wptr->value ) { sptr->event_handler = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"event_handler_enabled") ))
		{
			if ( wptr->value ) { sptr->event_handler_enabled = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"low_flap_threshold") ))
		{
			if ( wptr->value ) { sptr->low_flap_threshold = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"high_flap_threshold") ))
		{
			if ( wptr->value ) { sptr->high_flap_threshold = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"flap_detection_enabled") ))
		{
			if ( wptr->value ) { sptr->flap_detection_enabled = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"flap_detection_options") ))
		{
			if ( wptr->value ) { sptr->flap_detection_options = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"process_perf_data") ))
		{
			if ( wptr->value ) { sptr->process_perf_data = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"retain_status_information") ))
		{
			if ( wptr->value ) { sptr->retain_status_information = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"retain_nonstatus_information") ))
		{
			if ( wptr->value ) { sptr->retain_nonstatus_information = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"contacts") ))
		{
			if ( wptr->value ) { sptr->contacts = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"contact_groups") ))
		{
			if ( wptr->value ) { sptr->contact_groups = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"notification_interval") ))
		{
			if ( wptr->value ) { sptr->notification_interval = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"first_notification_delay") ))
		{
			if ( wptr->value ) { sptr->first_notification_delay = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"notification_period") ))
		{
			if ( wptr->value ) { sptr->notification_period = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"notification_options") ))
		{
			if ( wptr->value ) { sptr->notification_options = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"notifications_enabled") ))
		{
			if ( wptr->value ) { sptr->notifications_enabled = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"stalking_options") ))
		{
			if ( wptr->value ) { sptr->stalking_options = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"notes") ))
		{
			if ( wptr->value ) { sptr->notes = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"notes_url") ))
		{
			if ( wptr->value ) { sptr->notes_url = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"action_url") ))
		{
			if ( wptr->value ) { sptr->action_url = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"icon_image") ))
		{
			if ( wptr->value ) { sptr->icon_image = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"icon_image_alt") ))
		{
			if ( wptr->value ) { sptr->icon_image_alt = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"vrml_image") ))
		{
			if ( wptr->value ) { sptr->vrml_image = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"statusmap_image") ))
		{
			if ( wptr->value ) { sptr->statusmap_image = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"twod_coords") ))
		{
			if ( wptr->value ) { sptr->twod_coords = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"threed_coords") ))
		{
			if ( wptr->value ) { sptr->threed_coords = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"status") ))
		{
			if ( wptr->value ) { sptr->status = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------	*/
/*	r e s t _ o c c i _ n a g i o s _ h o s t 	*/
/*	------------------------------------------	*/
public int rest_occi_nagios_host(FILE * fh,struct nagios_host * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.alias='%s'\r\n",prefix,nptr,(sptr->alias?sptr->alias:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.display_name='%s'\r\n",prefix,nptr,(sptr->display_name?sptr->display_name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.address='%s'\r\n",prefix,nptr,(sptr->address?sptr->address:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.parents='%u'\r\n",prefix,nptr,sptr->parents);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.hostgroups='%u'\r\n",prefix,nptr,sptr->hostgroups);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.check_command='%s'\r\n",prefix,nptr,(sptr->check_command?sptr->check_command:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.initial_state='%s'\r\n",prefix,nptr,(sptr->initial_state?sptr->initial_state:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.max_check_attempts='%u'\r\n",prefix,nptr,sptr->max_check_attempts);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.check_interval='%u'\r\n",prefix,nptr,sptr->check_interval);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.retry_interval='%u'\r\n",prefix,nptr,sptr->retry_interval);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.active_checks_enabled='%u'\r\n",prefix,nptr,sptr->active_checks_enabled);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.passive_checks_enabled='%u'\r\n",prefix,nptr,sptr->passive_checks_enabled);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.check_period='%s'\r\n",prefix,nptr,(sptr->check_period?sptr->check_period:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.obsess_over_host='%u'\r\n",prefix,nptr,sptr->obsess_over_host);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.check_freshness='%u'\r\n",prefix,nptr,sptr->check_freshness);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.freshness_threshold='%u'\r\n",prefix,nptr,sptr->freshness_threshold);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.event_handler='%s'\r\n",prefix,nptr,(sptr->event_handler?sptr->event_handler:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.event_handler_enabled='%u'\r\n",prefix,nptr,sptr->event_handler_enabled);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.low_flap_threshold='%u'\r\n",prefix,nptr,sptr->low_flap_threshold);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.high_flap_threshold='%u'\r\n",prefix,nptr,sptr->high_flap_threshold);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.flap_detection_enabled='%u'\r\n",prefix,nptr,sptr->flap_detection_enabled);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.flap_detection_options='%s'\r\n",prefix,nptr,(sptr->flap_detection_options?sptr->flap_detection_options:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.process_perf_data='%u'\r\n",prefix,nptr,sptr->process_perf_data);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.retain_status_information='%u'\r\n",prefix,nptr,sptr->retain_status_information);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.retain_nonstatus_information='%u'\r\n",prefix,nptr,sptr->retain_nonstatus_information);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.contacts='%u'\r\n",prefix,nptr,sptr->contacts);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.contact_groups='%u'\r\n",prefix,nptr,sptr->contact_groups);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.notification_interval='%u'\r\n",prefix,nptr,sptr->notification_interval);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.first_notification_delay='%u'\r\n",prefix,nptr,sptr->first_notification_delay);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.notification_period='%s'\r\n",prefix,nptr,(sptr->notification_period?sptr->notification_period:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.notification_options='%s'\r\n",prefix,nptr,(sptr->notification_options?sptr->notification_options:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.notifications_enabled='%u'\r\n",prefix,nptr,sptr->notifications_enabled);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.stalking_options='%s'\r\n",prefix,nptr,(sptr->stalking_options?sptr->stalking_options:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.notes='%s'\r\n",prefix,nptr,(sptr->notes?sptr->notes:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.notes_url='%s'\r\n",prefix,nptr,(sptr->notes_url?sptr->notes_url:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.action_url='%s'\r\n",prefix,nptr,(sptr->action_url?sptr->action_url:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.icon_image='%s'\r\n",prefix,nptr,(sptr->icon_image?sptr->icon_image:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.icon_image_alt='%s'\r\n",prefix,nptr,(sptr->icon_image_alt?sptr->icon_image_alt:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.vrml_image='%s'\r\n",prefix,nptr,(sptr->vrml_image?sptr->vrml_image:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.statusmap_image='%s'\r\n",prefix,nptr,(sptr->statusmap_image?sptr->statusmap_image:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.twod_coords='%s'\r\n",prefix,nptr,(sptr->twod_coords?sptr->twod_coords:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.threed_coords='%s'\r\n",prefix,nptr,(sptr->threed_coords?sptr->threed_coords:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.status='%u'\r\n",prefix,nptr,sptr->status);
	return(0);

}

#endif	/* _nagioshost_cnagioshost_c_ */
