#ifndef	_orga_trans_c
#define	_orga_trans_c

/* 	--------------------------------------		*/
/*	   b u i l d _ t r a n s a c t i o n		*/
/* 	--------------------------------------		*/
/*	struct  orga_transaction
	{
	        char *  id;
	        char *  type;
	        char *  account;
	        char *  label;
	        char *  value;
	        char *  currency;
	}						*/
/*	struct  cords_transaction
	{
	        char *  id;
	        char *  description;
	        char *  reference;
	        int     when;
	        int     validity;
	        char *  source;
	        int     state;
	        int     type;
	        char *  account;
	        char *  price;
	        char *  authorization;
	}						*/
/*	cords_price
	{
	       	char *  id;
        	char *  name;
        	int     state;
        	char *  period;
        	char *  currency;
        	char *  rate;
        	char *  units;
        	char *  fixed;
        	char *  description;
        	char *  operator;
	}						*/
/* 	--------------------------------------		*/

private	int	build_transaction( struct cords_transaction * pptr, struct orga_transaction * tptr )
{
	struct	occi_response * zptr;
	struct	occi_element * eptr;

	if (!( rest_valid_string( pptr->account ) ))
		return(0);
	else
	{

		if ((zptr = occi_simple_get( pptr->account, get_default_agent(), default_tls() )) != (struct occi_response *) 0)
		{
			if ((eptr = occi_locate_element( zptr->first, "occi.core.id" )) != (struct occi_element *) 0)
				tptr->account = allocate_string( eptr->value );
			zptr = occi_remove_response( zptr );
		}
	}

	if ( rest_valid_string( pptr->price ) )
	{
		if ((zptr = occi_simple_get( pptr->price, get_default_agent(), default_tls() )) != (struct occi_response *) 0)
		{
			if ((eptr = occi_locate_element( zptr->first, "occi.price.currency" )) != (struct occi_element *) 0)
				tptr->currency = allocate_string( eptr->value );
			if ((eptr = occi_locate_element( zptr->first, "occi.price.rate" )) != (struct occi_element *) 0)
				tptr->value = allocate_string( eptr->value );
			if ((eptr = occi_locate_element( zptr->first, "occi.price.description" )) != (struct occi_element *) 0)
				tptr->label = allocate_string( eptr->value );
			zptr = occi_remove_response( zptr );
		}
	}

	return( 1 );
}

/* 	--------------------------------------		*/
/*	 r e le a s e _ t r a n s a c t i o n		*/
/* 	--------------------------------------		*/
private	int	release_transaction( struct orga_transaction * tptr )
{
	if ( tptr )
	{
		if ( tptr->value )
			tptr->value = liberate( tptr->value );
		if ( tptr->account )
			tptr->account = liberate( tptr->account );
		if ( tptr->label )
			tptr->label = liberate( tptr->label );
		if ( tptr->currency )
			tptr->currency = liberate( tptr->currency );
	}
	return(0);
}

/*	-------------------------------------------	*/
/* 	      c r e a t e _ t r a n s a c t i o n  	*/
/*	-------------------------------------------	*/
private	int	create_transaction(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	rest_response * aptr;
	struct	orga_subscription * sptr;
	struct	occi_kind_node * nptr;
	struct cords_transaction * pptr;
	struct	orga_transaction Transaction;
	memset( &Transaction, 0, sizeof( struct orga_transaction ));
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( sptr = check_orga_subscription(_ORGA_CLIENT_CONFIG) ))
		return( 0 );
	else if (!( build_transaction( pptr, &Transaction ) ))
		return( 0 );
	else if (!( aptr = orga_create_transaction( sptr, &Transaction ) ))
	{
		release_transaction( &Transaction );
		return( 0 );
	}
	else
	{
		release_transaction( &Transaction );
		aptr = rest_liberate_response( aptr );
		return( 0 );
	}
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ t r a n s a c t i o n  	*/
/*	-------------------------------------------	*/
private	int	retrieve_transaction(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct cords_transaction * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ t r a n s a c t i o n  	*/
/*	-------------------------------------------	*/
private	int	update_transaction(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	rest_response * aptr;
	struct	orga_subscription * sptr;
	struct	occi_kind_node * nptr;
	struct cords_transaction * pptr;
	struct	orga_transaction Transaction;
	memset( &Transaction, 0, sizeof( struct orga_transaction ));
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( sptr = check_orga_subscription(_ORGA_CLIENT_CONFIG) ))
		return( 0 );
	else if (!( build_transaction( pptr, &Transaction ) ))
		return( 0 );
	else if (!( aptr = orga_update_transaction( sptr, &Transaction ) ))
	{
		release_transaction( &Transaction );
		return( 0 );
	}
	else
	{
		release_transaction( &Transaction );
		aptr = rest_liberate_response( aptr );
		return( 0 );
	}
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ t r a n s a c t i o n	*/
/*	-------------------------------------------	*/
private	int	delete_transaction(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	rest_response * aptr;
	struct	orga_subscription * sptr;
	struct	occi_kind_node * nptr;
	struct cords_transaction * pptr;
	struct	orga_transaction Transaction;
	memset( &Transaction, 0, sizeof( struct orga_transaction ));
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( sptr = check_orga_subscription(_ORGA_CLIENT_CONFIG) ))
		return( 0 );
	else if (!( build_transaction( pptr, &Transaction ) ))
		return( 0 );
	else if (!( aptr = orga_delete_transaction( sptr, &Transaction ) ))
	{
		release_transaction( &Transaction );
		return( 0 );
	}
	else
	{
		release_transaction( &Transaction );
		aptr = rest_liberate_response( aptr );
		return( 0 );
	}
}

private	struct	occi_interface	transaction_interface = {
	create_transaction,
	retrieve_transaction,
	update_transaction,
	delete_transaction
	};

	/* ------------- */
#endif	/* _orga_trans_c */
	/* ------------- */


