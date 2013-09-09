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
#ifndef	_occiauth_c
#define	_occiauth_c

#include "occiauth.h"
#include "cordslang.h"
#include "occiresolver.h"
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
	char 	buffer[1024];
	char *	host;

	if (!( xauth ))
		return(1);
	else	return(1);

	printf("occi_resolve_authorization( %s )\n",xauth);

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
		{
			aptr->ttl = ( time((long *) 0) + Heap.ttl);
			return( 1 );
		}
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
	{
		printf("occi get failure\n");
		return( 0 );
	}
	else if (!( rptr = zptr->response ))
	{
		printf("no http response\n");
		zptr = occi_remove_response( zptr );
		return(0);
	}
	else if ( rptr->status >= 400 )
	{
		printf("incorrect status : %d\n",rptr->status);
		zptr = occi_remove_response( zptr );
		return(0);
	}
	else
	{
		zptr = occi_remove_response( zptr );
		if (!( aptr = add_occi_authorization_item( &Heap ) ))
		{
			printf("failure to allocate authiorization cache item\n");
			return(0);
		}
		else if (!( aptr->token = allocate_string( xauth ) ))
		{
			printf("allocation failure\n");
			aptr = drop_occi_authorization_item( aptr );
			return( 0 );
		}
		else 
		{
			printf("succes\n");
			aptr->ttl = ( time((long *) 0) + Heap.ttl);
			return(1);
		}
	}
}


	/* ----------- */
#endif	/* _occiauth_c */
	/* ----------- */



