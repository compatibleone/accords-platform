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
