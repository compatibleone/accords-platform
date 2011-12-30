/* ---------------------------------------------------------------------------- */
/* Advanced Capabilities for Compatible One Resources Delivery System - ACCORDS	*/
/* (C) 2011 by Iain James Marshall <ijm667@hotmail.com>				*/
/* ---------------------------------------------------------------------------- */
/*										*/
/* This is free software; you can redistribute it and/or modify it		*/
/* under the terms of the GNU Lesser General Public License as			*/
/* published by the Free Software Foundation; either version 2.1 of		*/
/* the License, or (at your option) any later version.				*/
/*										*/
/* This software is distributed in the hope that it will be useful,		*/
/* but WITHOUT ANY WARRANTY; without even the implied warranty of		*/
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU		*/
/* Lesser General Public License for more details.				*/
/*										*/
/* You should have received a copy of the GNU Lesser General Public		*/
/* License along with this software; if not, write to the Free			*/
/* Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA		*/
/* 02110-1301 USA, or see the FSF site: http://www.fsf.org.			*/
/*										*/
/* ---------------------------------------------------------------------------- */

/* STRUKT WARNING : this file has been generated and should not be modified by hand */
#ifndef _optimise_c_
#define _optimise_c_

#include "element.h"

#include "optimise.h"

/*	----------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ o p t i m i s e 	*/
/*	----------------------------------------------	*/
public struct cords_optimise * liberate_cords_optimise(struct cords_optimise * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->category )
			 sptr->category = liberate(sptr->category);
		if ( sptr->selection )
			 sptr->selection = liberate(sptr->selection);
		sptr = liberate( sptr );
	}
	return((struct cords_optimise *) 0);

}

/*	----------------------------------------	*/
/*	r e s e t _ c o r d s _ o p t i m i s e 	*/
/*	----------------------------------------	*/
public struct cords_optimise * reset_cords_optimise(struct cords_optimise * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->category = (char*) 0;
		sptr->selection = (char*) 0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	----------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ o p t i m i s e 	*/
/*	----------------------------------------------	*/
public struct cords_optimise * allocate_cords_optimise()
{
	struct cords_optimise * sptr;
	if (!( sptr = allocate( sizeof( struct cords_optimise ) ) ))
		return( sptr );
	else	return( reset_cords_optimise(sptr) );
}

/*	----------------------------------------	*/
/*	x m l i n _ c o r d s _ o p t i m i s e 	*/
/*	----------------------------------------	*/
public int xmlin_cords_optimise(struct cords_optimise * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"category") ))
		{
			if ( wptr->value ) { sptr->category = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"selection") ))
		{
			if ( wptr->value ) { sptr->selection = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ o p t i m i s e 	*/
/*	------------------------------------------------	*/
public int rest_occi_cords_optimise(FILE * fh,struct cords_optimise * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.category='%s'\r\n",prefix,nptr,(sptr->category?sptr->category:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.selection='%s'\r\n",prefix,nptr,(sptr->selection?sptr->selection:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _optimise_c_ */
