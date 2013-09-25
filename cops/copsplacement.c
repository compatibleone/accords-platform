#ifndef	_copsplacement_c
#define	_copsplacement_c

#include "cspi.h"

#include "link_backend.h"

/*	-----------------------------------------------------------	*/
/*			s e l e c t _ p l a c e m e n t			*/
/*	-----------------------------------------------------------	*/
private	char *	select_placement( struct occi_response * zptr )
{
	struct	occi_element 	* eptr;
	for (	eptr = zptr->first;
		eptr != (struct occi_element*) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;
		else 	return( allocate_string( eptr->value ) );
	}
	return((char *) 0);
}

/*	-----------------------------------------------------------	*/
/*			d e f a u l t _ p l a c e m e n t		*/
/*	-----------------------------------------------------------	*/
private	int	default_placement(
		struct occi_category * optr, 
		struct cords_placement * pptr,
		char * agent,
		char * tls )
{
	struct	occi_response 	* zptr;
	if (!( zptr = occi_resolver( pptr->provider, agent ) ))
		return( 48 );
	else if (!( pptr->solution = select_placement( zptr ) ))
		return( 30 );
	else	return( 0  );
}

/*	-----------------------------------------------------------	*/
/*		p r i c e _ b a s e d _ p l a c e m e n t		*/
/*	-----------------------------------------------------------	*/
private	int	price_based_placement(
		struct occi_category * optr, 
		struct cords_placement * pptr,
		char * agent,
		char * tls )
{
	struct	occi_response 	* zptr;
	if (!( strcmp( pptr->price, "any" ) ))
		return( default_placement( optr, pptr, agent, tls ) );
	else if (!( zptr = occi_resolve_by_price( pptr->provider,  pptr->price, agent ) ))
		return( 48 );
	else if (!( pptr->solution = select_placement( zptr ) ))
		return( 30 );
	else	return( 0  );
}

/*	-----------------------------------------------------------	*/
/*		s e c u r i t y _ b a s e d _ p l a c e m e n t		*/
/*	-----------------------------------------------------------	*/
private	int	security_based_placement(
		struct occi_category * optr, 
		struct cords_placement * pptr,
		char * agent,
		char * tls )
{
	struct	occi_response 	* zptr;
	if (!( pptr->security ))
		return( default_placement( optr, pptr, agent, tls ) );
	else if (!( strcmp( pptr->security, "any" ) ))
		return( default_placement( optr, pptr, agent, tls ) );
	else if (!( zptr = occi_resolve_by_security( pptr->provider, pptr->security, agent ) ))
		return( 48 );
	else if (!( pptr->solution = select_placement( zptr ) ))
		return( 30 );
	else	return( 0  );
}

/*	-----------------------------------------------------------	*/
/*		e n e r g y _ b a s e d _ p l a c e m e n t		*/
/*	-----------------------------------------------------------	*/
private	int	energy_based_placement(
		struct occi_category * optr, 
		struct cords_placement * pptr,
		char * agent,
		char * tls )
{
	struct	occi_response 	* zptr;
	if (!( pptr->energy ))
		return( default_placement( optr, pptr, agent, tls ) );
	else if (!( strcmp( pptr->energy, "any" ) ))
		return( default_placement( optr, pptr, agent, tls ) );
	else if (!( zptr = occi_resolve_by_energy( pptr->provider, pptr->energy, agent ) ))
		return( 48 );
	else if (!( pptr->solution = select_placement( zptr ) ))
		return( 30 );
	else	return( 0  );
}

/*	-----------------------------------------------------------	*/
/*		  z o n e _ b a s e d _ p l a c e m e n t		*/
/*	-----------------------------------------------------------	*/
private	int	zone_based_placement(
		struct occi_category * optr, 
		struct cords_placement * pptr,
		char * agent,
		char * tls )
{
	struct	occi_response 	* zptr;
	if (!( pptr->zone ))
		return( default_placement( optr, pptr, agent, tls ) );
	else if (!( strcmp( pptr->zone, "any" ) ))
		return( default_placement( optr, pptr, agent, tls ) );
	else if (!( zptr = occi_resolve_by_zone( pptr->provider, pptr->zone, agent ) ))
		return( 48 );
	else if (!( pptr->solution = select_placement( zptr ) ))
		return( 30 );
	else	return( 0  );
}

/*	-----------------------------------------------------------	*/
/*		o p e r a t o r _ b a s e d _ p l a c e m e n t		*/
/*	-----------------------------------------------------------	*/
private	int	operator_based_placement(
		struct occi_category * optr, 
		struct cords_placement * pptr,
		char * agent,
		char * tls )
{
	struct	occi_response 	* zptr;
	if (!( pptr->operator ))
		return( default_placement( optr, pptr, agent, tls ) );
	else if (!( strcmp( pptr->operator, "any" ) ))
		return( default_placement( optr, pptr, agent, tls ) );
	else if (!( zptr = occi_resolve_by_operator( pptr->provider, pptr->operator, agent ) ))
		return( 48 );
	else if (!( pptr->solution = select_placement( zptr ) ))
		return( 30 );
	else	return( 0  );
}

/*	-----------------------------------------------------------	*/
/*		s c o r e _ b a s e d _ p l a c e m e n t		*/
/*	-----------------------------------------------------------	*/
private	int	score_based_placement(
		struct occi_category * optr, 
		struct cords_placement * pptr,
		char * agent,
		char * tls )
{
	struct	occi_response 	* zptr;
	if (!( pptr->opinion ))
		return( default_placement( optr, pptr, agent, tls ) );
	else if (!( strcmp( pptr->opinion, "any" ) ))
		return( default_placement( optr, pptr, agent, tls ) );
	else if (!( zptr = occi_resolve_by_score( pptr->provider, pptr->opinion, agent ) ))
		return( 48 );
	else if (!( pptr->solution = select_placement( zptr ) ))
		return( 30 );
	else	return( 0  );
}

/*	------------------------------------------------------------	*/
/*		q u o t a _ b a s e d _ p l a c e m e n t		*/
/*	------------------------------------------------------------	*/
/*	this placement makes use of the new provider quota algorithm	*/
/*	------------------------------------------------------------	*/
private	int	quota_based_placement(
		struct occi_category * optr, 
		struct cords_placement * pptr,
		char * agent,
		char * tls )
{
	if (!( pptr->solution = resolve_cops_solution( pptr ) ))
		return( 30 );
	else	return( 0  );
}

/*	------------------------------------------------------------	*/
/*	     e v a l u a t e _ a l g o r i t h m _ s c r i p t 		*/
/*	------------------------------------------------------------	*/
private	int	evaluate_algorithm_script( struct cords_placement * pptr, char * algorithm )
{
	char *	argv[10];
	int	argc=0;
	char *	result;
	char	reference[1024];
	if (!( rest_valid_string( algorithm ) ))
		return( 0 );
	else
	{
		sprintf(reference,"%s/%s/%s",Cops.identity,_CORDS_PLACEMENT,pptr->id);
		argv[argc++] = "cops.algorithm";
		argv[argc++] = reference;
		argv[argc++] = default_publisher();
		argv[argc++] = default_operator();
		argv[argc++] = default_tls();
		argv[argc] = (char *) 0;
		if (!( result = evaluate_cordscript( algorithm, argc, argv )))
			return( 0 );
		else if (!( pptr->solution = allocate_string( result ) ))
			return( 0 );
		else	return( 0 );
	}
}

/*	------------------------------------------------------------	*/
/*	     a l g o r i t h m _ b a s e d _ p l a c e m e n t		*/
/*	------------------------------------------------------------	*/
/*	this placement makes use of the scripted algorithm placement	*/
/*	------------------------------------------------------------	*/
private	int	use_scripted_algorithm=1;
private	int	algorithm_based_placement(
		struct occi_category * optr, 
		struct cords_placement * pptr,
		char * agent,
		char * tls )
{
	struct	occi_element  * header;
	struct	occi_element  * eptr;
	struct	occi_response * yptr;
	struct	occi_response * uptr;
	char *	tptr=(char *) 0;
	char *	vptr=(char *) 0;

	if (!( use_scripted_algorithm ))
		return( 55 );
	else if (!( rest_valid_string( pptr->algorithm ) ))
		return( 118 );
	if (!( yptr = cords_retrieve_named_instance_list( _CORDS_ALGORITHM, "occi.algorithm.name", pptr->algorithm, agent,tls ) ))
		return( 404 );
	else if (!( uptr = cords_retrieve_named_instance( yptr, agent, tls )))
	{
		yptr = occi_remove_response( yptr );
		return( 404 );
	}
	else
	{
		for (	eptr = uptr->first;
			eptr != (struct occi_element *) 0;
			eptr = eptr->next )
		{
			if ( strcmp( eptr->name, "occi.algorithm.expression"  ) != 0)
				continue;
			else
			{
				evaluate_algorithm_script( pptr, eptr->value );
				yptr = occi_remove_response( yptr );
				uptr = occi_remove_response( uptr );
				return( 0 );
			}
		}
		return( 48 );
	}
}

/*	--------------------------------------------------	*/
/*	c r e a t e _ p l a c e m e n t _ s o l u t i o n	*/
/*	--------------------------------------------------	*/
/*	this will analyse the placement request and choose	*/
/*	an appropriate placement based on the node, zone &	*/
/*	provider information.					*/
/*	--------------------------------------------------	*/
private	int	create_placement_solution(
		struct occi_category * optr, 
		struct cords_placement * pptr,
		char * agent,
		char * tls )
{
	if ( pptr->solution )
	{
		liberate( pptr->solution );
		pptr->solution = (char *) 0;
	}

	/* -------------------------------------------------- */
	/* ensure at least the default algorithm is specified */
	/* -------------------------------------------------- */
	if (!( pptr->algorithm ))
		pptr->algorithm = allocate_string( "default" );

	else if (!( rest_valid_string( pptr->algorithm ) ))
	{
		pptr->algorithm = liberate( pptr->algorithm );
		if (!( pptr->algorithm = allocate_string( "default" ) ))
			return( 27 );
	}

	/* -------------------------------------------------- */
	/* select by  indicated placement algorithm operation */
	/* -------------------------------------------------- */
	if (!( strncmp( pptr->algorithm, "quota", strlen( "quota" ) ) ))
		return( quota_based_placement( optr, pptr, agent, tls ) );
	else if (!( algorithm_based_placement( optr, pptr, agent, tls ) ))
		return( 0 );
	else if (!( strcmp( pptr->algorithm, "default" ) ))
		return( default_placement( optr, pptr, agent, tls ) );
	else if (!( strcmp( pptr->algorithm, "price" ) ))
		return( price_based_placement( optr, pptr, agent, tls ) );
	else if (!( strcmp( pptr->algorithm, "zone" ) ))
		return( zone_based_placement( optr, pptr, agent, tls ) );
	else if (!( strcmp( pptr->algorithm, "energy" ) ))
		return( energy_based_placement( optr, pptr, agent, tls ) );
	else if (!( strcmp( pptr->algorithm, "security" ) ))
		return( security_based_placement( optr, pptr, agent, tls ) );
	else if (!( strcmp( pptr->algorithm, "operator" ) ))
		return( operator_based_placement( optr, pptr, agent, tls ) );
	else if (!( strcmp( pptr->algorithm, "scoring" ) ))
		return( score_based_placement( optr, pptr, agent, tls ) );
	else	return( 78 );
}

/*	-----------------------------------------------------------	*/
/*		i n v o k e _ p l a c e m e n t _ a c t i o n		*/
/*	-----------------------------------------------------------	*/
private	int	invoke_placement_action( struct cords_placement * pptr, char * action )
{
	const struct cords_xlink *lptr;
	struct	occi_response * zptr;

	if (!( pptr ))
		return(0);
	else if (!( rest_valid_string( pptr->id ) ))
		return( 0 );

	for (lptr = initialise_and_get_first_link(pptr->id); NULL != lptr; lptr = next_link()) {
		if (!( zptr = cords_invoke_action( lptr->target, action, _CORDS_CONTRACT_AGENT, default_tls() )))
			continue;
		else	zptr = occi_remove_response( zptr );
	}
	return(0);
}

	
/*	-------------------------------------------	*/
/* 	    c o n s u m e  _ p l a c e m e n t 		*/
/*	-------------------------------------------	*/
/*	the placement has been provisioned and is	*/
/*	now active and deployed.			*/
/*	-------------------------------------------	*/
private	struct rest_response * consume_placement(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_placement * pptr;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Incorrect Message Category" ) );
	else if ( pptr->state != 1 )
		return( rest_html_response( aptr, 200, "OK" ) );
	else 
	{
		pptr->state=2;
		autosave_cords_placement_nodes();
		invoke_placement_action( pptr, "consume" );
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	    r e s t o r e  _ p l a c e m e n t 		*/
/*	-------------------------------------------	*/
/*	the placement is reserved but no longer is	*/
/*	provisioned and deployed.			*/
/*	-------------------------------------------	*/
private	struct rest_response * restore_placement(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_placement * pptr;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Incorrect Message Category" ) );
	else if ( pptr->state != 2 )
		return( rest_html_response( aptr, 200, "OK" ) );
	else 
	{
		pptr->state=1;
		autosave_cords_placement_nodes();
		invoke_placement_action( pptr, "restore" );
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}
	
/*	-------------------------------------------	*/
/* 	    r e l e a s e  _ p l a c e m e n t 		*/
/*	-------------------------------------------	*/
/*	the placement is no longer reserved		*/
/*	-------------------------------------------	*/
private	struct rest_response * release_placement(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_placement * pptr;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Incorrect Message Category" ) );
	else if (!( pptr->state ))
		return( rest_html_response( aptr, 200, "OK" ) );
	else 
	{
		pptr->state=0;
		autosave_cords_placement_nodes();
		invoke_placement_action( pptr, "release" );
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}
	
/*	-------------------------------------------	*/
/* 	      c h o o s e  _ p l a c e m e n t 		*/
/*	-------------------------------------------	*/
private	struct rest_response * choose_placement(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_placement * pptr;
	int	status;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Incorrect Message Category" ) );
	else if ( pptr->state > 0 )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if ((status = create_placement_solution(optr, pptr, _CORDS_CONTRACT_AGENT, default_tls() )) != 0)
		return( rest_html_response( aptr, status, "PLACEMENT FAILURE" ) );
	else 
	{
		pptr->state = 1;
		autosave_cords_placement_nodes();
		invoke_placement_action( pptr, "reserve" );
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-----------------------------------------------------------	*/
/* 	      d e l e t e _ p l a c e m e n t _ q u a n t i t i e s	*/
/*	-----------------------------------------------------------	*/
private	int	delete_placement_quantities( struct cords_placement * pptr )
{
	struct	occi_response 	* zptr;
	const struct cords_xlink	* lptr;
	char 			  buffer[2048];
	/* ----------------------------------------------------- */
	/* for all defined contract nodes of the current service */
	/* ----------------------------------------------------- */
	buffer[0] = 0;
	for (lptr = initialise_and_get_first_link(pptr->id); NULL != lptr; lptr = next_link()) {
        if (!( buffer[0] ))
            strcpy( buffer, lptr->source );
        if ((zptr = occi_simple_delete( lptr->target, _CORDS_SERVICE_AGENT, default_tls())) != (struct occi_response *) 0)
            zptr = occi_remove_response ( zptr );
	}

	/* ----------------------------- */
	/* delete links from this object */
	/* ----------------------------- */
	if ( strlen( buffer ) )
		if ((zptr = occi_delete_links( buffer, _CORDS_SERVICE_AGENT, default_tls())) != (struct occi_response *) 0)
				zptr = occi_remove_response ( zptr );

	return(0);
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ p l a c e m e n t  		*/
/*	-------------------------------------------	*/
private	int	delete_placement(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	cords_placement * pptr;
	pptr = vptr;
    return(delete_placement_quantities(pptr));
}

private	struct	occi_interface	placement_interface = 
{
    NULL,
    NULL,
    NULL,
	delete_placement
};

	/* ---------------- */
#endif	/* _copsplacement_c */
	/* ---------------- */



