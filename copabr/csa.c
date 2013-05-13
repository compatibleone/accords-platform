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
#ifndef _csa_c_
#define _csa_c_

#include "csa.h"

/*	----------------------------------------------------	*/
/*	l i b e r a t e _ c o r d s c r i p t _ a c t i o n 	*/
/*	----------------------------------------------------	*/
public struct cordscript_action * liberate_cordscript_action(struct cordscript_action * sptr)
{
	if ( sptr )
	{
		if ( sptr->lvalue )
			 sptr->lvalue = liberate_cordscript_element(sptr->lvalue);
		if ( sptr->rvalue )
			 sptr->rvalue = liberate_cordscript_element(sptr->rvalue);
		sptr = liberate( sptr );
	}
	return((struct cordscript_action *) 0);

}

/*	----------------------------------------------	*/
/*	r e s e t _ c o r d s c r i p t _ a c t i o n 	*/
/*	----------------------------------------------	*/
public struct cordscript_action * reset_cordscript_action(struct cordscript_action * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct cordscript_action*) 0;
		sptr->next = (struct cordscript_action*) 0;
		sptr->type =  0;
		sptr->lvalue = (struct cordscript_element*) 0;
		sptr->rvalue = (struct cordscript_element*) 0;
		sptr->status =  0;
	}
	return(sptr);

}

/*	----------------------------------------------------	*/
/*	a l l o c a t e _ c o r d s c r i p t _ a c t i o n 	*/
/*	----------------------------------------------------	*/
public struct cordscript_action * allocate_cordscript_action()
{
	struct cordscript_action * sptr;
	if (!( sptr = allocate( sizeof( struct cordscript_action ) ) ))
		return( sptr );
	else	return( reset_cordscript_action(sptr) );
}

#endif	/* _csa_c_ */
