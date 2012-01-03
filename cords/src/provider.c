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
#ifndef _provider_c_
#define _provider_c_

#include "element.h"

#include "provider.h"

/*	----------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ p r o v i d e r 	*/
/*	----------------------------------------------	*/
public struct cords_provider * liberate_cords_provider(struct cords_provider * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->category )
			 sptr->category = liberate(sptr->category);
		if ( sptr->profile )
			 sptr->profile = liberate(sptr->profile);
		if ( sptr->operator )
			 sptr->operator = liberate(sptr->operator);
		if ( sptr->price )
			 sptr->price = liberate(sptr->price);
		sptr = liberate( sptr );
	}
	return((struct cords_provider *) 0);

}

/*	----------------------------------------	*/
/*	r e s e t _ c o r d s _ p r o v i d e r 	*/
/*	----------------------------------------	*/
public struct cords_provider * reset_cords_provider(struct cords_provider * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->category = (char*) 0;
		sptr->profile = (char*) 0;
		sptr->operator = (char*) 0;
		sptr->price = (char*) 0;
	}
	return(sptr);

}

/*	----------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ p r o v i d e r 	*/
/*	----------------------------------------------	*/
public struct cords_provider * allocate_cords_provider()
{
	struct cords_provider * sptr;
	if (!( sptr = allocate( sizeof( struct cords_provider ) ) ))
		return( sptr );
	else	return( reset_cords_provider(sptr) );
}

/*	----------------------------------------	*/
/*	x m l i n _ c o r d s _ p r o v i d e r 	*/
/*	----------------------------------------	*/
public int xmlin_cords_provider(struct cords_provider * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"category") ))
		{
			if ( wptr->value ) { sptr->category = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"profile") ))
		{
			if ( wptr->value ) { sptr->profile = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"operator") ))
		{
			if ( wptr->value ) { sptr->operator = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"price") ))
		{
			if ( wptr->value ) { sptr->price = allocate_string(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ p r o v i d e r 	*/
/*	------------------------------------------------	*/
public int rest_occi_cords_provider(FILE * fh,struct cords_provider * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.category='%s'\r\n",prefix,nptr,(sptr->category?sptr->category:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.profile='%s'\r\n",prefix,nptr,(sptr->profile?sptr->profile:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.operator='%s'\r\n",prefix,nptr,(sptr->operator?sptr->operator:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.price='%s'\r\n",prefix,nptr,(sptr->price?sptr->price:""));
	return(0);

}

#endif	/* _provider_c_ */
