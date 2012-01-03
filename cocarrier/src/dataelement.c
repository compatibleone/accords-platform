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
#ifndef _dataelement_c_
#define _dataelement_c_

#include "dataelement.h"

/*	------------------------------------------	*/
/*	l i b e r a t e _ d a t a _ e l e m e n t 	*/
/*	------------------------------------------	*/
public struct data_element * liberate_data_element(struct data_element * sptr)
{
	struct data_element * mptr;
	if ( sptr )
	{
		while ((mptr = sptr->first) != (struct data_element *) 0)
		{
			sptr->first = mptr->next;
			mptr = liberate_data_element( mptr );
		}
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->value )
			 sptr->value = liberate(sptr->value);
		sptr = liberate( sptr );
	}
	return((struct data_element *) 0);

}

/*	------------------------------------	*/
/*	r e s e t _ d a t a _ e l e m e n t 	*/
/*	------------------------------------	*/
public struct data_element * reset_data_element(struct data_element * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct data_element*) 0;
		sptr->next = (struct data_element*) 0;
		sptr->parent = (struct data_element*) 0;
		sptr->first = (struct data_element*) 0;
		sptr->last = (struct data_element*) 0;
		sptr->name = (char*) 0;
		sptr->value = (char*) 0;
	}
	return(sptr);

}

/*	------------------------------------------	*/
/*	a l l o c a t e _ d a t a _ e l e m e n t 	*/
/*	------------------------------------------	*/
public struct data_element * allocate_data_element()
{
	struct data_element * sptr;
	if (!( sptr = allocate( sizeof( struct data_element ) ) ))
		return( sptr );
	else	return( reset_data_element(sptr) );
}

/*	--------------------------------	*/
/*	a d d _ d a t a _ e l e m e n t 	*/
/*	--------------------------------	*/
public struct data_element * add_data_element(struct data_element * pptr)
{
	struct data_element * sptr;
	if (!( sptr = allocate( sizeof( struct data_element ) ) ))
		return( sptr );
	else if (!( sptr = reset_data_element(sptr) ))
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

/*	----------------------------------	*/
/*	d r o p _ d a t a _ e l e m e n t 	*/
/*	----------------------------------	*/
public struct data_element * drop_data_element(struct data_element * sptr)
{
	if ( sptr )
	{
		if (!( sptr->parent )) return(sptr);
		if (!( sptr->previous ))
		{
			if (!( sptr->parent->first = sptr->next ))
				sptr->parent->last = (struct data_element *) 0;
			else	sptr->parent->first->previous = (struct data_element *) 0;
		}
		else if (!( sptr->previous->next = sptr->next ))
			sptr->parent->last = sptr->previous;
		if (!( sptr->next ))
		{
			if (!( sptr->parent->last = sptr->previous ))
				sptr->parent->first = (struct data_element *) 0;
			else	sptr->parent->last->next = (struct data_element *) 0;
		}
		else if (!( sptr->next->previous = sptr->previous ))
			sptr->parent->first = sptr->next;
		sptr = liberate_data_element(sptr);
	}
	return((struct data_element *) 0);
}

#endif	/* _dataelement_c_ */
