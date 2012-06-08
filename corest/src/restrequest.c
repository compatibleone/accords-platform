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
#ifndef _restrequest_c_
#define _restrequest_c_

#include "restrequest.h"

/*	------------------------------------------	*/
/*	l i b e r a t e _ r e s t _ r e q u e s t 	*/
/*	------------------------------------------	*/
public struct rest_request * liberate_rest_request(struct rest_request * sptr)
{
	struct rest_header * mptr;
	if ( sptr )
	{
		while ((mptr = sptr->first) != (struct rest_header *) 0)
		{
			sptr->first = mptr->next;
			mptr = liberate_rest_header( mptr );
		}
		if ( sptr->method )
			 sptr->method = liberate(sptr->method);
		if ( sptr->object )
			 sptr->object = liberate(sptr->object);
		if ( sptr->parameters )
			 sptr->parameters = liberate(sptr->parameters);
		if ( sptr->version )
			 sptr->version = liberate(sptr->version);
		if ( sptr->body )
			 sptr->body = liberate(sptr->body);
		if ( sptr->host )
			 sptr->host = liberate(sptr->host);
		sptr = liberate( sptr );
	}
	return((struct rest_request *) 0);

}

/*	------------------------------------	*/
/*	r e s e t _ r e s t _ r e q u e s t 	*/
/*	------------------------------------	*/
public struct rest_request * reset_rest_request(struct rest_request * sptr)
{
	if ( sptr )
	{
		sptr->first = (struct rest_header*) 0;
		sptr->last = (struct rest_header*) 0;
		sptr->method = (char*) 0;
		sptr->object = (char*) 0;
		sptr->parameters = (char*) 0;
		sptr->version = (char*) 0;
		sptr->type =  0;
		sptr->body = (char*) 0;
		sptr->host = (char*) 0;
	}
	return(sptr);

}

/*	------------------------------------------	*/
/*	a l l o c a t e _ r e s t _ r e q u e s t 	*/
/*	------------------------------------------	*/
public struct rest_request * allocate_rest_request()
{
	struct rest_request * sptr;
	if (!( sptr = allocate( sizeof( struct rest_request ) ) ))
		return( sptr );
	else	return( reset_rest_request(sptr) );
}

#endif	/* _restrequest_c_ */
