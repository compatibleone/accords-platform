#ifndef	_comonsmonitor_c
#define	_comonsmonitor_c

/*	-------------------------------------------	*/
/* 		s t a r t _ m o n i t o r		*/
/*	-------------------------------------------	*/
private	struct rest_response * start_monitor(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_monitor * pptr;
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
				if (!(zptr = cords_invoke_action( lptr->target, _CORDS_START, _CORDS_SERVICE_AGENT, default_tls() )))
					return( rest_html_response( aptr, 801, "Control Start Failure" ) );
				else 	zptr = occi_remove_response( zptr );
			}
		}
		pptr->state = 1;
		autosave_cords_monitor_nodes();
		return( rest_html_response( aptr, 200, "OK" ) );
	}		
}

/*	-------------------------------------------	*/
/* 		s t o p _ m o n i t o r			*/
/*	-------------------------------------------	*/
private	struct rest_response * stop_monitor(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	int	status=0;
	struct	cords_monitor * pptr;
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
		buffer[0] = 0;
		for (	nptr=occi_last_link_node();
			nptr != (struct occi_link_node *) 0;
			nptr = nptr->previous )
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
				strcpy(buffer,lptr->source);
				liberate( wptr );
				if (!(zptr = cords_invoke_action( lptr->target, _CORDS_STOP, _CORDS_SERVICE_AGENT, default_tls() )))
					return( rest_html_response( aptr, 801, "Control Stop Failure" ) );
				else	zptr = occi_remove_response( zptr );
			}
		}
		pptr->state = 0;
		autosave_cords_monitor_nodes();
		if ( strlen(buffer) )
			if ((zptr = occi_delete_links( buffer, _CORDS_SERVICE_AGENT, default_tls())) != (struct occi_response *) 0)
				zptr = occi_remove_response( zptr );
		return( rest_html_response( aptr, 200, "OK" ) );
	}		
}

	/* ---------------- */
#endif	/* _comonsmonitor_c */
	/* ---------------- */


