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
#ifndef _occipar_c_
#define _occipar_c_

#include "occipar.h"

/*	______________________________________________	*/
/*	l i b e r a t e _ o c c i _ p a r a m e t e r 	*/
/*	______________________________________________	*/
public struct occi_parameter * liberate_occi_parameter(struct occi_parameter * sptr)
{
	if ( sptr )
	{
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		sptr = liberate( sptr );
	}
	return((struct occi_parameter *) 0);

}

/*	________________________________________	*/
/*	r e s e t _ o c c i _ p a r a m e t e r 	*/
/*	________________________________________	*/
public struct occi_parameter * reset_occi_parameter(struct occi_parameter * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct occi_parameter*) 0;
		sptr->next = (struct occi_parameter*) 0;
		sptr->parent = (struct occi_action*) 0;
		sptr->name = (char*) 0;
	}
	return(sptr);

}

/*	______________________________________________	*/
/*	a l l o c a t e _ o c c i _ p a r a m e t e r 	*/
/*	______________________________________________	*/
public struct occi_parameter * allocate_occi_parameter()
{
	struct occi_parameter * sptr;
	if (!( sptr = allocate( sizeof( struct occi_parameter ) ) ))
		return( sptr );
	else	return( reset_occi_parameter(sptr) );
}

/*	____________________________________	*/
/*	a d d _ o c c i _ p a r a m e t e r 	*/
/*	____________________________________	*/
public struct occi_parameter * add_occi_parameter(struct occi_action * pptr)
{
	struct occi_parameter * sptr;
	if (!( sptr = allocate( sizeof( struct occi_parameter ) ) ))
		return( sptr );
	else if (!( sptr = reset_occi_parameter(sptr) ))
		return( sptr );
	else
	{
		if (!( sptr->previous = pptr->last ))
			pptr->first = sptr;
		else	sptr->previous->next = sptr;
		pptr->last = sptr;
		sptr->parent = pptr;
		return( sptr );
	}

}

/*	______________________________________	*/
/*	d r o p _ o c c i _ p a r a m e t e r 	*/
/*	______________________________________	*/
public struct occi_parameter * drop_occi_parameter(struct occi_parameter * sptr)
{
	if ( sptr )
	{
		if (!( sptr->parent )) return(sptr);
		if (!( sptr->previous ))
		{
			if (!( sptr->parent->first = sptr->next ))
				sptr->parent->last = (struct occi_parameter *) 0;
			else	sptr->parent->first->previous = (struct occi_parameter *) 0;
		}
		else if (!( sptr->previous->next = sptr->next ))
			sptr->parent->last = sptr->previous;
		if (!( sptr->next ))
		{
			if (!( sptr->parent->last = sptr->previous ))
				sptr->parent->first = (struct occi_parameter *) 0;
			else	sptr->parent->last->next = (struct occi_parameter *) 0;
		}
		else if (!( sptr->next->previous = sptr->previous ))
			sptr->parent->first = sptr->next;
		sptr = liberate_occi_parameter(sptr);
	}
	return((struct occi_parameter *) 0);
}

#endif	/* _occipar_c_ */
