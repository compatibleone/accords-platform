/* ------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                   */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>    */
/* --------------------------------------------------------------------*/
/*  This is free software; you can redistribute it and/or modify it    */
/*  under the terms of the GNU Lesser General Public License as        */
/*  published by the Free Software Foundation; either version 2.1 of   */
/*  the License, or (at your option) any later version.                */
/*                                                                     */
/*  This software is distributed in the hope that it will be useful,   */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of     */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   */
/*  Lesser General Public License for more details.                    */
/*                                                                     */
/*  You should have received a copy of the GNU Lesser General Public   */
/*  License along with this software; if not, write to the Free        */
/*  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA */
/*  02110-1301 USA, or see the FSF site: http://www.fsf.org.           */
/* --------------------------------------------------------------------*/
#ifndef	_broker_service_c
#define	_broker_service_c

#include "occiresolver.h"
#include "cp.h"
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

	/* ------------------------------------------------------ */
	/* initialise the service report file and generate header */
	/* ------------------------------------------------------ */
	if ( generate_service_report )
	{
		sprintf(buffer,"service/%s",id);

		if (!( h = fopen(buffer,"w")))
			return(46);

		fprintf(h,"{ %s: %c%s%c, contracts: [\n",_CORDS_SERVICE,0x0022,id,0x0022 );
	}

	/* ----------------------------------------------------- */
	/* for all defined contract nodes of the current service */
	/* ----------------------------------------------------- */
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

		/* --------------------------------------------------- */
		/* launch / invoke the required action on the contract */
		/* --------------------------------------------------- */

		cords_invoke_action( lptr->target, action, _CORDS_SERVICE_AGENT, default_tls() );

		if ( contracts++ ) fprintf(h,",\n" );

		if ( generate_service_report )
		{
			fprintf(h,"{ %s: %c%s%c, attributs: { ",_CORDS_CONTRACT,0x0022,lptr->target,0x0022);
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
	{
		if ( pptr->state != _OCCI_IDLE )
		{
			service_action( pptr->id, _CORDS_SNAPSHOT );
			pptr->when  = time((long*) 0);
			autosave_cords_service_nodes();
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

/*	-----------------------------------------------------------	*/
/*		d e l e t e _ s e r v i c e _ c o n t r a c t		*/
/*	-----------------------------------------------------------	*/
private	int	delete_service_contract( struct occi_category * optr, struct cords_service * pptr )
{
	struct	occi_link_node  * nptr;
	struct	cords_xlink	* lptr;
	char 	buffer[1024];
	char 			* wptr;
	/* ----------------------------------------------------- */
	/* for all defined contract nodes of the current service */
	/* ----------------------------------------------------- */
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
			occi_simple_delete( lptr->target, _CORDS_SERVICE_AGENT, default_tls() );
		}
	}

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

/*	-------------------------------------------	*/
/* 	      c r e a t e _ s e r v i c e  		*/
/*	-------------------------------------------	*/
private	int	create_service(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_service * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return( 0 ); 
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ s e r v i c e  		*/
/*	-------------------------------------------	*/
private	int	retrieve_service(struct occi_category * optr, void * vptr)
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
private	int	update_service(struct occi_category * optr, void * vptr)
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
private	int	delete_service(struct occi_category * optr, void * vptr)
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
		else	return( optr );
	}
}

#endif	/* _broker_service_c */
	/* ------------------ */

