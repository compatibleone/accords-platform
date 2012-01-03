/* ------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                   */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>    */
/* --------------------------------------------------------------------*/
/*  This is free software; you can redistribute it and/or modify it    */
/*  under the terms of the GNU Lesser General Public License as        */
/*  published by the Free Software Foundation; either version 2.1 of   */
/*  the License, or (at your option) any later version.                */
/*                                                                     */
/*  This software is distributed in the hope that it will be useful,   */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of     */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   */
/*  Lesser General Public License for more details.                    */
/*                                                                     */
/*  You should have received a copy of the GNU Lesser General Public   */
/*  License along with this software; if not, write to the Free        */
/*  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA */
/*  02110-1301 USA, or see the FSF site: http://www.fsf.org.           */
/* --------------------------------------------------------------------*/
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
