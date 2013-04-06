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
#ifndef _onapp_c_
#define _onapp_c_

#include "element.h"

#include "onapp.h"

/*	----------------------------	*/
/*	l i b e r a t e _ o n a p p 	*/
/*	----------------------------	*/
public struct onapp * liberate_onapp(struct onapp * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->profile )
			 sptr->profile = liberate(sptr->profile);
		if ( sptr->description )
			 sptr->description = liberate(sptr->description);
		if ( sptr->node )
			 sptr->node = liberate(sptr->node);
		if ( sptr->access )
			 sptr->access = liberate(sptr->access);
		if ( sptr->agent )
			 sptr->agent = liberate(sptr->agent);
		if ( sptr->account )
			 sptr->account = liberate(sptr->account);
		if ( sptr->firewall )
			 sptr->firewall = liberate(sptr->firewall);
		if ( sptr->image )
			 sptr->image = liberate(sptr->image);
		if ( sptr->original )
			 sptr->original = liberate(sptr->original);
		if ( sptr->password )
			 sptr->password = liberate(sptr->password);
		if ( sptr->username )
			 sptr->username = liberate(sptr->username);
		if ( sptr->hostname )
			 sptr->hostname = liberate(sptr->hostname);
		if ( sptr->ipaddress )
			 sptr->ipaddress = liberate(sptr->ipaddress);
		if ( sptr->status )
			 sptr->status = liberate(sptr->status);
		if ( sptr->provider )
			 sptr->provider = liberate(sptr->provider);
		if ( sptr->region )
			 sptr->region = liberate(sptr->region);
		if ( sptr->zone )
			 sptr->zone = liberate(sptr->zone);
		if ( sptr->price )
			 sptr->price = liberate(sptr->price);
		if ( sptr->location )
			 sptr->location = liberate(sptr->location);
		sptr = liberate( sptr );
	}
	return((struct onapp *) 0);

}

/*	----------------------	*/
/*	r e s e t _ o n a p p 	*/
/*	----------------------	*/
public struct onapp * reset_onapp(struct onapp * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->profile = (char*) 0;
		sptr->description = (char*) 0;
		sptr->state =  0;
		sptr->node = (char*) 0;
		sptr->access = (char*) 0;
		sptr->agent = (char*) 0;
		sptr->account = (char*) 0;
		sptr->firewall = (char*) 0;
		sptr->image = (char*) 0;
		sptr->original = (char*) 0;
		sptr->password = (char*) 0;
		sptr->username = (char*) 0;
		sptr->hostname = (char*) 0;
		sptr->ipaddress = (char*) 0;
		sptr->status = (char*) 0;
		sptr->provider = (char*) 0;
		sptr->region = (char*) 0;
		sptr->zone = (char*) 0;
		sptr->price = (char*) 0;
		sptr->location = (char*) 0;
	}
	return(sptr);

}

/*	----------------------------	*/
/*	a l l o c a t e _ o n a p p 	*/
/*	----------------------------	*/
public struct onapp * allocate_onapp()
{
	struct onapp * sptr;
	if (!( sptr = allocate( sizeof( struct onapp ) ) ))
		return( sptr );
	else	return( reset_onapp(sptr) );
}

/*	----------------------	*/
/*	x m l i n _ o n a p p 	*/
/*	----------------------	*/
public int xmlin_onapp(struct onapp * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"profile") ))
		{
			if ( wptr->value ) { sptr->profile = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"description") ))
		{
			if ( wptr->value ) { sptr->description = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"node") ))
		{
			if ( wptr->value ) { sptr->node = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"access") ))
		{
			if ( wptr->value ) { sptr->access = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"agent") ))
		{
			if ( wptr->value ) { sptr->agent = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"account") ))
		{
			if ( wptr->value ) { sptr->account = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"firewall") ))
		{
			if ( wptr->value ) { sptr->firewall = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"image") ))
		{
			if ( wptr->value ) { sptr->image = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"original") ))
		{
			if ( wptr->value ) { sptr->original = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"password") ))
		{
			if ( wptr->value ) { sptr->password = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"username") ))
		{
			if ( wptr->value ) { sptr->username = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"hostname") ))
		{
			if ( wptr->value ) { sptr->hostname = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"ipaddress") ))
		{
			if ( wptr->value ) { sptr->ipaddress = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"status") ))
		{
			if ( wptr->value ) { sptr->status = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"provider") ))
		{
			if ( wptr->value ) { sptr->provider = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"region") ))
		{
			if ( wptr->value ) { sptr->region = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"zone") ))
		{
			if ( wptr->value ) { sptr->zone = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"price") ))
		{
			if ( wptr->value ) { sptr->price = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"location") ))
		{
			if ( wptr->value ) { sptr->location = allocate_string(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------	*/
/*	r e s t _ o c c i _ o n a p p 	*/
/*	------------------------------	*/
public int rest_occi_onapp(FILE * fh,struct onapp * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.profile='%s'\r\n",prefix,nptr,(sptr->profile?sptr->profile:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.description='%s'\r\n",prefix,nptr,(sptr->description?sptr->description:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.node='%s'\r\n",prefix,nptr,(sptr->node?sptr->node:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.access='%s'\r\n",prefix,nptr,(sptr->access?sptr->access:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.agent='%s'\r\n",prefix,nptr,(sptr->agent?sptr->agent:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.account='%s'\r\n",prefix,nptr,(sptr->account?sptr->account:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.firewall='%s'\r\n",prefix,nptr,(sptr->firewall?sptr->firewall:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.image='%s'\r\n",prefix,nptr,(sptr->image?sptr->image:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.original='%s'\r\n",prefix,nptr,(sptr->original?sptr->original:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.password='%s'\r\n",prefix,nptr,(sptr->password?sptr->password:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.username='%s'\r\n",prefix,nptr,(sptr->username?sptr->username:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.hostname='%s'\r\n",prefix,nptr,(sptr->hostname?sptr->hostname:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.ipaddress='%s'\r\n",prefix,nptr,(sptr->ipaddress?sptr->ipaddress:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.status='%s'\r\n",prefix,nptr,(sptr->status?sptr->status:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.provider='%s'\r\n",prefix,nptr,(sptr->provider?sptr->provider:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.region='%s'\r\n",prefix,nptr,(sptr->region?sptr->region:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.zone='%s'\r\n",prefix,nptr,(sptr->zone?sptr->zone:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.price='%s'\r\n",prefix,nptr,(sptr->price?sptr->price:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.location='%s'\r\n",prefix,nptr,(sptr->location?sptr->location:""));
	return(0);

}

#endif	/* _onapp_conapp_c_ */
