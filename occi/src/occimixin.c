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
#ifndef _occimixin_c_
#define _occimixin_c_

#include "occimixin.h"

/*	--------------------------------------	*/
/*	l i b e r a t e _ o c c i _ m i x i n 	*/
/*	--------------------------------------	*/
public struct occi_mixin * liberate_occi_mixin(struct occi_mixin * sptr)
{
	if ( sptr )
	{
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		sptr = liberate( sptr );
	}
	return((struct occi_mixin *) 0);

}

/*	--------------------------------	*/
/*	r e s e t _ o c c i _ m i x i n 	*/
/*	--------------------------------	*/
public struct occi_mixin * reset_occi_mixin(struct occi_mixin * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct occi_mixin*) 0;
		sptr->next = (struct occi_mixin*) 0;
		sptr->id = (char*) 0;
	}
	return(sptr);

}

/*	--------------------------------------	*/
/*	a l l o c a t e _ o c c i _ m i x i n 	*/
/*	--------------------------------------	*/
public struct occi_mixin * allocate_occi_mixin()
{
	struct occi_mixin * sptr;
	if (!( sptr = allocate( sizeof( struct occi_mixin ) ) ))
		return( sptr );
	else	return( reset_occi_mixin(sptr) );
}

#endif	/* _occimixin_c_ */
