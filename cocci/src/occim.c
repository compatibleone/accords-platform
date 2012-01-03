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
#ifndef _occim_c_
#define _occim_c_

#include "occim.h"

/*	------------------------------------------	*/
/*	l i b e r a t e _ o c c i _ m a n a g e r 	*/
/*	------------------------------------------	*/
public struct occi_manager * liberate_occi_manager(struct occi_manager * sptr)
{
	struct occi_client * mptr;
	if ( sptr )
	{
		if ( sptr->headers )
			 sptr->headers = liberate_rest_header(sptr->headers);
		while ((mptr = sptr->first) != (struct occi_client *) 0)
		{
			sptr->first = mptr->next;
			mptr = liberate_occi_client( mptr );
		}
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->mimetype )
			 sptr->mimetype = liberate(sptr->mimetype);
		sptr = liberate( sptr );
	}
	return((struct occi_manager *) 0);

}

/*	------------------------------------	*/
/*	r e s e t _ o c c i _ m a n a g e r 	*/
/*	------------------------------------	*/
public struct occi_manager * reset_occi_manager(struct occi_manager * sptr)
{
	if ( sptr )
	{
		sptr->headers = (struct rest_header*) 0;
		sptr->first = (struct occi_client*) 0;
		sptr->last = (struct occi_client*) 0;
		sptr->name = (char*) 0;
		sptr->mimetype = (char*) 0;
		sptr->clients =  0;
		sptr->optimise =  0;
	}
	return(sptr);

}

/*	------------------------------------------	*/
/*	a l l o c a t e _ o c c i _ m a n a g e r 	*/
/*	------------------------------------------	*/
public struct occi_manager * allocate_occi_manager()
{
	struct occi_manager * sptr;
	if (!( sptr = allocate( sizeof( struct occi_manager ) ) ))
		return( sptr );
	else	return( reset_occi_manager(sptr) );
}

#endif	/* _occim_c_ */
