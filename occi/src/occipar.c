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
