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

#ifndef _security_c_
#define _security_c_

#include "element.h"

#include "security.h"

/*	----------------------------------------------	*/
/*	l i b e r a t e _ c o r d s _ s e c u r i t y 	*/
/*	----------------------------------------------	*/
public struct cords_security * liberate_cords_security(struct cords_security * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->level )
			 sptr->level = liberate(sptr->level);
		if ( sptr->scheme )
			 sptr->scheme = liberate(sptr->scheme);
		sptr = liberate( sptr );
	}
	return((struct cords_security *) 0);

}

/*	----------------------------------------	*/
/*	r e s e t _ c o r d s _ s e c u r i t y 	*/
/*	----------------------------------------	*/
public struct cords_security * reset_cords_security(struct cords_security * sptr)
{
	if ( sptr )
	{
		sptr->id = (char*) 0;
		sptr->name = (char*) 0;
		sptr->level = (char*) 0;
		sptr->scheme = (char*) 0;
		sptr->state =  0;
	}
	return(sptr);

}

/*	----------------------------------------------	*/
/*	a l l o c a t e _ c o r d s _ s e c u r i t y 	*/
/*	----------------------------------------------	*/
public struct cords_security * allocate_cords_security()
{
	struct cords_security * sptr;
	if (!( sptr = allocate( sizeof( struct cords_security ) ) ))
		return( sptr );
	else	return( reset_cords_security(sptr) );
}

/*	----------------------------------------	*/
/*	x m l i n _ c o r d s _ s e c u r i t y 	*/
/*	----------------------------------------	*/
public int xmlin_cords_security(struct cords_security * sptr,struct xml_element * eptr)
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
		else if (!( strcmp(wptr->name,"level") ))
		{
			if ( wptr->value ) { sptr->level = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"scheme") ))
		{
			if ( wptr->value ) { sptr->scheme = allocate_string(wptr->value); }
		}
		else if (!( strcmp(wptr->name,"state") ))
		{
			if ( wptr->value ) { sptr->state = atoi(wptr->value); }
		}
	}
	return(0);

}

/*	------------------------------------------------	*/
/*	r e s t _ o c c i _ c o r d s _ s e c u r i t y 	*/
/*	------------------------------------------------	*/
public int rest_occi_cords_security(FILE * fh,struct cords_security * sptr,char * prefix, char * nptr)
{
	struct xml_element * wptr;
	if (!( sptr )) return(0);
	fprintf(fh,"POST /%s/ HTTP/1.1\r\n",nptr);
	fprintf(fh,"Category: %s; scheme='http://scheme.%s.org/occi/%s#'; class='kind';\r\n",nptr,prefix,prefix);
	fprintf(fh,"X-OCCI-Attribute: %s.%s.id='%s'\r\n",prefix,nptr,(sptr->id?sptr->id:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.name='%s'\r\n",prefix,nptr,(sptr->name?sptr->name:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.level='%s'\r\n",prefix,nptr,(sptr->level?sptr->level:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.scheme='%s'\r\n",prefix,nptr,(sptr->scheme?sptr->scheme:""));
	fprintf(fh,"X-OCCI-Attribute: %s.%s.state='%u'\r\n",prefix,nptr,sptr->state);
	return(0);

}

#endif	/* _security_c_ */
