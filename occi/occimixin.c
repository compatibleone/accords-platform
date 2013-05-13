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
