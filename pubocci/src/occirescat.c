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
#ifndef _occirescat_c_
#define _occirescat_c_

#include "occirescat.h"

/*	--------------------------------------------------------------	*/
/*	l i b e r a t e _ o c c i _ r e s o l v e d _ c a t e g o r y 	*/
/*	--------------------------------------------------------------	*/
public struct occi_resolved_category * liberate_occi_resolved_category(struct occi_resolved_category * sptr)
{
	struct occi_resolved_agency * mptr;
	if ( sptr )
	{
		if ( sptr->category )
			 sptr->category = liberate(sptr->category);
		while ((mptr = sptr->first) != (struct occi_resolved_agency *) 0)
		{
			sptr->first = mptr->next;
			mptr = liberate_occi_resolved_agency( mptr );
		}
		sptr = liberate( sptr );
	}
	return((struct occi_resolved_category *) 0);

}

/*	--------------------------------------------------------	*/
/*	r e s e t _ o c c i _ r e s o l v e d _ c a t e g o r y 	*/
/*	--------------------------------------------------------	*/
public struct occi_resolved_category * reset_occi_resolved_category(struct occi_resolved_category * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct occi_resolved_category*) 0;
		sptr->next = (struct occi_resolved_category*) 0;
		sptr->parent = (struct occi_resolver_manager*) 0;
		sptr->category = (char*) 0;
		sptr->first = (struct occi_resolved_agency*) 0;
		sptr->last = (struct occi_resolved_agency*) 0;
	}
	return(sptr);

}

/*	--------------------------------------------------------------	*/
/*	a l l o c a t e _ o c c i _ r e s o l v e d _ c a t e g o r y 	*/
/*	--------------------------------------------------------------	*/
public struct occi_resolved_category * allocate_occi_resolved_category()
{
	struct occi_resolved_category * sptr;
	if (!( sptr = allocate( sizeof( struct occi_resolved_category ) ) ))
		return( sptr );
	else	return( reset_occi_resolved_category(sptr) );
}

/*	----------------------------------------------------	*/
/*	a d d _ o c c i _ r e s o l v e d _ c a t e g o r y 	*/
/*	----------------------------------------------------	*/
public struct occi_resolved_category * add_occi_resolved_category(struct occi_resolver_manager * pptr)
{
	struct occi_resolved_category * sptr;
	if (!( sptr = allocate( sizeof( struct occi_resolved_category ) ) ))
		return( sptr );
	else if (!( sptr = reset_occi_resolved_category(sptr) ))
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

/*	------------------------------------------------------	*/
/*	d r o p _ o c c i _ r e s o l v e d _ c a t e g o r y 	*/
/*	------------------------------------------------------	*/
public struct occi_resolved_category * drop_occi_resolved_category(struct occi_resolved_category * sptr)
{
	if ( sptr )
	{
		if (!( sptr->parent )) return(sptr);
		if (!( sptr->previous ))
		{
			if (!( sptr->parent->first = sptr->next ))
				sptr->parent->last = (struct occi_resolved_category *) 0;
			else	sptr->parent->first->previous = (struct occi_resolved_category *) 0;
		}
		else if (!( sptr->previous->next = sptr->next ))
			sptr->parent->last = sptr->previous;
		if (!( sptr->next ))
		{
			if (!( sptr->parent->last = sptr->previous ))
				sptr->parent->first = (struct occi_resolved_category *) 0;
			else	sptr->parent->last->next = (struct occi_resolved_category *) 0;
		}
		else if (!( sptr->next->previous = sptr->previous ))
			sptr->parent->first = sptr->next;
		sptr = liberate_occi_resolved_category(sptr);
	}
	return((struct occi_resolved_category *) 0);
}

#endif	/* _occirescat_c_ */
