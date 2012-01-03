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
