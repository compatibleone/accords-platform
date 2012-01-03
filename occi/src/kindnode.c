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
#ifndef _kindnode_c_
#define _kindnode_c_

#include "kindnode.h"

/*	----------------------------------------------	*/
/*	l i b e r a t e _ o c c i _ k i n d _ n o d e 	*/
/*	----------------------------------------------	*/
public struct occi_kind_node * liberate_occi_kind_node(struct occi_kind_node * sptr)
{
	if ( sptr )
	{
		if ( sptr->firstmixin )
			 sptr->firstmixin = liberate_occi_mixin(sptr->firstmixin);
		if ( sptr->lastmixin )
			 sptr->lastmixin = liberate_occi_mixin(sptr->lastmixin);
		if ( sptr->firstlink )
			 sptr->firstlink = liberate_occi_link(sptr->firstlink);
		if ( sptr->lastlink )
			 sptr->lastlink = liberate_occi_link(sptr->lastlink);
		sptr = liberate( sptr );
	}
	return((struct occi_kind_node *) 0);

}

/*	----------------------------------------	*/
/*	r e s e t _ o c c i _ k i n d _ n o d e 	*/
/*	----------------------------------------	*/
public struct occi_kind_node * reset_occi_kind_node(struct occi_kind_node * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct occi_kind_node*) 0;
		sptr->next = (struct occi_kind_node*) 0;
		sptr->contents = (void*) 0;
		sptr->firstmixin = (struct occi_mixin*) 0;
		sptr->lastmixin = (struct occi_mixin*) 0;
		sptr->firstlink = (struct occi_link*) 0;
		sptr->lastlink = (struct occi_link*) 0;
	}
	return(sptr);

}

/*	----------------------------------------------	*/
/*	a l l o c a t e _ o c c i _ k i n d _ n o d e 	*/
/*	----------------------------------------------	*/
public struct occi_kind_node * allocate_occi_kind_node()
{
	struct occi_kind_node * sptr;
	if (!( sptr = allocate( sizeof( struct occi_kind_node ) ) ))
		return( sptr );
	else	return( reset_occi_kind_node(sptr) );
}

#endif	/* _kindnode_c_ */
