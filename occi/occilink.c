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
