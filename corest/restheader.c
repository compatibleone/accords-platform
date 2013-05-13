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
