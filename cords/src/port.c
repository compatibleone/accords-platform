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
		if ( sptr->direction )
			 sptr->direction = liberate(sptr->direction);
		if ( sptr->address )
			 sptr->address = liberate(sptr->address);
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
		sptr->direction = (char*) 0;
		sptr->address = (char*) 0;
		sptr->number =  0;
		sptr->target =  0;
		sptr->status =  0;
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
		else if (!( strcmp(wptr->name,"direction") ))
		{
			if ( wptr->value ) { sptr->direction = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"address") ))
		{
			if ( wptr->value ) { sptr->address = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"number") ))
		{
			if ( wptr->value ) { sptr->number = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"target") ))
		{
			if ( wptr->value ) { sptr->target = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"status") ))
		{
			if ( wptr->value ) { sptr->status = atoi(wptr->value); }
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
	fprintf(fh,"X-OCCI-Attribute: %s.%s.direction='%s'\r\n",prefix,nptr,(sptr->direction?sptr->direction:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.address='%s'\r\n",prefix,nptr,(sptr->address?sptr->address:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.number='%u'\r\n",prefix,nptr,sptr->number);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.target='%u'\r\n",prefix,nptr,sptr->target);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.status='%u'\r\n",prefix,nptr,sptr->status);
	return(0);

}

#endif	/* _port_c_ */
