/* ------------------------------------------------------------------------------------	*/
/*				 CompatibleOne Cloudware				*/
/* ------------------------------------------------------------------------------------ */
/*											*/
/* Ce fichier fait partie de ce(tte) oeuvre de Iain James Marshall et est mise a 	*/
/* disposition selon les termes de la licence Creative Commons Paternit‚ : 		*/
/*											*/
/*			 	Pas d'Utilisation Commerciale 				*/
/*				Pas de Modification 					*/
/*				3.0 non transcrit.					*/
/*											*/
/* ------------------------------------------------------------------------------------ */
/* 			Copyright (c) 2011 Iain James Marshall for Prologue 		*/
/*				   All rights reserved					*/
/* ------------------------------------------------------------------------------------ */

#ifndef	_occiresolver_c
#define	_occiresolver_c

#include "occiresolver.h"

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
/*			o c c i _ r e s o l v e r			*/
/*	---------------------------------------------------------	*/
/*	takes a category id/name or other naming string and sends	*/
/*	an equiry to the publication service manager to retrieve	*/
/*	list of current potential candidate targets.			*/
/*	---------------------------------------------------------	*/
public	struct	occi_response * occi_resolver( char * category, char * agent )
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
/*	 o c c i _ r e s o l v e _ c a t e g o r y _ p r o v i d e r 		*/
/*	-------------------------------------------------------------		*/
public	char *	occi_resolve_category_provider( char * category, char * agent, char * tls )
{
	struct	occi_response * zptr;
	struct	occi_element  * eptr;
	char *	hptr;

	/* ------------------------------------------------------ */
	/* attempt to resolve agencys of the "provider" category */
	/* ------------------------------------------------------ */
	if (!( zptr = occi_resolver( category, agent ) ))
		return((char *) 0);

	/* ------------------------------------------------------ */
	/*  scan the list to find their list of providers offered  */
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



	/* --------------- */
#endif	/* _occiresolver_c */
	/* --------------- */




