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
#ifndef _infrastructure_c_
#define _infrastructure_c_

#include "element.h"

#include "infrastructure.h"

/*	----------------------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ i n f r a s t r u c t u r e 	*/
/*	----------------------------------------------------------	*/
public struct cords_infrastructure * liberate_cords_infrastructure(struct cords_infrastructure * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->compute )
			 sptr->compute = liberate(sptr->compute);
		if ( sptr->network )
			 sptr->network = liberate(sptr->network);
		if ( sptr->storage )
			 sptr->storage = liberate(sptr->storage);
		sptr = liberate( sptr );
	}
	return((struct cords_infrastructure *) 0);

}

/*	----------------------------------------------------	*/
/*	r e s e t _ c o r d s _ i n f r a s t r u c t u r e 	*/
/*	----------------------------------------------------	*/
public struct cords_infrastructure * reset_cords_infrastructure(struct cords_infrastructure * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->compute = (char*) 0;
		sptr->network = (char*) 0;
		sptr->storage = (char*) 0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	----------------------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ i n f r a s t r u c t u r e 	*/
/*	----------------------------------------------------------	*/
public struct cords_infrastructure * allocate_cords_infrastructure()
{
	struct cords_infrastructure * sptr;
	if (!( sptr = allocate( sizeof( struct cords_infrastructure ) ) ))
		return( sptr );
	else	return( reset_cords_infrastructure(sptr) );
}

/*	----------------------------------------------------	*/
/*	x m l i n _ c o r d s _ i n f r a s t r u c t u r e 	*/
/*	----------------------------------------------------	*/
public int xmlin_cords_infrastructure(struct cords_infrastructure * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"compute") ))
		{
			if ( wptr->value ) { sptr->compute = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"network") ))
		{
			if ( wptr->value ) { sptr->network = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"storage") ))
		{
			if ( wptr->value ) { sptr->storage = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ i n f r a s t r u c t u r e 	*/
/*	------------------------------------------------------------	*/
public int rest_occi_cords_infrastructure(FILE * fh,struct cords_infrastructure * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.compute='%s'\r\n",prefix,nptr,(sptr->compute?sptr->compute:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.network='%s'\r\n",prefix,nptr,(sptr->network?sptr->network:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.storage='%s'\r\n",prefix,nptr,(sptr->storage?sptr->storage:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _infrastructure_c_ */
