#ifndef _commonsconnection_c 
#define _commonsconnection_c 

#include "cp.h"
#include "connection.h"
#include "packet.h"
#include "xlink.h"

#include <sys/types.h>
#include <sys/wait.h>

#include "link_backend.h"

private	int	purge_packets=0;

/*	---------------------------------------------------	*/
/*	    p u r g e _ c o n n e c t i o n _ p a c k e t s	*/
/*	---------------------------------------------------	*/
private	void	purge_connection_packets(struct cords_connection * pptr, char * packets, char * probe )
{
	struct	occi_client   * kptr;
	struct	occi_request  * qptr;
	struct	occi_response * yptr;
	struct	occi_response * zptr;
	struct	occi_element  * eptr;
	struct	occi_element  * dptr;
	char *	vptr;

	if (!( kptr = occi_create_client( packets, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return;
	else if (!( qptr = occi_create_request( kptr, kptr->target->object, _OCCI_NORMAL )))
	{
		kptr = occi_remove_client( kptr );
		return;
	}
	else if (!(dptr=occi_request_element(qptr,"occi.packet.probe", probe ) ))
	{
		kptr = occi_remove_client ( kptr );
		qptr = occi_remove_request( qptr );
		return;
	}
	else if (!( yptr = occi_client_get( kptr, qptr ) ))
	{
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return;
	}
	else
	{
		/* ----------------------------------- */
		/* retrieve all packets for this probe */
		/* ----------------------------------- */
		for (	eptr = yptr->first;
			eptr != (struct occi_element *) 0;
			eptr = eptr->next )
		{
			/* ------------------------ */
			/* check the response data  */
			/* ------------------------ */
			if (!( vptr = eptr->name ))
				continue;

			/* ------------------------ */
			/* simply delete the packet */
			/* ------------------------ */
			if ((zptr = occi_simple_delete( eptr->value, _CORDS_CONTRACT_AGENT, default_tls() )) != (struct occi_response *) 0)
				zptr = occi_remove_response( zptr );

			/* ------------------------ */
			/* - delete link from probe */
			/* ------------------------ */
			if ((zptr = occi_delete_link( probe, eptr->value, _CORDS_CONTRACT_AGENT, default_tls() )) != (struct occi_response *) 0)
				zptr = occi_remove_response( zptr );

		}
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return;
	}
}

/*	-------------------------------------------	*/
/* 	     c o n n e c t i o n _ w o r k e r		*/
/*	-------------------------------------------	*/
/*	the connection work is responsible for the 	*/
/*	consumption of the monitoring data packats	*/
/*	that are sent up from the probe inside the	*/
/*	cosacs enabled vm endpoint.			*/
/*	the standard connection work will simple	*/
/*	eat the received packets for now to ensure	*/
/*	that monitoring data does not bog down the	*/
/*	platform operation with a big data problem	*/
/*	-------------------------------------------	*/
private	int	connection_worker( struct cords_connection * pptr )
{
	int	inner=2;
	int	outer=5;
	const struct cords_xlink *lptr;
	char	packets[1024];
	sprintf(packets,"%s/%s/",get_identity(),_CORDS_PACKET);
	while (!( rest_server_signal()))
	{
		for (lptr = initialise_links_list(pptr->id); NULL != lptr; lptr = next_link(pptr->id)) {
            purge_connection_packets( pptr, packets, lptr->target );
            sleep(inner);
		}
		sleep(outer);
	}
	return(0);
}
	

/*	-------------------------------------------	*/
/*	   c o n s u m e _ m o n i t o r i n g		*/
/*	-------------------------------------------	*/
private	struct rest_response * consume_monitoring( struct cords_connection * pptr, struct rest_response * aptr )
{
	switch ((pptr->pid=fork()))
	{
	case	0  : 
		exit( connection_worker( pptr ) );
	case	-1 : 
		return( rest_html_response( aptr, 802, "Connection Start Failure" ) );
	default	   : 
		autosave_cords_connection_nodes();
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/*	   c a p t u r e _ m o n i t o r i n g		*/
/*	-------------------------------------------	*/
private	void	capture_monitoring( struct cords_connection * pptr )
{
	int	status;
	/* ------------------------------ */
	/* disactivate connection monitor */
	/* ------------------------------ */
	if ( pptr->pid )
	{
		kill( pptr->pid, SIGTERM );
		waitpid(pptr->pid,&status,0);
		pptr->id = 0;
	}
	return;
}

/*	-------------------------------------------	*/
/* 	   c a p t u r e _ c o n n e c t i o n		*/
/*	-------------------------------------------	*/
private	struct rest_response * capture_connection(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_connection * pptr;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Failure" ) );
	else if (!( pptr->state ))
		return( rest_html_response( aptr, 200, "OK" ) );
	else if (!( pptr->pid ))
		return( rest_html_response( aptr, 200, "OK" ) );
	else
	{
		capture_monitoring( pptr ) ;
		autosave_cords_connection_nodes();
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	    c o n s u m e _ c o n n e c t i o n		*/
/*	-------------------------------------------	*/
private	struct rest_response * consume_connection(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_connection * pptr;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Failure" ) );
	else if (!( pptr->state ))
		return( rest_html_response( aptr, 200, "OK" ) );
	else if ( pptr->pid )
		return( rest_html_response( aptr, 200, "OK" ) );
	else	return( consume_monitoring( pptr, aptr ) );
}

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
	const struct cords_xlink * lptr;
	struct	occi_response * zptr;
	struct	occi_element  * eptr;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Failure" ) );
	else if ( pptr->state )
		return( rest_html_response( aptr, 200, "OK" ) );
	else
	{
	    pptr->probes=0;
	    for (lptr = initialise_links_list(pptr->id); NULL != lptr; lptr = next_link(pptr->id)) {
            if (!(zptr = cords_invoke_action( lptr->target, _CORDS_START, _CORDS_SERVICE_AGENT, default_tls() )))
                return( rest_html_response( aptr, 801, "Probe Start Failure" ) );
            else if ( cords_check_invocation( zptr, aptr ) != 0 )
            {
                zptr = occi_remove_response( zptr );
                return( rest_html_response( aptr, aptr->status, aptr->message ) );
            }
            else
            {
                zptr = occi_remove_response( zptr );
                pptr->probes++;
            }
		}
		pptr->state = 1;
		autosave_cords_connection_nodes();
		if ( rest_valid_string( pptr->monitor ) )
		{
			if (!(zptr = cords_invoke_action( pptr->monitor, _CORDS_START, _CORDS_SERVICE_AGENT, default_tls() )))
				return( rest_html_response( aptr, 801, "Monitor Start Failure" ) );
			else if ( cords_check_invocation( zptr, aptr ) != 0 )
			{
				zptr = occi_remove_response( zptr );
				return( rest_html_response( aptr, aptr->status, aptr->message ) );
			}
			else
			{
				zptr = occi_remove_response( zptr );
				return( rest_html_response( aptr, 200, "OK" ) );
			}
		}
		else if (!( purge_packets ))
			return( rest_html_response( aptr, 200, "OK" ) );
		else	return( consume_monitoring( pptr, aptr ) );
	}		
}



/*	------------------------------------------------	*/
/* 	  s t o p _ c o n n e c t i o n _  p r o b e s 		*/
/*	------------------------------------------------	*/
private	int	stop_connection_probes( struct cords_connection * pptr )
{
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	struct	occi_response * zptr;
	struct	occi_element  * eptr;
	int	status;
	char *	wptr;
	char	buffer[2048];

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
			if (!(zptr = cords_invoke_action( lptr->target, _CORDS_STOP, _CORDS_SERVICE_AGENT, default_tls() )))
				return( 801 );
			else if ((status = cords_check_invocation( zptr, (struct rest_response *) 0 )) != 0 )
			{
				zptr = occi_remove_response( zptr );
				return( status );
			}
			else
			{
				zptr = occi_remove_response( zptr );
				if ((zptr = occi_simple_delete( lptr->target, _CORDS_SERVICE_AGENT, default_tls() )) != (struct occi_response *) 0)
				{
					zptr = occi_remove_response( zptr );
					if ( pptr->probes )
						pptr->probes--;
				}
			}
		}
	}

	if (!( rest_valid_string( pptr->monitor ) ))
		capture_monitoring( pptr );
	pptr->state = 0;
	autosave_cords_connection_nodes();
	if ( strlen(buffer) )
		if ((zptr = occi_delete_links( buffer, _CORDS_SERVICE_AGENT, default_tls())) != (struct occi_response *) 0)
			zptr = occi_remove_response( zptr );
	if ( rest_valid_string( pptr->monitor ) )
	{
		if (!(zptr = cords_invoke_action( pptr->monitor, _CORDS_STOP, _CORDS_SERVICE_AGENT, default_tls() )))
			return( 801 );
		else if ((status = cords_check_invocation( zptr, (struct rest_response *) 0 )) != 0 )
		{
			zptr = occi_remove_response( zptr );
			return( status );
		}
		else
		{
			zptr = occi_remove_response( zptr );
			return( 0 );
		}
	}
	else 	return( 0 );
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
	int	status=0;
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Failure" ) );
	else if (!( pptr->state ))
		return( rest_html_response( aptr, 200, "OK" ) );
	else
	{
		if (!( status = stop_connection_probes( pptr )))
			return( rest_html_response( aptr, 200, "OK" ) );
		else	return( rest_html_response( aptr, status, "Failure" ) );
	}
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ c o n n e c t i o n  	*/
/*	-------------------------------------------	*/
private	int	delete_connection(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	cords_connection * pptr;
	struct	occi_response * zptr;
	pptr = vptr;
    if ( pptr->state )
        stop_connection_probes( pptr );
    if ( pptr->monitor )
    {
        if ((zptr = occi_simple_delete( pptr->monitor, _CORDS_SERVICE_AGENT, default_tls() )) != (struct occi_response *) 0)
            zptr = occi_remove_response( zptr );
        pptr->monitor = liberate( pptr->monitor );
    }
    return(0);
}

private	struct	occi_interface	connection_interface = {
	NULL,
	NULL,
	NULL,
	delete_connection
	};

/*	------------------------------------------------------------------	*/
/*		c o m o n s _ c o n n e c t i o n _ b u i l d e r		*/
/*	------------------------------------------------------------------	*/
public	struct occi_category * comons_connection_builder( char * domain )
{
	struct	occi_category * optr;

	if (!( optr = occi_cords_connection_builder( domain, _CORDS_CONNECTION ) ))
		return( optr );
	else
	{
		optr->callback = &connection_interface;
		optr->access |= ( _OCCI_NO_PRICING | _OCCI_PRIVATE | _OCCI_CONSUMER );

		if (!( optr = occi_add_action( optr,_CORDS_START,"",start_connection)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_STOP,"",stop_connection)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"capture","",capture_connection)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"consume","",consume_connection)))
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

