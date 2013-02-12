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
#ifndef _paas_c_
#define _paas_c_

#include "element.h"

#include "paas.h"

/*	--------------------------	*/
/*	l i b e r a t e _ p a a s 	*/
/*	--------------------------	*/
public struct paas * liberate_paas(struct paas * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->environment )
			 sptr->environment = liberate(sptr->environment);
		if ( sptr->envfile )
			 sptr->envfile = liberate(sptr->envfile);
		if ( sptr->application )
			 sptr->application = liberate(sptr->application);
		if ( sptr->appfile )
			 sptr->appfile = liberate(sptr->appfile);
		if ( sptr->profile )
			 sptr->profile = liberate(sptr->profile);
		if ( sptr->node )
			 sptr->node = liberate(sptr->node);
		if ( sptr->price )
			 sptr->price = liberate(sptr->price);
		if ( sptr->account )
			 sptr->account = liberate(sptr->account);
		if ( sptr->authorization )
			 sptr->authorization = liberate(sptr->authorization);
		if ( sptr->number )
			 sptr->number = liberate(sptr->number);
		if ( sptr->rootpass )
			 sptr->rootpass = liberate(sptr->rootpass);
		if ( sptr->reference )
			 sptr->reference = liberate(sptr->reference);
		if ( sptr->access )
			 sptr->access = liberate(sptr->access);
		if ( sptr->publicaddr )
			 sptr->publicaddr = liberate(sptr->publicaddr);
		if ( sptr->privateaddr )
			 sptr->privateaddr = liberate(sptr->privateaddr);
		if ( sptr->zone )
			 sptr->zone = liberate(sptr->zone);
		if ( sptr->hostname )
			 sptr->hostname = liberate(sptr->hostname);
		if ( sptr->workload )
			 sptr->workload = liberate(sptr->workload);
		if ( sptr->agent )
			 sptr->agent = liberate(sptr->agent);
		sptr = liberate( sptr );
	}
	return((struct paas *) 0);

}

/*	--------------------	*/
/*	r e s e t _ p a a s 	*/
/*	--------------------	*/
public struct paas * reset_paas(struct paas * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->environment = (char*) 0;
		sptr->envfile = (char*) 0;
		sptr->application = (char*) 0;
		sptr->appfile = (char*) 0;
		sptr->profile = (char*) 0;
		sptr->node = (char*) 0;
		sptr->price = (char*) 0;
		sptr->account = (char*) 0;
		sptr->authorization = (char*) 0;
		sptr->number = (char*) 0;
		sptr->rootpass = (char*) 0;
		sptr->reference = (char*) 0;
		sptr->access = (char*) 0;
		sptr->publicaddr = (char*) 0;
		sptr->privateaddr = (char*) 0;
		sptr->zone = (char*) 0;
		sptr->hostname = (char*) 0;
		sptr->workload = (char*) 0;
		sptr->agent = (char*) 0;
		sptr->when =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	--------------------------	*/
/*	a l l o c a t e _ p a a s 	*/
/*	--------------------------	*/
public struct paas * allocate_paas()
{
	struct paas * sptr;
	if (!( sptr = allocate( sizeof( struct paas ) ) ))
		return( sptr );
	else	return( reset_paas(sptr) );
}

/*	--------------------	*/
/*	x m l i n _ p a a s 	*/
/*	--------------------	*/
public int xmlin_paas(struct paas * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"environment") ))
		{
			if ( wptr->value ) { sptr->environment = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"envfile") ))
		{
			if ( wptr->value ) { sptr->envfile = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"application") ))
		{
			if ( wptr->value ) { sptr->application = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"appfile") ))
		{
			if ( wptr->value ) { sptr->appfile = allocate_string(wptr->value); }
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
		else if (!( strcmp(wptr->name,"authorization") ))
		{
			if ( wptr->value ) { sptr->authorization = allocate_string(wptr->value); }
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
		else if (!( strcmp(wptr->name,"access") ))
		{
			if ( wptr->value ) { sptr->access = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"publicaddr") ))
		{
			if ( wptr->value ) { sptr->publicaddr = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"privateaddr") ))
		{
			if ( wptr->value ) { sptr->privateaddr = allocate_string(wptr->value); }
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
		else if (!( strcmp(wptr->name,"agent") ))
		{
			if ( wptr->value ) { sptr->agent = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"when") ))
		{
			if ( wptr->value ) { sptr->when = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------	*/
/*	r e s t _ o c c i _ p a a s 	*/
/*	----------------------------	*/
public int rest_occi_paas(FILE * fh,struct paas * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.environment='%s'\r\n",prefix,nptr,(sptr->environment?sptr->environment:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.envfile='%s'\r\n",prefix,nptr,(sptr->envfile?sptr->envfile:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.application='%s'\r\n",prefix,nptr,(sptr->application?sptr->application:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.appfile='%s'\r\n",prefix,nptr,(sptr->appfile?sptr->appfile:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.profile='%s'\r\n",prefix,nptr,(sptr->profile?sptr->profile:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.node='%s'\r\n",prefix,nptr,(sptr->node?sptr->node:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.price='%s'\r\n",prefix,nptr,(sptr->price?sptr->price:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.account='%s'\r\n",prefix,nptr,(sptr->account?sptr->account:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.authorization='%s'\r\n",prefix,nptr,(sptr->authorization?sptr->authorization:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.number='%s'\r\n",prefix,nptr,(sptr->number?sptr->number:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.rootpass='%s'\r\n",prefix,nptr,(sptr->rootpass?sptr->rootpass:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.reference='%s'\r\n",prefix,nptr,(sptr->reference?sptr->reference:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.access='%s'\r\n",prefix,nptr,(sptr->access?sptr->access:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.publicaddr='%s'\r\n",prefix,nptr,(sptr->publicaddr?sptr->publicaddr:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.privateaddr='%s'\r\n",prefix,nptr,(sptr->privateaddr?sptr->privateaddr:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.zone='%s'\r\n",prefix,nptr,(sptr->zone?sptr->zone:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.hostname='%s'\r\n",prefix,nptr,(sptr->hostname?sptr->hostname:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.workload='%s'\r\n",prefix,nptr,(sptr->workload?sptr->workload:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.agent='%s'\r\n",prefix,nptr,(sptr->agent?sptr->agent:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.when='%u'\r\n",prefix,nptr,sptr->when);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _paas_cpaas_c_ */
