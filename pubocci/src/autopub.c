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
#ifndef _autopub_c_
#define _autopub_c_

#include "autopub.h"

/*	--------------------------------------------------	*/
/*	l i b e r a t e _ a u t o _ p u b l i c a t i o n 	*/
/*	--------------------------------------------------	*/
public struct auto_publication * liberate_auto_publication(struct auto_publication * sptr)
{
	if ( sptr )
	{
		if ( sptr->user )
			 sptr->user = liberate(sptr->user);
		if ( sptr->password )
			 sptr->password = liberate(sptr->password);
		if ( sptr->url )
			 sptr->url = liberate(sptr->url);
		if ( sptr->agent )
			 sptr->agent = liberate(sptr->agent);
		sptr = liberate( sptr );
	}
	return((struct auto_publication *) 0);

}

/*	--------------------------------------------	*/
/*	r e s e t _ a u t o _ p u b l i c a t i o n 	*/
/*	--------------------------------------------	*/
public struct auto_publication * reset_auto_publication(struct auto_publication * sptr)
{
	if ( sptr )
	{
		sptr->item =  0;
		sptr->user = (char*) 0;
		sptr->password = (char*) 0;
		sptr->url = (char*) 0;
		sptr->agent = (char*) 0;
	}
	return(sptr);

}

/*	--------------------------------------------------	*/
/*	a l l o c a t e _ a u t o _ p u b l i c a t i o n 	*/
/*	--------------------------------------------------	*/
public struct auto_publication * allocate_auto_publication()
{
	struct auto_publication * sptr;
	if (!( sptr = allocate( sizeof( struct auto_publication ) ) ))
		return( sptr );
	else	return( reset_auto_publication(sptr) );
}

#endif	/* _autopub_c_ */
