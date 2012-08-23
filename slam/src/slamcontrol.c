#ifndef	_slamcontrol_c
#define	_slamcontrol_c

/*	-------------------------------------------	*/
/* 		s t a r t _ c o n t r o l		*/
/*	-------------------------------------------	*/
private	struct rest_response * start_control(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_control * pptr;
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	struct	occi_response * zptr;
	struct	occi_element  * eptr;
	char *	wptr;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Failure" ) );
	else if ( pptr->state )
		return( rest_html_response( aptr, 200, "OK" ) );
	else
	{
		pptr->state = 1;
		autosave_cords_control_nodes();
		return( rest_html_response( aptr, 200, "OK" ) );
	}		
}

/*	-------------------------------------------	*/
/* 		s t o p _ c o n t r o l			*/
/*	-------------------------------------------	*/
private	struct rest_response * stop_control(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	int	status=0;
	struct	cords_control * pptr;
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	struct	occi_response * zptr;
	struct	occi_element  * eptr;
	char *	wptr;
	char	buffer[2048];
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Failure" ) );
	else if (!( pptr->state ))
		return( rest_html_response( aptr, 200, "OK" ) );
	else
	{
		if ( pptr->probe )
			pptr->probe = liberate( pptr->probe );
		if ( pptr->session )
			pptr->session = liberate( pptr->session );
		if ( pptr->monitor )
			pptr->monitor = liberate( pptr->monitor );
		if ( pptr->connection )
			pptr->connection = liberate( pptr->connection );

		pptr->state = 0;
		autosave_cords_control_nodes();
		return( rest_html_response( aptr, 200, "OK" ) );
	}		
}

	/* -------------- */
#endif	/* _slamcontrol_c */
	/* -------------- */

