#ifndef _commonsconnection_c 
#define _commonsconnection_c 

#include "cp.h"

/*	-------------------------------------------	*/
/* 		s t a r t _ c o n n e c t i o n		*/
/*	-------------------------------------------	*/
private	struct rest_response * start_connection(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_connection * pptr;
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
		for (	pptr->probes=0,
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
					return( rest_html_response( aptr, 801, "Probe Start Failure" ) );
				else
				{
					zptr = occi_remove_response( zptr );
					pptr->probes++;
				}
			}
		}
		pptr->state = 1;
		autosave_cords_connection_nodes();
		return( rest_html_response( aptr, 200, "OK" ) );
	}		
}

/*	-------------------------------------------	*/
/* 		s t o p _ c o n n e c t i o n		*/
/*	-------------------------------------------	*/
private	struct rest_response * stop_connection(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_connection * pptr;
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
		for (	pptr->probes=0,
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
				strcpy(buffer,lptr->source);
				liberate( wptr );
				if (!(zptr = cords_invoke_action( lptr->target, "stop", _CORDS_SERVICE_AGENT, default_tls() )))
					return( rest_html_response( aptr, 801, "Probe Stop Failure" ) );
				else
				{
					zptr = occi_remove_response( zptr );
					if (( occi_simple_delete( lptr->target, _CORDS_SERVICE_AGENT, default_tls() )) != (struct occi_response *) 0)
					{
						zptr = occi_remove_response( zptr );
						if ( pptr->probes )
							pptr->probes--;
					}
				}
			}
		}
		pptr->state = 0;
		autosave_cords_connection_nodes();
		if ((zptr = cords_delete_links( buffer, _CORDS_SERVICE_AGENT, default_tls())) != (struct occi_response *) 0)
			zptr = occi_remove_response( zptr );
		return( rest_html_response( aptr, 200, "OK" ) );
	}		
}

/*	------------------------------------------------------------------	*/
/*		c o m o n s _ c o n n e c t i o n _ b u i l d e r		*/
/*	------------------------------------------------------------------	*/
public	struct occi_category * comons_connection_builder( char * domain )
{
	struct	occi_category * optr;

	if (!( optr = occi_cords_connection_builder( domain, "connection" ) ))
		return( optr );
	else
	{
		optr->callback = (void *) 0;
		optr->access |= ( _OCCI_NO_PRICING | _OCCI_PRIVATE | _OCCI_CONSUMER );

		if (!( optr = occi_add_action( optr,"start","",start_connection)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"stop","",stop_connection)))
			return( optr );
		else	return( optr );
	}
}

/*	------------------------------------------------------------------	*/
/*			 o m o n s _ p a c k e t _ b u i l d e r		*/
/*	------------------------------------------------------------------	*/
public	struct occi_category * comons_packet_builder( char * domain, char * filename )
{
	struct	occi_category * optr;

	if (!( optr = occi_cords_packet_builder( domain, "packet" ) ))
		return( optr );
	else
	{
		set_autosave_cords_packet_name( filename );
		optr->callback = (void *) 0;
		optr->access |= ( _OCCI_NO_PRICING | _OCCI_PRIVATE );

		return( optr );
	}
}

	/* -------------------- */
#endif	/* _commonsconnection_c */
	/* -------------------- */

