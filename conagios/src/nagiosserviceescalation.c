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
#ifndef _nagiosserviceescalation_c_
#define _nagiosserviceescalation_c_

#include "element.h"

#include "nagiosserviceescalation.h"

/*	--------------------------------------------------------------------	*/
/*	l i b e r a t e _ n a g i o s _ s e r v i c e _ e s c a l a t i o n 	*/
/*	--------------------------------------------------------------------	*/
public struct nagios_service_escalation * liberate_nagios_service_escalation(struct nagios_service_escalation * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->host_name )
			 sptr->host_name = liberate(sptr->host_name);
		if ( sptr->hostgroup_name )
			 sptr->hostgroup_name = liberate(sptr->hostgroup_name);
		if ( sptr->service_description )
			 sptr->service_description = liberate(sptr->service_description);
		if ( sptr->escalation_period )
			 sptr->escalation_period = liberate(sptr->escalation_period);
		if ( sptr->escalation_options )
			 sptr->escalation_options = liberate(sptr->escalation_options);
		sptr = liberate( sptr );
	}
	return((struct nagios_service_escalation *) 0);

}

/*	--------------------------------------------------------------	*/
/*	r e s e t _ n a g i o s _ s e r v i c e _ e s c a l a t i o n 	*/
/*	--------------------------------------------------------------	*/
public struct nagios_service_escalation * reset_nagios_service_escalation(struct nagios_service_escalation * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->host_name = (char*) 0;
		sptr->hostgroup_name = (char*) 0;
		sptr->service_description = (char*) 0;
		sptr->first_notification =  0;
		sptr->last_notification =  0;
		sptr->notification_interval =  0;
		sptr->escalation_period = (char*) 0;
		sptr->escalation_options = (char*) 0;
		sptr->contacts =  0;
		sptr->status =  0;
	}
	return(sptr);

}

/*	--------------------------------------------------------------------	*/
/*	a l l o c a t e _ n a g i o s _ s e r v i c e _ e s c a l a t i o n 	*/
/*	--------------------------------------------------------------------	*/
public struct nagios_service_escalation * allocate_nagios_service_escalation()
{
	struct nagios_service_escalation * sptr;
	if (!( sptr = allocate( sizeof( struct nagios_service_escalation ) ) ))
		return( sptr );
	else	return( reset_nagios_service_escalation(sptr) );
}

/*	--------------------------------------------------------------	*/
/*	x m l i n _ n a g i o s _ s e r v i c e _ e s c a l a t i o n 	*/
/*	--------------------------------------------------------------	*/
public int xmlin_nagios_service_escalation(struct nagios_service_escalation * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"host_name") ))
		{
			if ( wptr->value ) { sptr->host_name = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"hostgroup_name") ))
		{
			if ( wptr->value ) { sptr->hostgroup_name = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"service_description") ))
		{
			if ( wptr->value ) { sptr->service_description = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"first_notification") ))
		{
			if ( wptr->value ) { sptr->first_notification = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"last_notification") ))
		{
			if ( wptr->value ) { sptr->last_notification = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"notification_interval") ))
		{
			if ( wptr->value ) { sptr->notification_interval = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"escalation_period") ))
		{
			if ( wptr->value ) { sptr->escalation_period = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"escalation_options") ))
		{
			if ( wptr->value ) { sptr->escalation_options = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"contacts") ))
		{
			if ( wptr->value ) { sptr->contacts = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"status") ))
		{
			if ( wptr->value ) { sptr->status = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------------------------------------	*/
/*	r e s t _ o c c i _ n a g i o s _ s e r v i c e _ e s c a l a t i o n 	*/
/*	----------------------------------------------------------------------	*/
public int rest_occi_nagios_service_escalation(FILE * fh,struct nagios_service_escalation * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.host_name='%s'\r\n",prefix,nptr,(sptr->host_name?sptr->host_name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.hostgroup_name='%s'\r\n",prefix,nptr,(sptr->hostgroup_name?sptr->hostgroup_name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.service_description='%s'\r\n",prefix,nptr,(sptr->service_description?sptr->service_description:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.first_notification='%u'\r\n",prefix,nptr,sptr->first_notification);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.last_notification='%u'\r\n",prefix,nptr,sptr->last_notification);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.notification_interval='%u'\r\n",prefix,nptr,sptr->notification_interval);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.escalation_period='%s'\r\n",prefix,nptr,(sptr->escalation_period?sptr->escalation_period:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.escalation_options='%s'\r\n",prefix,nptr,(sptr->escalation_options?sptr->escalation_options:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.contacts='%u'\r\n",prefix,nptr,sptr->contacts);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.status='%u'\r\n",prefix,nptr,sptr->status);
	return(0);

}

#endif	/* _nagiosserviceescalation_cnagiosserviceescalation_c_ */
