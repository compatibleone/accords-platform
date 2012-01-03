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
#ifndef _occipub_c_
#define _occipub_c_

#include "occipub.h"

/*	--------------------------------------------------	*/
/*	l i b e r a t e _ o c c i _ p u b l i c a t i o n 	*/
/*	--------------------------------------------------	*/
public struct occi_publication * liberate_occi_publication(struct occi_publication * sptr)
{
	if ( sptr )
	{
		if ( sptr->category )
			 sptr->category = liberate(sptr->category);
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		sptr = liberate( sptr );
	}
	return((struct occi_publication *) 0);

}

/*	--------------------------------------------	*/
/*	r e s e t _ o c c i _ p u b l i c a t i o n 	*/
/*	--------------------------------------------	*/
public struct occi_publication * reset_occi_publication(struct occi_publication * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct occi_publication*) 0;
		sptr->next = (struct occi_publication*) 0;
		sptr->parent = (struct occi_publisher*) 0;
		sptr->category = (char*) 0;
		sptr->id = (char*) 0;
	}
	return(sptr);

}

/*	--------------------------------------------------	*/
/*	a l l o c a t e _ o c c i _ p u b l i c a t i o n 	*/
/*	--------------------------------------------------	*/
public struct occi_publication * allocate_occi_publication()
{
	struct occi_publication * sptr;
	if (!( sptr = allocate( sizeof( struct occi_publication ) ) ))
		return( sptr );
	else	return( reset_occi_publication(sptr) );
}

/*	----------------------------------------	*/
/*	a d d _ o c c i _ p u b l i c a t i o n 	*/
/*	----------------------------------------	*/
public struct occi_publication * add_occi_publication(struct occi_publisher * pptr)
{
	struct occi_publication * sptr;
	if (!( sptr = allocate( sizeof( struct occi_publication ) ) ))
		return( sptr );
	else if (!( sptr = reset_occi_publication(sptr) ))
		return( sptr );
	else
	{
		if (!( sptr->previous = pptr->last ))
			pptr->first = sptr;
		else	sptr->previous->next = sptr;
		pptr->last = sptr;
		sptr->parent = pptr;
		return( sptr );
	}

}

/*	------------------------------------------	*/
/*	d r o p _ o c c i _ p u b l i c a t i o n 	*/
/*	------------------------------------------	*/
public struct occi_publication * drop_occi_publication(struct occi_publication * sptr)
{
	if ( sptr )
	{
		if (!( sptr->parent )) return(sptr);
		if (!( sptr->previous ))
		{
			if (!( sptr->parent->first = sptr->next ))
				sptr->parent->last = (struct occi_publication *) 0;
			else	sptr->parent->first->previous = (struct occi_publication *) 0;
		}
		else if (!( sptr->previous->next = sptr->next ))
			sptr->parent->last = sptr->previous;
		if (!( sptr->next ))
		{
			if (!( sptr->parent->last = sptr->previous ))
				sptr->parent->first = (struct occi_publication *) 0;
			else	sptr->parent->last->next = (struct occi_publication *) 0;
		}
		else if (!( sptr->next->previous = sptr->previous ))
			sptr->parent->first = sptr->next;
		sptr = liberate_occi_publication(sptr);
	}
	return((struct occi_publication *) 0);
}

#endif	/* _occipub_c_ */
