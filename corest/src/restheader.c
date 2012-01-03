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
#ifndef _restheader_c_
#define _restheader_c_

#include "restheader.h"

/*	----------------------------------------	*/
/*	l i b e r a t e _ r e s t _ h e a d e r 	*/
/*	----------------------------------------	*/
public struct rest_header * liberate_rest_header(struct rest_header * sptr)
{
	if ( sptr )
	{
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->value )
			 sptr->value = liberate(sptr->value);
		sptr = liberate( sptr );
	}
	return((struct rest_header *) 0);

}

/*	----------------------------------	*/
/*	r e s e t _ r e s t _ h e a d e r 	*/
/*	----------------------------------	*/
public struct rest_header * reset_rest_header(struct rest_header * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct rest_header*) 0;
		sptr->next = (struct rest_header*) 0;
		sptr->parent = (struct rest_request*) 0;
		sptr->name = (char*) 0;
		sptr->value = (char*) 0;
	}
	return(sptr);

}

/*	----------------------------------------	*/
/*	a l l o c a t e _ r e s t _ h e a d e r 	*/
/*	----------------------------------------	*/
public struct rest_header * allocate_rest_header()
{
	struct rest_header * sptr;
	if (!( sptr = allocate( sizeof( struct rest_header ) ) ))
		return( sptr );
	else	return( reset_rest_header(sptr) );
}

/*	------------------------------	*/
/*	a d d _ r e s t _ h e a d e r 	*/
/*	------------------------------	*/
public struct rest_header * add_rest_header(struct rest_request * pptr)
{
	struct rest_header * sptr;
	if (!( sptr = allocate( sizeof( struct rest_header ) ) ))
		return( sptr );
	else if (!( sptr = reset_rest_header(sptr) ))
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

/*	--------------------------------	*/
/*	d r o p _ r e s t _ h e a d e r 	*/
/*	--------------------------------	*/
public struct rest_header * drop_rest_header(struct rest_header * sptr)
{
	if ( sptr )
	{
		if (!( sptr->parent )) return(sptr);
		if (!( sptr->previous ))
		{
			if (!( sptr->parent->first = sptr->next ))
				sptr->parent->last = (struct rest_header *) 0;
			else	sptr->parent->first->previous = (struct rest_header *) 0;
		}
		else if (!( sptr->previous->next = sptr->next ))
			sptr->parent->last = sptr->previous;
		if (!( sptr->next ))
		{
			if (!( sptr->parent->last = sptr->previous ))
				sptr->parent->first = (struct rest_header *) 0;
			else	sptr->parent->last->next = (struct rest_header *) 0;
		}
		else if (!( sptr->next->previous = sptr->previous ))
			sptr->parent->first = sptr->next;
		sptr = liberate_rest_header(sptr);
	}
	return((struct rest_header *) 0);
}

#endif	/* _restheader_c_ */
