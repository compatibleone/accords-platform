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
#ifndef _restresponse_c_
#define _restresponse_c_

#include "restresponse.h"

/*	--------------------------------------------	*/
/*	l i b e r a t e _ r e s t _ r e s p o n s e 	*/
/*	--------------------------------------------	*/
public struct rest_response * liberate_rest_response(struct rest_response * sptr)
{
	struct rest_header * mptr;
	if ( sptr )
	{
		while ((mptr = sptr->first) != (struct rest_header *) 0)
		{
			sptr->first = mptr->next;
			mptr = liberate_rest_header( mptr );
		}
		if ( sptr->version )
			 sptr->version = liberate(sptr->version);
		if ( sptr->message )
			 sptr->message = liberate(sptr->message);
		if ( sptr->body )
			 sptr->body = liberate(sptr->body);
		sptr = liberate( sptr );
	}
	return((struct rest_response *) 0);

}

/*	--------------------------------------	*/
/*	r e s e t _ r e s t _ r e s p o n s e 	*/
/*	--------------------------------------	*/
public struct rest_response * reset_rest_response(struct rest_response * sptr)
{
	if ( sptr )
	{
		sptr->first = (struct rest_header*) 0;
		sptr->last = (struct rest_header*) 0;
		sptr->status =  0;
		sptr->version = (char*) 0;
		sptr->message = (char*) 0;
		sptr->type =  0;
		sptr->body = (char*) 0;
	}
	return(sptr);

}

/*	--------------------------------------------	*/
/*	a l l o c a t e _ r e s t _ r e s p o n s e 	*/
/*	--------------------------------------------	*/
public struct rest_response * allocate_rest_response()
{
	struct rest_response * sptr;
	if (!( sptr = allocate( sizeof( struct rest_response ) ) ))
		return( sptr );
	else	return( reset_rest_response(sptr) );
}

#endif	/* _restresponse_c_ */
