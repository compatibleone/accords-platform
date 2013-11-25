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
#ifndef	_procci_contract_c
#define	_procci_contract_c

#include "occiclient.h"
#include "occiresolver.h"
#include "cp.h"

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
		return( 404 );
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
		else if (!( strcmp( sptr,"workload" ) ))
		{
			if ( pptr->workload ) pptr->workload = liberate( pptr->workload );
			if (!( pptr->workload = occi_unquoted_value( fptr->value ) ))
				return( 400 );
		}
		else if (!( strcmp( sptr,"blob" ) ))
		{
			if ( pptr->blob ) pptr->blob = liberate( pptr->blob );
			if (!( pptr->blob = occi_unquoted_value( fptr->value ) ))
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
private	int	contract_instructions( struct cords_contract * pptr, char * contract, char * provision )
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
	for (	pptr->instructions=0,
		eptr = yptr->first;
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
				pptr->instructions++;
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
	else if (!( rest_valid_string( pptr->common ) ))
		return( 0 );
	else	return( 1 );
}

/*	--------------------------------------------	*/
/*	     i s _ v a l i d _ p r o v i d e r		*/
/*	--------------------------------------------	*/
private	int	is_valid_provider( struct cords_contract * pptr )
{
	if (!( pptr ))
		return( 0 );
	else if (!( rest_valid_string( pptr->provider ) ))
		return( 0 );
	else	return( 1 );
}

/*	--------------------------------------------	*/
/*		i s _ c o m m o n _ s c o p e		*/
/*	--------------------------------------------	*/
private	int	is_common_scope( struct cords_contract * pptr )
{
	if (!( pptr ))
		return( 0 );
	else if (!( rest_valid_string( pptr->scope ) ))
		return( 0 );
	else if (!( strcmp( pptr->scope, _CORDS_COMMON ) ))
		return( 1 );
	else	return( 0 );
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
	int	status;
	struct	occi_response * zptr;
	struct	cords_contract * pptr;
	char	fullid[2048];

	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		if ( pptr->state == _OCCI_IDLE )
		{
			pptr->stamp  = time((long*)0); 
			/* ---------------------------------------- */
			/* any common node definitions pass by here */
			/* ---------------------------------------- */
			if ( is_common_contract( pptr ) )
			{
				if (!(zptr = cords_invoke_action( pptr->common, _CORDS_START,
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}
			/* ---------------------------------------- */
			/* basic type infrastructure and image node */
			/* ---------------------------------------- */
			else if ((!( rest_valid_string( pptr->type ) ))
			||  (!( strcmp( pptr->type, _CORDS_SIMPLE ) )))
			{
				sprintf(fullid,"%s/%s/%s",Procci.identity,_CORDS_CONTRACT,pptr->id);
				contract_instructions( pptr, fullid, pptr->provider );
				if (!(zptr = cords_invoke_action( pptr->provider, _CORDS_START, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
				retrieve_provider_information( pptr );
			}
			/* ---------------------------------------- */
			/* extension category type defined contract */
			/* ---------------------------------------- */
			else if (( rest_valid_string( pptr->category ) != 0)
			     &&  ( strcmp( pptr->category, _CORDS_MANIFEST ) != 0))
			{
				sprintf(fullid,"%s/%s/%s",Procci.identity,_CORDS_CONTRACT,pptr->id);
				contract_instructions( pptr, fullid, pptr->provider );
				if (!(zptr = cords_invoke_action( pptr->provider, _CORDS_START, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
				retrieve_provider_information( pptr );
			}
			/* ---------------------------------------- */
			/* a standard manifest defined complex node */
			/* ---------------------------------------- */
			else if ( rest_valid_string( pptr->service ) != 0 )
			{
				if (!(zptr = cords_invoke_action( pptr->service, _CORDS_START, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}

			/* ------------------------ */
			/* consume placement quotas */
			/* ------------------------ */
			if ( rest_valid_string( pptr->placement ) != 0 )
			{
				if ( cords_validate_action( 
					(struct occi_client *) 0,
					_CORDS_PLACEMENT, _CORDS_CONSUME, 
					pptr->placement ) )
				{
					if (!(zptr = cords_invoke_action( pptr->placement, "consume", 
						_CORDS_CONTRACT_AGENT, default_tls() )))
						return( rest_html_response( aptr, 900, "Failure to Consume Placement Quota" ) );
					else if ((status = zptr->response->status) > 299 )
					{
						zptr = occi_remove_response ( zptr );
						return( rest_html_response( aptr, status, "Consume Quota Action Invocation Error" ) );
					}
					else	zptr = occi_remove_response ( zptr );
				}
			}

			/* ------------------------ */
			/* start monitoring session */
			/* ------------------------ */
			if ( rest_valid_string( pptr->session ) != 0)
			{
				if (!(zptr = cords_invoke_action( pptr->session, _CORDS_START, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}
			pptr->startduration = (time((long*)0) - pptr->stamp);
			pptr->stamp  = time((long*)0); 
			pptr->state = _OCCI_RUNNING;
			pptr->commons= 1;
			autosave_cords_contract_node(pptr);
		}
		else if ( is_common_scope( pptr ) )
		{
			pptr->commons++;
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	   r e s o l v e _ c o n t r a c t		*/
/*	-------------------------------------------	*/
private	struct	rest_response * resolve_contract(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	int	status;
	struct	occi_response * zptr;
	struct	cords_contract * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( rest_valid_string( pptr->service ) != 0 )
	 	return( rest_html_response( aptr, 200, "OK" ) );
	else if ( is_common_contract( pptr ) )
	{
		retrieve_provider_information( pptr );
	 	return( rest_html_response( aptr, 200, "OK" ) );
	}
	else if ((!( rest_valid_string( pptr->type ) )) ||  (!( strcmp( pptr->type, _CORDS_SIMPLE ) )))
	{
		retrieve_provider_information( pptr );
	 	return( rest_html_response( aptr, 200, "OK" ) );
	}
	else	return( rest_html_response( aptr, 200, "OK" ) );
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
	int	status;
	struct	occi_response * zptr;
	struct	cords_contract * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		if ( pptr->state == _OCCI_SUSPENDED )
		{
			if ( is_common_contract( pptr ) )
			{
				if (!(zptr = cords_invoke_action( pptr->common, _CORDS_RESTART, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}
			else if ((!( rest_valid_string( pptr->type ) ))
			||  (!( strcmp( pptr->type, _CORDS_SIMPLE ) )))
			{
				if (!(zptr = cords_invoke_action( pptr->provider, _CORDS_RESTART, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}
			else if (( rest_valid_string( pptr->category ) != 0)
			     && (!( strcmp( pptr->category, _CORDS_MANIFEST ) )))
			{
				if (!(zptr = cords_invoke_action( pptr->provider, _CORDS_RESTART, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}
			else if ( rest_valid_string( pptr->service ) != 0 )
			{
				if (!(zptr = cords_invoke_action( pptr->service, _CORDS_RESTART, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}
			pptr->stamp  = time((long*)0); 
			pptr->state = _OCCI_RUNNING;
			autosave_cords_contract_node(pptr);
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
	int	status;
	struct	occi_response * zptr;
	struct	cords_contract * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		if ( pptr->state == _OCCI_RUNNING )
		{
			if ( is_common_contract( pptr ) )
			{
				if (!(zptr = cords_invoke_action( pptr->common, _CORDS_SUSPEND,
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}
			else if ((!( rest_valid_string( pptr->type ) ))
			||  (!( strcmp( pptr->type, _CORDS_SIMPLE ) )))
			{
				if (!(zptr = cords_invoke_action( pptr->provider, _CORDS_SUSPEND, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}
			else if (( rest_valid_string( pptr->category ) != 0)
			     && (!( strcmp( pptr->category, _CORDS_MANIFEST ) )))
			{
				if (!(zptr = cords_invoke_action( pptr->provider, _CORDS_SUSPEND, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}
			else if ( rest_valid_string( pptr->service ) )
			{
				if (!(zptr = cords_invoke_action( pptr->service, _CORDS_SUSPEND, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}
			pptr->stamp  = time((long*) 0);
			pptr->state = _OCCI_SUSPENDED;
			autosave_cords_contract_node(pptr);
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/*		r e s e t _ c o n t r a c t		*/
/*	-------------------------------------------	*/
private	void	reset_contract( struct cords_contract * pptr )
{
	if (pptr->reference) pptr->reference = liberate( pptr->reference );
	if (pptr->rootpass ) pptr->rootpass  = liberate( pptr->rootpass  );
	if (pptr->hostname ) pptr->hostname  = liberate( pptr->hostname  );
	if (pptr->scaling  ) pptr->scaling   = liberate( pptr->scaling   );
	if (pptr->workload ) pptr->workload  = liberate( pptr->workload  );
	if (pptr->blob ) pptr->blob  = liberate( pptr->blob  );
	pptr->reference = allocate_string("");
	pptr->rootpass  = allocate_string("");
	pptr->hostname  = allocate_string("");
	pptr->scaling   = allocate_string("");
	pptr->workload  = allocate_string("");
	pptr->blob  = allocate_string("");
	pptr->stamp  = time((long*) 0);
	pptr->state = _OCCI_IDLE;
	return;
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
	int	status;
	struct	occi_response * zptr;
	struct	cords_contract * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		if ( pptr->state != _OCCI_IDLE )
		{
			pptr->stamp  = time((long*)0); 
			/* ------------------------ */
			/* stop monitoring session */
			/* ------------------------ */
			if ( rest_valid_string( pptr->session ) != 0 )
			{
				if (!(zptr = cords_invoke_action( pptr->session, _CORDS_STOP, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}
			if ( pptr->commons )
				pptr->commons--;
			if ( pptr->commons )
				return( rest_html_response( aptr, 200, "OK" ) );
			else if ( is_common_contract( pptr ) )
			{
				if (!(zptr = cords_invoke_action( pptr->common, _CORDS_STOP,
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}
			else if ((!( rest_valid_string( pptr->type ) ))
			||  (!( strcmp( pptr->type, _CORDS_SIMPLE ) )))
			{
				if (!(zptr = cords_invoke_action( pptr->provider, _CORDS_STOP, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}
			else if (( rest_valid_string( pptr->category ) != 0)
			     && ( strcmp( pptr->category, _CORDS_MANIFEST ) != 0 ))
			{
				if (!(zptr = cords_invoke_action( pptr->provider, _CORDS_STOP, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}
			else if ( rest_valid_string( pptr->service ) != 0 )
			{
				if (!(zptr = cords_invoke_action( pptr->service, _CORDS_STOP, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}
			/* ------------------------ */
			/* restore placement quotas */
			/* ------------------------ */
			if ( rest_valid_string( pptr->placement ) != 0 )
			{
				if ( cords_validate_action( 
					(struct occi_client *) 0,
					_CORDS_PLACEMENT, _CORDS_RESTORE,
					pptr->placement ) )
				{
					if (!(zptr = cords_invoke_action( pptr->placement, _CORDS_RESTORE, 
						_CORDS_CONTRACT_AGENT, default_tls() )))
						return( rest_html_response( aptr, 900, "Failure to Restore Placement Quota" ) );
					else if ((status = zptr->response->status) > 299 )
					{
						zptr = occi_remove_response ( zptr );
						return( rest_html_response( aptr, status, "Restore Quota Action Invocation Error" ) );
					}
					else	zptr = occi_remove_response ( zptr );
				}
			}
			pptr->stopduration = (time((long*)0) - pptr->stamp);
			reset_contract( pptr );
			autosave_cords_contract_node(pptr);
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}


/*	-------------------------------------------	*/
/*	    c o n t r a c t _ c a n _ s c a l e		*/
/*	-------------------------------------------	*/
private	char *	contract_can_scale( struct cords_contract * pptr )
{
	struct	rest_client * cptr;
	struct	occi_response * zptr;
	char	buffer[2048];
	char *	prefix;
	char *	vptr;
	struct	occi_element * eptr;
	if ( rest_valid_string( pptr->scaling ) != 0 )
		return( pptr->scaling );
	else if (!( rest_valid_string( pptr->hostname ) ))
		return( (char *) 0 );
	else if (!( prefix = rest_http_prefix() ))
		return( (char *) 0 );
	else if (!( rest_test_client( pptr->hostname, 8386, default_tls(), 1, 1 ) ))
		return( (char *) 0 );
	else
	{
		sprintf(buffer,"%s://%s:8386/job/",prefix,pptr->hostname);
		if (!( zptr = occi_simple_get( buffer,_CORDS_CONTRACT_AGENT, default_tls() )))
			return( (char *) 0 );
		else if (!( zptr->response ))
			return( (char *) 0 );
		else if ( zptr->response->status != 200 )
			return( (char *) 0 );
		else if (!( eptr = zptr->first ))
			return((char *) 0);
		else if (!( eptr->value ))
			return((char *) 0);
		else if (!( vptr = allocate_string( eptr->value ) ))
			return((char *) 0);
		else if (!( vptr = occi_unquoted_value( vptr ) ))
			return((char *) 0);
		else if (!( pptr->scaling = vptr ))
			return((char *) 0);
		else
		{
			autosave_cords_contract_node(pptr);
			return( pptr->scaling );
		}
	}
}


/*	-------------------------------------------	*/
/* 	   	s c a l e u p _ c o n t r a c t		*/
/*	-------------------------------------------	*/
private	struct	rest_response * scaleup_contract(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	int	status;
	struct	occi_response * zptr;
	struct	cords_contract * pptr;
	char *	target=(char *) 0;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		if ( pptr->state != _OCCI_IDLE )
		{
			/* ------------------ */
			/* dont scale commons */
			/* ------------------ */
			if ( pptr->commons > 1 )
				return( rest_html_response( aptr, 202, "Accepted" ) );
			else if ( is_common_contract( pptr ) )
				return( rest_html_response( aptr, 202, "Accepted" ) );
			else if ((!( rest_valid_string( pptr->type ) ))
			||  (!( strcmp( pptr->type, _CORDS_SIMPLE ) )))
			{
				/* ------------------------------ */
				/* detect scale up interface here */
				/* ------------------------------ */
				if (!( target = contract_can_scale( pptr ) ))
					return( rest_html_response( aptr, 202, "Accepted" ) );
				else
				{
					if (!(zptr = cords_invoke_action( target, "scaleup", 
						_CORDS_CONTRACT_AGENT, default_tls() )))
							return( rest_html_response( aptr, 900, "ScaleUp Invocation Failure" ) );
					else if ((status = zptr->response->status) > 299 )
					{
						zptr = occi_remove_response ( zptr );
						return( rest_html_response( aptr, status, "Scaleup Invocation Error" ) );
					}
					else	zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, 200, "OK" ) );
				}
			}
			else if (( rest_valid_string( pptr->category ) != 0)
			     && ( strcmp( pptr->category, _CORDS_MANIFEST ) != 0 ))
				return( rest_html_response( aptr, 202, "Accepted" ) );
			else if ( rest_valid_string( pptr->service ) != 0 )
			{
				/* ---------------------------------- */
				/* propogate scale up to service here */
				/* ---------------------------------- */
				if (!(zptr = cords_invoke_action( pptr->service, "scaleup", 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	    s c a l e d o w n_ c o n t r a c t		*/
/*	-------------------------------------------	*/
private	struct	rest_response * scaledown_contract(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	int	status;
	struct	occi_response * zptr;
	struct	cords_contract * pptr;
	char *	target;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		if ( pptr->state != _OCCI_IDLE )
		{
			/* ------------------ */
			/* dont scale commons */
			/* ------------------ */
			if ( pptr->commons > 1 )
				return( rest_html_response( aptr, 202, "Accepted" ) );
			else if ( is_common_contract( pptr ) )
				return( rest_html_response( aptr, 202, "Accepted" ) );
			else if ((!( rest_valid_string( pptr->type ) ))
			||  (!( strcmp( pptr->type, _CORDS_SIMPLE ) )))
			{
				/* -------------------------------- */
				/* detect scale down interface here */
				/* -------------------------------- */
				if (!( target = contract_can_scale( pptr ) ))
					return( rest_html_response( aptr, 202, "Accepted" ) );
				else
				{
					if (!(zptr = cords_invoke_action( target, "scaledown", 
						_CORDS_CONTRACT_AGENT, default_tls() )))
						return( rest_html_response( aptr, 900, "ScaleDown Invocation Failure" ) );
					else if ((status = zptr->response->status) > 299 )
					{
						zptr = occi_remove_response ( zptr );
						return( rest_html_response( aptr, status, "ScaleDown Invocation Error" ) );
					}
					else	zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, 200, "OK" ) );
				}
			}
			else if (( rest_valid_string( pptr->category ) != 0)
			     && ( strcmp( pptr->category, _CORDS_MANIFEST ) != 0 ))
				return( rest_html_response( aptr, 202, "Accepted" ) );
			else if ( rest_valid_string( pptr->service ) != 0 )
			{
				/* ----------------------------------- */
				/* propogate scaledown to service here */
				/* ----------------------------------- */
				if (!(zptr = cords_invoke_action( pptr->service, "scaledown", 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}
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
	int	status;
	struct	occi_response * zptr;
	struct	cords_contract * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		if ( pptr->state != _OCCI_IDLE )
		{
			pptr->stamp  = time((long*) 0);
			if ( is_common_contract( pptr ) )
			{
				if (!( zptr = cords_invoke_action( pptr->common, _CORDS_SAVE, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}
			else if ((!( rest_valid_string( pptr->type ) ))
			||  (!( strcmp( pptr->type, _CORDS_SIMPLE ) )))
			{
				if (!( zptr = cords_invoke_action( pptr->provider, _CORDS_SAVE, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
				retrieve_provider_information( pptr );
			}
			else if (( rest_valid_string( pptr->category ) != 0)
			     && ( strcmp( pptr->category, _CORDS_MANIFEST ) != 0))
			{
				if (!( zptr = cords_invoke_action( pptr->provider, _CORDS_SAVE, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
				retrieve_provider_information( pptr );
			}
			else if ( rest_valid_string( pptr->service ) != 0 )
			{
				if (!( zptr = cords_invoke_action( pptr->service, _CORDS_SAVE, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}
			pptr->saveduration = (time((long*)0) - pptr->stamp);
			pptr->stamp  = time((long*) 0);
			autosave_cords_contract_node(pptr);
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	    s n a p s h o t _ c o n t r a c t		*/
/*	-------------------------------------------	*/
private	struct	rest_response * snapshot_contract(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	int	status;
	struct	occi_response * zptr;
	struct	cords_contract * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		if ( pptr->state != _OCCI_IDLE )
		{
			if ( is_common_contract( pptr ) )
			{
				if (!( zptr = cords_invoke_action( pptr->common, _CORDS_SNAPSHOT, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}
			else if ((!( rest_valid_string( pptr->type ) ))
			||  (!( strcmp( pptr->type, _CORDS_SIMPLE ) )))
			{
				if (!( zptr = cords_invoke_action( pptr->provider, _CORDS_SNAPSHOT, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}
			else if (( rest_valid_string( pptr->category ) != 0)
			     && (  strcmp( pptr->category, _CORDS_MANIFEST ) != 0 ))
			{
				if (!( zptr = cords_invoke_action( pptr->provider, _CORDS_SNAPSHOT, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
			}
			else if ( rest_valid_string( pptr->service ) != 0 )
			{
				if (!( zptr = cords_invoke_action( pptr->service, _CORDS_SNAPSHOT, 
					_CORDS_CONTRACT_AGENT, default_tls() )))
					return( rest_html_response( aptr, 900, "Action Invocation Failure" ) );
				else if ((status = zptr->response->status) > 299 )
				{
					zptr = occi_remove_response ( zptr );
					return( rest_html_response( aptr, status, "Action Invocation Error" ) );
				}
				else	zptr = occi_remove_response ( zptr );
				retrieve_provider_information( pptr );
			}
			pptr->stamp  = time((long*) 0);
			autosave_cords_contract_node(pptr);
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-----------------------------------------------------------	*/
/*		d e l e t e _ g e n e r i c _ c o n t r a c t		*/
/*	-----------------------------------------------------------	*/
private	int	delete_generic_contract( struct occi_category * optr, struct cords_contract * pptr )
{
	struct	occi_response 	* zptr = NULL;
	struct	occi_link_node  * nptr = NULL;
	struct	cords_xlink	* lptr = NULL;
	char 			* wptr = NULL;
	char			buffer[2048];

	buffer[0] = 0;

	/* ------------------------------------------------------- */
	/* delete the contract if simple or complex but not common */
	/* ------------------------------------------------------- */
	if (!( is_common_contract( pptr ) ))
	{
		if ((!( rest_valid_string( pptr->type ) ))
		||  (!( strcmp( pptr->type, _CORDS_SIMPLE ) )))
			zptr = occi_simple_delete( pptr->provider, _CORDS_SERVICE_AGENT, default_tls() );
		else if ( rest_valid_string( pptr->service ) != 0 )
			zptr = occi_simple_delete( pptr->service, _CORDS_SERVICE_AGENT, default_tls() );
		if ( zptr )
			zptr = occi_remove_response( zptr );
	}

	/* ----------------------------------------------------- */
	/* for all defined instructions of the current contract  */
	/* ----------------------------------------------------- */
	for (	nptr=occi_first_link_node(); 
		nptr != (struct occi_link_node *) 0;
		nptr = occi_next_link_node(nptr) )
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
				strcpy(buffer, lptr->source );
			if (!( zptr = occi_simple_delete( lptr->target, _CORDS_SERVICE_AGENT, default_tls() )))
				continue;
			else	zptr = occi_remove_response( zptr );
		}
	}

	/* -------------------------------------------- */
	/* delete a monitoring session if one is active */
	/* -------------------------------------------- */
	if ( rest_valid_string( pptr->session ) != 0 )
	{
		if (( zptr = occi_simple_delete( pptr->session, _CORDS_SERVICE_AGENT, default_tls() )) != (struct occi_response *) 0)
			zptr = occi_remove_response( zptr );
		pptr->session = liberate( pptr->session );
	}

	/* ------------------------ */
	/* consume placement quotas */
	/* ------------------------ */
	if ( rest_valid_string( pptr->placement ) != 0 )
	{
		if ( cords_validate_action( 
			(struct occi_client *) 0,
			_CORDS_PLACEMENT, _CORDS_RELEASE,
			pptr->placement ) )
		{
			if ((zptr = cords_invoke_action( pptr->placement, _CORDS_RELEASE, 
				_CORDS_CONTRACT_AGENT, default_tls() )) != (struct occi_response *) 0)
				zptr = occi_remove_response ( zptr );
		}
		if ((zptr = occi_simple_delete( pptr->placement, _CORDS_SERVICE_AGENT, default_tls() )) != (struct occi_response *) 0)
			zptr = occi_remove_response( zptr );
	}

	/* -------------------------------------- */
	/* delete eventual links from this object */
	/* -------------------------------------- */
	if ( strlen(buffer) )
		if ((zptr = occi_delete_links( buffer, _CORDS_SERVICE_AGENT, default_tls() )) != (struct occi_response *) 0)
			zptr = occi_remove_response( zptr );

	return(0);
}

/*	-------------------------------------------	*/
/* 	      c r e a t e _ c o n t r a c t  		*/
/*	-------------------------------------------	*/
private	int	create_contract(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_contract * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return( 0 ); 
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ c o n t r a c t  		*/
/*	-------------------------------------------	*/
private	int	retrieve_contract(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_contract * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ c o n t r a c t  		*/
/*	-------------------------------------------	*/
private	int	update_contract(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_contract * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ c o n t r a c t	  	*/
/*	-------------------------------------------	*/
private	int	delete_contract(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	cords_contract * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(delete_generic_contract(optr, pptr));
}

private	struct	occi_interface	contract_interface = {
	create_contract,
	retrieve_contract,
	update_contract,
	delete_contract
	};


/*	------------------------------------------------------------------	*/
/*		p r o c c i _ c o n t r a c t _ b u i l d e r			*/
/*	------------------------------------------------------------------	*/
private	struct	occi_category *	procci_contract_builder( char * domain, char * category )
{
	struct	occi_category * optr;
	initialise_occi_resolver( _DEFAULT_PUBLISHER, (char *) 0, (char *) 0, (char *) 0 );
	if (!( optr = occi_cords_contract_builder( domain ,category ) ))
		return( optr );
	else 
	{
		optr->callback  = &contract_interface;

		if (!( optr = occi_add_action( optr,_CORDS_START,"",start_contract)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_SUSPEND,"",suspend_contract)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_RESTART,"",restart_contract)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_SAVE,"",save_contract)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_SNAPSHOT,"",snapshot_contract)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_STOP,"",stop_contract)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_RESOLVE,"",resolve_contract)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"scaleup","",scaleup_contract)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"scaledown","",scaledown_contract)))
			return( optr );
		else	return( optr );
	}
}

#endif	/* _procci_contract_c */
	/* ----------------- */

