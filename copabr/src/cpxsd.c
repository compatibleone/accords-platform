#ifndef	_cpxsd_c
#define	_cpxsd_c

/*	---------------------------------------------------	*/
/*			x s d _ e l e m e n t 			*/
/*	---------------------------------------------------	*/
private	struct	xml_element *	xsd_element( struct xml_element * xsd, char * nptr )
{
	struct	xml_element * wptr;
	return((struct xml_element *) 0);
}

/*	---------------------------------------------------	*/
/*			x s d _ a t r i b u t 			*/
/*	---------------------------------------------------	*/
private	struct	xml_element *	xsd_atribut( struct xml_element * xsd, char * nptr )
{
	struct	xml_element * wptr;
	return((struct xml_element *) 0);
}

/*	---------------------------------------------------	*/
/*	      x s d _ v a l i d a t e _ e l e m e n t		*/
/*	---------------------------------------------------	*/
public	struct xml_element * xsd_validate_element( 
	struct xml_element * xsd,
	struct xml_element * eptr )
{
	struct	xml_atribut * aptr;
	struct	xml_element * wptr;
	struct	xml_element * bptr;
	if (!( xsd ))
		return( eptr );
	else if (!( eptr ))
		return( eptr );
	else if (!( wptr = xsd_element( xsd, eptr->name ) ))
		return( wptr );
	else
	{
		for (	aptr=eptr->firstatb;
			aptr != (struct xml_atribut *) 0;
			aptr = aptr->next )
		{
			if (!( bptr = xsd_atribut( wptr, aptr->name ) ))
				return((struct xml_element *) 0);
		}
		return( eptr );
	}
}

/*	---------------------------------------------------	*/
/*		d o c u m e n t _ p a r s e _ u r l		*/
/*	---------------------------------------------------	*/
public	struct xml_element * document_parse_url( char * url )
{
	struct	xml_element * document=(struct xml_element *) 0;
	struct	rest_header * hptr;
	struct	rest_response * rptr;
	if (!( rptr = rest_client_get_request( url, (char *) 0,"Xsd Client", (struct rest_header *) 0) ))
		return( document );
	else if ( rptr->status != 200 )
		return( document );
	else if (!( hptr = rest_resolve_header( rptr->first, _HTTP_CONTENT_LENGTH ) ))
		return( document );
	else if (!( hptr->value ))
		return( document );
	else if (!( atoi( hptr->value ) ))
		return( document );
	else if (!( rptr->body ))
		return( document );
	else	return((document = document_parse_file( rptr->body )));
}

/*	---------------------------------------------------	*/
/*		d o c u m e n t _ l o a d _ x s d		*/
/*	---------------------------------------------------	*/
public	struct xml_element * document_load_xsd( struct xml_element * document )
{
	struct	xml_element 	* xsd=(struct xml_element *) 0;
	struct	xml_atribut 	* aptr=(struct xml_atribut *) 0;
	char 			* sptr=(char *) 0;
	if (!( aptr = document_atribut( document,"xmlns" ) ))
	{
		if (!( sptr = allocate_string( _CORDS_NS ) ))
		{
			cords_append_error( document, 707,"namespace failure");
			return((struct xml_element *) 0 );
		}
	}
	else if (!( aptr->value ))
	{
		if (!( sptr = allocate_string( _CORDS_NS ) ))
		{
			cords_append_error( document, 708,"namespace failure");
			return((struct xml_element *) 0 );
		}
	}
	else if (!( sptr = occi_unquoted_value( aptr->value ) ))
	{
		cords_append_error( document, 709,"namespace failure");
		return((struct xml_element *) 0 );
	}

	if (!( xsd = document_parse_url( sptr ) ))
	{
		cords_append_error( document, 709,"namespace failure");
		liberate( sptr );
		return((struct xml_element *) 0 );
	}
	else
	{
		/* ------------------ */
		/* ok its been loaded */
		/* ------------------ */
		liberate( sptr );
		return( xsd );
	}
}

	/* -------- */
#endif	/* _cpxsd_c */
	/* -------- */

