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
#ifndef _restworker_c_
#define _restworker_c_

#include "restworker.h"

/*	________________________________________	*/
/*	l i b e r a t e _ r e s t _ w o r k e r 	*/
/*	________________________________________	*/
public struct rest_worker * liberate_rest_worker(struct rest_worker * sptr)
{
	if ( sptr )
	{
		sptr = liberate( sptr );
	}
	return((struct rest_worker *) 0);

}

/*	__________________________________	*/
/*	r e s e t _ r e s t _ w o r k e r 	*/
/*	__________________________________	*/
public struct rest_worker * reset_rest_worker(struct rest_worker * sptr)
{
	if ( sptr )
	{
		sptr->parent = (struct rest_server*) 0;
		sptr->previous = (struct rest_worker*) 0;
		sptr->next = (struct rest_worker*) 0;
		sptr->pid =  0;
		sptr->status =  0;
		sptr->started =  0;
	}
	return(sptr);

}

/*	________________________________________	*/
/*	a l l o c a t e _ r e s t _ w o r k e r 	*/
/*	________________________________________	*/
public struct rest_worker * allocate_rest_worker()
{
	struct rest_worker * sptr;
	if (!( sptr = allocate( sizeof( struct rest_worker ) ) ))
		return( sptr );
	else	return( reset_rest_worker(sptr) );
}

/*	______________________________	*/
/*	a d d _ r e s t _ w o r k e r 	*/
/*	______________________________	*/
public struct rest_worker * add_rest_worker(struct rest_server * pptr)
{
	struct rest_worker * sptr;
	if (!( sptr = allocate( sizeof( struct rest_worker ) ) ))
		return( sptr );
	else if (!( sptr = reset_rest_worker(sptr) ))
		return( sptr );
	else
	{
		if (!( sptr->previous = pptr->lastw ))
			pptr->firstw = sptr;
		else	sptr->previous->next = sptr;
		pptr->lastw = sptr;
		sptr->parent = pptr;
		return( sptr );
	}

}

/*	________________________________	*/
/*	d r o p _ r e s t _ w o r k e r 	*/
/*	________________________________	*/
public struct rest_worker * drop_rest_worker(struct rest_worker * sptr)
{
	if ( sptr )
	{
		if (!( sptr->parent )) return(sptr);
		if (!( sptr->previous ))
		{
			if (!( sptr->parent->firstw = sptr->next ))
				sptr->parent->lastw = (struct rest_worker *) 0;
			else	sptr->parent->firstw->previous = (struct rest_worker *) 0;
		}
		else if (!( sptr->previous->next = sptr->next ))
			sptr->parent->lastw = sptr->previous;
		if (!( sptr->next ))
		{
			if (!( sptr->parent->lastw = sptr->previous ))
				sptr->parent->firstw = (struct rest_worker *) 0;
			else	sptr->parent->lastw->next = (struct rest_worker *) 0;
		}
		else if (!( sptr->next->previous = sptr->previous ))
			sptr->parent->firstw = sptr->next;
		sptr = liberate_rest_worker(sptr);
	}
	return((struct rest_worker *) 0);
}

#endif	/* _restworker_c_ */
