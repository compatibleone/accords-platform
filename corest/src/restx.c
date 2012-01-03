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
#ifndef _restx_c_
#define _restx_c_

#include "restx.h"

/*	----------------------------------------------	*/
/*	l i b e r a t e _ r e s t _ e x t e n s i o n 	*/
/*	----------------------------------------------	*/
public struct rest_extension * liberate_rest_extension(struct rest_extension * sptr)
{
	if ( sptr )
	{
		sptr = liberate( sptr );
	}
	return((struct rest_extension *) 0);

}

/*	----------------------------------------	*/
/*	r e s e t _ r e s t _ e x t e n s i o n 	*/
/*	----------------------------------------	*/
public struct rest_extension * reset_rest_extension(struct rest_extension * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct rest_extension*) 0;
		sptr->next = (struct rest_extension*) 0;
		sptr->payload = (void*) 0;
		sptr->type =  0;
	}
	return(sptr);

}

/*	----------------------------------------------	*/
/*	a l l o c a t e _ r e s t _ e x t e n s i o n 	*/
/*	----------------------------------------------	*/
public struct rest_extension * allocate_rest_extension()
{
	struct rest_extension * sptr;
	if (!( sptr = allocate( sizeof( struct rest_extension ) ) ))
		return( sptr );
	else	return( reset_rest_extension(sptr) );
}

#endif	/* _restx_c_ */
