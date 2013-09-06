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
	const struct cords_xlink * lptr;
	struct	occi_response * zptr;
	struct	occi_element  * eptr;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Failure" ) );
	else if ( pptr->state )
		return( rest_html_response( aptr, 200, "OK" ) );
	else
	{
	    for (lptr = initialise_and_get_first_link(pptr->id); NULL != lptr; lptr = next_link(pptr->id)) {
            if (!(zptr = cords_invoke_action( lptr->target, _CORDS_START, _CORDS_SERVICE_AGENT, default_tls() )))
                return( rest_html_response( aptr, 801, "Control Start Failure" ) );
            else if ( cords_check_invocation( zptr, aptr ) != 0 )
            {
                zptr = occi_remove_response( zptr );
                return( rest_html_response( aptr, aptr->status, aptr->message ) );
            }
            else 	zptr = occi_remove_response( zptr );
		}
		pptr->state = 1;
		autosave_cords_monitor_nodes();
		return( rest_html_response( aptr, 200, "OK" ) );
	}		
}

/*	-------------------------------------------	*/
/* 	 s t o p _ m o n i t o r _ c o n t r o l s 	*/
/*	-------------------------------------------	*/
private	int	stop_monitor_controls( struct cords_monitor * pptr )
{
	int	status=0;
	const struct cords_xlink * lptr;
	struct	occi_response 	* zptr;
	struct	occi_element  	* eptr;
	char	buffer[2048];
	buffer[0] = 0;
	for (lptr = initialise_and_get_last_link(pptr->id); lptr != NULL; lptr = previous_link(pptr->id)) {
        strcpy(buffer,lptr->source);
        if (!(zptr = cords_invoke_action( lptr->target, _CORDS_STOP, _CORDS_SERVICE_AGENT, default_tls() )))
            break;
        else	zptr = occi_remove_response( zptr );
	}
	pptr->state = 0;
	autosave_cords_monitor_nodes();
	if ( strlen(buffer) )
		if ((zptr = occi_delete_links( buffer, _CORDS_SERVICE_AGENT, default_tls())) != (struct occi_response *) 0)
			zptr = occi_remove_response( zptr );
	return(0);
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
	struct	cords_monitor * pptr;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Failure" ) );
	else if (!( pptr->state ))
		return( rest_html_response( aptr, 200, "OK" ) );
	else
	{
		stop_monitor_controls(pptr);
		return( rest_html_response( aptr, 200, "OK" ) );
	}		
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ m o n i t o r	  	*/
/*	-------------------------------------------	*/
private	int	delete_monitor(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	cords_monitor * pptr;
	pptr = vptr;
	if (!( pptr->state ))
		return( 0 );
	else	return( stop_monitor_controls( pptr ) );
}

private	struct	occi_interface	monitor_interface = {
	NULL,
	NULL,
	NULL,
	delete_monitor
	};


	/* ---------------- */
#endif	/* _comonsmonitor_c */
	/* ---------------- */


