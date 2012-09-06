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
#ifndef _guarantee_c_
#define _guarantee_c_

#include "element.h"

#include "guarantee.h"

/*	------------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ g u a r a n t e e 	*/
/*	------------------------------------------------	*/
public struct cords_guarantee * liberate_cords_guarantee(struct cords_guarantee * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->description )
			 sptr->description = liberate(sptr->description);
		if ( sptr->obligated )
			 sptr->obligated = liberate(sptr->obligated);
		if ( sptr->importance )
			 sptr->importance = liberate(sptr->importance);
		if ( sptr->scope )
			 sptr->scope = liberate(sptr->scope);
		if ( sptr->variable )
			 sptr->variable = liberate(sptr->variable);
		sptr = liberate( sptr );
	}
	return((struct cords_guarantee *) 0);

}

/*	------------------------------------------	*/
/*	r e s e t _ c o r d s _ g u a r a n t e e 	*/
/*	------------------------------------------	*/
public struct cords_guarantee * reset_cords_guarantee(struct cords_guarantee * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->description = (char*) 0;
		sptr->obligated = (char*) 0;
		sptr->importance = (char*) 0;
		sptr->scope = (char*) 0;
		sptr->variable = (char*) 0;
		sptr->values =  0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	------------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ g u a r a n t e e 	*/
/*	------------------------------------------------	*/
public struct cords_guarantee * allocate_cords_guarantee()
{
	struct cords_guarantee * sptr;
	if (!( sptr = allocate( sizeof( struct cords_guarantee ) ) ))
		return( sptr );
	else	return( reset_cords_guarantee(sptr) );
}

/*	------------------------------------------	*/
/*	x m l i n _ c o r d s _ g u a r a n t e e 	*/
/*	------------------------------------------	*/
public int xmlin_cords_guarantee(struct cords_guarantee * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"obligated") ))
		{
			if ( wptr->value ) { sptr->obligated = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"importance") ))
		{
			if ( wptr->value ) { sptr->importance = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"scope") ))
		{
			if ( wptr->value ) { sptr->scope = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"variable") ))
		{
			if ( wptr->value ) { sptr->variable = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"values") ))
		{
			if ( wptr->value ) { sptr->values = atoi(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	--------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ g u a r a n t e e 	*/
/*	--------------------------------------------------	*/
public int rest_occi_cords_guarantee(FILE * fh,struct cords_guarantee * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.description='%s'\r\n",prefix,nptr,(sptr->description?sptr->description:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.obligated='%s'\r\n",prefix,nptr,(sptr->obligated?sptr->obligated:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.importance='%s'\r\n",prefix,nptr,(sptr->importance?sptr->importance:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.scope='%s'\r\n",prefix,nptr,(sptr->scope?sptr->scope:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.variable='%s'\r\n",prefix,nptr,(sptr->variable?sptr->variable:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.values='%u'\r\n",prefix,nptr,sptr->values);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _guarantee_c_ */
