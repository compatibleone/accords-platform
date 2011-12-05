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
#ifndef _cse_c_
#define _cse_c_

#include "cse.h"

/*	------------------------------------------------------	*/
/*	l i b e r a t e _ c o r d s c r i p t _ e l e m e n t 	*/
/*	------------------------------------------------------	*/
public struct cordscript_element * liberate_cordscript_element(struct cordscript_element * sptr)
{
	if ( sptr )
	{
		if ( sptr->prefix )
			 sptr->prefix = liberate(sptr->prefix);
		if ( sptr->value )
			 sptr->value = liberate(sptr->value);
		sptr = liberate( sptr );
	}
	return((struct cordscript_element *) 0);

}

/*	------------------------------------------------	*/
/*	r e s e t _ c o r d s c r i p t _ e l e m e n t 	*/
/*	------------------------------------------------	*/
public struct cordscript_element * reset_cordscript_element(struct cordscript_element * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct cordscript_element*) 0;
		sptr->next = (struct cordscript_element*) 0;
		sptr->type =  0;
		sptr->prefix = (char*) 0;
		sptr->value = (char*) 0;
	}
	return(sptr);

}

/*	------------------------------------------------------	*/
/*	a l l o c a t e _ c o r d s c r i p t _ e l e m e n t 	*/
/*	------------------------------------------------------	*/
public struct cordscript_element * allocate_cordscript_element()
{
	struct cordscript_element * sptr;
	if (!( sptr = allocate( sizeof( struct cordscript_element ) ) ))
		return( sptr );
	else	return( reset_cordscript_element(sptr) );
}

#endif	/* _cse_c_ */
