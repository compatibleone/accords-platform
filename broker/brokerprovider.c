#ifndef	_brokerprovider_c
#define	_brokerprovider_c


/*	-------------------------------------------	*/
/* 	      c r e a t e _ p r o v i d e r  		*/
/*	-------------------------------------------	*/
private	int	create_provider(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	cords_provider * pptr;
	char	buffer[256];
	pptr = vptr;
	
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


private	struct	occi_interface	provider_interface = {
	create_provider,
	NULL,
	NULL,
	NULL
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

