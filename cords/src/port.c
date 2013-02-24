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
#ifndef _port_c_
#define _port_c_

#include "element.h"

#include "port.h"

/*	--------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ p o r t 	*/
/*	--------------------------------------	*/
public struct cords_port * liberate_cords_port(struct cords_port * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->description )
			 sptr->description = liberate(sptr->description);
		if ( sptr->protocol )
			 sptr->protocol = liberate(sptr->protocol);
		if ( sptr->range )
			 sptr->range = liberate(sptr->range);
		if ( sptr->from )
			 sptr->from = liberate(sptr->from);
		if ( sptr->to )
			 sptr->to = liberate(sptr->to);
		if ( sptr->direction )
			 sptr->direction = liberate(sptr->direction);
		if ( sptr->target )
			 sptr->target = liberate(sptr->target);
		sptr = liberate( sptr );
	}
	return((struct cords_port *) 0);

}

/*	--------------------------------	*/
/*	r e s e t _ c o r d s _ p o r t 	*/
/*	--------------------------------	*/
public struct cords_port * reset_cords_port(struct cords_port * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->description = (char*) 0;
		sptr->protocol = (char*) 0;
		sptr->range = (char*) 0;
		sptr->from = (char*) 0;
		sptr->to = (char*) 0;
		sptr->direction = (char*) 0;
		sptr->target = (char*) 0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	--------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ p o r t 	*/
/*	--------------------------------------	*/
public struct cords_port * allocate_cords_port()
{
	struct cords_port * sptr;
	if (!( sptr = allocate( sizeof( struct cords_port ) ) ))
		return( sptr );
	else	return( reset_cords_port(sptr) );
}

/*	--------------------------------	*/
/*	x m l i n _ c o r d s _ p o r t 	*/
/*	--------------------------------	*/
public int xmlin_cords_port(struct cords_port * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"description") ))
		{
			if ( wptr->value ) { sptr->description = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"protocol") ))
		{
			if ( wptr->value ) { sptr->protocol = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"range") ))
		{
			if ( wptr->value ) { sptr->range = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"from") ))
		{
			if ( wptr->value ) { sptr->from = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"to") ))
		{
			if ( wptr->value ) { sptr->to = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"direction") ))
		{
			if ( wptr->value ) { sptr->direction = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"target") ))
		{
			if ( wptr->value ) { sptr->target = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ p o r t 	*/
/*	----------------------------------------	*/
public int rest_occi_cords_port(FILE * fh,struct cords_port * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.description='%s'\r\n",prefix,nptr,(sptr->description?sptr->description:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.protocol='%s'\r\n",prefix,nptr,(sptr->protocol?sptr->protocol:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.range='%s'\r\n",prefix,nptr,(sptr->range?sptr->range:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.from='%s'\r\n",prefix,nptr,(sptr->from?sptr->from:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.to='%s'\r\n",prefix,nptr,(sptr->to?sptr->to:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.direction='%s'\r\n",prefix,nptr,(sptr->direction?sptr->direction:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.target='%s'\r\n",prefix,nptr,(sptr->target?sptr->target:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _port_cport_c_ */
