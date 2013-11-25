/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>     */
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License"); 	*/
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at 				*/
/*  									*/
/*  http://www.apache.org/licenses/LICENSE-2.0 				*/
/*  									*/
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 	*/
/* implied. 								*/
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License. 					*/
/* -------------------------------------------------------------------- */
#ifndef	_broker_service_c
#define	_broker_service_c

#include "occiresolver.h"
#include "cp.h"
#include "cb.h"
#include "cordslang.h"

					/* ---------------------------------------------------- */
private	int generate_service_report=1;	/* controls the generation of the service report 	*/
					/* this report contains the current values of the state */
					/* variables of the service and contracts.		*/
					/* This file is used by the co-command for service stop */
					/* start and save operations on a service basis.	*/
					/* ---------------------------------------------------- */

/*	-------------------------------------------------	*/
/*	u p d a t e _ i n s t r u c t i o n _ v a l u e s	*/
/*	-------------------------------------------------	*/
/*	here we must update the configuration instruction	*/
/*	category instances for the current contract using	*/
/*	the values available in the new provider contract	*/
/*	category instance property list.			*/
/*	-------------------------------------------------	*/
private	int	update_instruction_values(
		struct occi_response * rptr,
		char * contractid,
		char *  agent )
{
	char	*	ihost;
	char 	*	vptr;
	struct	occi_response * zptr=(struct occi_response *) 0;
	struct	occi_response * zzptr=(struct occi_response *) 0;
	struct	occi_response * yptr=(struct occi_response *) 0;
	struct	occi_element  * eptr=(struct occi_element  *) 0;
	struct	occi_element  * fptr=(struct occi_element  *) 0;
	struct	occi_element  * gptr=(struct occi_element  *) 0;
	struct	occi_client   * kptr=(struct occi_client   *) 0;
	struct	occi_request  * qptr=(struct occi_request  *) 0;
	char	buffer[4096];
	char	tempname[4096];
	int	length=0;

	/* --------------------------------------------------------------------- */
	/* select / resolve the instruction category service provider identifier */
	/* --------------------------------------------------------------------- */
	if (!( ihost = occi_resolve_category_provider( _CORDS_INSTRUCTION, agent, default_tls() ) ))
	 	return( 401 );

	sprintf(buffer,"%s/%s/",ihost,_CORDS_INSTRUCTION);
	liberate( ihost );
	length = strlen(buffer);

	/* ----------------------------------------------------------------- */
	/* retrieve the list of configuration instruction category instances */
	/* ----------------------------------------------------------------- */
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

	/* ------------------------------------------------------- */
	/* for each configuration instruction instance in the list */
	/* ------------------------------------------------------- */
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

		/* -------------------------------------------------------- */
		/* retrieve the configuration instruction category instance */
		/* -------------------------------------------------------- */
		if (( zptr = occi_simple_get( buffer, agent, default_tls() )) != (struct occi_response *) 0)
		{
			if (!(fptr = occi_locate_element( zptr->first, "occi.instruction.property" )))
				zptr = occi_remove_response ( zptr );
			else
			{
				/* ----------------------------------------------------- */
				/* retrieve the OCCI property from the Contract Response */
				/* ----------------------------------------------------- */
				sprintf(tempname,"occi.contract.%s",fptr->value );

				if (!(fptr = occi_locate_element( rptr->first, tempname ) ))
					zptr = occi_remove_response ( zptr );

				/* --------------------------------------- */
				/* retrieve the instruction value property */
				/* --------------------------------------- */
				else if (!(gptr = occi_locate_element(zptr->first,"occi.instruction.value" )))
					zptr = occi_remove_response ( zptr );
				else
				{
					/* -------------------------------------------- */
					/* store the new value of the instruction value */
					/* -------------------------------------------- */
					if ( gptr->value ) gptr->value = liberate( gptr->value );
					gptr->value = allocate_string( fptr->value );

					/* ----------------------------------------- */
					/* update this instruction category instance */				
					/* ----------------------------------------- */
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

/*	--------------------------------------------	*/
/* 	        s e r v i c e _ a c t i o n		*/
/*	--------------------------------------------	*/
/*	runs the list of linked contracts and sends 	*/
/*	action requests for processing to each of 	*/
/*	contracts in turn. configuration instruction	*/
/*	category instances will be updated at each 	*/
/*	stage of the operation to provide the newly	*/
/*	aquired values for use in building following	*/
/*	contract provisioning instances.		*/
/*	--------------------------------------------	*/
/*	The service report file will be updated as a	*/
/*	result of the operation and will reflect the	*/
/*	current state of the service and contracts.	*/
/*	--------------------------------------------	*/
/*	Optimised Configuration Instruction Proccess	*/
/*	--------------------------------------------	*/
private	int	service_action( struct cords_service * pptr, char * id, char * action )
{
	int	items=0;
	int	flags=0;
	struct	occi_response * zptr;
	struct	occi_element  * eptr;
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	char			* mptr;
	char 			* wptr;
	int			status=0;
	FILE *			  h;
	char			buffer[1024];

	/* ------------------------------------------------------ */
	/* initialise the service report file and generate header */
	/* ------------------------------------------------------ */
	if ( generate_service_report )
	{
		sprintf(buffer,"service/%s",id);

		if (!( h = fopen(buffer,"w")))
			return(46);

		fprintf(h,"{ \"%s\" : %c%s%c, \"contracts\" : [\n",_CORDS_SERVICE,0x0022,id,0x0022 );
	}

	/* ----------------------------------------------------- */
	/* for all defined contract nodes of the current service */
	/* ----------------------------------------------------- */
	for (	pptr->contracts=0,
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
		else if ( strcmp( wptr, id ) != 0)
		{
			liberate( wptr );
			continue;
		}
		else	liberate( wptr );

		/* --------------------------------------------------- */
		/* launch / invoke the required action on the contract */
		/* --------------------------------------------------- */
		if (!( zptr = occi_simple_get( lptr->target, _CORDS_SERVICE_AGENT, default_tls() ) ))
			continue;
		else
		{
			if (!( eptr = occi_locate_element( zptr->first, "occi.contract.flags" ) ))
				flags = 0;
			else if (!( rest_valid_string( eptr->value ) ))
				flags = 0;
			else	flags = atoi( eptr->value );

			/* ----------------------------------------------- */
			/* start not inhibited so contract will be updated */
			/* ----------------------------------------------- */
			if (!( flags & _INHIBIT_AUTOSTART ))
				zptr = occi_remove_response ( zptr );
		}

		/* --------------------------------------------------- */
		/* launch / invoke the required action on the contract */
		/* --------------------------------------------------- */
		if (!( flags & _INHIBIT_AUTOSTART ))
		{
			if (!(zptr = cords_invoke_action( lptr->target, action, _CORDS_SERVICE_AGENT, default_tls() )))
				status = 118;
			else
			{
				status = cords_check_invocation( zptr, (struct html_response *) 0 );
				zptr = occi_remove_response( zptr );
			}
		}
		else	status = 0;

		if ( pptr->contracts++ ) fprintf(h,",\n" );

		if ( generate_service_report )
		{
			fprintf(h,"{ \"%s\" : %c%s%c, \"attributs\" : { ",_CORDS_CONTRACT,0x0022,lptr->target,0x0022);
		}

		/* ------------------------------------------------- */
		/* retrieve the resulting contract category instance */
		/* ------------------------------------------------- */
		if (!( flags & _INHIBIT_AUTOSTART ))
			zptr = occi_simple_get( lptr->target , _CORDS_SERVICE_AGENT, default_tls() );

		if ( zptr )
		{
			items=0;

			/* ---------------------------------------------------------- */
			/* first save each property of the contract category instance */
			/* to the service report file for use by co-command functions */
			/* ---------------------------------------------------------- */
			for ( 	eptr = zptr->first;
				eptr != (struct occi_element *) 0;
				eptr = eptr->next )
			{
				if ( generate_service_report )
				{
					if ( items++ )
						fprintf(h,",\n");

					/* ------------------------------------ */
					/* output information to service report */
					/* ------------------------------------ */
					fprintf(h,"'%s': '%s'", eptr->name, eptr->value);	
				}
			}

			/* ---------------------------------------------------------- */
			/* update configuration instruction values from this contract */
			/* ---------------------------------------------------------- */
			if (!( flags & _INHIBIT_AUTOSTART ))
				update_instruction_values( zptr, lptr->target, _CORDS_SERVICE_AGENT );

			zptr = occi_remove_response ( zptr );
			
		}
		if ( generate_service_report )
		{
			fprintf(h," } }");
		}
	}

	if ( generate_service_report )
	{
		fprintf(h," ] }\n");
		fclose(h);
	}

	return(status);
}


/*	--------------------------------------------	*/
/* 	r e v e r s e _ s e r v i c e _ a c t i o n	*/
/*	--------------------------------------------	*/
/*	runs the list of linked contracts and sends 	*/
/*	action requests for processing to each of 	*/
/*	contracts in turn. configuration instruction	*/
/*	category instances will be updated at each 	*/
/*	stage of the operation to provide the newly	*/
/*	aquired values for use in building following	*/
/*	contract provisioning instances.		*/
/*	--------------------------------------------	*/
/*	The service report file will be updated as a	*/
/*	result of the operation and will reflect the	*/
/*	current state of the service and contracts.	*/
/*	--------------------------------------------	*/
/*	Optimised Configuration Instruction Proccess	*/
/*	--------------------------------------------	*/
private	int	reverse_service_action( struct cords_service * pptr, char * id, char * action )
{
	int	items=0;
	struct	occi_response * zptr;
	struct	occi_element  * eptr;
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	char			* mptr;
	char 			* wptr;
	int			status;
	FILE *			  h;
	char			buffer[1024];

	/* ------------------------------------------------------ */
	/* initialise the service report file and generate header */
	/* ------------------------------------------------------ */
	if ( generate_service_report )
	{
		sprintf(buffer,"service/%s",id);

		if (!( h = fopen(buffer,"w")))
			return(46);

		fprintf(h,"{ \"%s\" : %c%s%c, \"contracts\" : [\n",_CORDS_SERVICE,0x0022,id,0x0022 );
	}

	/* ----------------------------------------------------- */
	/* for all defined contract nodes of the current service */
	/* ----------------------------------------------------- */
	for (	pptr->contracts=0,
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
		else if ( strcmp( wptr, id ) != 0)
		{
			liberate( wptr );
			continue;
		}
		else	liberate( wptr );

		/* --------------------------------------------------- */
		/* launch / invoke the required action on the contract */
		/* --------------------------------------------------- */

		if (!(zptr = cords_invoke_action( lptr->target, action, _CORDS_SERVICE_AGENT, default_tls() )))
			status = 118;
		else
		{
			status = cords_check_invocation( zptr, (struct html_response *) 0 );
			zptr = occi_remove_response( zptr );
		}

		if ( pptr->contracts++ ) fprintf(h,",\n" );

		if ( generate_service_report )
		{
			fprintf(h,"{ \"%s\" : %c%s%c, \"attributs\" : { ",_CORDS_CONTRACT,0x0022,lptr->target,0x0022);
		}

		/* ------------------------------------------------- */
		/* retrieve the resulting contract category instance */
		/* ------------------------------------------------- */
		if ((zptr = occi_simple_get( lptr->target , _CORDS_SERVICE_AGENT, "" )) 
			!= (struct occi_response *) 0)
		{
			items=0;

			/* ---------------------------------------------------------- */
			/* first save each property of the contract category instance */
			/* to the service report file for use by co-command functions */
			/* ---------------------------------------------------------- */
			for ( eptr=zptr->first;
				eptr != (struct occi_element *) 0;
				eptr = eptr->next )
			{
				if ( generate_service_report )
				{
					if ( items++ )
						fprintf(h,",\n");

					/* ------------------------------------ */
					/* output information to service report */
					/* ------------------------------------ */
					fprintf(h,"%c%s%c: %c%s%c",
						0x0022,eptr->name,0x0022,
						0x0022,eptr->value,0x0022);	
				}
			}

			/* ---------------------------------------------------------- */
			/* update configuration instruction values from this contract */
			/* ---------------------------------------------------------- */
			update_instruction_values( zptr, lptr->target, _CORDS_SERVICE_AGENT );
			zptr = occi_remove_response ( zptr );
			
		}
		if ( generate_service_report )
		{
			fprintf(h," } }");
		}
	}

	if ( generate_service_report )
	{
		fprintf(h," ] }\n");
		fclose(h);
	}

	return(status);
}

/*	-------------------------------------------	*/
/*		s e r v i c e _ p e r i o d 		*/
/*	-------------------------------------------	*/
private	int	service_period( struct cords_service * pptr )
{
	int	status=0;
	if ( rest_valid_string( pptr->initiation ) )
	{
		if ((!( strcasecmp( pptr->initiation, "any" ) ))
		||  (!( strcasecmp( pptr->initiation, "now" ) )))
			status = 0;
		else if ( atoi( pptr->initiation ) > time((long *) 0) )
			return(1);
	}

	if ( rest_valid_string( pptr->expiration ) )
	{
		if (!( strcasecmp( pptr->expiration, "never" ) ))
			status = 0;
		else if ( atoi( pptr->expiration ) < time((long *) 0) )
			return(2);
	}
	return( status );
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
			switch ( service_period( pptr ) )
			{
			case	1	:
				return( rest_html_response( aptr, 409, "Before Service Initiation" ) );
			case	2	:
				return( rest_html_response( aptr, 410, "After Service Expiration" ) );
			case	0	:
				pptr->state = _OCCI_WORKING;
				pptr->stamp  = time((long*) 0);
				autosave_cords_service_node(pptr);
				service_action( pptr, pptr->id, _CORDS_START );
				pptr->stamp  = time((long*) 0);
				pptr->state = _OCCI_RUNNING;
				autosave_cords_service_node(pptr);
			}

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
			pptr->state = _OCCI_WORKING;
			pptr->stamp  = time((long*) 0);
			service_action( pptr, pptr->id, _CORDS_SUSPEND );
			pptr->stamp  = time((long*) 0);
			pptr->state = _OCCI_SUSPENDED;
			autosave_cords_service_node(pptr);
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
		pptr->state = _OCCI_WORKING;
		pptr->stamp  = time((long*) 0);
		autosave_cords_service_node(pptr);
		reverse_service_action( pptr, pptr->id, _CORDS_STOP  );
		service_action( pptr, pptr->id, _CORDS_START );
		pptr->stamp  = time((long*) 0);
		pptr->state = _OCCI_RUNNING;
		autosave_cords_service_node(pptr);
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}
	
/*	-------------------------------------------	*/
/* 	   	s a v e _ s e r v i c e			*/
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
	else
	{
		if ( pptr->state == _OCCI_RUNNING )
		{
			pptr->state = _OCCI_WORKING;
			pptr->stamp  = time((long*) 0);
			autosave_cords_service_node(pptr);
			service_action( pptr, pptr->id, _CORDS_SAVE );
			pptr->state = _OCCI_RUNNING;
			pptr->stamp  = time((long*) 0);
			autosave_cords_service_node(pptr);
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	   s n a p s h o t _ s e r v i c e		*/
/*	-------------------------------------------	*/
private	struct	rest_response * snapshot_service(
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
			pptr->state = _OCCI_WORKING;
			pptr->stamp  = time((long*) 0);
			autosave_cords_service_node(pptr);
			service_action( pptr, pptr->id, _CORDS_SNAPSHOT );
			pptr->state = _OCCI_RUNNING;
			pptr->stamp  = time((long*) 0);
			autosave_cords_service_node(pptr);
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	   	s t o p _ s e r v i c e			*/
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
	else
	{
		if ( pptr->state == _OCCI_RUNNING )
		{
			pptr->state = _OCCI_WORKING;
			pptr->stamp  = time((long*) 0);
			autosave_cords_service_node(pptr);
			reverse_service_action( pptr, pptr->id, _CORDS_STOP );
			pptr->stamp  = time((long*) 0);
			pptr->state = _OCCI_IDLE;
			autosave_cords_service_node(pptr);
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	   	s c a l e u p _ s e r v i c e		*/
/*	-------------------------------------------	*/
private	struct	rest_response * scaleup_service(
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
			pptr->state = _OCCI_WORKING;
			pptr->stamp  = time((long*) 0);
			autosave_cords_service_node(pptr);
			service_action( pptr, pptr->id, "scaleup" );
			pptr->stamp  = time((long*) 0);
			pptr->state = _OCCI_RUNNING;
			autosave_cords_service_node(pptr);
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	     s c a l e d o w n _ s e r v i c e		*/
/*	-------------------------------------------	*/
private	struct	rest_response * scaledown_service(
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
			pptr->state = _OCCI_WORKING;
			pptr->stamp  = time((long*) 0);
			autosave_cords_service_node(pptr);
			reverse_service_action( pptr, pptr->id, "scaledown" );
			pptr->stamp  = time((long*) 0);
			pptr->state = _OCCI_RUNNING;
			autosave_cords_service_node(pptr);
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-----------------------------------------------------------	*/
/*		d e l e t e _ s e r v i c e _ c o n t r a c t		*/
/*	-----------------------------------------------------------	*/
private	int	delete_service_contract( struct occi_category * optr, struct cords_service * pptr )
{
	struct	occi_response 	* zptr;
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	char 			  buffer[2048];
	char 			* wptr;
	/* ----------------------------------------------------- */
	/* for all defined contract nodes of the current service */
	/* ----------------------------------------------------- */
	buffer[0] = 0;
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
			if (!( buffer[0] ))
				strcpy( buffer, lptr->source );
			if ((zptr = occi_simple_delete( lptr->target, _CORDS_SERVICE_AGENT, default_tls())) != (struct occi_response *) 0)
				zptr = occi_remove_response ( zptr );
		}
	}

	/* ----------------------------- */
	/* delete links from this object */
	/* ----------------------------- */
	if ( strlen( buffer ) )
		if ((zptr = occi_delete_links( buffer, _CORDS_SERVICE_AGENT, default_tls())) != (struct occi_response *) 0)
				zptr = occi_remove_response ( zptr );

	/* ------------------------------- */
	/* remove service description file */
	/* ------------------------------- */
	if ( generate_service_report )
	{
		sprintf(buffer,"rm -f service/%s",pptr->id);
		system(buffer);
	}

	return(0);
}

/*	------------------------------------------------	*/
/*		s e r v i c e _ d u r a t i o n			*/
/*	------------------------------------------------	*/
/*	[+] 		increment				*/
/*	[ <integer>D ]	day count				*/
/*	[ <integer>H ]	hour count				*/
/*	[ <integer>M ]	minute count				*/
/*	[ <integer>S ]	second count				*/
/*	------------------------------------------------	*/
private	int	service_duration( char * sptr )
{
	int	result=0;
	int	value=0;
	if ( sptr )
	{
		while ( *sptr )
		{
			while (( *sptr == ' ' ) || ( *sptr == '+' ))
				sptr++;
			if (!( *sptr ))
				break;
			else	value = atoi(sptr);
			while ((*sptr >= '0') && ( *sptr <= '9'))
				sptr++;
			if (( *sptr == 'd' )
			||  ( *sptr == 'D' ))
			{
				sptr++;
				result += (value * (24 * 60 * 60 ));
			}
			else if (( *sptr == 'h' )
			     ||  ( *sptr == 'H' ))
			{
				sptr++;
				result += (value * (60 * 60 ));
			}
			else if (( *sptr == 'm' )
			     ||  ( *sptr == 'M' ))
			{
				sptr++;
				result += (value * 60 );
			}
			else if (( *sptr == 's' )
			     ||  ( *sptr == 'S' ))
			{
				sptr++;
				result += value;
			}
			else	break;
		}
	}
	return( result );
}

/*	------------------------------------------------	*/
/*	e s t a b l i s h _ s e r v i c e _ p e r i o d		*/
/*	------------------------------------------------	*/
private	void	establish_service_period( struct cords_service * pptr )
{
	int	seconds=0;
	char	buffer[256];
	char *	sptr;
	struct	occi_element * dptr;
	struct	occi_response * zptr;

	if ( rest_valid_string( pptr->sla ) )
	{
		/* ---------------------- */
		/* retrieve the agreement */
		/* ---------------------- */
		if (!( zptr = occi_simple_get( pptr->sla, _CORDS_CONTRACT_AGENT, default_tls() )))
			return;
		else
		{
			/* ------------------------------------------------ */
			/* locate the SLA contract initiation date and time */
			/* ------------------------------------------------ */
			if ((dptr = occi_locate_element( zptr->first, "occi.agreement.initiation" )) != (struct occi_element *) 0)
			{
				if ( pptr->initiation )
					liberate( pptr->initiation );
				if (!( pptr->initiation = allocate_string( dptr->value ) ))
					return;
			}

			/* ------------------------------------------------ */
			/* locate the SLA contract expiration date and time */
			/* ------------------------------------------------ */
			if ((dptr = occi_locate_element( zptr->first, "occi.agreement.expiration" )) != (struct occi_element *) 0)
			{
				if ( pptr->expiration )
					liberate( pptr->expiration );
				if (!( pptr->expiration = allocate_string( dptr->value ) ))
					return;
			}
			zptr = occi_remove_response( zptr );
		}
	}

	if (!( rest_valid_string( pptr->initiation ) ))
		if (!( pptr->initiation = allocate_string( "now" ) ))
			return;

	if (!( rest_valid_string( pptr->expiration ) ))
		if (!( pptr->expiration = allocate_string( "never" ) ))
			return;

	/* ------------------------ */
	/* detect and establish now */
	/* ------------------------ */
	if (!( strcasecmp(( sptr = pptr->initiation ), "now" ) ))
	{
		liberate( pptr->initiation );
		sprintf(buffer,"%u",time((long*)0));
		if (!( pptr->initiation = allocate_string( buffer ) ))
			return;

	}
	else if ( *sptr == '+' )
	{
		seconds = service_duration( (sptr+1) );
		liberate( pptr->initiation );
		sprintf(buffer,"%u",time((long*)0)+seconds);
		if (!( pptr->initiation = allocate_string( buffer ) ))
			return;
	}

	/* ----------------------------------- */
	/* detect and establish expiration now */
	/* ----------------------------------- */
	if ((sptr = pptr->expiration) != (char *) 0)
	{
		if (!( strcasecmp( sptr,"never" ) ))
			return;
		else if ( *pptr->expiration == '+' )
		{
			seconds = service_duration( (sptr+1) );
			liberate( pptr->expiration );
			sprintf(buffer,"%u",time((long*)0)+seconds);
			if (!( pptr->expiration = allocate_string( buffer ) ))
				return;
		}
	}
	return;
}

/*	-------------------------------------------	*/
/* 	  c r e a t e _ s e r v i c e _ g r a p h 	*/
/*	-------------------------------------------	*/
private	int	create_service_graph(struct occi_category * optr, struct cords_service * pptr )
{
	char *	result;
	char *	host;
	char	buffer[1024];

	/* ------------------------------------------- */
	/* check parameters and build full instance ID */
	/* ------------------------------------------- */
	if (!( pptr ))
		return(118);
	else if (!( pptr->id ))
		return(118);
	else if (!( host = default_identity()))
		return(118);
	else	sprintf(buffer,"%s/%s/%s",host,_CORDS_SERVICE,pptr->id);

	establish_service_period( pptr );

	/* --------------------------------------------- */
	/* attempt to build a new service instance graph */
	/* --------------------------------------------- */
	if (!(result = cords_service_broker(
			_DEFAULT_PUBLISHER,
			buffer,
			pptr->name,
			pptr->plan,
			pptr->manifest,
			pptr->sla,
			&pptr->vender,
			_CORDS_CONTRACT_AGENT,
			default_tls(), 
			(struct xml_element **) 0 ) ))
		return( 999 );
	else
	{
		/* ------------------------------------- */
		/* store the resolve account information */
		/* ------------------------------------- */
		if ( pptr->account ) pptr->account = liberate( pptr->account );
		if (!( pptr->account = allocate_string( result ) ))
			return( 999 );
		else	return( 0 );
	}
}

/*	-------------------------------------------	*/
/* 	      c r e a t e _ s e r v i c e  		*/
/*	-------------------------------------------	*/
private	int	create_service(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_service * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( pptr->sla ))
		return(0);
	else if (!( strlen( pptr->sla ) ))
		return(0);
	else if (!( rest_valid_string( pptr->sla ) ))
		return(0);
	else	return( create_service_graph( optr, pptr ) ); 
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ s e r v i c e  		*/
/*	-------------------------------------------	*/
private	int	retrieve_service(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_service * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ s e r v i c e  		*/
/*	-------------------------------------------	*/
private	int	update_service(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_service * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ s e r v i c e	  	*/
/*	-------------------------------------------	*/
private	int	delete_service(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_service * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(delete_service_contract(optr, pptr));
}

private	struct	occi_interface	service_interface = {
	create_service,
	retrieve_service,
	update_service,
	delete_service
	};


/*	------------------------------------------------------------------	*/
/*		b r o k e r _ s e r v i c e _ b u i l d e r			*/
/*	------------------------------------------------------------------	*/
private	struct	occi_category *	broker_service_builder( char * domain, char * category )
{
	struct	occi_category * optr;
	initialise_occi_resolver( _DEFAULT_PUBLISHER, (char *) 0, (char *) 0, (char *) 0 );
	if (!( optr = occi_cords_service_builder( domain ,category ) ))
		return( optr );
	else
	{
		optr->callback  = &service_interface;

		if (!( optr = occi_add_action( optr,_CORDS_START,"",start_service)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_SUSPEND,"",suspend_service)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_RESTART,"",restart_service)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_SAVE,"",save_service)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_SNAPSHOT,"",snapshot_service)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_STOP,"",stop_service)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"scaleup","",scaleup_service)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"scaledown","",scaledown_service)))
			return( optr );
		else	return( optr );
	}
}

#endif	/* _broker_service_c */
	/* ------------------ */

