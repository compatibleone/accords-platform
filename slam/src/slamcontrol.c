#ifndef	_slamcontrol_c
#define	_slamcontrol_c

#define	_CONTROL_SIGNAL SIGKILL
#define	_DEFAULT_CONTROL_PERIOD	60

#include "cb.h"
#include "cspi.h"

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


/*	------------------------------------------------------------------	*/
/*	      s c r i p t e d _ b u s i n e s s _ p r o c e s s i n g		*/
/*	------------------------------------------------------------------	*/
private	int	scripted_business_processing( 
		struct cords_control * pptr,
		char * expression,
		char * nature, 
		char * packet )

{
	char *	argv[10];
	int	argc=0;
	char *	result;
	char 	buffer[2048];
	if (!( rest_valid_string( expression ) ))
		return( 0 );
	else
	{
		/* ----------------------------------------------------------------------- */
		/* CORDSCRIPT: CALLER, CONTROLID, NATURE, PACKET, PUBLISHER, OPERATOR, TLS */
		/* ----------------------------------------------------------------------- */
		sprintf(buffer,"%s/%s/%s",get_identity(), _CORDS_CONTROL, pptr->id);
		argv[argc++] = "slam.control";
		argv[argc++] = buffer;
		argv[argc++] = nature;
		argv[argc++] = packet;
		argv[argc++] = default_publisher();
		argv[argc++] = default_operator();
		argv[argc++] = default_tls();
		argv[argc] = (char *) 0;
		if (!( result = evaluate_cordscript( expression, argc, argv )))
			return( 0 );
		else	return( atoi( result ) );
	}
}


/*	--------------------------------------------------------	*/
/*	      e x e c u t e _ b u s i n e s s _ v a l u e  		*/
/*	--------------------------------------------------------	*/
private	int	execute_business_value( 
		struct cords_control * pptr,
		char * expression,
		char * nature, 
		struct occi_response * guarantee,
		struct occi_response * business,
		char * packetid, 
		struct occi_response * message, 
		char * packetdata, 
		int sequence )
{
	char 	buffer[4096];
	if (!( rest_valid_string( expression ) ))
		return(0);
	else if (!( strncasecmp( expression, "cordscript:", strlen("cordscript:")  ) ))
	{
		expression += strlen("cordscript:");
		sprintf(buffer,"sla %s seq=%u",nature,sequence);
		rest_log_message( buffer );
		rest_log_message( expression );
		scripted_business_processing( pptr, expression, nature, packetid );
		return(1);
	}
	else
	{
		sprintf(buffer,"sla %s seq=%u ",nature,sequence);
		rest_log_message( buffer );
		rest_log_message( expression );
		scripted_business_processing( pptr, expression, nature, packetid );
		return(1);
	}

}

/*	--------------------------------------------------------	*/
/*		i n v o k e _ b u s i n e s s _ v a l u e s 		*/
/*	--------------------------------------------------------	*/
private	int	invoke_business_values( char * nature, struct cords_control * pptr, char * packet, struct occi_response * message, char * data, int sequence )
{
	struct	occi_response 	* zptr;
	struct	occi_response 	* yptr;
	struct	occi_element 	* eptr;
	struct	occi_element 	* dptr;
	int	items=0;
	char *	sptr;

	/* ----------------------------------------------------- */
	/* retrieve the list of business values defined for the  */
	/* guarantee indicated by the control reference property */
	/* ----------------------------------------------------- */
	if (!( rest_valid_string( pptr->reference )))
		return(0);
	else if (!( zptr = occi_simple_get( pptr->reference, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return(0);
	else
	{
		/* ----------------------------------- */
		/* for each business value in the list */
		/* ----------------------------------- */
		for (	eptr = cords_first_link( zptr );
			eptr != (struct occi_element *) 0;
			eptr = cords_next_link( eptr ))
		{
			if (!( eptr->value ))
				continue;
			else if (!( sptr =  occi_unquoted_link( eptr->value ) ))
				continue;
			else if (!( yptr = occi_simple_get( sptr, _CORDS_CONTRACT_AGENT, default_tls() ) ))
			{
				liberate( sptr );
				continue;
			}
			else if (!( dptr = occi_locate_element( yptr->first, "occi.business.nature" ) ))
			{
				yptr = occi_remove_response( yptr );
				liberate( sptr );
				continue;
			}
			else if (!( dptr->value ))
			{
				yptr = occi_remove_response( yptr );
				liberate( sptr );
				continue;
			}
			else if ( strcasecmp( dptr->value, nature ) != 0)
			{
				yptr = occi_remove_response( yptr );
				liberate( sptr );
				continue;
			}
			else if (!( dptr = occi_locate_element( yptr->first, "occi.business.expression" ) ))
			{
				yptr = occi_remove_response( yptr );
				liberate( sptr );
				continue;
			}
			else
			{
				/* --------------------------------------- */
				/* we now have a business value expression */
				/* of the right kind that needs evaluated  */
				/* --------------------------------------- */
				items += execute_business_value( pptr, dptr->value, nature, zptr, yptr, packet, message, data, sequence );
				yptr = occi_remove_response( yptr );
				liberate( sptr );
				continue;
			}
		}
		return(items);
	}
}

/*	--------------------------------------------------------	*/
/*		i s s u e _ c o n t r o l _ r e w a r d			*/
/*	--------------------------------------------------------	*/
private	void	issue_control_reward( struct cords_control * pptr, char * packet, struct occi_response * message, char * data, int sequence )
{
	struct	occi_response * zptr;
	char	buffer[208];

	sprintf(buffer, "%s/%s/%s",get_identity(),_CORDS_CONTROL,pptr->id);

	/* ---------------------------------------------- */
	/* invoke the business values of the penalty type */
	/* ---------------------------------------------- */
	if (!( invoke_business_values( _CORDS_REWARD, pptr, packet, message, data, sequence ) ))
	{
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
	}
	message = occi_remove_response( message );
	return;
}

/*	--------------------------------------------------------	*/
/*		i s s u e _ c o n t r o l _ p e n a l t y 		*/
/*	--------------------------------------------------------	*/
private	void	issue_control_penalty( struct cords_control * pptr, char * packet, struct occi_response * message, char * data, int sequence )
{
	struct	occi_response * zptr;
	struct	occi_element * dptr;
	char *	ihost;
	char	buffer[2048];
	char	penalty[2048];
	char	now[64];
	char	what[64];

	sprintf(buffer, "%s/%s/%s"	,get_identity(),_CORDS_CONTROL,pptr->id);
	sprintf(penalty,"%s/%s/"	,get_identity(),_CORDS_PENALTY);
	sprintf(what,	"%u"		,sequence);
	sprintf(now,	"%lu"		,time((long*) 0));

	/* ---------------------------------------------- */
	/* invoke the business values of the penalty type */
	/* ---------------------------------------------- */
	if (!( invoke_business_values( _CORDS_PENALTY, pptr, packet, message, data, sequence ) ))
	{

		/* --------------------------------------------------------------------- */
		/* create a penalty category instance to show the arrival of the penalty */
		/* --------------------------------------------------------------------- */
		if ((!( dptr = occi_create_element(       "occi.penalty.account",    	pptr->account	) ))
		||  (!( dptr = occi_append_element( dptr, "occi.penalty.agreement",  	pptr->agreement	) ))
		||  (!( dptr = occi_append_element( dptr, "occi.penalty.contract",	pptr->contract	) ))
		||  (!( dptr = occi_append_element( dptr, "occi.penalty.control", 	buffer		) ))
		||  (!( dptr = occi_append_element( dptr, "occi.penalty.timestamp", 	now		) ))
		||  (!( dptr = occi_append_element( dptr, "occi.penalty.sequence", 	what		) ))
		||  (!( dptr = occi_append_element( dptr, "occi.penalty.data", 		data		) )))
		{
			message = occi_remove_response( message );
			return;
		}
		else if (!( zptr = occi_simple_post( penalty, dptr, _CORDS_CONTRACT_AGENT, default_tls() ) ))
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
		/* simply delete the packet */
		/* ------------------------ */
		if ((zptr = occi_simple_delete( packet, _CORDS_CONTRACT_AGENT, default_tls() )) != (struct occi_response *) 0)
			zptr = occi_remove_response( zptr );
	
		/* ------------------------ */
		/* - create link to control */
		/* ------------------------ */
		if ((zptr = occi_create_link( buffer, ihost, _CORDS_CONTRACT_AGENT, default_tls() )) != (struct occi_response *) 0)
			zptr = occi_remove_response( zptr );

		ihost = liberate( ihost );
	}
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
	int	lvalue=0;
	int	rvalue=0;

	/* -------------------------------- */
	/* normalise the probe sample value */
	/* -------------------------------- */
	vptr = lptr;
	while ( *vptr == ' ' ) vptr++;
	lvalue = rest_normalise_value( vptr, 'U' );		

	/* -------------------------------- */
	/* normalise the sl objective value */
	/* -------------------------------- */
	vptr = rptr;
	while ( *vptr == ' ' ) vptr++;
	rvalue = rest_normalise_value( vptr, 'U' );		
		
	liberate( lptr );

	/* ------------------------------- */
	/* evaluate conditional expression */
	/* ------------------------------- */
	switch ( compare )
	{
	case	1 :
		if ( lvalue == rvalue )
			return(1);
		else	return(0);
	case	6 :
		if ( lvalue != rvalue )
			return(1);
		else	return(0);
	case	2 :
		if ( lvalue > rvalue )
			return(1);
		else	return(0);
	case	5 :
		if ( lvalue <= rvalue )
			return(1);
		else	return(0);
	case	4 :
		if ( lvalue < rvalue )
			return(1);
		else	return(0);
	case	3 :
		if ( lvalue >= rvalue )
			return(1);
		else	return(0);
	default	:
		return(0);
	}
}

/*	---------------------------------------------------	*/
/*		e v a l u a t e _ p a c k e t _ d a t a		*/
/*	---------------------------------------------------	*/
private	int	evaluate_packet_data( struct cords_control * pptr, char * packet, char ** data, int * sequence, struct occi_response * message )
{
	struct	occi_element * dptr;
	char *	vptr;
	char *	sptr;
	char *	optr;
	char *	cptr;

	*data = (char *) 0;
	*sequence = 0;

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
		*data = dptr->value;

		if (((dptr = occi_locate_element( message->first, "occi.packet.sequence" )) != (struct occi_element *) 0)
		&&  ( rest_valid_string( dptr->value ) != 0 ))
			*sequence = atoi( dptr->value );

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
	char *	data=(char *) 0;
	int	sequence=0;

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
			else if ( evaluate_packet_data( pptr, eptr->value, &data, &sequence, zptr ) )
				issue_control_reward( pptr, eptr->value, zptr, data, sequence );
			else	issue_control_penalty( pptr, eptr->value, zptr, data, sequence );

		}				
		yptr = occi_remove_response( yptr );
		qptr = occi_remove_request( qptr );
		kptr = occi_remove_client( kptr );
		return;
	}
}

/*	-------------------------------------------	*/
/*		c o n t r o l _ p e r i o d		*/
/*	-------------------------------------------	*/
private	int	control_period( struct cords_control * pptr )
{
	int	period;
	struct	occi_response * zptr;
	struct	occi_element * dptr;
	if (!( pptr ))
		period =  _DEFAULT_CONTROL_PERIOD;
	else if (!( rest_valid_string( pptr->metric ) ))
		period =  _DEFAULT_CONTROL_PERIOD;
	else if (!( zptr = occi_simple_get( pptr->metric, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		period =  _DEFAULT_CONTROL_PERIOD;
	else if ((!( dptr = occi_locate_element( zptr->first, "occi.metric.period" ) ))
	     ||  (!( rest_valid_string( dptr->value ) )))
	{
		zptr = occi_remove_response( zptr );
		period =  _DEFAULT_CONTROL_PERIOD;
	}
	else
	{
		period = atoi( dptr->value );
		zptr = occi_remove_response( zptr );
	}
	return( period );
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
	int	tempo=_DEFAULT_CONTROL_PERIOD;
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	struct	occi_response 	* zptr; 
	struct	occi_element	* dptr;
	char *	wptr;
	char	packets[1024];

	sleep(2);

	if (!( pptr ))
		return( 0 );
	else if (!( rest_valid_string( pptr->probe )))
		return(0);
	else if (!( tempo = control_period( pptr ) ))
		tempo = _DEFAULT_CONTROL_PERIOD;

	sprintf(packets,"%s/%s/",get_identity(),_CORDS_PACKET);
	while (1)
	{
		/* ------------------------------------- */
		/* if no penalties or rewards just purge */
		/* ------------------------------------- */
		if (!( rest_valid_string( pptr->reference ) ))
		{
			purge_control_packets( pptr, packets, pptr->probe );
			sleep(tempo);
		}
		else
		{
			evaluate_control_packets( pptr, packets, pptr->probe );
			sleep(tempo);
		}
	}
	return(0);
}

	
/*	-------------------------------------------	*/
/*	      t i m e r _ c o n t r o l 		*/
/*	-------------------------------------------	*/
private	struct rest_response * timer_control( struct cords_control * pptr, struct rest_response * aptr )
{
	struct	occi_response * zptr;
	struct	occi_element  * dptr;
	char	buffer[2048];
	char	timer[2048];
	char	period[256];
	int	tempo=_DEFAULT_CONTROL_PERIOD;
	char *	ihost;

	if (!( rest_valid_string( pptr->timer ) ))
	{
		if (!( tempo = control_period( pptr ) ))
			tempo = _DEFAULT_CONTROL_PERIOD;
		if (!( ihost = occi_resolve_category_provider( _CORDS_TIMER, _CORDS_CONTRACT_AGENT, default_tls() ) ))
			return( rest_html_response( aptr, 478, "Timer ServiceFailure" ) );
		else
		{
			sprintf(buffer, "%s/%s/%s",get_identity(),_CORDS_CONTROL,pptr->id);
			sprintf(period, "%u",tempo);
			sprintf(timer,  "%s/%s/",ihost,_CORDS_TIMER);
			ihost = liberate( ihost );
		}

		/* --------------------------------------------------------------------- */
		/* create a penalty category instance to show the arrival of the penalty */
		/* --------------------------------------------------------------------- */
		if ((!( dptr = occi_create_element(       "occi.timer.name",    	pptr->name	) ))
		||  (!( dptr = occi_append_element( dptr, "occi.timer.target",		buffer		) ))
		||  (!( dptr = occi_append_element( dptr, "occi.timer.source", 		"verify"	) ))
		||  (!( dptr = occi_append_element( dptr, "occi.timer.nature", 		"periodic"	) ))
		||  (!( dptr = occi_append_element( dptr, "occi.timer.period", 		period 		) ))
		||  (!( dptr = occi_append_element( dptr, "occi.timer.account",	  	pptr->account	) )))
		{
			return( rest_html_response( aptr, 500, "Timer Activation Failure" ) );
		}
		else if (!( zptr = occi_simple_post( timer, dptr, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		{
			return( rest_html_response( aptr, 501, "Timer Activation Failure" ) );
		}
		else if (!( ihost = occi_extract_location( zptr ) ))
		{
			zptr = occi_remove_response( zptr );
			return( rest_html_response( aptr, 502, "Timer Activation Failure" ) );
		}
		else if (!( pptr->timer = allocate_string( ihost ) ))
		{
			zptr = occi_remove_response( zptr );
			return( rest_html_response( aptr, 503, "Timer Activation Failure" ) );
		}
		else	zptr = occi_remove_response( zptr );
	}

	pptr->state = 1;
	autosave_cords_control_nodes();

	if (( zptr = cords_invoke_action( pptr->timer, _CORDS_START, _CORDS_CONTRACT_AGENT, default_tls() )) != (struct occi_response *) 0)
		zptr = occi_remove_response( zptr );


	return( rest_html_response( aptr, 200, "OK" ) );
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
	else if (!( rest_valid_string( pptr->mode ) ))
		return( consume_control( pptr, aptr ) );
	else if (!( strcmp( pptr->mode, _CORDS_TIMER ) ))
		return( timer_control( pptr, aptr ) );
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
		else if ( rest_valid_string( pptr->timer ) )
		{
			if (( zptr = cords_invoke_action( pptr->timer, _CORDS_STOP, _CORDS_CONTRACT_AGENT, default_tls() )) != (struct occi_response *) 0)
				zptr = occi_remove_response( zptr );
		}
		pptr->state = 0;
		autosave_cords_control_nodes();
		return( rest_html_response( aptr, 200, "OK" ) );
	}		
}

/*	-------------------------------------------	*/
/* 		v e r i f y _ c o n t r o l		*/
/*	-------------------------------------------	*/
private	struct rest_response * verify_control(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_control * pptr;
	char	packets[1024];
	if (!( pptr = vptr ))
		return( rest_html_response( aptr, 400, "Failure" ) );
	else if (!( pptr->state ))
		return( rest_html_response( aptr, 200, "OK" ) );
	else
	{
		sprintf(packets,"%s/%s/",get_identity(),_CORDS_PACKET);
		/* ------------------------------------- */
		/* if no penalties or rewards just purge */
		/* ------------------------------------- */
		if (!( rest_valid_string( pptr->reference ) ))
			purge_control_packets( pptr, packets, pptr->probe );
		else	evaluate_control_packets( pptr, packets, pptr->probe );
		return( rest_html_response( aptr, 200, "OK" ) );
	}		
}

	/* -------------- */
#endif	/* _slamcontrol_c */
	/* -------------- */

