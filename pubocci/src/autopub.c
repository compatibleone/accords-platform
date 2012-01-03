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
#ifndef _autopub_c_
#define _autopub_c_

#include "autopub.h"

/*	--------------------------------------------------	*/
/*	l i b e r a t e _ a u t o _ p u b l i c a t i o n 	*/
/*	--------------------------------------------------	*/
public struct auto_publication * liberate_auto_publication(struct auto_publication * sptr)
{
	if ( sptr )
	{
		if ( sptr->user )
			 sptr->user = liberate(sptr->user);
		if ( sptr->password )
			 sptr->password = liberate(sptr->password);
		if ( sptr->url )
			 sptr->url = liberate(sptr->url);
		if ( sptr->agent )
			 sptr->agent = liberate(sptr->agent);
		sptr = liberate( sptr );
	}
	return((struct auto_publication *) 0);

}

/*	--------------------------------------------	*/
/*	r e s e t _ a u t o _ p u b l i c a t i o n 	*/
/*	--------------------------------------------	*/
public struct auto_publication * reset_auto_publication(struct auto_publication * sptr)
{
	if ( sptr )
	{
		sptr->item =  0;
		sptr->user = (char*) 0;
		sptr->password = (char*) 0;
		sptr->url = (char*) 0;
		sptr->agent = (char*) 0;
	}
	return(sptr);

}

/*	--------------------------------------------------	*/
/*	a l l o c a t e _ a u t o _ p u b l i c a t i o n 	*/
/*	--------------------------------------------------	*/
public struct auto_publication * allocate_auto_publication()
{
	struct auto_publication * sptr;
	if (!( sptr = allocate( sizeof( struct auto_publication ) ) ))
		return( sptr );
	else	return( reset_auto_publication(sptr) );
}

#endif	/* _autopub_c_ */
