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
#ifndef _quota_c_
#define _quota_c_

#include "element.h"

#include "quota.h"

/*	----------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ q u o t a 	*/
/*	----------------------------------------	*/
public struct cords_quota * liberate_cords_quota(struct cords_quota * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->description )
			 sptr->description = liberate(sptr->description);
		if ( sptr->operator )
			 sptr->operator = liberate(sptr->operator);
		if ( sptr->price )
			 sptr->price = liberate(sptr->price);
		if ( sptr->zone )
			 sptr->zone = liberate(sptr->zone);
		if ( sptr->opinion )
			 sptr->opinion = liberate(sptr->opinion);
		sptr = liberate( sptr );
	}
	return((struct cords_quota *) 0);

}

/*	----------------------------------	*/
/*	r e s e t _ c o r d s _ q u o t a 	*/
/*	----------------------------------	*/
public struct cords_quota * reset_cords_quota(struct cords_quota * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->description = (char*) 0;
		sptr->operator = (char*) 0;
		sptr->price = (char*) 0;
		sptr->zone = (char*) 0;
		sptr->opinion = (char*) 0;
		sptr->ceiling =  0;
		sptr->offered =  0;
		sptr->reserved =  0;
		sptr->consumed =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	----------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ q u o t a 	*/
/*	----------------------------------------	*/
public struct cords_quota * allocate_cords_quota()
{
	struct cords_quota * sptr;
	if (!( sptr = allocate( sizeof( struct cords_quota ) ) ))
		return( sptr );
	else	return( reset_cords_quota(sptr) );
}

/*	----------------------------------	*/
/*	x m l i n _ c o r d s _ q u o t a 	*/
/*	----------------------------------	*/
public int xmlin_cords_quota(struct cords_quota * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"operator") ))
		{
			if ( wptr->value ) { sptr->operator = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"price") ))
		{
			if ( wptr->value ) { sptr->price = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"zone") ))
		{
			if ( wptr->value ) { sptr->zone = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"opinion") ))
		{
			if ( wptr->value ) { sptr->opinion = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"ceiling") ))
		{
			if ( wptr->value ) { sptr->ceiling = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"offered") ))
		{
			if ( wptr->value ) { sptr->offered = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"reserved") ))
		{
			if ( wptr->value ) { sptr->reserved = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"consumed") ))
		{
			if ( wptr->value ) { sptr->consumed = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ q u o t a 	*/
/*	------------------------------------------	*/
public int rest_occi_cords_quota(FILE * fh,struct cords_quota * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.description='%s'\r\n",prefix,nptr,(sptr->description?sptr->description:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.operator='%s'\r\n",prefix,nptr,(sptr->operator?sptr->operator:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.price='%s'\r\n",prefix,nptr,(sptr->price?sptr->price:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.zone='%s'\r\n",prefix,nptr,(sptr->zone?sptr->zone:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.opinion='%s'\r\n",prefix,nptr,(sptr->opinion?sptr->opinion:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.ceiling='%u'\r\n",prefix,nptr,sptr->ceiling);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.offered='%u'\r\n",prefix,nptr,sptr->offered);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.reserved='%u'\r\n",prefix,nptr,sptr->reserved);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.consumed='%u'\r\n",prefix,nptr,sptr->consumed);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _quota_c_ */
