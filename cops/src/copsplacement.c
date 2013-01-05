#ifndef	_copsplacement_c
#define	_copsplacement_c

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
/*	this placement makes use of the new provider quota algorithm
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
	char *	wptr;
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	struct	occi_response * zptr;

	if (!( pptr ))
		return(0);
	else if (!( rest_valid_string( pptr->id ) ))
		return( 0 );

	for (	nptr=occi_first_link_node();
		nptr != (struct occi_link_node *) 0;
		nptr = nptr->next )
	{
		if (!( lptr = nptr->contents ))
			continue;
		else if (!( lptr->source ))
			continue;
		else if (!( lptr->target ))
			continue;
		else if (!( wptr = occi_category_id( lptr->source ) ))
			continue;
		else if ( strcmp( wptr, pptr->id ) != 0)
		{
			liberate( wptr );
			continue;
		}
		else	liberate( wptr );

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

/*	-------------------------------------------	*/
/* 	      c r e a t e _ p l a c e m e n t  		*/
/*	-------------------------------------------	*/
private	int	create_placement(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_placement * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( pptr->node ))
		return( 0 ); 
	else	return( 0 );
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ p l a c e m e n t  	*/
/*	-------------------------------------------	*/
private	int	retrieve_placement(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_placement * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ p l a c e m e n t 	 	*/
/*	-------------------------------------------	*/
private	int	update_placement(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_placement * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-----------------------------------------------------------	*/
/* 	      d e l e t e _ p l a c e m e n t _ q u a n t i t i e s	*/
/*	-----------------------------------------------------------	*/
private	int	delete_placement_quantities( struct cords_placement * pptr )
{
	struct	occi_response 	* zptr;
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	char 			  buffer[2048];
	char 			* wptr;
	/* ----------------------------------------------------- */
	/* for all defined contract nodes of the current service */
	/* ----------------------------------------------------- */
	buffer[0] = 0;
	for (	nptr=occi_first_link_node();
		nptr != (struct occi_link_node *) 0;
		nptr = nptr->next )
	{
		if (!( lptr = nptr->contents ))
			continue;
		else if (!( lptr->source ))
			continue;
		else if (!( lptr->target ))
			continue;
		else if (!( wptr = occi_category_id( lptr->source ) ))
			continue;
		else if ( strcmp( wptr, pptr->id ) != 0)
		{
			liberate( wptr );
			continue;
		}
		else
		{
			liberate( wptr );
			if (!( buffer[0] ))
				strcpy( buffer, lptr->source );
			if ((zptr = occi_simple_delete( lptr->target, _CORDS_SERVICE_AGENT, default_tls())) != (struct occi_response *) 0)
				zptr = occi_remove_response ( zptr );
		}
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
	struct	occi_kind_node * nptr;
	struct	cords_placement * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(delete_placement_quantities(pptr));
}

private	struct	occi_interface	placement_interface = 
{
	create_placement,
	retrieve_placement,
	update_placement,
	delete_placement
};

	/* ---------------- */
#endif	/* _copsplacement_c */
	/* ---------------- */



