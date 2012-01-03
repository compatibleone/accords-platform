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
