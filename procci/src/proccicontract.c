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
#ifndef	_procci_contract_c
#define	_procci_contract_c

#include "occiclient.h"
#include "occiresolver.h"
#include "cp.h"

#define	_MAX_PROVIDERS	6

private	char *	occi_providers[_MAX_PROVIDERS] = {
	(char *) _CORDS_OPENSTACK,
	(char *) _CORDS_OPENNEBULA,
	(char *) _CORDS_WINDOWSAZURE,
	(char *) _CORDS_PROACTIVE,
	(char *) _CORDS_SLAPOS,
	(char *) "amazon"
	};


/*	---------------------------------------------------------	*/
/*	r e t r i e v e _ p r o v i d e r _ i n f o r m a t i o n	*/
/*	---------------------------------------------------------	*/
/*	retrieve the provider specific contract instance details	*/
/*	the hostname, password and instance reference to be used	*/
/*	to access and configure the provisioned instance.		*/
/*	---------------------------------------------------------	*/
private	int	retrieve_provider_information( struct cords_contract * pptr )
{
	struct	occi_response *	zptr;
	struct	occi_element *  fptr;
	struct	occi_client *   cptr;
	struct	occi_request *  qptr;
	char	*		sptr;
	char	buffer[1024];

	sprintf(buffer,"occi.%s.",pptr->profile);

	if (!( zptr = occi_simple_get( pptr->provider, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( 400 );
	for (	fptr = zptr->first;
		fptr != (struct occi_element *) 0;
		fptr = fptr->next )
	{
		if (!( sptr = fptr->name ))
			continue;
		else if ( strncmp( sptr, buffer,strlen(buffer) ) != 0 )
			continue;
		else 	sptr += strlen( buffer );
		if (!( strcmp( sptr,"hostname" ) ))
		{
			if ( pptr->hostname ) pptr->hostname = liberate( pptr->hostname );
			if (!( pptr->hostname = occi_unquoted_value( fptr->value ) ))
		 		return( 400 );
		}
		else if (!( strcmp( sptr,"rootpass" ) ))
		{
			if ( pptr->rootpass ) pptr->rootpass = liberate( pptr->rootpass );
			if (!( pptr->rootpass = occi_unquoted_value( fptr->value ) ))
				return( 400 );
		}
		else if (!( strcmp( sptr,"reference" ) ))
		{
			if ( pptr->reference ) pptr->reference = liberate( pptr->reference );
			if (!( pptr->reference = occi_unquoted_value( fptr->value ) ))
				return( 400 );
		}
	}
	return( 0 );
}

/*	-------------------------------------------	*/
/* 	 c o n t r a c t _ i n s t r u c t i o n s	*/
/*	-------------------------------------------	*/
/*	this function retrives the list of contract	*/
/*	configuration instructions selected by the 	*/
/*	target or configured machine contract id	*/
/*	and this with an aim to registering the		*/
/*	provider contract id allowing recognition	*/
/*	of the same collection through the provider	*/
/*	contract id.					*/
/*	-------------------------------------------	*/
private	int	contract_instructions( char * contract, char * provision )
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

	/* ---------------------------------------------------------------- */
	/* select / retrieve instruction category service provider identity */
	/* ---------------------------------------------------------------- */
	if (!( ihost = occi_resolve_category_provider( _CORDS_INSTRUCTION, _CORDS_CONTRACT_AGENT, default_tls() ) ))
	 	return( 401 );

	/* ---------------------------------------------------------------- */
	/* retrieve the collection of instructions for the current contract */
	/* ---------------------------------------------------------------- */
	sprintf(buffer,"%s/%s/",ihost,_CORDS_INSTRUCTION);
	liberate( ihost );
	length = strlen(buffer);

	if (!( kptr = occi_create_client( buffer, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( 401 );

	else if (!(qptr = occi_create_request( 
			kptr, 
			kptr->target->object, 
			_OCCI_NORMAL )))
		return(401);
	else if (!( fptr = occi_request_element( 
			qptr,
			"occi.instruction.target", contract ) ))
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

	/* ---------------------------------------------------- */
	/* for each of the instructions of the current contract */
	/* ---------------------------------------------------- */
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

		/* ----------------------------------------- */
		/* retrieve the current instruction instance */
		/* ----------------------------------------- */
		if (( zptr = occi_simple_get( buffer, _CORDS_CONTRACT_AGENT, default_tls() )) != (struct occi_response *) 0)
		{
			if (!(fptr = occi_locate_element( zptr->first, "occi.instruction.provision" )))
				zptr = occi_remove_response ( zptr );
			else
			{
				/* ---------------------------------------------------------------------- */
				/* update this instruction instance with the provider contract identifier */
				/* ---------------------------------------------------------------------- */
				if ( fptr->value ) fptr->value = liberate( fptr->value );
				fptr->value = allocate_string( provision );
				zzptr = occi_simple_put( buffer, zptr->first, _CORDS_CONTRACT_AGENT, default_tls() );
				zzptr = occi_remove_response ( zzptr );
				zptr = occi_remove_response ( zptr );
			}
		}

		/* ----------------------- */
		/* quick reset of base url */
		/* ----------------------- */
		buffer[length] = 0;
	}

	yptr = occi_remove_response ( yptr );

	return(0);
}

/*	--------------------------------------------	*/
/*	     i s _ c o m m o n _ c o n t r a c t	*/
/*	--------------------------------------------	*/
private	int	is_common_contract( struct cords_contract * pptr )
{
	if (!( pptr ))
		return( 0 );
	else if (!( pptr->common ))
		return( 0 );
	else if (!( strlen( pptr->common ) ))
		return( 0 );
	else if (!( strcmp( pptr->common, _CORDS_NULL ) ))
		return( 0 );
	else	return( 1 );
}

/*	-------------------------------------------	*/
/* 	       s t a r t _ c o n t r a c t		*/
/*	-------------------------------------------	*/
private	struct	rest_response * start_contract(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_contract * pptr;
	char	fullid[2048];
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		if ( pptr->state == _OCCI_IDLE )
		{
			if ( is_common_contract( pptr ) )
			{
				cords_invoke_action( pptr->common, _CORDS_START, 
					_CORDS_CONTRACT_AGENT, default_tls() );
			}
			else if ((!( pptr->type ))
			||  (!( strcmp( pptr->type, _CORDS_SIMPLE ) )))
			{
				sprintf(fullid,"%s/%s/%s",Procci.identity,_CORDS_CONTRACT,pptr->id);
				contract_instructions( fullid, pptr->provider );
				cords_invoke_action( pptr->provider, _CORDS_START, 
					_CORDS_CONTRACT_AGENT, default_tls() );
				retrieve_provider_information( pptr );
			}
			else if ( pptr->service )
			{
				cords_invoke_action( pptr->service, _CORDS_START, 
					_CORDS_CONTRACT_AGENT, default_tls() );
			}
			pptr->when  = time((long*)0); 
			pptr->state = _OCCI_RUNNING;
			autosave_cords_contract_nodes();
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}


/*	-------------------------------------------	*/
/* 	   r e s t a r t _ c o n t r a c t		*/
/*	-------------------------------------------	*/
private	struct	rest_response * restart_contract(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_contract * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		if ( pptr->state == _OCCI_SUSPENDED )
		{
			if ( is_common_contract( pptr ) )
			{
				cords_invoke_action( pptr->common, _CORDS_RESTART, 
					_CORDS_CONTRACT_AGENT, default_tls() );
			}
			else if ((!( pptr->type ))
			||  (!( strcmp( pptr->type, _CORDS_SIMPLE ) )))
			{
				cords_invoke_action( pptr->provider, _CORDS_RESTART, 
					_CORDS_CONTRACT_AGENT, default_tls() );
			}
			else if ( pptr->service )
			{
				cords_invoke_action( pptr->service, _CORDS_RESTART, 
					_CORDS_CONTRACT_AGENT, default_tls() );
			}
			pptr->when  = time((long*)0); 
			pptr->state = _OCCI_RUNNING;
			autosave_cords_contract_nodes();
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}



/*	-------------------------------------------	*/
/* 	   s u s p e n d _ c o n t r a c t		*/
/*	-------------------------------------------	*/
private	struct	rest_response * suspend_contract(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_contract * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		if ( pptr->state == _OCCI_RUNNING )
		{
			if ( is_common_contract( pptr ) )
			{
				cords_invoke_action( pptr->common, _CORDS_SUSPEND,
					_CORDS_CONTRACT_AGENT, default_tls() );
			}
			else if ((!( pptr->type ))
			||  (!( strcmp( pptr->type, _CORDS_SIMPLE ) )))
			{
				cords_invoke_action( pptr->provider, _CORDS_SUSPEND, 
					_CORDS_CONTRACT_AGENT, default_tls() );
			}
			else if ( pptr->service )
			{
				cords_invoke_action( pptr->service, _CORDS_SUSPEND, 
					_CORDS_CONTRACT_AGENT, default_tls() );
			}
			pptr->when  = time((long*) 0);
			pptr->state = _OCCI_SUSPENDED;
			autosave_cords_contract_nodes();
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	   	s t o p _ c o n t r a c t		*/
/*	-------------------------------------------	*/
private	struct	rest_response * stop_contract(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_contract * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	{
		if ( pptr->state != _OCCI_IDLE )
		{
			if ( is_common_contract( pptr ) )
			{
				cords_invoke_action( pptr->common, _CORDS_STOP,
					_CORDS_CONTRACT_AGENT, default_tls() );
			}
			else if ((!( pptr->type ))
			||  (!( strcmp( pptr->type, _CORDS_SIMPLE ) )))
			{
				cords_invoke_action( pptr->provider, _CORDS_STOP, 
					_CORDS_CONTRACT_AGENT, default_tls() );
			}
			else if ( pptr->service )
			{
				cords_invoke_action( pptr->service, _CORDS_STOP, 
					_CORDS_CONTRACT_AGENT, default_tls() );
			}
			if (pptr->reference) pptr->reference = liberate( pptr->reference );
			if (pptr->rootpass ) pptr->rootpass  = liberate( pptr->rootpass  );
			if (pptr->hostname ) pptr->hostname  = liberate( pptr->hostname  );
			pptr->reference =allocate_string("");
			pptr->rootpass  =allocate_string("");
			pptr->hostname  =allocate_string("");
			pptr->when  = time((long*) 0);
			pptr->state = _OCCI_IDLE;
			autosave_cords_contract_nodes();
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	   	s a v e _ c o n t r a c t		*/
/*	-------------------------------------------	*/
private	struct	rest_response * save_contract(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_contract * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		if ( pptr->state != _OCCI_IDLE )
		{
			if ( is_common_contract( pptr ) )
			{
				cords_invoke_action( pptr->common, _CORDS_SAVE, 
					_CORDS_CONTRACT_AGENT, default_tls() );
			}
			else if ((!( pptr->type ))
			||  (!( strcmp( pptr->type, _CORDS_SIMPLE ) )))
			{
				cords_invoke_action( pptr->provider, _CORDS_SAVE, 
					_CORDS_CONTRACT_AGENT, default_tls() );
			}
			else if ( pptr->service )
			{
				cords_invoke_action( pptr->service, _CORDS_SAVE, 
					_CORDS_CONTRACT_AGENT, default_tls() );
			}
			pptr->when  = time((long*) 0);
			autosave_cords_contract_nodes();
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	------------------------------------------------------------------	*/
/*		p r o c c i _ c o n t r a c t _ b u i l d e r			*/
/*	------------------------------------------------------------------	*/
private	struct	occi_category *	procci_contract_builder( char * domain, char * category )
{
	struct	occi_category * optr;
	initialise_occi_resolver( _DEFAULT_PUBLISHER, (char *) 0, (char *) 0, (char *) 0 );
	if (!( optr = occi_cords_contract_builder( domain ,category ) ))
		return( optr );
	else if (!( optr = occi_add_action( optr,_CORDS_START,"",start_contract)))
		return( optr );
	else if (!( optr = occi_add_action( optr,_CORDS_SUSPEND,"",suspend_contract)))
		return( optr );
	else if (!( optr = occi_add_action( optr,_CORDS_RESTART,"",restart_contract)))
		return( optr );
	else if (!( optr = occi_add_action( optr,_CORDS_SAVE,"",save_contract)))
		return( optr );
	else if (!( optr = occi_add_action( optr,_CORDS_STOP,"",stop_contract)))
		return( optr );
	else	return( optr );
}

#endif	/* _procci_contract_c */
	/* ----------------- */

