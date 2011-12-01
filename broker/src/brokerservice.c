/* ------------------------------------------------------------------------------------	*/
/*				 CompatibleOne Cloudware				*/
/* ------------------------------------------------------------------------------------ */
/*											*/
/* Ce fichier fait partie de ce(tte) oeuvre de Iain James Marshall et est mise a 	*/
/* disposition selon les termes de la licence Creative Commons Paternit‚ : 		*/
/*											*/
/*			 	Pas d'Utilisation Commerciale 				*/
/*				Pas de Modification 					*/
/*				3.0 non transcrit.					*/
/*											*/
/* ------------------------------------------------------------------------------------ */
/* 			Copyright (c) 2011 Iain James Marshall for Prologue 		*/
/*				   All rights reserved					*/
/* ------------------------------------------------------------------------------------ */

#ifndef	_broker_service_c
#define	_broker_service_c

#include "occiresolver.h"
#include "cp.h"
#include "cordslang.h"

#ifdef	_DO_SERVICE_CONFIGURATION

/*	-------------------------------------------	*/
/* 	 s e r v i c e _ c o n f i g u r a t i o n	*/
/*	-------------------------------------------	*/
/*	update the configuration information of a	*/
/*	service contract such that it can be made	*/
/*	made available by other service contracts.	*/
/*	-------------------------------------------	*/

private	int	service_configuration( 
		char * category, char *  agent,
		char * id, char * contractid, 
		char * infoname, char * infovalue )
{
	struct	occi_response * zptr=(struct occi_response *) 0;
	struct	occi_response * yptr=(struct occi_response *) 0;
	struct	occi_element  * fptr=(struct occi_element  *) 0;
	struct	occi_element  * eptr=(struct occi_element  *) 0;
	struct	occi_client   * kptr=(struct occi_client   *) 0;
	struct	occi_request  * qptr=(struct occi_request  *) 0;
	char	buffer[4096];

	if (!( zptr = occi_resolver( category, agent ) ))
	 	return( 401 );
	else
	{
		for (	eptr = zptr->first;
			eptr != (struct occi_element*) 0;
			eptr = eptr->next )
		{
			if (!( eptr->name ))
				continue;
			else if (!( eptr->value ))
				continue;
			sprintf(buffer,"%s/%s/",eptr->value,category);
			if (!( kptr = occi_create_client( buffer, agent, default_tls() ) ))
				continue;
			else if (!(qptr = occi_create_request( 
					kptr, 
					kptr->target->object, 
					_OCCI_NORMAL )))
				continue;
			else if (!( fptr = occi_request_element( 
					qptr,
					"occi.parameter.service", id ) ))
				continue;
			else if (!( fptr = occi_request_element( 
					qptr,
					"occi.parameter.contract", contractid ) ))
				continue;
			else if (!( fptr = occi_request_element( 
					qptr,
					"occi.parameter.name", infoname ) ))
				continue;
			else if (!( fptr = occi_request_element( 
					qptr,
					"occi.parameter.value", infovalue ) ))
				continue;
			else if (!( yptr = occi_client_post( kptr, qptr ) ))
				continue;
			else 	break;
		}
	}
	zptr = occi_remove_response ( zptr );
	yptr = occi_remove_response ( yptr );
	return(0);
}

#endif	/* DO SERVICE CONFIGURATION */

/*	-------------------------------------------	*/
/* 	   i n s t r u c t i o n _ v a l u e s		*/
/*	-------------------------------------------	*/
/*	update the configuration instructions with	*/
/*	values provided by the provisioning of a 	*/
/*	service contract for use by other contracts	*/
/*	-------------------------------------------	*/

private	int	instruction_values(
		char *  agent,
		char * contractid, 
		char * infoname, char * infovalue )
{
	char	*	ihost;
	char 	*	vptr;
	struct	occi_response * zptr=(struct occi_response *) 0;
	struct	occi_response * zzptr=(struct occi_response *) 0;
	struct	occi_response * yptr=(struct occi_response *) 0;
	struct	occi_element  * fptr=(struct occi_element  *) 0;
	struct	occi_element  * eptr=(struct occi_element  *) 0;
	struct	occi_client   * kptr=(struct occi_client   *) 0;
	struct	occi_request  * qptr=(struct occi_request  *) 0;
	char	buffer[4096];
	char	tempname[4096];
	int	length=0;

	if (!( ihost = occi_resolve_category_provider( _CORDS_INSTRUCTION, agent, default_tls() ) ))
	 	return( 401 );

	sprintf(buffer,"%s/%s/",ihost,_CORDS_INSTRUCTION);
	liberate( ihost );
	length = strlen(buffer);

	if (!( kptr = occi_create_client( buffer, agent, default_tls() ) ))
		return( 401 );

	else if (!(qptr = occi_create_request( 
			kptr, 
			kptr->target->object, 
			_OCCI_NORMAL )))
		return(401);
	else if (!( fptr = occi_request_element( 
			qptr,
			"occi.instruction.source", contractid ) ))
	{
		qptr = occi_remove_request( qptr );
		return( 401 );
	}
	else if (!( yptr = occi_client_get( kptr, qptr ) ))
	{
		qptr = occi_remove_request( qptr );
		return( 401 );
	}

	qptr = occi_remove_request ( qptr );

	for (	eptr = yptr->first;
		eptr != (struct occi_element*) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;
		else if (!( vptr = allocate_string( eptr->value )))
			continue;
		else if (!( vptr = occi_category_id( vptr ) ))
			continue;
		else
		{
			strcat( buffer, vptr );
			liberate( vptr );
		}

		if (( zptr = occi_simple_get( buffer, agent, default_tls() )) != (struct occi_response *) 0)
		{
			if (!(fptr = occi_locate_element( zptr->first, "occi.instruction.property" )))
				zptr = occi_remove_response ( zptr );
			else
			{
				sprintf(tempname,"occi.contract.%s",fptr->value );
				if ( strcmp( tempname, infoname ) )
					zptr = occi_remove_response ( zptr );
				else if (!(fptr = occi_locate_element(zptr->first,"occi.instruction.value" )))
					zptr = occi_remove_response ( zptr );
				else
				{
					if ( fptr->value ) fptr->value = liberate( fptr->value );
					fptr->value = allocate_string( infovalue );
					zzptr = occi_simple_put( buffer, zptr->first, agent, default_tls() );
					zzptr = occi_remove_response ( zzptr );
					zptr = occi_remove_response ( zptr );
				}
			}
		}

		/* quick reset of base url */
		/* ----------------------- */
		buffer[length] = 0;
	}

	yptr = occi_remove_response ( yptr );

	return(0);
}

/*	-------------------------------------------	*/
/* 	        s e r v i c e _ a c t i o n		*/
/*	-------------------------------------------	*/
/*	runs the list of linked contracts and sends 	*/
/*	action request for processing.			*/
/*	-------------------------------------------	*/

private	int	service_action( char * id, char * action )
{
	int	items=0;
	int	contracts=0;
	struct	occi_response * zptr;
	struct	occi_element  * eptr;
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	char			* mptr;
	char 			* wptr;
	FILE *			  h;
	char			buffer[1024];
	
	sprintf(buffer,"service/%s",id);

	if (!( h = fopen(buffer,"w")))
		return(46);

	fprintf(h,"{ %s: %c%s%c, contracts: [\n",_CORDS_SERVICE,0x0022,id,0x0022 );

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
		else if ( strcmp( wptr, id ) != 0)
		{
			liberate( wptr );
			continue;
		}
		else	liberate( wptr );

		cords_invoke_action( lptr->target, action, _CORDS_SERVICE_AGENT, default_tls() );

		if ( contracts++ ) fprintf(h,",\n" );

		fprintf(h,"{ %s: %c%s%c, attributs: { ",_CORDS_CONTRACT,0x0022,lptr->target,0x0022);

		if ((zptr = occi_simple_get( lptr->target , _CORDS_SERVICE_AGENT, "" )) 
			!= (struct occi_response *) 0)
		{
			if ( contracts++ )
				fprintf(h,",\n");
			items=0;
			for ( eptr=zptr->first;
				eptr != (struct occi_element *) 0;
				eptr = eptr->next )
			{
				if ( items++ )
					fprintf(h,",\n");

				/* output information to service report */
				/* ------------------------------------ */
				fprintf(h,"%c%s%c: %c%s%c",
					0x0022,eptr->name,0x0022,
					0x0022,eptr->value,0x0022);	

				/* establish configuration instructions */
				/* ------------------------------------ */
				instruction_values(
					_CORDS_SERVICE_AGENT, 
					lptr->target, eptr->name, eptr->value );
			}
		}
		fprintf(h," } }");

	}

	fprintf(h," ] }\n");

	fclose(h);

	return(0);
}


/*	-------------------------------------------	*/
/* 	        s t a r t _ s e r v i c e
/*	-------------------------------------------	*/
private	struct	rest_response * start_service(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_service * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		if ( pptr->state == _OCCI_IDLE )
		{
			service_action( pptr->id, _CORDS_START );
			pptr->when  = time((long*) 0);
			pptr->state = _OCCI_RUNNING;
			autosave_cords_service_nodes();
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	   s u s p e n d _ s e r v i c e
/*	-------------------------------------------	*/
private	struct	rest_response * suspend_service(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	char			* mptr;
	char 			* wptr;
	struct	cords_service * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		if ( pptr->state == _OCCI_RUNNING )
		{
			service_action( pptr->id, _CORDS_SUSPEND );
			pptr->when  = time((long*) 0);
			pptr->state = _OCCI_SUSPENDED;
			autosave_cords_service_nodes();
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	   r e s t a r t _ s e r v i c e
/*	-------------------------------------------	*/
private	struct	rest_response * restart_service(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	char			* mptr;
	char 			* wptr;
	struct	cords_service * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		if ( pptr->state == _OCCI_SUSPENDED )
		{
			service_action( pptr->id, _CORDS_RESTART );
			pptr->when  = time((long*) 0);
			pptr->state = _OCCI_RUNNING;
			autosave_cords_service_nodes();
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	   	s a v e _ s e r v i c e		*/
/*	-------------------------------------------	*/
private	struct	rest_response * save_service(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	char			* mptr;
	char 			* wptr;
	struct	cords_service * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	{
		if ( pptr->state != _OCCI_IDLE )
		{
			service_action( pptr->id, _CORDS_SAVE );
			pptr->when  = time((long*) 0);
			autosave_cords_service_nodes();
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	   	s t o p _ s e r v i c e
/*	-------------------------------------------	*/
private	struct	rest_response * stop_service(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	char			* mptr;
	char 			* wptr;
	struct	cords_service * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	{
		if ( pptr->state != _OCCI_IDLE )
		{
			service_action( pptr->id, _CORDS_STOP );
			pptr->when  = time((long*) 0);
			pptr->state = _OCCI_IDLE;
			autosave_cords_service_nodes();
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	------------------------------------------------------------------	*/
/*		b r o k e r _ s e r v i c e _ b u i l d e r			*/
/*	------------------------------------------------------------------	*/
private	struct	occi_category *	broker_service_builder( char * domain, char * category )
{
	struct	occi_category * optr;
	initialise_occi_resolver( _DEFAULT_PUBLISHER, (char *) 0, (char *) 0, (char *) 0 );
	if (!( optr = occi_cords_service_builder( domain ,category ) ))
		return( optr );
	else if (!( optr = occi_add_action( optr,_CORDS_START,"",start_service)))
		return( optr );
	else if (!( optr = occi_add_action( optr,_CORDS_SUSPEND,"",suspend_service)))
		return( optr );
	else if (!( optr = occi_add_action( optr,_CORDS_RESTART,"",restart_service)))
		return( optr );
	else if (!( optr = occi_add_action( optr,_CORDS_SAVE,"",save_service)))
		return( optr );
	else if (!( optr = occi_add_action( optr,_CORDS_STOP,"",stop_service)))
		return( optr );
	else	return( optr );
}

#endif	/* _broker_service_c */
	/* ------------------ */

