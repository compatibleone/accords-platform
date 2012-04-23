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
#ifndef	_stdnode_c
#define	_stdnode_c

#include "stdnode.h"

/*	-----------------------------------------------------------------	*/
/*		r e l e a s e _ s t a n d a r d _ m e s s a g e 		*/
/*	-----------------------------------------------------------------	*/
private	void	release_standard_message( struct standard_message * mptr )
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
public	struct	standard_node * get_standard_node( char * id , char * agent, char * tls )
{
	struct	standard_node 	* nptr;
	int	status;

	if (!( nptr = allocate_standard_node()))
		return( nptr );

	/* ---------------------------- */
	/* recover the node description */
	/* ---------------------------- */
	else if ((status = get_standard_message( &nptr->node, id, agent, tls )) != 0)
		return( drop_standard_node( nptr ) );

	/* -------------------------------------- */
	/* recover the infrastructure description */
	/* -------------------------------------- */
	if (!( id = occi_extract_atribut( nptr->node.message, "occi", 
		_CORDS_NODE, _CORDS_INFRASTRUCTURE ) ))
		return( drop_standard_node( nptr ) );
	else if ((status = get_standard_message( &nptr->infrastructure, id, agent, tls )) != 0)
		return( drop_standard_node( nptr ) );

	/* ------------------------------- */
	/* recover the compute description */
	/* ------------------------------- */
	if (!( id = occi_extract_atribut( nptr->infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_COMPUTE ) ))
		return( drop_standard_node( nptr ) );
	else if ((status = get_standard_message( &nptr->compute, id, agent, tls )) != 0)
		return( drop_standard_node( nptr ) );

	/* ------------------------------- */
	/* recover the network description */
	/* ------------------------------- */
	if (!( id = occi_extract_atribut( nptr->infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_NETWORK ) ))
		return( drop_standard_node( nptr ) );
	else if ((status = get_standard_message( &nptr->network, id, agent, tls )) != 0)
		return( drop_standard_node( nptr ) );

	/* ------------------------------- */
	/* recover the storage description */
	/* ------------------------------- */
	if (!( id = occi_extract_atribut( nptr->infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_STORAGE ) ))
		return( drop_standard_node( nptr ) );
	else if ((status = get_standard_message( &nptr->storage, id, agent, tls )) != 0)
		return( drop_standard_node( nptr ) );

	/* ----------------------------- */
	/* recover the image description */
	/* ----------------------------- */
	if (!( id = occi_extract_atribut( nptr->node.message, "occi", 
		_CORDS_NODE, _CORDS_IMAGE ) ))
		return( drop_standard_node( nptr ) );
	else if ((status = get_standard_message( &nptr->image, id, agent, tls )) != 0)
		return( drop_standard_node( nptr ) );

	/* ------------------------------ */
	/* recover the system description */
	/* ------------------------------ */
	if (!( id = occi_extract_atribut( nptr->image.message, "occi", 
		_CORDS_IMAGE, _CORDS_SYSTEM ) ))
		return( drop_standard_node( nptr ) );
	else if ((status = get_standard_message( &nptr->system, id, agent, tls )) != 0)
		return( drop_standard_node( nptr ) );

	return( nptr );	

}

	/* ---------- */
#endif	/* _stdnode_c */
	/* ---------- */


