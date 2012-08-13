#ifndef _commonsconnection_c 
#define _commonsconnection_c 

#include "cp.h"

/*	--------------------------------------------	*/
/*	    p u r g e _ p r o b e _ p a c k e t s	*/
/*	--------------------------------------------	*/
private	void	purge_probe_packets(struct cords_connection * pptr, char * packets, char * probe )
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
			else if ( strcmp(vptr, _CORDS_PACKET ) != 0 )
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
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	char *	wptr;
	char	packets[1024];
	char *	get_identity();
	sprintf(packets,"%s/%s/",get_identity(),_CORDS_PACKET);
	while (1)
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
				purge_probe_packets( pptr, packets, lptr->target );
				sleep(inner);
				continue;
			}
		}
		sleep(outer);
	}
	return(0);
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
		/* ------------------------------ */
		/* disactivate connection monitor */
		/* ------------------------------ */
		if ( pptr->pid )
		{
			kill( pptr->pid, SIGTERM );
			pptr->id = 0;
		}
		pptr->state = 0;
		autosave_cords_connection_nodes();
		if ( strlen(buffer) )
			if ((zptr = occi_delete_links( buffer, _CORDS_SERVICE_AGENT, default_tls())) != (struct occi_response *) 0)
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

