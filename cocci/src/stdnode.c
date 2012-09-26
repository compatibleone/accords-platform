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
#ifndef	_stdnode_c
#define	_stdnode_c

#include "stdnode.h"

/*	-----------------------------------------------------------------	*/
/*		r e l e a s e _ s t a n d a r d _ m e s s a g e 		*/
/*	-----------------------------------------------------------------	*/
public	void	release_standard_message( struct standard_message * mptr )
{
	if ( mptr->id )
		mptr->id = liberate( mptr->id );
	if ( mptr->message )
		mptr->message = occi_remove_response( mptr->message );
	return;
}

/*	-----------------------------------------------------------------	*/
/*			g e t _ s t a n d a r d _ m e s s a g e 		*/
/*	-----------------------------------------------------------------	*/
public	int	get_standard_message( struct standard_message * mptr, char * id, char * agent, char * tls )
{
	/* ---------------------------- */
	/* release the previous message */
	/* ---------------------------- */
	release_standard_message( mptr );

	/* ---------------------------- */
	/* store the message identifier */
	/* ---------------------------- */
	if (!( mptr->id = allocate_string( id ) ))
	{
		release_standard_message( mptr );
		return(27);
	}

	/* ------------------------------- */
	/* retrieve the identified message */
	/* ------------------------------- */
	else if (!( mptr->message = occi_simple_get( mptr->id, agent, tls ) ))
	{
		release_standard_message( mptr );
		return(40);
	}
	else	return(0);
}

/*	-----------------------------------------------------------	*/
/*	   f i r s t _ s t a n d a r d _ m e s s a g e _ l i n k	*/
/*	-----------------------------------------------------------	*/
public	struct occi_element * 	first_standard_message_link( struct occi_response * zptr )
{
	struct	occi_element  * eptr;
	for (	eptr=zptr->first;
		eptr != (struct occi_element *) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;
		else if ( strcasecmp(eptr->name,"LINK") != 0 )
			continue;
		else	break;
	}
	return( eptr );
}

/*	-----------------------------------------------------------	*/
/*	    n e x t _ s t a n d a r d _ m e s s a g e _ l i n k		*/
/*	-----------------------------------------------------------	*/
public	struct occi_element * 	next_standard_message_link( struct occi_element * eptr )
{
	for (	eptr=eptr->next;
		eptr != (struct occi_element *) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;
		else if ( strcasecmp(eptr->name,"LINK") != 0 )
			continue;
		else	break;
	}
	return( eptr );
}

/*	-----------------------------------------------------------------	*/
/*	      s t a n d a r d _ m e s s a g e _ l i n k _ v a l u e		*/
/*	-----------------------------------------------------------------	*/
public	char *	standard_message_link_value( char * sptr )
{
	char *	rptr;
	if (!( sptr ))
		return( sptr );
	else if ( *sptr != '<' )
		return( sptr );
	else if (!( rptr = allocate_string( (sptr+1) )))
		return( rptr );
	else 
	{
		for ( sptr=rptr; *sptr != 0; sptr++ )
		{
			if ( *sptr == '>' )
			{
				*sptr = 0;
				break;
			}
		}
		return( rptr );
	}
}

/*	-----------------------------------------------------------------	*/
/*		l i b e r a t e _ s t a n d a r d _ n o d e			*/
/*	-----------------------------------------------------------------	*/
public	struct	standard_node * liberate_standard_node( struct	standard_node *  nptr )
{
	if ( nptr )
	{
		release_standard_message( &nptr->node );
		release_standard_message( &nptr->infrastructure );
		release_standard_message( &nptr->compute );
		release_standard_message( &nptr->network );
		release_standard_message( &nptr->storage );
		release_standard_message( &nptr->image );
		release_standard_message( &nptr->system );
		release_standard_message( &nptr->package );
		release_standard_message( &nptr->firewall );
		release_standard_message( &nptr->port );
		liberate( nptr );
	}
	return( (struct standard_node *) 0  );
}

/*	-----------------------------------------------------------------	*/
/*		a l l o c a t e _ s t a n d a r d _ n o d e			*/
/*	-----------------------------------------------------------------	*/
public	struct	standard_node * allocate_standard_node()
{
	struct	standard_node * nptr;
	if (!( nptr = (struct standard_node *) allocate( sizeof( struct standard_node ) ) ))
		return( nptr );
	else
	{
		memset( nptr, 0, sizeof( struct standard_node ));
		return( nptr );
	}
}

/*	-----------------------------------------------------------------	*/
/*			d r o p _ s t a n d a r d _ n o d e			*/
/*	-----------------------------------------------------------------	*/
public	struct	standard_node *	drop_standard_node( struct standard_node *  nptr )
{
	return( liberate_standard_node( nptr ) );
}

/*	-----------------------------------------------------------------	*/
/*			g e t _ s t a n d a r d _ n o d e			*/
/*	-----------------------------------------------------------------	*/
public	struct	standard_node * get_standard_node( char * id , char * agent, char * tls, int * result )
{
	struct	standard_node 	* nptr;
	int	status;

	*result = 0;

	if (!( nptr = allocate_standard_node()))
	{
		*result=1;
		return( nptr );
	}

	/* ---------------------------- */
	/* recover the node description */
	/* ---------------------------- */
	else if ((status = get_standard_message( &nptr->node, id, agent, tls )) != 0)
	{
		*result=2;
		return( drop_standard_node( nptr ) );
	}
	else if (!( id = occi_extract_atribut( nptr->node.message, "occi", 
		_CORDS_NODE, _CORDS_TYPE ) ))
	{
		*result=21;
		return( drop_standard_node( nptr ) );
	}
	else if ( strcasecmp( id, "simple" ) )
	{
		*result=0;
		return( drop_standard_node( nptr ) );
	}
	
	/* -------------------------------------- */
	/* recover the infrastructure description */
	/* -------------------------------------- */
	if (!( id = occi_extract_atribut( nptr->node.message, "occi", 
		_CORDS_NODE, _CORDS_INFRASTRUCTURE ) ))
	{
		*result=3;
		return( drop_standard_node( nptr ) );
	}
	else if ((status = get_standard_message( &nptr->infrastructure, id, agent, tls )) != 0)
	{
		*result=4;
		return( drop_standard_node( nptr ) );
	}

	/* ------------------------------- */
	/* recover the compute description */
	/* ------------------------------- */
	if (!( id = occi_extract_atribut( nptr->infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_COMPUTE ) ))
	{
		*result=5;
		return( drop_standard_node( nptr ) );
	}
	else if ((status = get_standard_message( &nptr->compute, id, agent, tls )) != 0)
	{
		*result=6;
		return( drop_standard_node( nptr ) );
	}

	/* ------------------------------- */
	/* recover the network description */
	/* ------------------------------- */
	if (!( id = occi_extract_atribut( nptr->infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_NETWORK ) ))
	{
		*result=7;
		return( drop_standard_node( nptr ) );
	}
	else if ((status = get_standard_message( &nptr->network, id, agent, tls )) != 0)
	{
		*result=8;
		return( drop_standard_node( nptr ) );
	}

	/* ------------------------------- */
	/* recover the storage description */
	/* ------------------------------- */
	if (!( id = occi_extract_atribut( nptr->infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_STORAGE ) ))
	{
		*result=9;
		return( drop_standard_node( nptr ) );
	}
	else if ((status = get_standard_message( &nptr->storage, id, agent, tls )) != 0)
	{
		*result=10;
		return( drop_standard_node( nptr ) );
	}

	/* ----------------------------- */
	/* recover the image description */
	/* ----------------------------- */
	if (!( id = occi_extract_atribut( nptr->node.message, "occi", 
		_CORDS_NODE, _CORDS_IMAGE ) ))
	{
		*result=11;
		return( drop_standard_node( nptr ) );
	}
	else if ((status = get_standard_message( &nptr->image, id, agent, tls )) != 0)
	{
		*result=12;
		return( drop_standard_node( nptr ) );
	}

	/* ------------------------------ */
	/* recover the system description */
	/* ------------------------------ */
	if (!( id = occi_extract_atribut( nptr->image.message, "occi", 
		_CORDS_IMAGE, _CORDS_SYSTEM ) ))
	{
		*result=13;
		return( drop_standard_node( nptr ) );
	}

	else if ((status = get_standard_message( &nptr->system, id, agent, tls )) != 0)
	{
		*result=14;
		return( drop_standard_node( nptr ) );
	}

	*result=0;
	return( nptr );	

}

	/* ---------- */
#endif	/* _stdnode_c */
	/* ---------- */


