#ifndef	_slamcontrol_c
#define	_slamcontrol_c

#define	_CONTROL_SIGNAL SIGKILL

/*	---------------------------------------------------	*/
/*	     p u r g e _ c o n t r o l _ p a c k e t s		*/
/*	---------------------------------------------------	*/
private	void	purge_control_packets(struct cords_control * pptr, char * packets, char * probe )
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

/*	--------------------------------------------------------	*/
/*		i s s u e _ c o n t r o l _ r e w a r d			*/
/*	--------------------------------------------------------	*/
private	void	issue_control_reward( struct cords_control * pptr, char * packet, struct occi_response * message )
{
	struct	occi_response * zptr;
	char	buffer[208];

	/* ----------------------------------- */
	/* TODO : Scan list of Business Values */
	/* to detect the penalties inccured by */
	/* the obligated party for failure to  */
	/* comply with objectives.             */
	/* ----------------------------------- */
	/* for now we simply delete the packet */
	/* ----------------------------------- */

	sprintf(buffer, "%s/%s/%s",get_identity(),_CORDS_CONTROL,pptr->id);

	/* ------------------------ */
	/* simply delete the packet */
	/* ------------------------ */
	if ((zptr = occi_simple_delete( packet, _CORDS_CONTRACT_AGENT, default_tls() )) != (struct occi_response *) 0)
		zptr = occi_remove_response( zptr );

	/* ------------------------ */
	/* - delete link from probe */
	/* ------------------------ */
	if ((zptr = occi_delete_link( pptr->probe, packet, _CORDS_CONTRACT_AGENT, default_tls() )) != (struct occi_response *) 0)
		zptr = occi_remove_response( zptr );

	message = occi_remove_response( message );
	return;
}

/*	--------------------------------------------------------	*/
/*		i s s u e _ c o n t r o l _ p e n a l t y 		*/
/*	--------------------------------------------------------	*/
private	void	issue_control_penalty( struct cords_control * pptr, char * packet, struct occi_response * message )
{
	struct	occi_response * zptr;
	struct	occi_element * dptr;
	char *	ihost;
	char	buffer[2048];
	char	now[64];

	sprintf(buffer, "%s/%s/%s",get_identity(),_CORDS_CONTROL,pptr->id);
	sprintf(now,"%lu",time((long*) 0));

	/* --------------------------------------------------------------------- */
	/* create a penalty category instance to show the arrival of the penalty */
	/* --------------------------------------------------------------------- */
	if ((!( dptr = occi_create_element(       "occi.penalty.account",    	pptr->account	) ))
	||  (!( dptr = occi_append_element( dptr, "occi.penalty.agreement",  	pptr->agreement	) ))
	||  (!( dptr = occi_append_element( dptr, "occi.penalty.contract",	pptr->contract	) ))
	||  (!( dptr = occi_append_element( dptr, "occi.penalty.timestamp", 	now		) ))
	||  (!( dptr = occi_append_element( dptr, "occi.penalty.control", 	buffer		) ))
	||  (!( dptr = occi_append_element( dptr, "occi.penalty.packet", 	packet		) )))
	{
		message = occi_remove_response( message );
		return;
	}
	else if (!( zptr = occi_simple_post( buffer, dptr, _CORDS_CONTRACT_AGENT, default_tls() ) ))
	{
		message = occi_remove_response( message );
		return;
	}
	else if (!( ihost = occi_extract_location( zptr ) ))
	{
		zptr = occi_remove_response( zptr );
		message = occi_remove_response( message );
		return;
	}
	else if (!( ihost = allocate_string( ihost ) ))
	{
		zptr = occi_remove_response( zptr );
		message = occi_remove_response( message );
		return;
	}
	else	zptr = occi_remove_response( zptr );


	/* ------------------------ */
	/* - delete link from probe */
	/* ------------------------ */
	if ((zptr = occi_delete_link( pptr->probe, packet, _CORDS_CONTRACT_AGENT, default_tls() )) != (struct occi_response *) 0)
		zptr = occi_remove_response( zptr );

	/* ------------------------ */
	/* - create link to control */
	/* ------------------------ */
	if ((zptr = occi_create_link( buffer, ihost, _CORDS_CONTRACT_AGENT, default_tls() )) != (struct occi_response *) 0)
		zptr = occi_remove_response( zptr );

	ihost = liberate( ihost );
	message = occi_remove_response( message );
	return;
}

/*	---------------------------------------------------	*/
/*		c o m p a r e _ p a c k e t _ d a t a		*/
/*	---------------------------------------------------	*/
private	int	compare_packet_data( int compare, char * lptr, char * rptr )
{
	int	result=0;
	char *	vptr;

	vptr = lptr;
	while ( *vptr == ' ' ) vptr++;

	switch ( compare )
	{
	case	1 :
		if ( atoi(vptr) == atoi( rptr ) )
			result = 1;
		else	result = 0;
		break;
	case	6 :
		if ( atoi(vptr) != atoi( rptr ) )
			result = 1;
		else	result = 0;
		break;
	case	2 :
		if ( atoi(vptr) > atoi( rptr ) )
			result = 1;
		else	result = 0;
		break;
	case	5 :
		if ( atoi(vptr) <= atoi( rptr ) )
				result = 1;
		else	result = 0;
		break;
	case	4 :
		if ( atoi(vptr) < atoi( rptr ) )
			result = 1;
		else	result = 0;
		break;
	case	3 :
		if ( atoi(vptr) >= atoi( rptr ) )
			result = 1;
		else	result = 0;
		break;
	}
	liberate( lptr );
	return( result );
}

/*	---------------------------------------------------	*/
/*		e v a l u a t e _ p a c k e t _ d a t a		*/
/*	---------------------------------------------------	*/
private	int	evaluate_packet_data( struct cords_control * pptr, char * packet, struct occi_response * message )
{
	struct	occi_element * dptr;
	char *	vptr;
	char *	sptr;
	char *	optr;
	char *	cptr;

	/* -------------------------------------------------- */
	/* the packet is ok if the evaluation is not possible */
	/* -------------------------------------------------- */
	if (!( rest_valid_string( pptr->property ) ))
		return( 1 );
	else if (!( rest_valid_string((cptr = pptr->condition)) ))
		return( 1 );
	else if (!( rest_valid_string((optr= pptr->objective)) ))
		return( 1 );
	else if (!( rest_valid_string( pptr->metric ) ))
		return( 1 );
	/* ----------------------------------------------- */
	/* recover packet data and ok if no data available */
	/* ----------------------------------------------- */
	else if (!(dptr = occi_locate_element( message->first, "occi.packet.data" )))
		return( 1 );
	else if (!( rest_valid_string( dptr->value ) ))
		return( 1 );
	else if (!( sptr = rest_decode_html( dptr->value ) ))
		return( 1 );
	else
	{

		if ((!( strcasecmp( cptr, "eq" ) ))
		||  (!( strcasecmp( cptr, "equal" ) )))
			return( compare_packet_data( 1, sptr, optr ) );

		else if ((!( strcasecmp( cptr, "gr"      ) ))
		||  (!( strcasecmp( cptr, "greater" ) )))
			return( compare_packet_data( 2, sptr, optr ) );

		else if ((!( strcasecmp( cptr, "ls" ) ))
		||  (!( strcasecmp( cptr, "less" ) )))
			return( compare_packet_data( 4, sptr, optr ) );

		else if ((!( strcasecmp( cptr, "ne" ) ))
		||  (!( strcasecmp( cptr, "notequal" ) )))
			return( compare_packet_data( 6, sptr, optr ) );

		else if ((!( strcasecmp( cptr, "le" ) ))
		||  (!( strcasecmp( cptr, "notgreater" ) )))
			return( compare_packet_data( 5, sptr, optr ) );

		else if ((!( strcasecmp( cptr, "ge" ) ))
		||  (!( strcasecmp( cptr, "notless" ) )))
			return( compare_packet_data( 3, sptr, optr ) );

		else	return(1);
	}
}


/*	--------------------------------------------	*/
/*	  p u r g e _ c o n t r o l _ p a c k e t s	*/
/*	--------------------------------------------	*/
private	void	evaluate_control_packets(struct cords_control * pptr, char * packets, char * probe )
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

			/* ------------------- */
			/* retrieve the packet */
			/* ------------------- */
			if (!(zptr = occi_simple_get( eptr->value, _CORDS_CONTRACT_AGENT, default_tls() )))
				continue;

			/* evaluate packet data to be true */
			/* ------------------------------- */
			else if ( evaluate_packet_data( pptr, eptr->value, zptr ) )
				issue_control_reward( pptr, eptr->value, zptr );
			else	issue_control_penalty( pptr, eptr->value, zptr );

		}				
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return;
	}
}

/*	-------------------------------------------	*/
/* 	     c o n t r o l _ w o r k e r		*/
/*	-------------------------------------------	*/
/*	the control worker is responsible for the 	*/
/*	consumption of the monitoring data packats	*/
/*	that are sent up from the probe inside the	*/
/*	cosacs enabled vm endpoint.			*/
/*	the standard control worker will simply		*/
/*	eat the received packets for now to ensure	*/
/*	that monitoring data does not bog down the	*/
/*	platform operation with a big data problem	*/
/*	-------------------------------------------	*/
private	int	control_worker( struct cords_control * pptr )
{
	int	inner=2;
	int	outer=5;
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	char *	wptr;
	char	packets[1024];

	if (!( pptr ))
		return( 0 );
	else if (!( rest_valid_string( pptr->probe )))
		return(0);
	else
	{
		sprintf(packets,"%s/%s/",get_identity(),_CORDS_PACKET);
		while (1)
		{
			/* ------------------------------------- */
			/* if no penalties or rewards just purge */
			/* ------------------------------------- */
			if (!( rest_valid_string( pptr->reference ) ))
			{
				purge_control_packets( pptr, packets, pptr->probe );
				sleep(inner);
			}
			else
			{
				evaluate_control_packets( pptr, packets, pptr->probe );
				sleep(inner);
			}
		}
		return(0);
	}
}

	
/*	-------------------------------------------	*/
/*	   c o n s u m e _ c o n t r o l 		*/
/*	-------------------------------------------	*/
private	struct rest_response * consume_control( struct cords_control * pptr, struct rest_response * aptr )
{
	switch ((pptr->process=fork()))
	{
	case	0  : 
		exit( control_worker( pptr ) );
	case	-1 : 
		return( rest_html_response( aptr, 802, "Control Start Failure" ) );
	default	   : 
		pptr->state = 1;
		autosave_cords_control_nodes();
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}



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
	else	return( consume_control( pptr, aptr ) );
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

		if ( pptr->process > 0 )
		{
			kill( pptr->process, _CONTROL_SIGNAL );
			waitpid(pptr->process,&status,0);
			pptr->process = 0;
		}
		pptr->state = 0;
		autosave_cords_control_nodes();
		return( rest_html_response( aptr, 200, "OK" ) );
	}		
}

	/* -------------- */
#endif	/* _slamcontrol_c */
	/* -------------- */

