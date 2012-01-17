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
#ifndef	_occiauth_c
#define	_occiauth_c

#include "occiauth.h"
#include "authitem.c"

/*	----------------------------------------------------	*/
/*	This file provides an OCCI authorization token cache	*/
/*	management mechanism where by authorization tokens 	*/
/*	can be validated to ensure that they represent real	*/
/*	authentication credentials. 				*/
/*	----------------------------------------------------	*/

private	struct	occi_authorization_cache Heap = 
{
	(struct occi_authorization_item *) 0,
	(struct occi_authorization_item *) 0,
	_AUTH_TTL 
};

/*	---------------------------------------------------	*/
/*	o c c i _ a u t h o r i z a t i o n _ s e t _ t t l 	*/
/*	---------------------------------------------------	*/
public	void	occi_authorization_set_ttl( int value )
{
	Heap.ttl = value;
	return;
}

/*	---------------------------------------------------	*/
/*	o c c i _ r e l e a s e _ a u t h o r i z a t i o n	*/
/*	---------------------------------------------------	*/
public	void	occi_release_authorization_cache()
{
	struct	occi_authorization_item * aptr;

	while ((aptr = Heap.first) != (struct occi_authorization_item *) 0)
		aptr = drop_occi_authorization_item( aptr );
	return;
}

/*	---------------------------------------------------	*/
/*	o c c i _ r e s o l v e _ a u t h o r i z a t i o n	*/
/*	---------------------------------------------------	*/
public	int	occi_resolve_authorization( char * xauth )
{
	struct	occi_authorization_item * aptr;
	struct	rest_response * rptr;
	struct	occi_response * zptr;
	if (!( xauth ))
		return( 1 );

	/* --------------------------------------------------- */
	/* scan the list of TTL controlled authorization items */
	/* --------------------------------------------------- */
	for (	aptr=Heap.first;
		aptr != (struct occi_authorization_item *) 0;
		aptr = aptr->next )
	{
		if (!( aptr->token ))
			continue;
		else if ( strcmp( aptr->token, xauth ) )
			continue;
		else if ( aptr->ttl > time((long *) 0) )
			return( 1 );
		else
		{
			aptr = drop_occi_authorization_item( aptr );
			break;
		}
	}

	/* ------------------------------------------------------ */
	/* arrival here requires the authorization to be resolved */
	/* ------------------------------------------------------ */
	if (!( zptr = occi_simple_get( xauth, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( 0 );
	else if (!( rptr = zptr->response ))
	{
		zptr = occi_remove_response( zptr );
		return(0);
	}
	else if ( rptr->status >= 400 )
	{
		zptr = occi_remove_response( zptr );
		return(0);
	}
	else
	{
		zptr = occi_remove_response( zptr );
		if (!( aptr = add_occi_authorization_item( &Heap ) ))
			return(0);
		else if (!( aptr->token = allocate_string( xauth ) ))
		{
			aptr = drop_occi_authorization_item( aptr );
			return( 0 );
		}
		else 
		{
			aptr->ttl = Heap.ttl;
			return(1);
		}
	}
}


	/* ----------- */
#endif	/* _occiauth_c */
	/* ----------- */



