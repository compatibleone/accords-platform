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
#ifndef _price_c_
#define _price_c_

#include "element.h"

#include "price.h"

/*	----------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ p r i c e 	*/
/*	----------------------------------------	*/
public struct cords_price * liberate_cords_price(struct cords_price * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->operator )
			 sptr->operator = liberate(sptr->operator);
		if ( sptr->description )
			 sptr->description = liberate(sptr->description);
		if ( sptr->currency )
			 sptr->currency = liberate(sptr->currency);
		if ( sptr->fixed )
			 sptr->fixed = liberate(sptr->fixed);
		if ( sptr->rate )
			 sptr->rate = liberate(sptr->rate);
		if ( sptr->units )
			 sptr->units = liberate(sptr->units);
		if ( sptr->period )
			 sptr->period = liberate(sptr->period);
		sptr = liberate( sptr );
	}
	return((struct cords_price *) 0);

}

/*	----------------------------------	*/
/*	r e s e t _ c o r d s _ p r i c e 	*/
/*	----------------------------------	*/
public struct cords_price * reset_cords_price(struct cords_price * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->operator = (char*) 0;
		sptr->description = (char*) 0;
		sptr->currency = (char*) 0;
		sptr->fixed = (char*) 0;
		sptr->rate = (char*) 0;
		sptr->units = (char*) 0;
		sptr->period = (char*) 0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	----------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ p r i c e 	*/
/*	----------------------------------------	*/
public struct cords_price * allocate_cords_price()
{
	struct cords_price * sptr;
	if (!( sptr = allocate( sizeof( struct cords_price ) ) ))
		return( sptr );
	else	return( reset_cords_price(sptr) );
}

/*	----------------------------------	*/
/*	x m l i n _ c o r d s _ p r i c e 	*/
/*	----------------------------------	*/
public int xmlin_cords_price(struct cords_price * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"operator") ))
		{
			if ( wptr->value ) { sptr->operator = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"description") ))
		{
			if ( wptr->value ) { sptr->description = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"currency") ))
		{
			if ( wptr->value ) { sptr->currency = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"fixed") ))
		{
			if ( wptr->value ) { sptr->fixed = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"rate") ))
		{
			if ( wptr->value ) { sptr->rate = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"units") ))
		{
			if ( wptr->value ) { sptr->units = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"period") ))
		{
			if ( wptr->value ) { sptr->period = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ p r i c e 	*/
/*	------------------------------------------	*/
public int rest_occi_cords_price(FILE * fh,struct cords_price * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.operator='%s'\r\n",prefix,nptr,(sptr->operator?sptr->operator:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.description='%s'\r\n",prefix,nptr,(sptr->description?sptr->description:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.currency='%s'\r\n",prefix,nptr,(sptr->currency?sptr->currency:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.fixed='%s'\r\n",prefix,nptr,(sptr->fixed?sptr->fixed:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.rate='%s'\r\n",prefix,nptr,(sptr->rate?sptr->rate:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.units='%s'\r\n",prefix,nptr,(sptr->units?sptr->units:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.period='%s'\r\n",prefix,nptr,(sptr->period?sptr->period:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _price_c_ */
