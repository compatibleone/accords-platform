#ifndef	_procci_instruction_c
#define	_procci_instruction_c

/*	-------------------------------------------	*/
/* 	     c r e a t e _ in s t r u c t i o n  	*/
/*	-------------------------------------------	*/
private	int	create_instruction(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_instruction * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	   r e t r i e v e _ in s t r u c t i o n  	*/
/*	-------------------------------------------	*/
private	int	retrieve_instruction(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_instruction * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	    u p d a t e _ in s t r u c t i o n  	*/
/*	-------------------------------------------	*/
private	int	update_instruction(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_instruction * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	     d e l e t e _ in s t r u c t i o n	  	*/
/*	-------------------------------------------	*/
private	int	delete_instruction(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node 		* nptr;
	struct	cords_instruction 	* pptr;
	struct occi_response 		* zptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( rest_valid_string( pptr->value ) ))
		return( 0 );
	else if ( strcmp( pptr->type, "method" ) != 0 )
		return( 0 );
	else if ( strcmp( pptr->method, _CORDS_MONITOR ) != 0 )
		return( 0 );
	else
	{
		if ((zptr = occi_simple_delete( pptr->value, _CORDS_CONTRACT_AGENT, default_tls() )) != (struct occi_response *) 0)
			zptr = occi_remove_response( zptr );
		pptr->value = liberate( pptr->value );
		return( 0 );
	}
}

private	struct	occi_interface	instruction_interface = {
	create_instruction,
	retrieve_instruction,
	update_instruction,
	delete_instruction
	};

	/* --------------------- */
#endif	/* _procci_instruction_c */
	/* --------------------- */




