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
#ifndef _rests_c_
#define _rests_c_

#include "rests.h"

/*	----------------------------------------	*/
/*	l i b e r a t e _ r e s t _ s e r v e r 	*/
/*	----------------------------------------	*/
public struct rest_server * liberate_rest_server(struct rest_server * sptr)
{
	struct rest_client * mptr;
	if ( sptr )
	{
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->user )
			 sptr->user = liberate(sptr->user);
		if ( sptr->password )
			 sptr->password = liberate(sptr->password);
		if ( sptr->headers )
			 sptr->headers = liberate_rest_header(sptr->headers);
		while ((mptr = sptr->first) != (struct rest_client *) 0)
		{
			sptr->first = mptr->next;
			mptr = liberate_rest_client( mptr );
		}
		if ( sptr->active )
			 sptr->active = liberate_rest_client(sptr->active);
		sptr = liberate( sptr );
	}
	return((struct rest_server *) 0);

}

/*	----------------------------------	*/
/*	r e s e t _ r e s t _ s e r v e r 	*/
/*	----------------------------------	*/
public struct rest_server * reset_rest_server(struct rest_server * sptr)
{
	if ( sptr )
	{
		sptr->port =  0;
		sptr->name = (char*) 0;
		sptr->user = (char*) 0;
		sptr->password = (char*) 0;
		sptr->seconds =  0;
		sptr->wakeup =  0;
		sptr->fraction =  0;
		sptr->workers =  0;
		sptr->threads =  0;
		sptr->headers = (struct rest_header*) 0;
		sptr->first = (struct rest_client*) 0;
		sptr->last = (struct rest_client*) 0;
		sptr->active = (struct rest_client*) 0;
	}
	return(sptr);

}

/*	----------------------------------------	*/
/*	a l l o c a t e _ r e s t _ s e r v e r 	*/
/*	----------------------------------------	*/
public struct rest_server * allocate_rest_server()
{
	struct rest_server * sptr;
	if (!( sptr = allocate( sizeof( struct rest_server ) ) ))
		return( sptr );
	else	return( reset_rest_server(sptr) );
}

#endif	/* _rests_c_ */
