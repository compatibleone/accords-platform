/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>     */
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License"); 	*/
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at 				*/
/*  									*/
/*  http://www.apache.org/licenses/LICENSE-2.0 				*/
/*  									*/
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 	*/
/* implied. 								*/
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License. 					*/
/* -------------------------------------------------------------------- */
#ifndef _occipub_c_
#define _occipub_c_

#include "occipub.h"

/*	--------------------------------------------------	*/
/*	l i b e r a t e _ o c c i _ p u b l i c a t i o n 	*/
/*	--------------------------------------------------	*/
public struct occi_publication * liberate_occi_publication(struct occi_publication * sptr)
{
	if ( sptr )
	{
		if ( sptr->category )
			 sptr->category = liberate(sptr->category);
		if ( sptr->id )
			 sptr->id = liberate(sptr->id);
		sptr = liberate( sptr );
	}
	return((struct occi_publication *) 0);

}

/*	--------------------------------------------	*/
/*	r e s e t _ o c c i _ p u b l i c a t i o n 	*/
/*	--------------------------------------------	*/
public struct occi_publication * reset_occi_publication(struct occi_publication * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct occi_publication*) 0;
		sptr->next = (struct occi_publication*) 0;
		sptr->parent = (struct occi_publisher*) 0;
		sptr->category = (char*) 0;
		sptr->id = (char*) 0;
	}
	return(sptr);

}

/*	--------------------------------------------------	*/
/*	a l l o c a t e _ o c c i _ p u b l i c a t i o n 	*/
/*	--------------------------------------------------	*/
public struct occi_publication * allocate_occi_publication()
{
	struct occi_publication * sptr;
	if (!( sptr = allocate( sizeof( struct occi_publication ) ) ))
		return( sptr );
	else	return( reset_occi_publication(sptr) );
}

/*	----------------------------------------	*/
/*	a d d _ o c c i _ p u b l i c a t i o n 	*/
/*	----------------------------------------	*/
public struct occi_publication * add_occi_publication(struct occi_publisher * pptr)
{
	struct occi_publication * sptr;
	if (!( sptr = allocate( sizeof( struct occi_publication ) ) ))
		return( sptr );
	else if (!( sptr = reset_occi_publication(sptr) ))
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

/*	------------------------------------------	*/
/*	d r o p _ o c c i _ p u b l i c a t i o n 	*/
/*	------------------------------------------	*/
public struct occi_publication * drop_occi_publication(struct occi_publication * sptr)
{
	if ( sptr )
	{
		if (!( sptr->parent )) return(sptr);
		if (!( sptr->previous ))
		{
			if (!( sptr->parent->first = sptr->next ))
				sptr->parent->last = (struct occi_publication *) 0;
			else	sptr->parent->first->previous = (struct occi_publication *) 0;
		}
		else if (!( sptr->previous->next = sptr->next ))
			sptr->parent->last = sptr->previous;
		if (!( sptr->next ))
		{
			if (!( sptr->parent->last = sptr->previous ))
				sptr->parent->first = (struct occi_publication *) 0;
			else	sptr->parent->last->next = (struct occi_publication *) 0;
		}
		else if (!( sptr->next->previous = sptr->previous ))
			sptr->parent->first = sptr->next;
		sptr = liberate_occi_publication(sptr);
	}
	return((struct occi_publication *) 0);
}

#endif	/* _occipub_c_ */
