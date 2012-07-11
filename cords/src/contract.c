/* ------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                   */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>    */
/* --------------------------------------------------------------------*/
/*  This is free software; you can redistribute it and/or modify it    */
/*  under the terms of the GNU Lesser General Public License as        */
/*  published by the Free Software Foundation; either version 2.1 of   */
/*  the License, or (at your option) any later version.                */
/*                                                                     */
/*  This software is distributed in the hope that it will be useful,   */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of     */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   */
/*  Lesser General Public License for more details.                    */
/*                                                                     */
/*  You should have received a copy of the GNU Lesser General Public   */
/*  License along with this software; if not, write to the Free        */
/*  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA */
/*  02110-1301 USA, or see the FSF site: http://www.fsf.org.           */
/* --------------------------------------------------------------------*/

/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _contract_c_
#define _contract_c_

#include "element.h"

#include "contract.h"

/*	----------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ c o n t r a c t 	*/
/*	----------------------------------------------	*/
public struct cords_contract * liberate_cords_contract(struct cords_contract * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->node )
			 sptr->node = liberate(sptr->node);
		if ( sptr->provider )
			 sptr->provider = liberate(sptr->provider);
		if ( sptr->profile )
			 sptr->profile = liberate(sptr->profile);
		if ( sptr->reference )
			 sptr->reference = liberate(sptr->reference);
		if ( sptr->hostname )
			 sptr->hostname = liberate(sptr->hostname);
		if ( sptr->rootpass )
			 sptr->rootpass = liberate(sptr->rootpass);
		if ( sptr->workload )
			 sptr->workload = liberate(sptr->workload);
		if ( sptr->price )
			 sptr->price = liberate(sptr->price);
		if ( sptr->access )
			 sptr->access = liberate(sptr->access);
		if ( sptr->common )
			 sptr->common = liberate(sptr->common);
		if ( sptr->scope )
			 sptr->scope = liberate(sptr->scope);
		if ( sptr->type )
			 sptr->type = liberate(sptr->type);
		if ( sptr->service )
			 sptr->service = liberate(sptr->service);
		if ( sptr->firewall )
			 sptr->firewall = liberate(sptr->firewall);
		sptr = liberate( sptr );
	}
	return((struct cords_contract *) 0);

}

/*	----------------------------------------	*/
/*	r e s e t _ c o r d s _ c o n t r a c t 	*/
/*	----------------------------------------	*/
public struct cords_contract * reset_cords_contract(struct cords_contract * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->node = (char*) 0;
		sptr->provider = (char*) 0;
		sptr->profile = (char*) 0;
		sptr->reference = (char*) 0;
		sptr->hostname = (char*) 0;
		sptr->rootpass = (char*) 0;
		sptr->workload = (char*) 0;
		sptr->price = (char*) 0;
		sptr->access = (char*) 0;
		sptr->common = (char*) 0;
		sptr->scope = (char*) 0;
		sptr->type = (char*) 0;
		sptr->service = (char*) 0;
		sptr->firewall = (char*) 0;
		sptr->instructions =  0;
		sptr->commons =  0;
		sptr->when =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	----------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ c o n t r a c t 	*/
/*	----------------------------------------------	*/
public struct cords_contract * allocate_cords_contract()
{
	struct cords_contract * sptr;
	if (!( sptr = allocate( sizeof( struct cords_contract ) ) ))
		return( sptr );
	else	return( reset_cords_contract(sptr) );
}

/*	----------------------------------------	*/
/*	x m l i n _ c o r d s _ c o n t r a c t 	*/
/*	----------------------------------------	*/
public int xmlin_cords_contract(struct cords_contract * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"node") ))
		{
			if ( wptr->value ) { sptr->node = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"provider") ))
		{
			if ( wptr->value ) { sptr->provider = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"profile") ))
		{
			if ( wptr->value ) { sptr->profile = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"reference") ))
		{
			if ( wptr->value ) { sptr->reference = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"hostname") ))
		{
			if ( wptr->value ) { sptr->hostname = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"rootpass") ))
		{
			if ( wptr->value ) { sptr->rootpass = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"workload") ))
		{
			if ( wptr->value ) { sptr->workload = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"price") ))
		{
			if ( wptr->value ) { sptr->price = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"access") ))
		{
			if ( wptr->value ) { sptr->access = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"common") ))
		{
			if ( wptr->value ) { sptr->common = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"scope") ))
		{
			if ( wptr->value ) { sptr->scope = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"type") ))
		{
			if ( wptr->value ) { sptr->type = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"service") ))
		{
			if ( wptr->value ) { sptr->service = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"firewall") ))
		{
			if ( wptr->value ) { sptr->firewall = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"instructions") ))
		{
			if ( wptr->value ) { sptr->instructions = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"commons") ))
		{
			if ( wptr->value ) { sptr->commons = atoi(wptr->value); }
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

/*	------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ c o n t r a c t 	*/
/*	------------------------------------------------	*/
public int rest_occi_cords_contract(FILE * fh,struct cords_contract * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.node='%s'\r\n",prefix,nptr,(sptr->node?sptr->node:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.provider='%s'\r\n",prefix,nptr,(sptr->provider?sptr->provider:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.profile='%s'\r\n",prefix,nptr,(sptr->profile?sptr->profile:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.reference='%s'\r\n",prefix,nptr,(sptr->reference?sptr->reference:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.hostname='%s'\r\n",prefix,nptr,(sptr->hostname?sptr->hostname:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.rootpass='%s'\r\n",prefix,nptr,(sptr->rootpass?sptr->rootpass:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.workload='%s'\r\n",prefix,nptr,(sptr->workload?sptr->workload:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.price='%s'\r\n",prefix,nptr,(sptr->price?sptr->price:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.access='%s'\r\n",prefix,nptr,(sptr->access?sptr->access:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.common='%s'\r\n",prefix,nptr,(sptr->common?sptr->common:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.scope='%s'\r\n",prefix,nptr,(sptr->scope?sptr->scope:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.type='%s'\r\n",prefix,nptr,(sptr->type?sptr->type:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.service='%s'\r\n",prefix,nptr,(sptr->service?sptr->service:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.firewall='%s'\r\n",prefix,nptr,(sptr->firewall?sptr->firewall:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.instructions='%u'\r\n",prefix,nptr,sptr->instructions);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.commons='%u'\r\n",prefix,nptr,sptr->commons);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.when='%u'\r\n",prefix,nptr,sptr->when);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _contract_c_ */
