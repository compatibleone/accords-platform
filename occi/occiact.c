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
#ifndef _occiact_c_
#define _occiact_c_

#include "occiact.h"

public	char *	occi_html_body(struct occi_category * cptr,struct rest_header  * hptr );

/*	________________________________________	*/
/*	l i b e r a t e _ o c c i _ a c t i o n 	*/
/*	________________________________________	*/
public struct occi_action * liberate_occi_action(struct occi_action * sptr)
{
	struct occi_parameter * mptr;
	if ( sptr )
	{
		while ((mptr = sptr->first) != (struct occi_parameter *) 0)
		{
			sptr->first = mptr->next;
			mptr = liberate_occi_parameter( mptr );
		}
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		sptr = liberate( sptr );
	}
	return((struct occi_action *) 0);

}

/*	__________________________________	*/
/*	r e s e t _ o c c i _ a c t i o n 	*/
/*	__________________________________	*/
public struct occi_action * reset_occi_action(struct occi_action * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct occi_action*) 0;
		sptr->next = (struct occi_action*) 0;
		sptr->parent = (struct occi_category*) 0;
		sptr->first = (struct occi_parameter*) 0;
		sptr->last = (struct occi_parameter*) 0;
		sptr->name = (char*) 0;
		sptr->html_rendering = occi_html_body;
	}
	return(sptr);

}

/*	________________________________________	*/
/*	a l l o c a t e _ o c c i _ a c t i o n 	*/
/*	________________________________________	*/
public struct occi_action * allocate_occi_action()
{
	struct occi_action * sptr;
	if (!( sptr = allocate( sizeof( struct occi_action ) ) ))
		return( sptr );
	else	return( reset_occi_action(sptr) );
}

/*	______________________________	*/
/*	a d d _ o c c i _ a c t i o n 	*/
/*	______________________________	*/
public struct occi_action * add_occi_action(struct occi_category * pptr)
{
	struct occi_action * sptr;
	if (!( sptr = allocate( sizeof( struct occi_action ) ) ))
		return( sptr );
	else if (!( sptr = reset_occi_action(sptr) ))
		return( sptr );
	else
	{
		if (!( sptr->previous = pptr->lastact ))
			pptr->firstact = sptr;
		else	sptr->previous->next = sptr;
		pptr->lastact = sptr;
		sptr->parent = pptr;
		return( sptr );
	}

}

/*	________________________________	*/
/*	d r o p _ o c c i _ a c t i o n 	*/
/*	________________________________	*/
public struct occi_action * drop_occi_action(struct occi_action * sptr)
{
	if ( sptr )
	{
		if (!( sptr->parent )) return(sptr);
		if (!( sptr->previous ))
		{
			if (!( sptr->parent->firstact = sptr->next ))
				sptr->parent->lastact = (struct occi_action *) 0;
			else	sptr->parent->firstact->previous = (struct occi_action *) 0;
		}
		else if (!( sptr->previous->next = sptr->next ))
			sptr->parent->lastact = sptr->previous;
		if (!( sptr->next ))
		{
			if (!( sptr->parent->lastact = sptr->previous ))
				sptr->parent->firstact = (struct occi_action *) 0;
			else	sptr->parent->lastact->next = (struct occi_action *) 0;
		}
		else if (!( sptr->next->previous = sptr->previous ))
			sptr->parent->firstact = sptr->next;
		sptr = liberate_occi_action(sptr);
	}
	return((struct occi_action *) 0);
}

#endif	/* _occiact_c_ */
