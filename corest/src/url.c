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
#ifndef _url_c_
#define _url_c_

#include "url.h"

/*	------------------------	*/
/*	l i b e r a t e _ u r l 	*/
/*	------------------------	*/
public struct url * liberate_url(struct url * sptr)
{
	if ( sptr )
	{
		if ( sptr->service )
			 sptr->service = liberate(sptr->service);
		if ( sptr->host )
			 sptr->host = liberate(sptr->host);
		if ( sptr->object )
			 sptr->object = liberate(sptr->object);
		if ( sptr->tls )
			 sptr->tls = liberate(sptr->tls);
		sptr = liberate( sptr );
	}
	return((struct url *) 0);

}

/*	------------------	*/
/*	r e s e t _ u r l 	*/
/*	------------------	*/
public struct url * reset_url(struct url * sptr)
{
	if ( sptr )
	{
		sptr->service = (char*) 0;
		sptr->host = (char*) 0;
		sptr->port =  0;
		sptr->object = (char*) 0;
		sptr->tls = (char*) 0;
	}
	return(sptr);

}

/*	------------------------	*/
/*	a l l o c a t e _ u r l 	*/
/*	------------------------	*/
public struct url * allocate_url()
{
	struct url * sptr;
	if (!( sptr = allocate( sizeof( struct url ) ) ))
		return( sptr );
	else	return( reset_url(sptr) );
}

#endif	/* _url_c_ */
