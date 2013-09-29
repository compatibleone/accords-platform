#ifndef	_corc_asynch_c
#define	_corc_asynch_c

struct	corcs_asynch_request
{
	struct	corcs_asynch_request * previous;
	struct	corcs_asynch_request * next;
	struct	rest_response * response;
	char *	identity;
	char *	message;
	char *	action;
	int	status;
};

struct	corcs_asynch_context
{
	struct rest_response * (*method)( 
		struct rest_response * aptr, 
		struct xml_element * sptr, 
		struct rest_request * rptr,
		struct corcs_asynch_request * qptr );

	struct	corcs_asynch_request * result;
	struct	rest_request  * request;
	struct	rest_response * response;
	struct xml_element    * message;
};

private	struct	corcs_asynch_request * AsynchRoot=(struct corcs_asynch_request *) 0;
private	struct	corcs_asynch_request * AsynchFoot=(struct corcs_asynch_request *) 0;

/*	-------------------------------------------------	*/
/*	d r o p _ c o r c s _ a s y n c h _ r e q u e s t	*/
/*	-------------------------------------------------	*/
private	struct	corcs_asynch_request * drop_corcs_asynch_request( struct corcs_asynch_request * qptr )
{
	if ( qptr )
	{
		if ( qptr->action )
			qptr->action = liberate( qptr->action );
		if ( qptr->message )
			qptr->message = liberate( qptr->message );
		liberate( qptr );
	}
	return((struct corcs_asynch_request *) 0);
}

/*	-------------------------------------------------	*/
/*	 a d d _ c o r c s _ a s y n c h _ r e q u e s t	*/
/*	-------------------------------------------------	*/
private	struct	corcs_asynch_request * add_corcs_asynch_request( char * action )
{
	struct	corcs_asynch_request * qptr;
	if (!( qptr = allocate( sizeof( struct corcs_asynch_request ) ) ))
		return( qptr );
	else 	memset( qptr, 0, sizeof( struct corcs_asynch_request ));

	if (!( qptr->previous = AsynchFoot ))
		AsynchRoot = qptr;
	else	qptr->previous->next = qptr;
	AsynchFoot = qptr;

	if (!( qptr->identity = rest_allocate_uuid() ))
		return( drop_corcs_asynch_request( qptr ) );
	else if (!( action ))
		return( qptr );
	else if (!( qptr->action = allocate_string( action ) ))
		return( drop_corcs_asynch_request( qptr ) );
	else	return( qptr );
}

/*	-------------------------------------------------	*/
/*	f i n d _ c o r c s _ a s y n c h _ r e q u e s t	*/
/*	-------------------------------------------------	*/
private	struct	corcs_asynch_request * find_corcs_asynch_request( char * id )
{
	struct	corcs_asynch_request * qptr;
	for (	qptr=AsynchRoot;
		qptr != (struct corcs_asynch_request *) 0;
		qptr = qptr->next )
	{
		if (!( qptr->identity ))
			continue;
		else if (!( strcmp( qptr->identity, id ) ))
			break;
		else	continue;
	}
	return( qptr );
}

/*	-------------------------------------------------	*/
/*	 a d d _ c o r c s _ a s y n c h _ r e q u e s t	*/
/*	-------------------------------------------------	*/
private	struct	corcs_asynch_context * allocate_asynch_context( struct corcs_asynch_request * rptr )
{
	struct	corcs_asynch_context * qptr;
	if (!( qptr = allocate( sizeof( struct corcs_asynch_context ) ) ))
		return( qptr );
	else 
	{
		memset( qptr, 0, sizeof( struct corcs_asynch_context ));
		qptr->result = rptr;
		return( qptr );
	}
}


	/* -------------- */
#endif	/* _corc_asynch_c */
	/* -------------- */

