
#ifndef	_orgaccount_c
#define	_orgaccount_c

private	char *	resolve_account_nature( struct cords_account * pptr )
{
	char *	operator;

	if (!( operator = default_operator() ))
		return((char *) 0 );
	else if (!( pptr->name ))
		return( (char * ) 0 );
	else if (!( strcmp( pptr->name, operator ) ))
		return( "Provider" );
	else	return( "Customer" );
}

/*	-------------------------------------------	*/
/* 	      c r e a t e _ a c c o u n t  		*/
/*	-------------------------------------------	*/
private	int	create_account(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	rest_response * aptr;
	struct	orga_subscription * sptr;
	struct	occi_kind_node * nptr;
	struct cords_account * pptr;
	char *	nature;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( sptr = check_orga_subscription(_ORGA_CLIENT_CONFIG,resolve_provider_account()) ))
		return( 0 );
	else if (!( nature = resolve_account_nature( pptr ) ))
		return( 0 );
	else if (!( aptr = orga_create_user( sptr, pptr, nature ) ))
		return( 0 );
	else
	{
		aptr = rest_liberate_response( aptr );
		return( 0 );
	}
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ a c c o u n t  		*/
/*	-------------------------------------------	*/
private	int	retrieve_account(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	orga_subscription * sptr;
	struct	occi_kind_node * nptr;
	struct cords_account * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( sptr = check_orga_subscription(_ORGA_CLIENT_CONFIG,resolve_provider_account()) ))
		return( 0 );
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ a c c o u n t  		*/
/*	-------------------------------------------	*/
private	int	update_account(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	orga_subscription * sptr;
	struct	occi_kind_node * nptr;
	struct cords_account * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( sptr = check_orga_subscription(_ORGA_CLIENT_CONFIG,resolve_provider_account()) ))
		return( 0 );
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ a c c o u n t		*/
/*	-------------------------------------------	*/
private	int	delete_account(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	rest_response * aptr;
	struct	orga_subscription * sptr;
	struct	occi_kind_node * nptr;
	struct cords_account * pptr;
	char *	nature;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( sptr = check_orga_subscription(_ORGA_CLIENT_CONFIG,resolve_provider_account()) ))
		return( 0 );
	else if (!( nature = resolve_account_nature( pptr ) ))
		return( 0 );
	else if (!( aptr = orga_delete_user( sptr, pptr, nature ) ))
		return( 0 );
	else
	{
		aptr = rest_liberate_response( aptr );
		return( 0 );
	}
}

private	struct	occi_interface	account_interface = {
	create_account,
	retrieve_account,
	update_account,
	delete_account
	};

	/* ------------- */
#endif	/* _orgaccount_c */
	/* ------------- */


