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
#ifndef _kindnode_c_
#define _kindnode_c_

#include "kindnode.h"

/*	----------------------------------------------	*/
/*	l i b e r a t e _ o c c i _ k i n d _ n o d e 	*/
/*	----------------------------------------------	*/
public struct occi_kind_node * liberate_occi_kind_node(struct occi_kind_node * sptr)
{
	if ( sptr )
	{
		if ( sptr->firstmixin )
			 sptr->firstmixin = liberate_occi_mixin(sptr->firstmixin);
		if ( sptr->lastmixin )
			 sptr->lastmixin = liberate_occi_mixin(sptr->lastmixin);
		if ( sptr->firstlink )
			 sptr->firstlink = liberate_occi_link(sptr->firstlink);
		if ( sptr->lastlink )
			 sptr->lastlink = liberate_occi_link(sptr->lastlink);
		sptr = liberate( sptr );
	}
	return((struct occi_kind_node *) 0);

}

/*	----------------------------------------	*/
/*	r e s e t _ o c c i _ k i n d _ n o d e 	*/
/*	----------------------------------------	*/
public struct occi_kind_node * reset_occi_kind_node(struct occi_kind_node * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct occi_kind_node*) 0;
		sptr->next = (struct occi_kind_node*) 0;
		sptr->contents = (void*) 0;
		sptr->firstmixin = (struct occi_mixin*) 0;
		sptr->lastmixin = (struct occi_mixin*) 0;
		sptr->firstlink = (struct occi_link*) 0;
		sptr->lastlink = (struct occi_link*) 0;
	}
	return(sptr);

}

/*	----------------------------------------------	*/
/*	a l l o c a t e _ o c c i _ k i n d _ n o d e 	*/
/*	----------------------------------------------	*/
public struct occi_kind_node * allocate_occi_kind_node()
{
	struct occi_kind_node * sptr;
	if (!( sptr = allocate( sizeof( struct occi_kind_node ) ) ))
		return( sptr );
	else	return( reset_occi_kind_node(sptr) );
}

#endif	/* _kindnode_c_ */
