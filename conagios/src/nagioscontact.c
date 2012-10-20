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
#ifndef _nagioscontact_c_
#define _nagioscontact_c_

#include "element.h"

#include "nagioscontact.h"

/*	----------------------------------------------	*/
/*	l i b e r a t e _ n a g i o s _ c o n t a c t 	*/
/*	----------------------------------------------	*/
public struct nagios_contact * liberate_nagios_contact(struct nagios_contact * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->alias )
			 sptr->alias = liberate(sptr->alias);
		if ( sptr->host_notification_period )
			 sptr->host_notification_period = liberate(sptr->host_notification_period);
		if ( sptr->service_notification_period )
			 sptr->service_notification_period = liberate(sptr->service_notification_period);
		if ( sptr->host_notification_options )
			 sptr->host_notification_options = liberate(sptr->host_notification_options);
		if ( sptr->service_notification_options )
			 sptr->service_notification_options = liberate(sptr->service_notification_options);
		if ( sptr->email )
			 sptr->email = liberate(sptr->email);
		if ( sptr->pager )
			 sptr->pager = liberate(sptr->pager);
		if ( sptr->addressx )
			 sptr->addressx = liberate(sptr->addressx);
		sptr = liberate( sptr );
	}
	return((struct nagios_contact *) 0);

}

/*	----------------------------------------	*/
/*	r e s e t _ n a g i o s _ c o n t a c t 	*/
/*	----------------------------------------	*/
public struct nagios_contact * reset_nagios_contact(struct nagios_contact * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->alias = (char*) 0;
		sptr->host_notification_period = (char*) 0;
		sptr->service_notification_period = (char*) 0;
		sptr->host_notification_options = (char*) 0;
		sptr->service_notification_options = (char*) 0;
		sptr->email = (char*) 0;
		sptr->pager = (char*) 0;
		sptr->addressx = (char*) 0;
		sptr->host_notifications_enabled =  0;
		sptr->service_notifications_enabled =  0;
		sptr->can_submit_commands =  0;
		sptr->retain_status_information =  0;
		sptr->retain_nonstatus_information =  0;
		sptr->status =  0;
	}
	return(sptr);

}

/*	----------------------------------------------	*/
/*	a l l o c a t e _ n a g i o s _ c o n t a c t 	*/
/*	----------------------------------------------	*/
public struct nagios_contact * allocate_nagios_contact()
{
	struct nagios_contact * sptr;
	if (!( sptr = allocate( sizeof( struct nagios_contact ) ) ))
		return( sptr );
	else	return( reset_nagios_contact(sptr) );
}

/*	----------------------------------------	*/
/*	x m l i n _ n a g i o s _ c o n t a c t 	*/
/*	----------------------------------------	*/
public int xmlin_nagios_contact(struct nagios_contact * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"host_notification_period") ))
		{
			if ( wptr->value ) { sptr->host_notification_period = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"service_notification_period") ))
		{
			if ( wptr->value ) { sptr->service_notification_period = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"host_notification_options") ))
		{
			if ( wptr->value ) { sptr->host_notification_options = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"service_notification_options") ))
		{
			if ( wptr->value ) { sptr->service_notification_options = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"email") ))
		{
			if ( wptr->value ) { sptr->email = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"pager") ))
		{
			if ( wptr->value ) { sptr->pager = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"addressx") ))
		{
			if ( wptr->value ) { sptr->addressx = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"host_notifications_enabled") ))
		{
			if ( wptr->value ) { sptr->host_notifications_enabled = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"service_notifications_enabled") ))
		{
			if ( wptr->value ) { sptr->service_notifications_enabled = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"can_submit_commands") ))
		{
			if ( wptr->value ) { sptr->can_submit_commands = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"retain_status_information") ))
		{
			if ( wptr->value ) { sptr->retain_status_information = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"retain_nonstatus_information") ))
		{
			if ( wptr->value ) { sptr->retain_nonstatus_information = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"status") ))
		{
			if ( wptr->value ) { sptr->status = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------------	*/
/*	r e s t _ o c c i _ n a g i o s _ c o n t a c t 	*/
/*	------------------------------------------------	*/
public int rest_occi_nagios_contact(FILE * fh,struct nagios_contact * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.alias='%s'\r\n",prefix,nptr,(sptr->alias?sptr->alias:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.host_notification_period='%s'\r\n",prefix,nptr,(sptr->host_notification_period?sptr->host_notification_period:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.service_notification_period='%s'\r\n",prefix,nptr,(sptr->service_notification_period?sptr->service_notification_period:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.host_notification_options='%s'\r\n",prefix,nptr,(sptr->host_notification_options?sptr->host_notification_options:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.service_notification_options='%s'\r\n",prefix,nptr,(sptr->service_notification_options?sptr->service_notification_options:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.email='%s'\r\n",prefix,nptr,(sptr->email?sptr->email:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.pager='%s'\r\n",prefix,nptr,(sptr->pager?sptr->pager:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.addressx='%s'\r\n",prefix,nptr,(sptr->addressx?sptr->addressx:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.host_notifications_enabled='%u'\r\n",prefix,nptr,sptr->host_notifications_enabled);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.service_notifications_enabled='%u'\r\n",prefix,nptr,sptr->service_notifications_enabled);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.can_submit_commands='%u'\r\n",prefix,nptr,sptr->can_submit_commands);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.retain_status_information='%u'\r\n",prefix,nptr,sptr->retain_status_information);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.retain_nonstatus_information='%u'\r\n",prefix,nptr,sptr->retain_nonstatus_information);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.status='%u'\r\n",prefix,nptr,sptr->status);
	return(0);

}

#endif	/* _nagioscontact_cnagioscontact_c_ */
