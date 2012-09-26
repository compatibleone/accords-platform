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
#ifndef _cse_c_
#define _cse_c_

#include "cse.h"

/*	------------------------------------------------------	*/
/*	l i b e r a t e _ c o r d s c r i p t _ e l e m e n t 	*/
/*	------------------------------------------------------	*/
public struct cordscript_element * liberate_cordscript_element(struct cordscript_element * sptr)
{
	if ( sptr )
	{
		if ( sptr->prefix )
			 sptr->prefix = liberate(sptr->prefix);
		if ( sptr->value )
			 sptr->value = liberate(sptr->value);
		sptr = liberate( sptr );
	}
	return((struct cordscript_element *) 0);

}

/*	------------------------------------------------	*/
/*	r e s e t _ c o r d s c r i p t _ e l e m e n t 	*/
/*	------------------------------------------------	*/
public struct cordscript_element * reset_cordscript_element(struct cordscript_element * sptr)
{
	if ( sptr )
	{
		sptr->previous = (struct cordscript_element*) 0;
		sptr->next = (struct cordscript_element*) 0;
		sptr->type =  0;
		sptr->prefix = (char*) 0;
		sptr->value = (char*) 0;
	}
	return(sptr);

}

/*	------------------------------------------------------	*/
/*	a l l o c a t e _ c o r d s c r i p t _ e l e m e n t 	*/
/*	------------------------------------------------------	*/
public struct cordscript_element * allocate_cordscript_element()
{
	struct cordscript_element * sptr;
	if (!( sptr = allocate( sizeof( struct cordscript_element ) ) ))
		return( sptr );
	else	return( reset_cordscript_element(sptr) );
}

#endif	/* _cse_c_ */
