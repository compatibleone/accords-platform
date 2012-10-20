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
#ifndef _nagioshostdependancy_c_
#define _nagioshostdependancy_c_

#include "element.h"

#include "nagioshostdependancy.h"

/*	--------------------------------------------------------------	*/
/*	l i b e r a t e _ n a g i o s _ h o s t _ d e p e n d e n c y 	*/
/*	--------------------------------------------------------------	*/
public struct nagios_host_dependency * liberate_nagios_host_dependency(struct nagios_host_dependency * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->dependent_host_name )
			 sptr->dependent_host_name = liberate(sptr->dependent_host_name);
		if ( sptr->dependent_hostgroup_name )
			 sptr->dependent_hostgroup_name = liberate(sptr->dependent_hostgroup_name);
		if ( sptr->host_name )
			 sptr->host_name = liberate(sptr->host_name);
		if ( sptr->hostgroup_name )
			 sptr->hostgroup_name = liberate(sptr->hostgroup_name);
		if ( sptr->execution_failure_criteria )
			 sptr->execution_failure_criteria = liberate(sptr->execution_failure_criteria);
		if ( sptr->notification_failure_criteria )
			 sptr->notification_failure_criteria = liberate(sptr->notification_failure_criteria);
		if ( sptr->dependency_period )
			 sptr->dependency_period = liberate(sptr->dependency_period);
		sptr = liberate( sptr );
	}
	return((struct nagios_host_dependency *) 0);

}

/*	--------------------------------------------------------	*/
/*	r e s e t _ n a g i o s _ h o s t _ d e p e n d e n c y 	*/
/*	--------------------------------------------------------	*/
public struct nagios_host_dependency * reset_nagios_host_dependency(struct nagios_host_dependency * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->dependent_host_name = (char*) 0;
		sptr->dependent_hostgroup_name = (char*) 0;
		sptr->host_name = (char*) 0;
		sptr->hostgroup_name = (char*) 0;
		sptr->inherits_parent =  0;
		sptr->execution_failure_criteria = (char*) 0;
		sptr->notification_failure_criteria = (char*) 0;
		sptr->dependency_period = (char*) 0;
		sptr->status =  0;
	}
	return(sptr);

}

/*	--------------------------------------------------------------	*/
/*	a l l o c a t e _ n a g i o s _ h o s t _ d e p e n d e n c y 	*/
/*	--------------------------------------------------------------	*/
public struct nagios_host_dependency * allocate_nagios_host_dependency()
{
	struct nagios_host_dependency * sptr;
	if (!( sptr = allocate( sizeof( struct nagios_host_dependency ) ) ))
		return( sptr );
	else	return( reset_nagios_host_dependency(sptr) );
}

/*	--------------------------------------------------------	*/
/*	x m l i n _ n a g i o s _ h o s t _ d e p e n d e n c y 	*/
/*	--------------------------------------------------------	*/
public int xmlin_nagios_host_dependency(struct nagios_host_dependency * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"dependent_host_name") ))
		{
			if ( wptr->value ) { sptr->dependent_host_name = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"dependent_hostgroup_name") ))
		{
			if ( wptr->value ) { sptr->dependent_hostgroup_name = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"host_name") ))
		{
			if ( wptr->value ) { sptr->host_name = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"hostgroup_name") ))
		{
			if ( wptr->value ) { sptr->hostgroup_name = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"inherits_parent") ))
		{
			if ( wptr->value ) { sptr->inherits_parent = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"execution_failure_criteria") ))
		{
			if ( wptr->value ) { sptr->execution_failure_criteria = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"notification_failure_criteria") ))
		{
			if ( wptr->value ) { sptr->notification_failure_criteria = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"dependency_period") ))
		{
			if ( wptr->value ) { sptr->dependency_period = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"status") ))
		{
			if ( wptr->value ) { sptr->status = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------------------------------	*/
/*	r e s t _ o c c i _ n a g i o s _ h o s t _ d e p e n d e n c y 	*/
/*	----------------------------------------------------------------	*/
public int rest_occi_nagios_host_dependency(FILE * fh,struct nagios_host_dependency * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.dependent_host_name='%s'\r\n",prefix,nptr,(sptr->dependent_host_name?sptr->dependent_host_name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.dependent_hostgroup_name='%s'\r\n",prefix,nptr,(sptr->dependent_hostgroup_name?sptr->dependent_hostgroup_name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.host_name='%s'\r\n",prefix,nptr,(sptr->host_name?sptr->host_name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.hostgroup_name='%s'\r\n",prefix,nptr,(sptr->hostgroup_name?sptr->hostgroup_name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.inherits_parent='%u'\r\n",prefix,nptr,sptr->inherits_parent);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.execution_failure_criteria='%s'\r\n",prefix,nptr,(sptr->execution_failure_criteria?sptr->execution_failure_criteria:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.notification_failure_criteria='%s'\r\n",prefix,nptr,(sptr->notification_failure_criteria?sptr->notification_failure_criteria:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.dependency_period='%s'\r\n",prefix,nptr,(sptr->dependency_period?sptr->dependency_period:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.status='%u'\r\n",prefix,nptr,sptr->status);
	return(0);

}

#endif	/* _nagioshostdependancy_cnagioshostdependancy_c_ */
