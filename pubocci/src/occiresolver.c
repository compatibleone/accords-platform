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
#ifndef	_occiresolver_c
#define	_occiresolver_c

#include "occiresolver.h"
#include "cp.h"

private	struct	occi_publisher Resolver = {
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(struct occi_publication *) 0,
	(struct occi_publication *) 0
	};

private	struct	occi_resolver_manager ResManager = {
	_OCCI_TTL,
	1,	/* is optimised */
	(struct occi_resolved_category *) 0,
	(struct occi_resolved_category *) 0
	};

#include "occirespro.c"
#include "occirescat.c"

/*	---------------------------------------------------------	*/
/*	o c c i _ l o c a t e _ r e s o l v e d _ c a t e g o r y	*/
/*	---------------------------------------------------------	*/
private	struct	occi_resolved_category * occi_locate_resolved_category( char * nptr )
{
	struct	occi_resolved_category * cptr;
	for (	cptr=ResManager.first;
		cptr != (struct occi_resolved_category *) 0;
		cptr = cptr->next )
	{
		if (!( cptr->category ))
			continue;
		else if (!( strcmp( cptr->category, nptr ) ))
			break;
	}
	return( cptr );
}

/*	---------------------------------------------------------	*/
/*	o c c i _ l o c a t e _ r e s o l v e d _ a g e n c y	*/
/*	---------------------------------------------------------	*/
private	struct	occi_resolved_agency * occi_locate_resolved_agency(
	struct	occi_resolved_category * cptr,
	 char * nptr )
{
	struct	occi_resolved_agency * pptr;
	for (	pptr=cptr->first;
		pptr != (struct occi_resolved_agency *) 0;
		pptr = pptr->next )
	{
		if (!( pptr->id ))
			continue;
		else if (!( strcmp( pptr->id, nptr ) ))
			break;
	}
	return( pptr );
}

/*	---------------------------------------------------------	*/
/*	   o c c i _ a d d _ r e s o l v e d _ a g e n c y		*/
/*	---------------------------------------------------------	*/
private	struct	occi_resolved_agency * occi_add_resolved_agency( char * cname, char * pid)
{
	struct	occi_resolved_category * cptr;
	struct	occi_resolved_agency * pptr;
	if (!( cptr = occi_locate_resolved_category( cname ) ))
	{
		if (!( cptr = add_occi_resolved_category( &ResManager ) ))
			return((struct occi_resolved_agency *) 0);
		else if (!( cptr->category = allocate_string( cname ) ))
		{
			cptr = liberate_occi_resolved_category( cptr );
			return((struct occi_resolved_agency *) 0);
		}
	}
	if (( pptr = occi_locate_resolved_agency( cptr, pid )) != (struct occi_resolved_agency *) 0)
		return( pptr );
	else if (!( pptr = add_occi_resolved_agency( cptr ) ))
		return( pptr );
	else if (!( pptr->id = allocate_string( pid )))
		return( drop_occi_resolved_agency( pptr ) );
	else
	{
		pptr->ttl = (time((long*) 0) + ResManager.ttl);
		return( pptr );
	}
}	

/*	---------------------------------------------------------	*/
/*	   i n i t i a l i s e _ o c c i _ r e s o l v e r		*/
/*	---------------------------------------------------------	*/
public	int	initialise_occi_resolver( char * host, char * room, char * publication, char * enquiry )
{
	char	*	sptr;

	if (!( Resolver.tls ))
		Resolver.tls = allocate_string( default_tls() );

	if ( host )
	{
		if (!( sptr = allocate_string( host ) ))
			return( 27 );
		if ( Resolver.host )
			Resolver.host = liberate( Resolver.host );
		Resolver.host = sptr;
	}
	if ( room )
	{
		if (!( sptr = allocate_string( room ) ))
			return( 27 );
		if ( Resolver.room )
			Resolver.room = liberate( Resolver.room );
		Resolver.room = sptr;
	}
	if ( publication )
	{
		if (!( sptr = allocate_string( publication ) ))
			return( 27 );
		if ( Resolver.publication )
			Resolver.publication = liberate( Resolver.publication );
		Resolver.publication = sptr;
	}
	if ( enquiry )
	{
		if (!( sptr = allocate_string( enquiry ) ))
			return( 27 );
		if ( Resolver.enquiry )
			Resolver.enquiry = liberate( Resolver.enquiry );
		Resolver.enquiry = sptr;
	}
	return( 0 );
}

/*	---------------------------------------------------------	*/
/*	     t e r m i n a t e _ o c c i _ r e s o l v e r		*/
/*	---------------------------------------------------------	*/
public	int	terminate_occi_resolver()
{
	char	*	sptr;
	if ( Resolver.host )
		Resolver.host = liberate( Resolver.host );
	if ( Resolver.room )
		Resolver.room = liberate( Resolver.room );
	if ( Resolver.publication )
		Resolver.publication = liberate( Resolver.publication );
	if ( Resolver.enquiry )
		Resolver.enquiry = liberate( Resolver.enquiry );
	if ( Resolver.uri )
		Resolver.uri = liberate( Resolver.uri );
	return(0);
}

/*	---------------------------------------------------------	*/
/*		    r e s o l v e r _ d e f a u l t 			*/
/*	---------------------------------------------------------	*/
public	int	occi_resolver_default()
{
	if (!( Resolver.host ))
		if (!( Resolver.host = allocate_string( _DEFAULT_HOST ) ))
			return( 27 );

	if (!( Resolver.room ))
		if (!( Resolver.room = allocate_string( _DEFAULT_ROOM ) ))
			return( 27 );

	if (!( Resolver.publication ))
		if (!( Resolver.publication = allocate_string( _DEFAULT_PUBLICATION ) ))
			return( 27 );

	if (!( Resolver.enquiry ))
		if (!( Resolver.enquiry = allocate_string( _DEFAULT_ENQUIRY ) ))
			return( 27 );

	if (!( Resolver.uri ))
	{
		if (!( Resolver.uri = allocate( strlen( Resolver.host ) + strlen( Resolver.publication ) + 16 ) ))
			return(27);
		else	sprintf(Resolver.uri,"%s/%s/",Resolver.host,Resolver.publication);
	}
	return(0);
}

/*	---------------------------------------------------------	*/
/*	     o c c i _ r e s o l v e r _ o p e r a t i o n		*/
/*	---------------------------------------------------------	*/
/*	takes a category id/name or other naming string and sends	*/
/*	an equiry to the publication service manager to retrieve	*/
/*	list of current potential candidate targets.			*/
/*	---------------------------------------------------------	*/
private	struct	occi_response * occi_resolver_operation( char * category, char * element, char * criteria, char * agent )
{
	struct	occi_resolved_agency *pptr=(struct occi_resolved_agency*) 0;
	struct	occi_element 	*	tptr=(struct occi_element*) 0;
	struct	occi_element 	*	eptr=(struct occi_element*) 0;
	struct	occi_element 	*	eeptr=(struct occi_element*) 0;
	struct	occi_client  	*	cptr=(struct occi_client*) 0;
	struct	occi_client  	*	ccptr=(struct occi_client*) 0;
	struct	occi_request	*	rptr=(struct occi_request*) 0;
	struct	occi_request	*	rrptr=(struct occi_request*) 0;
	struct	occi_response	*	aptr=(struct occi_response*) 0;
	struct	occi_response	*	zptr=(struct occi_response*) 0;
	struct	occi_response	*	aaptr=(struct occi_response*) 0;
	char			*	vptr;
	char				buffer[4096];

	if ( check_debug() )
		printf("   OCCI Resolver( %s, %s )\n",category,agent);
	if ( occi_resolver_default() != 0 )
		return((struct occi_response * )0);

	else if (!( cptr = occi_create_client( Resolver.uri, agent, Resolver.tls ) ))
		return((struct occi_response * )0);

	else if (!(rptr = occi_create_request( cptr, Resolver.publication, _OCCI_NORMAL )))
	{
		cptr = occi_remove_client( cptr );
		return((struct occi_response * )0);
	}
	else if ((!( eptr = occi_request_element( rptr, "occi.publication.where",Resolver.room	)))
	     ||  (!( eptr = occi_request_element( rptr, "occi.publication.what",category 	))))
	{
		rptr = occi_remove_request( rptr );
		cptr = occi_remove_client( cptr );
		return((struct occi_response * )0);
	}
	else if ((criteria) && (!( eptr = occi_request_element( rptr, element ,criteria ))))
	{
		rptr = occi_remove_request( rptr );
		cptr = occi_remove_client( cptr );
		return((struct occi_response * )0);
	}
	else if (!( aptr = occi_client_get( cptr, rptr ) ))
	{
		rptr = occi_remove_request( rptr );
		cptr = occi_remove_client( cptr );
		return((struct occi_response * )0);
	}
	else if (!( zptr = allocate_occi_response() ))
	{
		aptr = occi_remove_response( aptr );
		rptr = occi_remove_request( rptr );
		cptr = occi_remove_client( cptr );
		return((struct occi_response * )0);
	}
	else
	{
		/* scan the list of responses and retrieve provide addresses */
		/* --------------------------------------------------------- */
		for (	eptr = aptr->first;
			eptr != (struct occi_element *) 0;
			eptr = eptr->next )
		{
			if (!( vptr = eptr->name ))
				continue;
			else if ( strcmp(vptr, Resolver.publication ) != 0 )
				continue;
			else if (!( vptr = occi_category_id( eptr->value ) ))
				continue;
			if ( ResManager.optimised )
			{
				if (!( pptr = occi_add_resolved_agency( category, vptr ) ))
				{
					liberate( vptr );
					continue;
				}
				else if (( pptr->host != (char *) 0 )	
				     &&  ( pptr->ttl > time((long *) 0) ))
				{
					liberate( vptr );
					if (!( eeptr = occi_response_element( zptr, category, pptr->host ) ))
						break;
					else	continue;
				}
			}
			sprintf(buffer,"%s%s",Resolver.uri,vptr);
			liberate( vptr );

			if (!( ccptr = occi_redirect_client( cptr, buffer ) ))
				break;
			else if (!(rrptr = occi_create_request( cptr, Resolver.publication, _OCCI_NORMAL)))
				break;
			else if (!( aaptr = occi_client_get( cptr, rptr ) ))
			{
				rrptr = occi_remove_request( rrptr );
				break;
			}
			else if (!( tptr = occi_locate_element( aaptr->first, "occi.publication.why" ) ))
			{
				aaptr = occi_remove_response( aaptr );
				rrptr = occi_remove_request( rrptr );
				continue;
			}
			if ( ResManager.optimised )
			{
				if (!( pptr->host = allocate_string( tptr->value ) ))
				{
					aaptr = occi_remove_response( aaptr );
					rrptr = occi_remove_request( rrptr );
					break;
				}
				else if (!( eeptr = occi_response_element( zptr, category, pptr->host ) ))
				{
					aaptr = occi_remove_response( aaptr );
					rrptr = occi_remove_request( rrptr );
					break;
				}
				else
				{
					pptr->ttl = ( time((long *) 0) + ResManager.ttl );
					aaptr = occi_remove_response( aaptr );
					rrptr = occi_remove_request( rrptr );
					continue;
				}
			}
			else
			{
				if (!( tptr->value ))
				{
					aaptr = occi_remove_response( aaptr );
					rrptr = occi_remove_request( rrptr );
					break;
				}
				else if (!( eeptr = occi_response_element( zptr, category, tptr->value ) ))
				{
					aaptr = occi_remove_response( aaptr );
					rrptr = occi_remove_request( rrptr );
					break;
				}
				else
				{
					aaptr = occi_remove_response( aaptr );
					rrptr = occi_remove_request( rrptr );
					continue;
				}
			}

		}
		aptr = occi_remove_response( aptr );
		rptr = occi_remove_request( rptr );
		cptr = occi_remove_client( cptr );
		return(zptr);
	}
}

/*	---------------------------------------------------------	*/
/*			o c c i _ r e s o l v e r			*/
/*	---------------------------------------------------------	*/
/*	takes a category id/name or other naming string and sends	*/
/*	an equiry to the publication service manager to retrieve	*/
/*	list of current potential candidate targets.			*/
/*	---------------------------------------------------------	*/
public	struct	occi_response * occi_resolver( char * category, char * agent )
{
	return( occi_resolver_operation( category, (char *) 0, (char *) 0, agent ) );
}

/*	---------------------------------------------------------	*/
/*		o c c i _ r e s o l v e _ b y _ z o n e 		*/
/*	---------------------------------------------------------	*/
/*	takes a category id/name or other naming string and sends	*/
/*	an equiry to the publication service manager to retrieve	*/
/*	list of current potential candidate targets and selects 	*/
/*	those that match the required zone				*/
/*	---------------------------------------------------------	*/
public	struct	occi_response * occi_resolve_by_zone( char * category, char * zone, char * agent )
{
	if (!( zone ))
		return( occi_resolver( category, agent ) );
	else	return( occi_resolver_operation( category, "occi.publication.zone", zone, agent ) );
}

/*	---------------------------------------------------------	*/
/*		o c c i _ r e s o l v e _ b y _ o p e r a t o r		*/
/*	---------------------------------------------------------	*/
public	struct	occi_response * occi_resolve_by_operator( char * category, char * operator, char * agent )
{
	if (!( operator ))
		return( occi_resolver( category, agent ) );
	else	return( occi_resolver_operation( category, "occi.publication.operator",operator, agent ) );
}

/*	---------------------------------------------------------	*/
/*		o c c i _ r e s o l v e _ b y _ s e c u r i t y		*/
/*	---------------------------------------------------------	*/
public	struct	occi_response * occi_resolve_by_security( char * category, char * security, char * agent )
{
	if (!( security ))
		return( occi_resolver( category, agent ) );
	else	return( occi_resolver_operation( category, "occi.publication.security",security, agent ) );
}

/*	---------------------------------------------------------	*/
/*		o c c i _ r e s o l v e _ b y _ p r i c e 		*/
/*	---------------------------------------------------------	*/
public	struct	occi_response * occi_resolve_by_price( char * category, char * price, char * agent )
{
	if (!( price ))
		return( occi_resolver( category, agent ) );
	else	return( occi_resolver_operation( category, "occi.publication.price",price, agent ) );
}

/*	---------------------------------------------------------	*/
/*		o c c i _ r e s o l v e _ b y _ e n e r g y 		*/
/*	---------------------------------------------------------	*/
public	struct	occi_response * occi_resolve_by_energy( char * category, char * energy, char * agent )
{
	if (!( energy ))
		return( occi_resolver( category, agent ) );
	else	return( occi_resolver_operation( category, "occi.publication.energy",energy, agent ) );
}

/*	---------------------------------------------------------	*/
/*		o c c i _ r e s o l v e _ b y _ s c o r e		*/
/*	---------------------------------------------------------	*/
public	struct	occi_response * occi_resolve_by_score( char * category, char * score, char * agent )
{
	if (!( score ))
		return( occi_resolver( category, agent ) );
	else	return( occi_resolver_operation( category, "occi.publication.opinion",score, agent ) );
}

/*	-------------------------------------------------------------		*/
/*	 o c c i _ r e s o l v e _ c a t e g o r y _ p r o v i d e r 		*/
/*	-------------------------------------------------------------		*/
public	char *	occi_resolve_category_provider( char * category, char * agent, char * tls )
{
	struct	occi_response * zptr;
	struct	occi_element  * eptr;
	char *	hptr;

	/* ---------------------------------------------------- */
	/* attempt to resolve agency of the "provider" category */
	/* ---------------------------------------------------- */
	if (!( zptr = occi_resolver( category, agent ) ))
		return((char *) 0);

	/* ------------------------------------------------------ */
	/*  scan the list to find their list of providers offered */
	/* ------------------------------------------------------ */
	for (	eptr = zptr->first;
		eptr != (struct occi_element*) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;
		else	break;
	}
	if (!( eptr ))
	{
		zptr = occi_remove_response( zptr );
		return( (char *) 0);
	}
	else if (!( hptr = allocate_string( eptr->value )))
	{
		zptr = occi_remove_response( zptr );
		return( (char *) 0);
	}
	else
	{
		zptr = occi_remove_response( zptr );
		return( hptr );
	}
}


/*	---------------------------------------------------------	*/
/*			o c c i _ p r i c i n g  			*/
/*	---------------------------------------------------------	*/
/*	takes a category id/name or other naming string and sends	*/
/*	an equiry to the publication service manager to retrieve	*/
/*	list of current potential candidate targets.			*/
/*	---------------------------------------------------------	*/
public	struct	occi_response * occi_pricing( char * category, char * agent )
{
	struct	occi_resolved_agency *pptr=(struct occi_resolved_agency*) 0;
	struct	occi_element 	*	tptr=(struct occi_element*) 0;
	struct	occi_element 	*	eptr=(struct occi_element*) 0;
	struct	occi_element 	*	eeptr=(struct occi_element*) 0;
	struct	occi_client  	*	cptr=(struct occi_client*) 0;
	struct	occi_client  	*	ccptr=(struct occi_client*) 0;
	struct	occi_request	*	rptr=(struct occi_request*) 0;
	struct	occi_request	*	rrptr=(struct occi_request*) 0;
	struct	occi_response	*	aptr=(struct occi_response*) 0;
	struct	occi_response	*	zptr=(struct occi_response*) 0;
	struct	occi_response	*	aaptr=(struct occi_response*) 0;
	char			*	vptr;
	char				buffer[4096];

	if ( check_debug() )
		printf("   OCCI Pricing( %s, %s )\n",category,agent);

	if ( occi_resolver_default() != 0 )
		return((struct occi_response * )0);

	else if (!( cptr = occi_create_client( Resolver.uri, agent, Resolver.tls ) ))
		return((struct occi_response * )0);

	else if (!(rptr = occi_create_request( cptr, Resolver.publication, _OCCI_NORMAL )))
	{
		cptr = occi_remove_client( cptr );
		return((struct occi_response * )0);
	}
	else if ((!( eptr = occi_request_element( rptr, "occi.publication.where",Resolver.room	)))
	     ||  (!( eptr = occi_request_element( rptr, "occi.publication.what",category 	))))
	{
		rptr = occi_remove_request( rptr );
		cptr = occi_remove_client( cptr );
		return((struct occi_response * )0);
	}
	else if (!( aptr = occi_client_get( cptr, rptr ) ))
	{
		rptr = occi_remove_request( rptr );
		cptr = occi_remove_client( cptr );
		return((struct occi_response * )0);
	}
	else if (!( zptr = allocate_occi_response() ))
	{
		aptr = occi_remove_response( aptr );
		rptr = occi_remove_request( rptr );
		cptr = occi_remove_client( cptr );
		return((struct occi_response * )0);
	}
	else
	{
		/* scan the list of responses and retrieve provide addresses */
		/* --------------------------------------------------------- */
		for (	eptr = aptr->first;
			eptr != (struct occi_element *) 0;
			eptr = eptr->next )
		{
			if (!( vptr = eptr->name ))
				continue;
			else if ( strcmp(vptr, Resolver.publication ) != 0 )
				continue;
			else if (!( vptr = occi_category_id( eptr->value ) ))
				continue;
			if ( ResManager.optimised )
			{
				if (!( pptr = occi_add_resolved_agency( category, vptr ) ))
				{
					liberate( vptr );
					continue;
				}
				else if (( pptr->host != (char *) 0 )	
				     &&  ( pptr->ttl > time((long *) 0) ))
				{
					liberate( vptr );
					if (!( eeptr = occi_response_element( zptr, category, pptr->host ) ))
						break;
					else	continue;
				}
			}
			sprintf(buffer,"%s%s",Resolver.uri,vptr);
			liberate( vptr );

			if (!( ccptr = occi_redirect_client( cptr, buffer ) ))
				break;
			else if (!(rrptr = occi_create_request( cptr, Resolver.publication, _OCCI_NORMAL)))
				break;
			else if (!( aaptr = occi_client_get( cptr, rptr ) ))
			{
				rrptr = occi_remove_request( rrptr );
				break;
			}
			else if (!( tptr = occi_locate_element( aaptr->first, "occi.publication.why" ) ))
			{
				aaptr = occi_remove_response( aaptr );
				rrptr = occi_remove_request( rrptr );
				continue;
			}
			if ( ResManager.optimised )
			{
				if (!( pptr->host = allocate_string( tptr->value ) ))
				{
					aaptr = occi_remove_response( aaptr );
					rrptr = occi_remove_request( rrptr );
					break;
				}
				else if (!( eeptr = occi_response_element( zptr, category, pptr->host ) ))
				{
					aaptr = occi_remove_response( aaptr );
					rrptr = occi_remove_request( rrptr );
					break;
				}
				else
				{
					pptr->ttl = ( time((long *) 0) + ResManager.ttl );
					aaptr = occi_remove_response( aaptr );
					rrptr = occi_remove_request( rrptr );
					continue;
				}
			}
			else
			{
				if (!( tptr->value ))
				{
					aaptr = occi_remove_response( aaptr );
					rrptr = occi_remove_request( rrptr );
					break;
				}
				else if (!( eeptr = occi_response_element( zptr, category, tptr->value ) ))
				{
					aaptr = occi_remove_response( aaptr );
					rrptr = occi_remove_request( rrptr );
					break;
				}
				else
				{
					aaptr = occi_remove_response( aaptr );
					rrptr = occi_remove_request( rrptr );
					continue;
				}
			}

		}
		aptr = occi_remove_response( aptr );
		rptr = occi_remove_request( rptr );
		cptr = occi_remove_client( cptr );
		return(zptr);
	}
}

/*	-------------------------------------------------------------		*/
/*	    o c c i _ r e s o l v e _ c a t e g o r y _ p r i c e 		*/
/*	-------------------------------------------------------------		*/
public	char *	occi_resolve_category_price( char * category, char * operator, char * agent, char * tls )
{
	struct	occi_response 	* aptr;
	struct	occi_element  	* eptr;
	char 			* host=(char *) 0;
	struct	occi_client	* cptr;
	struct	occi_request	* rptr;
	struct	occi_response 	* zptr;
	struct	occi_element  	* fptr;
	char 			* result=(char *) 0;

	if (!( aptr = cords_retrieve_named_instance_list( _CORDS_PRICE, "occi.price.name", category, agent, tls ) ))
		return( (char *) 0 );

	for (	eptr = aptr->first;
		eptr != (struct occi_element*) 0;
		eptr = eptr->next )
	{
		/* ------------------------------------- */
		/* ensure valid element list information */
		/* ------------------------------------- */
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;

		/* ------------------------------------- */
		/* build the price category instance ID  */
		/* ------------------------------------- */
		else if (!( host = cords_build_host( aptr, eptr->value) ))
			continue;

		/* ------------------------------------- */
		/* a null or wild operator selects this  */
		/* ------------------------------------- */
		else if (!( operator ))
			return( host );
		else if ( *operator == '*' )
			return( host );

		/* ------------------------------------- */
		/* attempt to locate the category price  */
		/* ------------------------------------- */
		else if (!( cptr = occi_create_client( host, agent, tls ) ))
			continue;
		else if (!( rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
			continue;
		else if (!( zptr = occi_client_get( cptr, rptr ) ))
			continue;
		else
		{
			/* ------------------------------------------------- */
			/* scan the list of elements and verify the password */
			/* ------------------------------------------------- */
			for (	fptr = zptr->first;
				fptr != (struct occi_element *) 0;
				fptr = fptr->next )
			{
				if (!( fptr->name ))
					continue;
				else if ( strcmp( fptr->name, "occi.price.operator"  ) )
					continue;
				else
				{
					if (!( strcmp( fptr->value, operator) ))
						result = allocate_string( host );
					break;
				}
			}
			zptr = occi_remove_response( zptr );
			rptr = occi_remove_request( rptr );
			cptr = occi_remove_client( cptr );
			host = liberate( host );
			if ( result ) break;
		}

	}
	aptr = occi_remove_response( aptr );
	return( result );
}

/*	-------------------------------------------------------------		*/
/*	    o c c i _ r e s o l v e _ c o n s u m e r				*/
/*	-------------------------------------------------------------		*/
public	char *	occi_resolve_consumer( char * consumer, char * agent, char * tls )
{
	struct	occi_response 	* aptr;
	struct	occi_element  	* eptr;
	char 			* host=(char *) 0;
	struct	occi_client	* cptr;
	struct	occi_request	* rptr;
	struct	occi_response 	* zptr;
	struct	occi_element  	* fptr;
	char 			* result=(char *) 0;

	if (!( aptr = cords_retrieve_named_instance_list( _CORDS_CONSUMER, "occi.consumer.name", consumer, agent, tls ) ))
		return( (char *) 0 );

	for (	eptr = aptr->first;
		eptr != (struct occi_element*) 0;
		eptr = eptr->next )
	{
		/* ------------------------------------- */
		/* ensure valid element list information */
		/* ------------------------------------- */
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;

		/* ---------------------------------------- */
		/* build the consumer category instance ID  */
		/* ---------------------------------------- */
		else if (!( host = cords_build_host( aptr, eptr->value) ))
			continue;

		/* ------------------------------------- */
		/* attempt to locate the category price  */
		/* ------------------------------------- */
		else if (!( cptr = occi_create_client( host, agent, tls ) ))
			continue;
		else if (!( rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
			continue;
		else if (!( zptr = occi_client_get( cptr, rptr ) ))
			continue;
		else
		{
			/* ------------------------------------------------- */
			/* scan the list of elements and verify the identity */
			/* ------------------------------------------------- */
			for (	fptr = zptr->first;
				fptr != (struct occi_element *) 0;
				fptr = fptr->next )
			{
				if (!( fptr->name ))
					continue;
				else if ( strcmp( fptr->value, consumer ) )
					continue;
				else if (!( result = allocate_string( host )))
					break;
				else	break;
			}
			zptr = occi_remove_response( zptr );
			rptr = occi_remove_request( rptr );
			cptr = occi_remove_client( cptr );
			host = liberate( host );
			if ( result ) break;
		}

	}
	aptr = occi_remove_response( aptr );
	return( result );
}

/*	-------------------------------------------------------------		*/
/*	    	o c c i _ r e s o l v e _ m e t r i c 				*/
/*	-------------------------------------------------------------		*/
public	char *	occi_resolve_metric( char * metric, char * agent, char * tls )
{
	struct	occi_response 	* aptr;
	struct	occi_element  	* eptr;
	char 			* host=(char *) 0;
	struct	occi_client	* cptr;
	struct	occi_request	* rptr;
	struct	occi_response 	* zptr;
	struct	occi_element  	* fptr;
	char 			* result=(char *) 0;

	if (!( aptr = cords_retrieve_named_instance_list( _CORDS_METRIC, "occi.metric.name", metric, agent, tls ) ))
		return( (char *) 0 );

	for (	eptr = aptr->first;
		eptr != (struct occi_element*) 0;
		eptr = eptr->next )
	{
		/* ------------------------------------- */
		/* ensure valid element list information */
		/* ------------------------------------- */
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;

		/* -------------------------------------- */
		/* build the metric category instance ID  */
		/* -------------------------------------- */
		else if (!( host = cords_build_host( aptr, eptr->value) ))
			continue;

		/* ------------------------------------- */
		/* attempt to locate the metric category */
		/* ------------------------------------- */
		else if (!( cptr = occi_create_client( host, agent, tls ) ))
			continue;
		else if (!( rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
			continue;
		else if (!( zptr = occi_client_get( cptr, rptr ) ))
			continue;
		else
		{
			/* ------------------------------------------------- */
			/* scan the list of elements and verify the identity */
			/* ------------------------------------------------- */
			for (	fptr = zptr->first;
				fptr != (struct occi_element *) 0;
				fptr = fptr->next )
			{
				if (!( fptr->name ))
					continue;
				else if ( strcmp( fptr->value, metric ) )
					continue;
				else if (!( result = allocate_string( host )))
					break;
				else	break;
			}
			zptr = occi_remove_response( zptr );
			rptr = occi_remove_request( rptr );
			cptr = occi_remove_client( cptr );
			host = liberate( host );
			if ( result ) break;
		}

	}
	aptr = occi_remove_response( aptr );
	return( result );
}

/*	-------------------------------------------------------------		*/
/*	 o c c i _ r e s o l v e _ c o n s u m e r _ i d e n t i t y		*/
/*	-------------------------------------------------------------		*/
public	char *	occi_resolve_consumer_identity( char * consumer, char * agent, char * tls )
{
	struct	occi_response 	* aptr;
	struct	occi_element  	* eptr;
	char 			* host=(char *) 0;
	struct	occi_client	* cptr;
	struct	occi_request	* rptr;
	struct	occi_response 	* zptr;
	struct	occi_element  	* fptr;
	char 			* result=(char *) 0;

	if (!( aptr = cords_retrieve_named_instance_list( _CORDS_CONSUMER, "occi.consumer.name", consumer, agent, tls ) ))
		return( (char *) 0 );

	for (	eptr = aptr->first;
		eptr != (struct occi_element*) 0;
		eptr = eptr->next )
	{
		/* ------------------------------------- */
		/* ensure valid element list information */
		/* ------------------------------------- */
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;

		/* ------------------------------------- */
		/* build the price category instance ID  */
		/* ------------------------------------- */
		else if (!( host = cords_build_host( aptr, eptr->value) ))
			continue;

		/* ------------------------------------- */
		/* attempt to locate the category price  */
		/* ------------------------------------- */
		else if (!( cptr = occi_create_client( host, agent, tls ) ))
			continue;
		else if (!( rptr = occi_create_request( cptr, cptr->target->object, _OCCI_NORMAL )))
			continue;
		else if (!( zptr = occi_client_get( cptr, rptr ) ))
			continue;
		else if (!( fptr = occi_locate_element( zptr->first, "occi.consumer.name" ) ))
			continue;
		else if ( strcmp( fptr->value , consumer ) )
			continue;
		else if (!( fptr = occi_locate_element( zptr->first, "occi.consumer.identity" ) ))
			continue;
		else if (!( result = allocate_string( fptr->value ) ))
			continue;
		else 
		{
			zptr = occi_remove_response( zptr );
			rptr = occi_remove_request( rptr );
			cptr = occi_remove_client( cptr );
			host = liberate( host );
			if ( result ) break;
		}

	}
	aptr = occi_remove_response( aptr );
	return( result );
}


	/* --------------- */
#endif	/* _occiresolver_c */
	/* --------------- */




