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

#ifndef _item_c_
#define _item_c_

#include "item.h"

/*	--------------------------	*/
/*	l i b e r a t e _ i t e m 	*/
/*	--------------------------	*/
public struct item * liberate_item(struct item * sptr)
{
	if ( sptr )
	{
		if ( sptr->name )
			 sptr->name = liberate(sptr->name);
		if ( sptr->basic )
			 sptr->basic = liberate(sptr->basic);
		if ( sptr->type )
			 sptr->type = liberate(sptr->type);
		sptr = liberate( sptr );
	}
	return((struct item *) 0);

}

/*	--------------------	*/
/*	r e s e t _ i t e m 	*/
/*	--------------------	*/
public struct item * reset_item(struct item * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct item*) 0;
		sptr->next = (struct item*) 0;
		sptr->name = (char*) 0;
		sptr->basic = (char*) 0;
		sptr->type = (char*) 0;
		sptr->dimension =  0;
		sptr->indirection =  0;
	}
	return(sptr);

}

/*	--------------------------	*/
/*	a l l o c a t e _ i t e m 	*/
/*	--------------------------	*/
public struct item * allocate_item()
{
	struct item * sptr;
	if (!( sptr = allocate( sizeof( struct item ) ) ))
		return( sptr );
	else	return( reset_item(sptr) );
}

#endif	/* _item_c_ */
