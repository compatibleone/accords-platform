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
#ifndef _profile_c_
#define _profile_c_

#include "element.h"

#include "profile.h"

/*	--------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ p r o f i l e 	*/
/*	--------------------------------------------	*/
public struct cords_profile * liberate_cords_profile(struct cords_profile * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->category )
			 sptr->category = liberate(sptr->category);
		if ( sptr->provider )
			 sptr->provider = liberate(sptr->provider);
		if ( sptr->tarification )
			 sptr->tarification = liberate(sptr->tarification);
		sptr = liberate( sptr );
	}
	return((struct cords_profile *) 0);

}

/*	--------------------------------------	*/
/*	r e s e t _ c o r d s _ p r o f i l e 	*/
/*	--------------------------------------	*/
public struct cords_profile * reset_cords_profile(struct cords_profile * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->category = (char*) 0;
		sptr->provider = (char*) 0;
		sptr->tarification = (char*) 0;
	}
	return(sptr);

}

/*	--------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ p r o f i l e 	*/
/*	--------------------------------------------	*/
public struct cords_profile * allocate_cords_profile()
{
	struct cords_profile * sptr;
	if (!( sptr = allocate( sizeof( struct cords_profile ) ) ))
		return( sptr );
	else	return( reset_cords_profile(sptr) );
}

/*	--------------------------------------	*/
/*	x m l i n _ c o r d s _ p r o f i l e 	*/
/*	--------------------------------------	*/
public int xmlin_cords_profile(struct cords_profile * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"provider") ))
		{
			if ( wptr->value ) { sptr->provider = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"tarification") ))
		{
			if ( wptr->value ) { sptr->tarification = allocate_string(wptr->value); }
		}
	}
	return(0);

}

/*	----------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ p r o f i l e 	*/
/*	----------------------------------------------	*/
public int rest_occi_cords_profile(FILE * fh,struct cords_profile * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.category='%s'\r\n",prefix,nptr,(sptr->category?sptr->category:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.provider='%s'\r\n",prefix,nptr,(sptr->provider?sptr->provider:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.tarification='%s'\r\n",prefix,nptr,(sptr->tarification?sptr->tarification:""));
	return(0);

}

#endif	/* _profile_c_ */
