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
#ifndef _occilink_c_
#define _occilink_c_

#include "occilink.h"

/*	------------------------------------	*/
/*	l i b e r a t e _ o c c i _ l i n k 	*/
/*	------------------------------------	*/
public struct occi_link * liberate_occi_link(struct occi_link * sptr)
{
	if ( sptr )
	{
		if ( sptr->from )
			 sptr->from = liberate(sptr->from);
		if ( sptr->to )
			 sptr->to = liberate(sptr->to);
		sptr = liberate( sptr );
	}
	return((struct occi_link *) 0);

}

/*	------------------------------	*/
/*	r e s e t _ o c c i _ l i n k 	*/
/*	------------------------------	*/
public struct occi_link * reset_occi_link(struct occi_link * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct occi_link*) 0;
		sptr->next = (struct occi_link*) 0;
		sptr->from = (char*) 0;
		sptr->to = (char*) 0;
	}
	return(sptr);

}

/*	------------------------------------	*/
/*	a l l o c a t e _ o c c i _ l i n k 	*/
/*	------------------------------------	*/
public struct occi_link * allocate_occi_link()
{
	struct occi_link * sptr;
	if (!( sptr = allocate( sizeof( struct occi_link ) ) ))
		return( sptr );
	else	return( reset_occi_link(sptr) );
}

#endif	/* _occilink_c_ */
