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
#ifndef _occim_c_
#define _occim_c_

#include "occim.h"

/*	------------------------------------------	*/
/*	l i b e r a t e _ o c c i _ m a n a g e r 	*/
/*	------------------------------------------	*/
public struct occi_manager * liberate_occi_manager(struct occi_manager * sptr)
{
	struct 	occi_client * mptr;
	struct	occi_header_manager * hptr;
	if ( sptr )
	{
		while (( hptr = sptr->headers) != (struct occi_header_manager*) 0)
		{
			sptr->headers = hptr->next;
			hptr = liberate_occi_header_manager(hptr);
		}

		while ((mptr = sptr->first) != (struct occi_client *) 0)
		{
			sptr->first = mptr->next;
			mptr = liberate_occi_client( mptr );
		}
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->mimetype )
			 sptr->mimetype = liberate(sptr->mimetype);
		if ( sptr->accepts )
			 sptr->accepts = liberate(sptr->accepts);
		if ( sptr->eventlog )
			 sptr->eventlog = liberate(sptr->eventlog);
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
		sptr->headers = (struct occi_header_manager*) 0;
		sptr->first = (struct occi_client*) 0;
		sptr->last = (struct occi_client*) 0;
		sptr->name = (char*) 0;
		sptr->mimetype = (char*) 0;
		sptr->accepts = (char*) 0;
		sptr->eventlog = (char*) 0;
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
