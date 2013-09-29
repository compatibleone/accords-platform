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
#ifndef _occihm_c_
#define _occihm_c_

#include "occihm.h"

/*	-------------------------------------------------------		*/
/*	l i b e r a t e _ o c c i _ h e a d e r _ m a n a g e r 	*/
/*	-------------------------------------------------------		*/
public struct occi_header_manager * liberate_occi_header_manager(struct occi_header_manager * sptr)
{
	if ( sptr )
	{
		if ( sptr->headers )
			 sptr->headers = liberate_rest_header(sptr->headers);

		sptr = liberate( sptr );
	}
	return((struct occi_header_manager *) 0);

}

/*	-------------------------------------------------	*/
/*	r e s e t _ o c c i _ h e a d e r _ m a n a g e r 	*/
/*	-------------------------------------------------	*/
public struct occi_header_manager * reset_occi_header_manager(struct occi_header_manager * sptr)
{
	if ( sptr )
	{
		memset(sptr,0,sizeof( struct occi_header_manager));
	}
	return(sptr);

}

/*	------------------------------------------------------- 	*/
/*	a l l o c a t e _ o c c i _ h e a d e r _ m a n a g e r 	*/
/*	------------------------------------------------------- 	*/
public struct occi_header_manager * allocate_occi_header_manager()
{
	struct occi_header_manager * sptr;
	if (!( sptr = allocate( sizeof( struct occi_header_manager ) ) ))
		return( sptr );
	else	return( reset_occi_header_manager(sptr) );
}

#endif	/* _occihm_c_ */
