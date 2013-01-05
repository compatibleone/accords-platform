#ifndef	_brokerprovider_c
#define	_brokerprovider_c


/*	-------------------------------------------	*/
/* 	      c r e a t e _ p r o v i d e r  		*/
/*	-------------------------------------------	*/
private	int	create_provider(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_provider * pptr;
	char	buffer[256];
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	
	if (!( rest_valid_string( pptr->opinion )))
	{

		sprintf(buffer,"%u",(time((long *)0) % 10)+1);
		pptr->opinion = allocate_string( buffer );
	}

	if (!( rest_valid_string( pptr->zone ) ))
		pptr->zone = allocate_string("europe");

	if (!( rest_valid_string( pptr->security ) ))
		pptr->security = allocate_string("public");

	if (!( rest_valid_string( pptr->operator ) ))
		pptr->operator = allocate_string("any");

	return(0);
	
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ p r o v i d e r  		*/
/*	-------------------------------------------	*/
private	int	retrieve_provider(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_provider * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ p r o v i d e r  		*/
/*	-------------------------------------------	*/
private	int	update_provider(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_provider * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ p r o v i d e r	  	*/
/*	-------------------------------------------	*/
private	int	delete_provider(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_provider * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

private	struct	occi_interface	provider_interface = {
	create_provider,
	retrieve_provider,
	update_provider,
	delete_provider
	};


/*	------------------------------------------------------------------	*/
/*		b r o k e r _ p r o v i d e r _ b u i l d e r			*/
/*	------------------------------------------------------------------	*/
private	struct	occi_category *	broker_provider_builder( char * domain, char * category )
{
	struct	occi_category * optr;
	initialise_occi_resolver( _DEFAULT_PUBLISHER, (char *) 0, (char *) 0, (char *) 0 );
	if (!( optr = occi_cords_provider_builder( domain ,category ) ))
		return( optr );
	else
	{
		optr->callback  = &provider_interface;
		return( optr );
	}
}

	/* ----------------- */
#endif	/* _brokerprovider_c */
	/* ----------------- */

