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

/* pyaccordsSDK: this code has been gnerated by pyaccordsSDK            */

#ifndef _amazonEc2_c_
#define _amazonEc2_c_
#include "element.h"
#include "amazonEc2.h"

/*---------------------------------------------------------------------------------------*/
/*                               Liberate_amazonEc2                                */
/*---------------------------------------------------------------------------------------*/
public struct amazonEc2 * liberate_amazonEc2(struct amazonEc2 * sptr)
{
	if( sptr )
	{
		if(sptr->id)
			sptr->id = liberate(sptr->id);
		if(sptr->name)
			sptr->name = liberate(sptr->name);
		if(sptr->flavor)
			sptr->flavor = liberate(sptr->flavor);
		if(sptr->image)
			sptr->image = liberate(sptr->image);
		if(sptr->original)
			sptr->original = liberate(sptr->original);
		if(sptr->profile)
			sptr->profile = liberate(sptr->profile);
		if(sptr->node)
			sptr->node = liberate(sptr->node);
		if(sptr->price)
			sptr->price = liberate(sptr->price);
		if(sptr->account)
			sptr->account = liberate(sptr->account);
		if(sptr->number)
			sptr->number = liberate(sptr->number);
		if(sptr->rootpass)
			sptr->rootpass = liberate(sptr->rootpass);
		if(sptr->reference)
			sptr->reference = liberate(sptr->reference);
		if(sptr->network)
			sptr->network = liberate(sptr->network);
		if(sptr->access)
			sptr->access = liberate(sptr->access);
		if(sptr->accessip)
			sptr->accessip = liberate(sptr->accessip);
		if(sptr->floating)
			sptr->floating = liberate(sptr->floating);
		if(sptr->floatingid)
			sptr->floatingid = liberate(sptr->floatingid);
		if(sptr->publicaddr)
			sptr->publicaddr = liberate(sptr->publicaddr);
		if(sptr->privateaddr)
			sptr->privateaddr = liberate(sptr->privateaddr);
		if(sptr->firewall)
			sptr->firewall = liberate(sptr->firewall);
		if(sptr->group)
			sptr->group = liberate(sptr->group);
		if(sptr->zone)
			sptr->zone = liberate(sptr->zone);
		if(sptr->hostname)
			sptr->hostname = liberate(sptr->hostname);
		if(sptr->workload)
			sptr->workload = liberate(sptr->workload);
		if(sptr->when)
			sptr->when = liberate(sptr->when);
		if(sptr->state)
			sptr->state = liberate(sptr->state);
		sptr = liberate(sptr);
	}
	return((struct amazonEc2 * )0);
}

/*---------------------------------------------------------------------------------------*/
/*                            Reset_amazonEc2                                      */
/*---------------------------------------------------------------------------------------*/
public struct amazonEc2 * reset_amazonEc2(struct amazonEc2 * sptr)
{
	if( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->flavor = (char*) 0;
		sptr->image = (char*) 0;
		sptr->original = (char*) 0;
		sptr->profile = (char*) 0;
		sptr->node = (char*) 0;
		sptr->price = (char*) 0;
		sptr->account = (char*) 0;
		sptr->number = (char*) 0;
		sptr->rootpass = (char*) 0;
		sptr->reference = (char*) 0;
		sptr->network = (char*) 0;
		sptr->access = (char*) 0;
		sptr->accessip = (char*) 0;
		sptr->floating = (char*) 0;
		sptr->floatingid = (char*) 0;
		sptr->publicaddr = (char*) 0;
		sptr->privateaddr = (char*) 0;
		sptr->firewall = (char*) 0;
		sptr->group = (char*) 0;
		sptr->zone = (char*) 0;
		sptr->hostname = (char*) 0;
		sptr->workload = (char*) 0;
		sptr->when = (char*) 0;
		sptr->state = (char*) 0;
	}
	return (sptr);
}

/*---------------------------------------------------------------------------------------*/
/*                            Allocate_amazonEc2                                   */
/*---------------------------------------------------------------------------------------*/
public struct amazonEc2 * allocate_amazonEc2()
{
	struct amazonEc2 * sptr;
	if (!( sptr = allocate( sizeof( struct amazonEc2 ) ) ))
		return( sptr );
	else  return( reset_amazonEc2(sptr) );
}

/*---------------------------------------------------------------------------------------*/
/*                             Xmlin_amazonEc2                                     */
/*---------------------------------------------------------------------------------------*/
public int xmlin_amazonEc2(struct amazonEc2 * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"flavor") ))
		{
			if ( wptr->value ) { sptr->flavor = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"image") ))
		{
			if ( wptr->value ) { sptr->image = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"original") ))
		{
			if ( wptr->value ) { sptr->original = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"profile") ))
		{
			if ( wptr->value ) { sptr->profile = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"node") ))
		{
			if ( wptr->value ) { sptr->node = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"price") ))
		{
			if ( wptr->value ) { sptr->price = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"account") ))
		{
			if ( wptr->value ) { sptr->account = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"number") ))
		{
			if ( wptr->value ) { sptr->number = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"rootpass") ))
		{
			if ( wptr->value ) { sptr->rootpass = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"reference") ))
		{
			if ( wptr->value ) { sptr->reference = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"network") ))
		{
			if ( wptr->value ) { sptr->network = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"access") ))
		{
			if ( wptr->value ) { sptr->access = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"accessip") ))
		{
			if ( wptr->value ) { sptr->accessip = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"floating") ))
		{
			if ( wptr->value ) { sptr->floating = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"floatingid") ))
		{
			if ( wptr->value ) { sptr->floatingid = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"publicaddr") ))
		{
			if ( wptr->value ) { sptr->publicaddr = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"privateaddr") ))
		{
			if ( wptr->value ) { sptr->privateaddr = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"firewall") ))
		{
			if ( wptr->value ) { sptr->firewall = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"group") ))
		{
			if ( wptr->value ) { sptr->group = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"zone") ))
		{
			if ( wptr->value ) { sptr->zone = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"hostname") ))
		{
			if ( wptr->value ) { sptr->hostname = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"workload") ))
		{
			if ( wptr->value ) { sptr->workload = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"when") ))
		{
			if ( wptr->value ) { sptr->when = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = allocate_string(wptr->value); }
		}
	}
	return(0);
}

/*---------------------------------------------------------------------------------------*/
/*                         Rest_occi_amazonEc2                                     */
/*---------------------------------------------------------------------------------------*/
public int rest_occi_amazonEc2(FILE * fh,struct amazonEc2 * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.flavor='%s'\r\n",prefix,nptr,(sptr->flavor?sptr->flavor:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.image='%s'\r\n",prefix,nptr,(sptr->image?sptr->image:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.original='%s'\r\n",prefix,nptr,(sptr->original?sptr->original:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.profile='%s'\r\n",prefix,nptr,(sptr->profile?sptr->profile:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.node='%s'\r\n",prefix,nptr,(sptr->node?sptr->node:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.price='%s'\r\n",prefix,nptr,(sptr->price?sptr->price:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.account='%s'\r\n",prefix,nptr,(sptr->account?sptr->account:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.number='%s'\r\n",prefix,nptr,(sptr->number?sptr->number:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.rootpass='%s'\r\n",prefix,nptr,(sptr->rootpass?sptr->rootpass:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.reference='%s'\r\n",prefix,nptr,(sptr->reference?sptr->reference:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.network='%s'\r\n",prefix,nptr,(sptr->network?sptr->network:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.access='%s'\r\n",prefix,nptr,(sptr->access?sptr->access:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.accessip='%s'\r\n",prefix,nptr,(sptr->accessip?sptr->accessip:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.floating='%s'\r\n",prefix,nptr,(sptr->floating?sptr->floating:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.floatingid='%s'\r\n",prefix,nptr,(sptr->floatingid?sptr->floatingid:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.publicaddr='%s'\r\n",prefix,nptr,(sptr->publicaddr?sptr->publicaddr:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.privateaddr='%s'\r\n",prefix,nptr,(sptr->privateaddr?sptr->privateaddr:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.firewall='%s'\r\n",prefix,nptr,(sptr->firewall?sptr->firewall:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.group='%s'\r\n",prefix,nptr,(sptr->group?sptr->group:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.zone='%s'\r\n",prefix,nptr,(sptr->zone?sptr->zone:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.hostname='%s'\r\n",prefix,nptr,(sptr->hostname?sptr->hostname:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.workload='%s'\r\n",prefix,nptr,(sptr->workload?sptr->workload:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.when='%s'\r\n",prefix,nptr,(sptr->when?sptr->when:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%s'\r\n",prefix,nptr,(sptr->state?sptr->state:""));
	return(0);
}

#endif
