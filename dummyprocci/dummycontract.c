/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2013  Iain JAmes Marshall (Prologue) <ijm667@hotmail.com>	*/
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License");      */
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at                              */
/*                                                                      */
/*  http://www.apache.org/licenses/LICENSE-2.0                          */
/*                                                                      */
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or      */
/* implied.                                                             */
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License.                                       */
/* -------------------------------------------------------------------- */
#ifndef	_dummycontract_c
#define	_dummycontract_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occiresolver.h"
#include "dummyconfig.h"
#include "dummyclient.h"
#include "cb.h"

#include "kindnode.h" // TODO Remove all references to kindnodes directly from the procci

#define	_CORDS_dummy_AGENT "DummyProcci/1.0"

struct	cords_vector
{
	char *	id;
	struct occi_response * message;
};

struct	cords_dummy_contract
{
	struct	dummy_subscription * subscription;
	struct	cords_vector	node;
	struct	cords_vector	infrastructure;
	struct	cords_vector	compute;
	struct	cords_vector	network;
	struct	cords_vector	firewall;
	struct	cords_vector	storage;
	struct	cords_vector	image;
	struct	cords_vector	system;
	struct	cords_vector	package;
	struct	dummy_response *	flavors;
	struct	dummy_response *	images;
};

private	int	terminate_dummy_contract( int status, struct cords_dummy_contract * cptr );
private	struct dummy_response * stop_dummy_provisioning( struct dummy * pptr );

/*	-------------------------------------------	*/
/* 	   		s t a r t _ d u m m y     	*/
/*	-------------------------------------------	*/
public	struct	rest_response * start_dummy(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	dummy * pptr;
	struct  dummy_config *      config;
	struct  dummy_response *    dummyptr;
	struct	data_element * eptr;
	struct	data_element * dptr;
	int	status;
	char *  sptr;
	char *	filename;
	char 	buffer[1024];
	char	reference[1024];

	strcpy(reference,"reference");

	/* --------------------------------- */
	/* retrieve the instance information */
	/* --------------------------------- */
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->state != _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );

	/* ------------------------------------- */
	/* retrieve the subscription information */
	/* ------------------------------------- */
	else if (!( config = use_dummy_configuration( pptr->profile ) ))
		return( rest_html_response( aptr, status, "Configuration Not Found" ) );
		
	sprintf(buffer,"contract=%s/%s/%s\npublisher=%s\n",
		Configuration.identity,_CORDS_DUMMY,pptr->id,Configuration.publisher);
		
	if ( rest_valid_string( pptr->price ) )
		occi_send_transaction( _CORDS_DUMMY, pptr->price, "action=start", pptr->account, reference );

	pptr->state = _OCCI_RUNNING;
		
	return( rest_html_response( aptr, 200, "OK" ) );	
}

/*	-------------------------------------------	*/
/* 	   	s a v e _ d u m m y         */
/*	-------------------------------------------	*/
public	struct	rest_response * save_dummy(
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
		/* ----------------------------------- */
		/* add code here td u m m yllocate resources */
		/* ----------------------------------- */
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	   	s t o p _ d u m m y         */
/*	-------------------------------------------	*/
public	struct	rest_response * stop_dummy(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	char	reference[512];
	struct	dummy_config * config=(struct dummy_config *) 0;
	struct	dummy_response * dummyptr;
	int		status;
	struct	dummy * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->state == _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );

	else if ((dummyptr = stop_dummy_provisioning( pptr )) == (struct dummy_response *) 0)
		return(0);
	else
	{
		dummyptr = dummy_liberate_response( dummyptr );

		sprintf(reference,"%s/%s/%s",Configuration.identity,_CORDS_DUMMY,pptr->id);
		if (!( rest_valid_string( pptr->price ) ))
			return( rest_html_response( aptr, 200, "OK" ) );
		else if ( occi_send_transaction( _CORDS_DUMMY, pptr->price, "action=stop", pptr->id, reference ) )
			return( rest_html_response( aptr, 200, "OK" ) );
		else	return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*  -----------------------------------------------------------------  */
/*      c r e a t e _ d u m m y _ c o n t r a c t          */
/*  -----------------------------------------------------------------  */
public	int	create_dummy_contract(
		struct occi_category * optr,
		struct dummy * pptr,
		struct rest_request  * rptr)
{
	
	struct	dummy_config *       config= (struct dummy_config *) 0;
	struct	cords_dummy_contract contract;
	struct	dummy_response *     instancetypes = (struct dummy_response *) 0;
	struct	dummy_response *     images = (struct dummy_response *) 0;
	int	status;
	char *	vptr;
	char buffer[256];

	if (!(config = use_dummy_configuration( pptr->profile )))
	{
		rest_log_message("Could Not Find Configuration");
		return( status );
	}
	else
	{
		memset( &contract, 0, sizeof( struct cords_dummy_contract ));
	}

	/* ---------------------------- */
	/* recover the node description */
	/* ---------------------------- */
	if (!( contract.node.id = pptr->node ))
		return( 0 );
	else if (!( contract.node.message = occi_simple_get( contract.node.id, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( terminate_dummy_contract( 1170, &contract ) );
	else
	{
		/* ---------------------------------------------------- */
		/* recover and store the public/private access property */
		/* ---------------------------------------------------- */
		if (!( vptr = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_ACCESS ) ))
			vptr = _CORDS_PUBLIC;
		if ( pptr->access ) pptr->access = liberate( pptr->access );
		if (!( pptr->access = allocate_string( vptr ) ))
			return( terminate_dummy_contract( 1127, &contract ) );
	}
	
	/* ---------------------------------- */
	/* recover the node image description */
	/* ---------------------------------- */
	if (!( contract.image.id = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_IMAGE ) ))
		return( terminate_dummy_contract( 1182, &contract ) );
	else if (!( contract.image.message = occi_simple_get( contract.image.id, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( terminate_dummy_contract( 1183, &contract ) );

	/* ------------------------ */
	/* retrieve the cosacs flag */
	/* ------------------------ */
	else if ((!( pptr->agent = occi_extract_atribut( contract.image.message, "occi", 
		_CORDS_IMAGE, "agent" ) ))
	     &&  (!( pptr->agent = allocate_string("cosacs") )))
		return( terminate_dummy_contract( 1283, &contract ) );

	else if (!( contract.system.id = occi_extract_atribut( contract.image.message, "occi", 
		_CORDS_IMAGE, _CORDS_SYSTEM ) ))
		return( terminate_dummy_contract( 1184, &contract ) );
	else if (!( contract.system.message = occi_simple_get( contract.system.id, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( terminate_dummy_contract( 1185, &contract ) );

	/* -------------------------------------- */
	/* recover the infrastructure description */
	/* -------------------------------------- */
	if (!( contract.infrastructure.id = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_INFRASTRUCTURE ) ))
		return( terminate_dummy_contract( 1171, &contract ) );
	else if (!( contract.infrastructure.message = occi_simple_get( contract.infrastructure.id, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( terminate_dummy_contract( 1172, &contract ) );

	else if (!( contract.compute.id = occi_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_COMPUTE ) ))
		return( terminate_dummy_contract( 1173, &contract ) );
	else if (!( contract.compute.message = occi_simple_get( contract.compute.id, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( terminate_dummy_contract( 1174, &contract ) );

	else if (!( contract.network.id = occi_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_NETWORK ) ))
		return( terminate_dummy_contract( 1175, &contract ) );
	else if (!( contract.network.message = occi_simple_get( contract.network.id, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( terminate_dummy_contract( 1177, &contract ) );

	else if (!( contract.storage.id = occi_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_STORAGE ) ))
		return( terminate_dummy_contract( 1178, &contract ) );
	else if (!( contract.storage.message = occi_simple_get( contract.storage.id, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( terminate_dummy_contract( 1179, &contract ) );

	/* ----------------------------------------------- */
	/* resolve any price information for this category */
	/* ----------------------------------------------- */
	pptr->price = occi_resolve_category_price( _CORDS_DUMMY, default_operator(), _CORDS_CONTRACT_AGENT, default_tls() );

	return( terminate_dummy_contract( 0, &contract ) );
}

/*  -----------------------------------------------------------------  */
/*      d e l e t e _ d u m m y _ c o n t r a c t          */
/*  -----------------------------------------------------------------  */
public	int	delete_dummy_contract(
		struct occi_category * optr,
		struct dummy * pptr,
		struct rest_request  * rptr)
{
	struct	dummy_response * dummyptr;
	
	if ( pptr->state == _OCCI_IDLE )
		return(0);
	else if ((dummyptr = stop_dummy_provisioning( pptr )) == (struct dummy_response *) 0)
		return(0);
	else
	{
		dummyptr = dummy_liberate_response( dummyptr );
		return(1);
	}
	
}


/*	-----------------------------------------------------------------  */
/* 	    u s e _ d u m m y _ c o n f i g u r a t i o n      */
/*	-----------------------------------------------------------------  */
private	struct dummy_config * use_dummy_configuration( char * nptr )
{
	struct	dummy_config * sptr;
	if (( sptr = resolve_dummy_configuration( nptr )) != (struct dummy_config *) 0)
		return( sptr );
	else if (!( nptr = get_operator_profile() ))
		return( sptr );
	else 	return( resolve_dummy_configuration( nptr ) );
}

/*  -----------------------------------------------------------------  */
/* 	    r e s o l v e _ d u m m y _ c o n f i g u r a t i o n               */
/*  -----------------------------------------------------------------  */
private	struct	dummy_config * resolve_dummy_configuration( char * sptr )
{
	struct	occi_kind_node * nptr;
	struct	dummy_config * pptr=(struct dummy_config *) 0;
	struct	occi_kind_node  * occi_first_dummy_config_node();

	for (	nptr = occi_first_dummy_config_node();
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next )
	{
		if (!( pptr = nptr->contents ))
		{
			rest_log_message("Contents not set");
			continue;
		}
		else if (!( pptr->name ))
		{
			rest_log_message("Name not set");
			continue;
		}
		else if (!( strcmp( pptr->name, sptr ) ))
			return( dummy_initialise_client(
				pptr->user, pptr->password, pptr->host, 
				_CORDS_dummy_AGENT, pptr->version, pptr->tls ));
	}
	return((struct dummy_config *) 0);
}


/*  -----------------------------------------------------------------  */
/*      	s t o p  _ d u m m y _ p r o v i s i o n i n g         */
/*  -----------------------------------------------------------------  */
private	struct dummy_response * stop_dummy_provisioning( struct dummy * pptr )
{
	int	   status;
	struct dummy_response * dummyptr;
	struct dummy_config * config=(struct dummy_config *) 0;
	struct	data_element * fptr;
	char   reference[512];
	char *	vptr;

	if (!( config = use_dummy_configuration( pptr->profile )))
		return((struct dummy_response *) 0);
	else
	{
		/* ------------------------------------------- */
		/* perform pre-release actions for destruction */
		/* ------------------------------------------- */
		sprintf(reference,"%s/%s/%s",Configuration.identity,_CORDS_DUMMY,pptr->id);

		if ( use_cosacs_agent( pptr->agent ) )
		{
			if ( cosacs_test_interface( pptr->hostname, _COSACS_STOP_TIMEOUT, _COSACS_STOP_RETRY ) )
			{
				cosacs_metadata_instructions( 
					pptr->hostname, _CORDS_RELEASE,
					reference, Configuration.publisher, pptr->account );
			}
		}

		pptr->state = _OCCI_IDLE;
		if ( pptr->hostname )
			pptr->hostname = liberate(pptr->hostname);
		if ( pptr->ipaddress )
			pptr->ipaddress = liberate(pptr->ipaddress);
		return( dummyptr );
	}
}

/*	-----------------------------------------------------------------  */
/*	    	t e r m i n a t e _ d u m m y _ c o n t r a c t    	   */
/*	-----------------------------------------------------------------  */
private	int	terminate_dummy_contract( int status, struct cords_dummy_contract * cptr )
{
	
	return( 0 );
}

	/* ------------- */
#endif	/* _dummycontract_c */
	/* ------------- */


