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
#ifndef _occiatb_c_
#define _occiatb_c_

#include "occiatb.h"

/*	______________________________________________	*/
/*	l i b e r a t e _ o c c i _ a t t r i b u t e 	*/
/*	______________________________________________	*/
public struct occi_attribute * liberate_occi_attribute(struct occi_attribute * sptr)
{
	if ( sptr )
	{
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		sptr = liberate( sptr );
	}
	return((struct occi_attribute *) 0);

}

/*	________________________________________	*/
/*	r e s e t _ o c c i _ a t t r i b u t e 	*/
/*	________________________________________	*/
public struct occi_attribute * reset_occi_attribute(struct occi_attribute * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct occi_attribute*) 0;
		sptr->next = (struct occi_attribute*) 0;
		sptr->parent = (struct occi_category*) 0;
		sptr->name = (char*) 0;
		sptr->mandatory =  0;
		sptr->immutable =  0;
	}
	return(sptr);

}

/*	______________________________________________	*/
/*	a l l o c a t e _ o c c i _ a t t r i b u t e 	*/
/*	______________________________________________	*/
public struct occi_attribute * allocate_occi_attribute()
{
	struct occi_attribute * sptr;
	if (!( sptr = allocate( sizeof( struct occi_attribute ) ) ))
		return( sptr );
	else	return( reset_occi_attribute(sptr) );
}

/*	____________________________________	*/
/*	a d d _ o c c i _ a t t r i b u t e 	*/
/*	____________________________________	*/
public struct occi_attribute * add_occi_attribute(struct occi_category * pptr)
{
	struct occi_attribute * sptr;
	if (!( sptr = allocate( sizeof( struct occi_attribute ) ) ))
		return( sptr );
	else if (!( sptr = reset_occi_attribute(sptr) ))
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

/*	______________________________________	*/
/*	d r o p _ o c c i _ a t t r i b u t e 	*/
/*	______________________________________	*/
public struct occi_attribute * drop_occi_attribute(struct occi_attribute * sptr)
{
	if ( sptr )
	{
		if (!( sptr->parent )) return(sptr);
		if (!( sptr->previous ))
		{
			if (!( sptr->parent->first = sptr->next ))
				sptr->parent->last = (struct occi_attribute *) 0;
			else	sptr->parent->first->previous = (struct occi_attribute *) 0;
		}
		else if (!( sptr->previous->next = sptr->next ))
			sptr->parent->last = sptr->previous;
		if (!( sptr->next ))
		{
			if (!( sptr->parent->last = sptr->previous ))
				sptr->parent->first = (struct occi_attribute *) 0;
			else	sptr->parent->last->next = (struct occi_attribute *) 0;
		}
		else if (!( sptr->next->previous = sptr->previous ))
			sptr->parent->first = sptr->next;
		sptr = liberate_occi_attribute(sptr);
	}
	return((struct occi_attribute *) 0);
}

#endif	/* _occiatb_c_ */
