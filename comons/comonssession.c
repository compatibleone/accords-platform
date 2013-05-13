#ifndef	_comonssession_c
#define	_comonssession_c

#include "cp.h"

/*	-------------------------------------------	*/
/* 		s t a r t _ s e s s i o n		*/
/*	-------------------------------------------	*/
private	struct rest_response * start_session(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_session * pptr;
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
		for (	pptr->connections=0,
			nptr=occi_first_link_node();
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
				if (!(zptr = cords_invoke_action( lptr->target, "start", _CORDS_SERVICE_AGENT, default_tls() )))
					return( rest_html_response( aptr, 801, "Connection Start Failure" ) );
				else if ( cords_check_invocation( zptr, aptr ) != 0 )
				{
					zptr = occi_remove_response( zptr );
					return( rest_html_response( aptr, aptr->status, aptr->message ) );
				}
				else
				{
					zptr = occi_remove_response( zptr );
					pptr->connections++;
				}
			}
		}
		pptr->state = 1;
		autosave_cords_session_nodes();
		return( rest_html_response( aptr, 200, "OK" ) );
	}		
}

/*	-------------------------------------------	*/
/* 		s t o p _ s e s s i o n			*/
/*	-------------------------------------------	*/
private	struct rest_response * stop_session(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_session * pptr;
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	struct	occi_response * zptr;
	struct	occi_element  * eptr;
	char *	wptr;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Failure" ) );
	else if (!( pptr->state ))
		return( rest_html_response( aptr, 200, "OK" ) );
	else
	{
		for (	pptr->connections=0,
			nptr=occi_last_link_node();
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
				liberate( wptr );
				if (!(zptr = cords_invoke_action( lptr->target, "stop", _CORDS_SERVICE_AGENT, default_tls() )))
					return( rest_html_response( aptr, 801, "Connection Stop Failure" ) );
				else if ( cords_check_invocation( zptr, aptr ) != 0 )
				{
					zptr = occi_remove_response( zptr );
					return( rest_html_response( aptr, aptr->status, aptr->message ) );
				}
				else
				{
					zptr = occi_remove_response( zptr );
					pptr->connections++;
				}
			}
		}
		pptr->state = 0;
		autosave_cords_session_nodes();
		return( rest_html_response( aptr, 200, "OK" ) );
	}		
}

/*	-------------------------------------------	*/
/* 	      c r e a t e _ c o n t r a c t  		*/
/*	-------------------------------------------	*/
private	int	create_session(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_session * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return( 0 ); 
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ c o n t r a c t  		*/
/*	-------------------------------------------	*/
private	int	retrieve_session(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_session * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ c o n t r a c t  		*/
/*	-------------------------------------------	*/
private	int	update_session(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_session * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ c o n t r a c t	  	*/
/*	-------------------------------------------	*/
private	int	delete_monitoring_session(struct occi_category * optr,struct cords_session * pptr)
{
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	struct	occi_response * zptr;
	struct	occi_element  * eptr;
	char 	buffer[2048];
	char *	wptr;
	buffer[0] = 0;
	for (	pptr->connections=0,
		nptr=occi_last_link_node();
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
			liberate( wptr );
			if (!( buffer[0] ))
				strcpy(buffer,lptr->source);
			if ((zptr = occi_simple_delete( lptr->target, _CORDS_CONTRACT_AGENT, default_tls() )) != (struct occi_response *) 0)
				zptr = occi_remove_response( zptr );
		}
	}
	if ( strlen(buffer) )
		if ((zptr = occi_delete_links( buffer, _CORDS_CONTRACT_AGENT, default_tls() )) != (struct occi_response *) 0)
			zptr = occi_remove_response( zptr );
	return(0);
}

private	int	delete_session(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_session * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(delete_monitoring_session(optr, pptr));
}

private	struct	occi_interface	session_interface = 
{
	create_session,
	retrieve_session,
	update_session,
	delete_session
};


/*	------------------------------------------------------------------	*/
/*			c o m o n s _ s e s s i o n _ b u i l d e r		*/
/*	------------------------------------------------------------------	*/
public	struct occi_category * comons_session_builder( char * domain )
{
	struct	occi_category * optr;

	if (!( optr = occi_cords_session_builder( domain, "session" ) ))
		return( optr );
	else
	{
		optr->callback = &session_interface;
		optr->access |= _OCCI_NO_PRICING;

		if (!( optr = occi_add_action( optr,"start","",start_session)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"stop","",stop_session)))
			return( optr );
		else	return( optr );
	}
}

	/* ---------------- */
#endif	/* _comonssession_c */
	/* ---------------- */

