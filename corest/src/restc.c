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
#ifndef _restc_c_
#define _restc_c_

#include "restc.h"

/*	----------------------------------------	*/
/*	l i b e r a t e _ r e s t _ c l i e n t 	*/
/*	----------------------------------------	*/
public struct rest_client * liberate_rest_client(struct rest_client * sptr)
{
	if ( sptr )
	{
		if ( sptr->agent )
			 sptr->agent = liberate(sptr->agent);
		if ( sptr->buffer )
			 sptr->buffer = liberate(sptr->buffer);
		if ( sptr->user )
			 sptr->user = liberate(sptr->user);
		if ( sptr->pass )
			 sptr->pass = liberate(sptr->pass);
		if ( sptr->headers )
			 sptr->headers = liberate_rest_header(sptr->headers);
		sptr = liberate( sptr );
	}
	return((struct rest_client *) 0);

}

/*	----------------------------------	*/
/*	r e s e t _ r e s t _ c l i e n t 	*/
/*	----------------------------------	*/
public struct rest_client * reset_rest_client(struct rest_client * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct rest_client*) 0;
		sptr->next = (struct rest_client*) 0;
		sptr->parent = (struct rest_server*) 0;
		sptr->agent = (char*) 0;
		sptr->buffer = (char*) 0;
		sptr->buffersize =  0;
		sptr->bytes =  0;
		sptr->consumed =  0;
		sptr->user = (char*) 0;
		sptr->pass = (char*) 0;
		sptr->headers = (struct rest_header*) 0;
	}
	return(sptr);

}

/*	----------------------------------------	*/
/*	a l l o c a t e _ r e s t _ c l i e n t 	*/
/*	----------------------------------------	*/
public struct rest_client * allocate_rest_client()
{
	struct rest_client * sptr;
	if (!( sptr = allocate( sizeof( struct rest_client ) ) ))
		return( sptr );
	else	return( reset_rest_client(sptr) );
}

/*	------------------------------	*/
/*	a d d _ r e s t _ c l i e n t 	*/
/*	------------------------------	*/
public struct rest_client * add_rest_client(struct rest_server * pptr)
{
	struct rest_client * sptr;
	if (!( sptr = allocate( sizeof( struct rest_client ) ) ))
		return( sptr );
	else if (!( sptr = reset_rest_client(sptr) ))
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
/*	d r o p _ r e s t _ c l i e n t 	*/
/*	--------------------------------	*/
public struct rest_client * drop_rest_client(struct rest_client * sptr)
{
	if ( sptr )
	{
		if ( sptr->parent )
		{
			if (!( sptr->previous ))
			{
				if (!( sptr->parent->first = sptr->next ))
					sptr->parent->last = (struct rest_client *) 0;
				else	sptr->parent->first->previous = (struct rest_client *) 0;
			}
			else if (!( sptr->previous->next = sptr->next ))
				sptr->parent->last = sptr->previous;
			if (!( sptr->next ))
			{
				if (!( sptr->parent->last = sptr->previous ))
					sptr->parent->first = (struct rest_client *) 0;
				else	sptr->parent->last->next = (struct rest_client *) 0;
			}
			else if (!( sptr->next->previous = sptr->previous ))
				sptr->parent->first = sptr->next;
		}
		sptr = liberate_rest_client(sptr);
	}
	return((struct rest_client *) 0);
}

#endif	/* _restc_c_ */
