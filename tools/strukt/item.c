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

#ifndef _item_c_
#define _item_c_

#include "item.h"

/*	--------------------------	*/
/*	l i b e r a t e _ i t e m 	*/
/*	--------------------------	*/
public struct item * liberate_item(struct item * sptr)
{
	if ( sptr )
	{
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->basic )
			 sptr->basic = liberate(sptr->basic);
		if ( sptr->type )
			 sptr->type = liberate(sptr->type);
		sptr = liberate( sptr );
	}
	return((struct item *) 0);

}

/*	--------------------	*/
/*	r e s e t _ i t e m 	*/
/*	--------------------	*/
public struct item * reset_item(struct item * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct item*) 0;
		sptr->next = (struct item*) 0;
		sptr->name = (char*) 0;
		sptr->basic = (char*) 0;
		sptr->type = (char*) 0;
		sptr->dimension =  0;
		sptr->indirection =  0;
	}
	return(sptr);

}

/*	--------------------------	*/
/*	a l l o c a t e _ i t e m 	*/
/*	--------------------------	*/
public struct item * allocate_item()
{
	struct item * sptr;
	if (!( sptr = allocate( sizeof( struct item ) ) ))
		return( sptr );
	else	return( reset_item(sptr) );
}

#endif	/* _item_c_ */
