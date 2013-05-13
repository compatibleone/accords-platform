/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>     */
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License"); 	*/
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at 				*/
/*  									*/
/*  http://www.apache.org/licenses/LICENSE-2.0 				*/
/*  									*/
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 	*/
/* implied. 								*/
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License. 					*/
/* -------------------------------------------------------------------- */
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
