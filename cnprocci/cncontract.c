/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2013 by Daniel Sprouse (ComputeNext) <daniel@computenext.com>   */
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
#ifndef	_cncontract_c	
#define	_cncontract_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occiresolver.h"
#include "occibuilder.h"
#include "cnclient.h"
#include "cb.h"

struct	cn_instancetype
{
	int	cores;
	int	speed;
	int memory;
	int storage;
	char    architecture[256];
	char *  id;
	char *  name;
	char *	provider;
	char *  providerId;
	char *	region;
	char *	location;
};

struct	cn_image
{
	char *	id;
	char *	name;
	char *	other;
	char *	provider;
	char *  providerId;
	char *	region;
	char *	location;
};

struct	cn_virtualmachine
{
	char *	id;
	char *	name;
	char *  other;
	char *	provider;
	char *  providerId;
	char *	region;
	char *	location;
	struct cn_image        image;
	struct cn_instancetype instancetype;
};


/*	-------------------------------------------	*/
/* 	   	s t a r t _ c o m p u t e n e x t       */
/*	-------------------------------------------	*/
public	struct	rest_response * start_computenext(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	computenext * pptr;
	struct  cn_config *      config;
	struct  cn_response *    cnptr;
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
	else if (!( config = use_computenext_configuration( pptr->profile ) ))
		return( rest_html_response( aptr, status, "Configuration Not Found" ) );
		
	sprintf(buffer,"contract=%s/%s/%s\npublisher=%s\n",
		Configuration.identity,_CORDS_COMPUTENEXT,pptr->id,Configuration.publisher);
		
	/* ------------------------------------------------------ */
	/* build the CN workload based on the contract resolution */
	/* ------------------------------------------------------ */
	if(!(pptr->workload = build_computenext_workload( config, pptr ) ))
	{
		config = cn_liberate_config( config );
		computenext_build_failure( pptr, 911, "Failure Creating Workload" );
	 	return( rest_html_response( aptr, 4004, "Server Failure : Cannot Create Workload" ) );
	}
	
	/* ---------------------- */
	/* create the transaction */
	/* ---------------------- */
	if (!( filename = cn_create_transaction_request( pptr->workload, "Paid" ) ))
	{
		config = cn_liberate_config( config );
		computenext_build_failure( pptr, 911, "Failure Creating Transaction Message" );
	 	return( rest_html_response( aptr, 4004, "Server Failure : Create Transaction Message" ) );
	}
	else if (!( cnptr = cn_create_transaction( config, filename ) ))
	{
		config = cn_liberate_config( config );
		computenext_build_failure( pptr, 911, "Failure Creating Transaction" );
	 	return( rest_html_response( aptr, 4008, "Server Failure : Create Transaction Request" ) );
	}
	else if(!(sptr = json_atribut( cnptr->jsonroot, "TransactionId" ) ))
	{
		config = cn_liberate_config( config );
		computenext_build_failure( pptr, 911, "Failure Finding TransactionId" );
	 	return( rest_html_response( aptr, 4008, "Server Failure : Find TransactionId" ) );
	}
	else if(!( pptr->transaction = allocate_string(sptr) ))
	{
		config = cn_liberate_config( config );
		computenext_build_failure( pptr, 911, "Failure Saving TransactionId" );
	 	return( rest_html_response( aptr, 4008, "Server Failure : Save TransactionId" ) );
	}
	else cnptr = cn_liberate_response( cnptr );

	/* ---------------------------------------- */
	/* poll transaction until started or failed */
	/* ---------------------------------------- */
	while ((cnptr=cn_get_transaction( config, pptr->transaction )) != (struct cn_response *) 0)
	{
		if (!( eptr = json_element( cnptr->jsonroot, "ResourceStatusDetails" )))
		{
			config = cn_liberate_config( config );
			cnptr = cn_liberate_response( cnptr );
			computenext_build_failure( pptr, 911, "Failure Finding Resource Details" );
			return( rest_html_response( aptr, 4008, "Server Failure : Poll Transaction Request" ) );
		}
		else if (!( sptr = json_atribut( eptr->first, "StatusCode" ) ))
		{
			config = cn_liberate_config( config );
			cnptr = cn_liberate_response( cnptr );
			computenext_build_failure( pptr, 911, "Failure Finding Status Code" );
			return( rest_html_response( aptr, 4008, "Server Failure : Poll Transaction Request" ) );
		}
		else if (!( strcmp( sptr, _CN_STATUS_FAILED ) ))
		{
			config = cn_liberate_config( config );
			cnptr = cn_liberate_response( cnptr );
			computenext_build_failure( pptr, 911, "Transaction In Failed State" );
			return( rest_html_response( aptr, 4008, "Server Failure : Poll Transaction Request" ) );
		}
		else if (!( strcmp( sptr, _CN_STATUS_STARTED ) ))
		{
			if (!( sptr = json_atribut( eptr->first, "PublicIpAddress" ) ))
			{
				config = cn_liberate_config( config );
				cnptr = cn_liberate_response( cnptr );
				computenext_build_failure( pptr, 911, "Failure Finding IpAddress" );
				return( rest_html_response( aptr, 4008, "Server Failure : Poll Transaction Request" ) );
			}
			else if ( *sptr == '{' )
			{
				if (!( dptr = json_parse_string( sptr )))
				{
					config = cn_liberate_config( config );
					cnptr = cn_liberate_response( cnptr );
					computenext_build_failure( pptr, 911, "Failure Finding IpAddress" );
					return( rest_html_response( aptr, 4008, "Server Failure : Poll Transaction Request" ) );
				}
				else if (!( sptr = json_atribut( dptr, "publicIP" ) ))
				{
					config = cn_liberate_config( config );
					cnptr = cn_liberate_response( cnptr );
					computenext_build_failure( pptr, 911, "Failure Finding IpAddress" );
					return( rest_html_response( aptr, 4008, "Server Failure : Poll Transaction Request" ) );
				}
			}
			if ((!( pptr->ipaddress = allocate_string( sptr )))
			||  (!( pptr->hostname = allocate_string( pptr->ipaddress ))))
			{
				config = cn_liberate_config( config );
				cnptr = cn_liberate_response( cnptr );
				computenext_build_failure( pptr, 911, "Failure Finding IpAddress" );
				return( rest_html_response( aptr, 4008, "Server Failure : Poll Transaction Request" ) );
			}
			else	break;
		}
		else
		{
			sleep( _CN_BUILD_SLEEP );
			cnptr = cn_liberate_response( cnptr );
		}
	}
	
	rest_log_message("*** CN PROCCI Transaction Started Successfully ***");
	rest_log_message( pptr->transaction );

	if ( rest_valid_string( pptr->price ) )
		occi_send_transaction( _CORDS_COMPUTENEXT, pptr->price, "action=start", pptr->account, reference );

	/* TODO set the public and private addresses */
	/* TODO Retrieve the keypair or password/username so ACCORDS can connect */
	config = cn_liberate_config( config );
	cnptr = cn_liberate_response( cnptr );
	
	pptr->state = _OCCI_RUNNING;
		
	return( rest_html_response( aptr, 200, "OK" ) );	
}

/*	-------------------------------------------	*/
/* 	   	s a v e _ c o m p u t e n e x t         */
/*	-------------------------------------------	*/
public	struct	rest_response * save_computenext(
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
		/* add code here to allocate resources */	
		/* ----------------------------------- */
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	   	s t o p _ c o m p u t e n e x t         */
/*	-------------------------------------------	*/
public	struct	rest_response * stop_computenext(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	char	reference[512];
	struct	cn_config * config=(struct cn_config *) 0;
	struct	cn_response * cnptr;
	int		status;
	struct	computenext * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->state == _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );

	else if ((cnptr = stop_computenext_provisioning( pptr )) == (struct cn_response *) 0)
		return(0);
	else
	{
		cnptr = cn_liberate_response( cnptr );

		sprintf(reference,"%s/%s/%s",Configuration.identity,_CORDS_COMPUTENEXT,pptr->id);
		if (!( rest_valid_string( pptr->price ) ))
			return( rest_html_response( aptr, 200, "OK" ) );
		else if ( occi_send_transaction( _CORDS_COMPUTENEXT, pptr->price, "action=stop", pptr->apikey, reference ) )
			return( rest_html_response( aptr, 200, "OK" ) );
		else	return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*  -----------------------------------------------------------------  */
/*      c r e a t e _ c o m p u t e n e x t _ c o n t r a c t          */
/*  -----------------------------------------------------------------  */
public	int	create_computenext_contract(
		struct occi_category * optr,
		struct computenext * pptr,
		struct rest_request  * rptr)
{
	
	struct	cn_config *       config= (struct cn_config *) 0;
	struct	cords_cn_contract contract;
	struct	cn_response *     instancetypes = (struct cn_response *) 0;
	struct	cn_response *     images = (struct cn_response *) 0;
	int	status;
	char *	vptr;
	char buffer[256];

	if (!(config = use_computenext_configuration( pptr->profile )))
	{
		rest_log_message("Could Not Find Configuration");
		return( status );
	}
	else
	{
		memset( &contract, 0, sizeof( struct cords_cn_contract ));
		contract.config = config;
	}

	/* ---------------------------- */
	/* recover the node description */
	/* ---------------------------- */
	if (!( contract.node.id = pptr->node ))
		return( 0 );
	else if (!( contract.node.message = occi_simple_get( contract.node.id, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( terminate_computenext_contract( 1170, &contract ) );
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
			return( terminate_computenext_contract( 1127, &contract ) );
	}
	
	/* ---------------------------------- */
	/* recover the node image description */
	/* ---------------------------------- */
	if (!( contract.image.id = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_IMAGE ) ))
		return( terminate_computenext_contract( 1182, &contract ) );
	else if (!( contract.image.message = occi_simple_get( contract.image.id, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( terminate_computenext_contract( 1183, &contract ) );

	/* ------------------------ */
	/* retrieve the cosacs flag */
	/* ------------------------ */
	else if ((!( pptr->agent = occi_extract_atribut( contract.image.message, "occi", 
		_CORDS_IMAGE, "agent" ) ))
	     &&  (!( pptr->agent = allocate_string("cosacs") )))
		return( terminate_computenext_contract( 1283, &contract ) );

	else if (!( contract.system.id = occi_extract_atribut( contract.image.message, "occi", 
		_CORDS_IMAGE, _CORDS_SYSTEM ) ))
		return( terminate_computenext_contract( 1184, &contract ) );
	else if (!( contract.system.message = occi_simple_get( contract.system.id, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( terminate_computenext_contract( 1185, &contract ) );

	/* --------------------------------------------------- */
	/* check if a valid image identifier has been provided */
	/* --------------------------------------------------- */
	if (!( rest_valid_string( pptr->image ) ))
	{
		/* ------------------------------------------------------ */
		/* retrieve detailed list of images and resolve contract  */
		/* ------------------------------------------------------ */
		if (!( contract.images = cn_list_images(config,"") ))
			return( terminate_computenext_contract( 1186, &contract ) );
		else if (!( pptr->image = resolve_contract_image( config, &contract ) ))
			return( terminate_computenext_contract( 1187, &contract ) );
	}
	
	/* -------------------------------------- */
	/* recover the infrastructure description */
	/* -------------------------------------- */
	sprintf(buffer,"provider=%s&region=%s", contract.provider, contract.region);
	
	if (!( contract.infrastructure.id = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_INFRASTRUCTURE ) ))
		return( terminate_computenext_contract( 1171, &contract ) );
	else if (!( contract.infrastructure.message = occi_simple_get( contract.infrastructure.id, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( terminate_computenext_contract( 1172, &contract ) );

	else if (!( contract.compute.id = occi_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_COMPUTE ) ))
		return( terminate_computenext_contract( 1173, &contract ) );
	else if (!( contract.compute.message = occi_simple_get( contract.compute.id, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( terminate_computenext_contract( 1174, &contract ) );

	else if (!( contract.network.id = occi_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_NETWORK ) ))
		return( terminate_computenext_contract( 1175, &contract ) );
	else if (!( contract.network.message = occi_simple_get( contract.network.id, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( terminate_computenext_contract( 1177, &contract ) );

	else if (!( contract.storage.id = occi_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_STORAGE ) ))
		return( terminate_computenext_contract( 1178, &contract ) );
	else if (!( contract.storage.message = occi_simple_get( contract.storage.id, _CORDS_CONTRACT_AGENT, default_tls() ) ))
		return( terminate_computenext_contract( 1179, &contract ) );

	/* -------------------------------------------------------------- */
	/* recover detailed list of CN instancetypes and resolve contract */
	/* -------------------------------------------------------------- */
	
	else if (!( contract.instancetypes = cn_list_instancetypes(config,buffer) ))
		return( terminate_computenext_contract( 1180, &contract ) );
	else if (!( pptr->instancetype = resolve_contract_instancetype( config, &contract ) ))
		return( terminate_computenext_contract( 1181, &contract ) );
	
	/* -------------------------------- */
	/* recover the firewall description */
	/* -------------------------------- */
	if (( contract.firewall.id = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_FIREWALL )) != (char * ) 0)
	{
		if (!( contract.firewall.message = occi_simple_get( contract.firewall.id, _CORDS_CONTRACT_AGENT, default_tls() ) ))
			return( terminate_computenext_contract( 1188, &contract ) );
			
		/* --------------------------------------------------------------- */
		/* recover detailed list of CN securitygroups and resolve contract */
		/* --------------------------------------------------------------- */
		else if (!( contract.securitygroups = cn_list_securitygroups(config) ))
			return( terminate_computenext_contract( 1190, &contract ) );
		else if (!( pptr->securitygroup = resolve_contract_securitygroup( config, &contract, pptr ) ))
			return( terminate_computenext_contract( 1191, &contract ) );
	}

	/* ----------------------------------------------- */
	/* resolve any price information for this category */
	/* ----------------------------------------------- */
	pptr->price = occi_resolve_category_price( _CORDS_COMPUTENEXT, default_operator(), _CORDS_CONTRACT_AGENT, default_tls() );

	return( terminate_computenext_contract( 0, &contract ) );
}

/*  -----------------------------------------------------------------  */
/*      d e l e t e _ c o m p u t e n e x t _ c o n t r a c t          */
/*  -----------------------------------------------------------------  */
public	int	delete_computenext_contract(
		struct occi_category * optr,
		struct computenext * pptr,
		struct rest_request  * rptr)
{
	struct	cn_response * cnptr;
	
	if ( pptr->state == _OCCI_IDLE )
		return(0);
	else if ((cnptr = stop_computenext_provisioning( pptr )) == (struct cn_response *) 0)
		return(0);
	else
	{
		cnptr = cn_liberate_response( cnptr );
		return(1);
	}
	
}


/*  -----------------------------------------------------------------  */
/*      r e s o l v e _ c o n t r a c t _ i m a g e                    */
/*  -----------------------------------------------------------------  */
private char * resolve_contract_image( struct cn_config * config, struct cords_cn_contract * cptr )
{
	struct  cn_image    request;
	struct  cn_image    image;
	struct  cn_image    best;
	char *  vptr;
	char *  iname=(char*) 0;
	char *  pname=(char*) 0;
	char *  rname=(char*) 0;
	char *  pid=(char*) 0;
	struct	data_element * eptr=(struct data_element *) 0;
	struct	data_element * dptr=(struct data_element *) 0;


	if (!( eptr = cptr->images->jsonroot ))
		return((char *) 0);

	/* ---------------------------------------------------------- */
	/* retrieve appropriate parameters from node image components */
	/* ---------------------------------------------------------- */
	if (!( vptr = occi_extract_atribut( cptr->system.message, "occi", 
		_CORDS_SYSTEM, _CORDS_NAME ) ))
		return((char *) 0);
	else	request.name = vptr;

	rest_log_message( "cn_contract system :");
	rest_log_message( request.name );

	if (!( vptr = occi_extract_atribut( cptr->image.message, "occi", 
		_CORDS_IMAGE, _CORDS_NAME ) ))
		return((char *) 0);
	else	request.other = vptr;

	rest_log_message( "cn_contract image :");
	rest_log_message( request.other );

	memset( &best, 0, sizeof( struct cn_image ));

	/* ------------------------------------------ */
	/* scan list for a perfect IPS produced match */
	/* ------------------------------------------ */
	if (!( iname = occi_category_id( cptr->image.id )))
		return((char *) 0);

	for ( 	dptr=eptr->first;
		dptr != (struct data_element *) 0;
		dptr = dptr->next )
	{
		if (!( vptr = json_atribut( dptr, "name" ) ))
			continue;
		else if ( strcmp( vptr, iname ) != 0 )
			continue;
		else if (!( vptr = json_atribut( dptr, "id" ) ))
			continue;
		else if (!( pname = json_atribut( dptr, "provider" ) ))
			continue;
		else if (!( pid = json_atribut( dptr, "providerId" ) ))
			continue;
		else if (!( rname = json_atribut( dptr, "region" ) ))
			continue;
		else
		{
			
			cptr->provider = allocate_string( pname );
			cptr->providerId = allocate_string( pid );
			cptr->region = allocate_string( rname );
			liberate( iname );
			liberate( pname );
			liberate( rname );
			liberate( pid );
			rest_log_message("cn_contract perfect match");
			rest_log_message( vptr );
			rest_log_message( cptr->provider );
			rest_log_message( cptr->region );
			return( allocate_string( vptr ) );
		}
	}
	liberate( iname );
	liberate( pname );
	liberate( rname );
	liberate( pid );

	/* --------------------------------------------------- */
	/* scan the image list for a system name partial match */
	/* --------------------------------------------------- */
	for ( 	dptr=eptr->first;
		dptr != (struct data_element *) 0;
		dptr = dptr->next )
	{
		/* ----------------------------------------------- */
		/* collect the information from the flavor element */
		/* ----------------------------------------------- */
		if (!( vptr = json_atribut( dptr, "id" ) ))
			continue;
		else	image.id = vptr;
		if (!( vptr = json_atribut( dptr, "name" ) ))
			continue;
		else	image.name = vptr;
		if (!( vptr = json_atribut( dptr, "provider" ) ))
			continue;
		else    image.provider = vptr;
		if (!( vptr = json_atribut( dptr, "providerId" ) ))
			continue;
		else    image.providerId = vptr;
		if (!( vptr = json_atribut( dptr, "region" ) ))
			continue;
		else    image.region = vptr;

		if ( (!( strncasecmp( request.name,  image.name, strlen( request.name  ) )))
		||   (!( strncasecmp( request.other, image.name, strlen( request.other ) ))))
		{
			best.id = image.id;
			best.name = image.name;
			best.provider = image.provider;
			best.providerId = image.providerId;
			best.region = image.region;
			rest_log_message("cn_contract found match");
			rest_log_message( image.name );
			rest_log_message( image.id );
			break;
		}		
		else	continue;
	}
	if (!( best.id ))
	{
		cptr->provider = allocate_string( image.provider );
		cptr->providerId = allocate_string( image.providerId );
		cptr->region = allocate_string( image.region );
		return(allocate_string( best.id ));
	}
	else
	{
		cptr->provider = allocate_string( best.provider );
		cptr->providerId = allocate_string( best.providerId );
		cptr->region = allocate_string( best.region );
		rest_log_message("cn_contract best match");
		rest_log_message( best.name );
		rest_log_message( best.id );
		rest_log_message( cptr->provider );
		rest_log_message( cptr->region );
		return(allocate_string( best.id ));
	}
}

/*	-----------------------------------------------------------------  */
/*      r e s o l v e _ c o n t r a c t _ i n s t a n c e t y p e      */
/*	-----------------------------------------------------------------  */
private char * resolve_contract_instancetype( struct cn_config * config, struct cords_cn_contract * cptr )
{
	struct	cn_instancetype	request;
	struct	cn_instancetype	instancetype;
	struct	cn_instancetype	best;
	char *  vptr;
	struct	data_element * eptr=(struct data_element *) 0;
	struct	data_element * dptr=(struct data_element *) 0;

	if (!( eptr = cptr->instancetypes->jsonroot ))
		return((char *) 0);

	rest_log_message("cn_contract resolving instancetype");
	/* -------------------------------------------------------------- */
	/* retrieve appropriate parameters from infrastructure components */
	/* -------------------------------------------------------------- */
	if (!( vptr = occi_extract_atribut( cptr->compute.message, "occi", 
		_CORDS_COMPUTE, _CORDS_MEMORY ) ))
		request.memory = 0;
	else	request.memory = rest_normalise_value( vptr,'U' );

	if (!( vptr = occi_extract_atribut( cptr->compute.message, "occi", 
		_CORDS_COMPUTE, _CORDS_CORES ) ))
		request.cores = 0;
	else	request.cores = rest_normalise_value( vptr,'U' );

	if (!( vptr = occi_extract_atribut( cptr->compute.message, "occi", 
		_CORDS_COMPUTE, _CORDS_SPEED ) ))
		request.speed = 0;
	else	request.speed = rest_normalise_value(vptr,'U');
	
	if (!( vptr = occi_extract_atribut( cptr->storage.message, "occi", 
		_CORDS_STORAGE, _CORDS_SIZE ) ))
		request.storage = 0;
	else	request.storage = rest_normalise_value(vptr,'U');

	/* --------------------------------------------- */
	/* collect and control the architecture required */
	/* --------------------------------------------- */
	if (!( vptr = occi_extract_atribut( cptr->compute.message, "occi", 
		_CORDS_COMPUTE, _CORDS_ARCHITECTURE ) ))
		strcpy(request.architecture,"32 Bit / 64 Bit" );
	else if ((!(strcasecmp( vptr, "x86"    		) ))
	     ||  (!(strcasecmp( vptr, "txt86" 		) )))
		strcpy(request.architecture,"32 Bit / 64 Bit" );
	else if ((!(strcasecmp( vptr, "x86_32" 	) ))
	     ||  (!(strcasecmp( vptr, "txt86_32"  	) )))
		strcpy(request.architecture,"32 Bit" );
	else if ((!(strcasecmp( vptr, "x86_64" 	  ) ))
	     ||  (!(strcasecmp( vptr, "txt86_64"  ) )))
	     strcpy(request.architecture,"64 Bit" );
	else	strcpy(request.architecture,"32 Bit / 64 Bit" );
	
	/* ----------------------------------------------- */
	/* for structures in instancetype message response */
	/* ----------------------------------------------- */
	memset( &best, 0, sizeof( struct cn_instancetype ));
	for ( 	dptr=eptr->first;
		dptr != (struct data_element *) 0;
		dptr = dptr->next )
	{
		/* ----------------------------------------------------- */
		/* collect the information from the instancetype element */
		/* ----------------------------------------------------- */
		if (!( vptr = json_atribut( dptr, "id" ) ))
			continue;
		else	instancetype.id = vptr;
		if (!( vptr = json_atribut( dptr, "name" ) ))
			instancetype.name=(char *) 0;
		else	instancetype.name = vptr;
		if (!( vptr = json_atribut( dptr, "localStorage" ) ))
			instancetype.storage = 0;
		else	instancetype.storage = rest_normalise_value(vptr,'G');
		if (!( vptr = json_atribut( dptr, "ram" ) ))
			instancetype.memory = 0;
		else	instancetype.memory = rest_normalise_value(vptr,'G');
		if (!( vptr = json_atribut( dptr, "cpuCount" ) ))
			instancetype.cores = 0;
		else	instancetype.cores = rest_normalise_value(vptr,'U');
		if (!( vptr = json_atribut( dptr, "cpuSpeed" ) ))
			instancetype.speed = 0;
		else	instancetype.speed = rest_normalise_value(vptr,'G');
		if (!( vptr = json_atribut( dptr, "provider" ) ))
			instancetype.provider = "";
		else	instancetype.provider = vptr;
		if (!( vptr = json_atribut( dptr, "region" ) ))
			instancetype.region = "";
		else	instancetype.region = vptr;
		if (!( vptr = json_atribut( dptr, "operatingSystemVersion" ) ))
			strcpy( instancetype.architecture, "32 Bit / 64 Bit" );
		else	strcpy( instancetype.architecture, vptr );
		
		/* ------------------------------------ */
		/* handle flavour architecture types    */
		/* ------------------------------------ */
		if ( ( strcasecmp( request.architecture, "32 Bit / 64 Bit" ) )
		&&   ( strncasecmp( request.architecture, instancetype.architecture, strlen( request.architecture ) )))
			continue; 
		else if (( request.storage ) && ( instancetype.storage ) && ( instancetype.storage < request.storage ))
			continue;
		else if (( request.memory  ) && ( instancetype.memory ) && ( instancetype.memory < request.memory ))
			continue;
		else if (( request.cores ) && ( instancetype.cores ) && ( instancetype.cores < request.cores ))
			continue;
		else if (( request.speed ) && ( instancetype.speed ) && ( instancetype.speed < request.speed ))
			continue;
		else if ( strcasecmp( cptr->provider, instancetype.provider ) != 0 )
			continue;
		else if ( strcasecmp( cptr->region, instancetype.region ) != 0 )
			continue;
			
		/* --------------------- */
		/* ok so its good enough */
		/* --------------------- */
		if (( best.cores ) && ( instancetype.cores ) && ( best.cores < instancetype.cores ))
			continue;
		if (( best.speed ) && ( instancetype.speed ) && ( best.speed < instancetype.speed ))
			continue;
		if (( best.memory ) && ( instancetype.memory ) && ( best.memory < instancetype.memory ))
			continue;
		if (( best.storage ) && ( instancetype.storage ) && ( best.storage < instancetype.storage ))
			continue;
		/* -------------------- */
		/* in fact it is better */
		/* -------------------- */
		rest_log_message("cn_contract best instance choice updated");
		rest_log_message(instancetype.name);
		best.cores = instancetype.cores;
		best.speed = instancetype.speed;
		best.memory = instancetype.memory;
		best.storage = instancetype.storage;
		best.id = instancetype.id;
	}
	if (!( best.id ))
		return( allocate_string( best.id ) );
	else	return(allocate_string( best.id ) );
}

/*	-----------------------------------------------------------------  */
/*      r e s o l v e _ c o n t r a c t _ s e c u r i t y g r o u p    	   */
/*	-----------------------------------------------------------------  */
private char * resolve_contract_securitygroup( struct cn_config * config, struct cords_cn_contract * cptr, struct computenext * pptr )
{
	struct	cn_response  * cnptr;
	struct	data_element * fptr;
	struct	data_element * dptr;
	char *	nptr;
	char *  vptr;
	
	/* ---------------------------------------- */
	/* look for existing network in computenext */
	/* ---------------------------------------- */
	rest_log_message("cn_contract resolving securitygroup");
	if (!( nptr = occi_extract_atribut( cptr->network.message, "occi", _CORDS_NETWORK, _CORDS_NAME ) ))
		return((char *) 0);
	else if (!( fptr = cptr->securitygroups->jsonroot ))
		return((char *) 0);
	
	for ( 	dptr=fptr->first;
		dptr != (struct data_element *) 0;
		dptr = dptr->next )
	{
		if (!( vptr = json_atribut( dptr, "ProviderName" ) ))
			continue;
		else if ( strcmp( vptr, cptr->provider ) )
			continue;
		else if (!( vptr = json_atribut( dptr, "Region" ) ))
			continue;
		else if ( strcmp( vptr, cptr->region ) )
			continue;
		else if (!( vptr = json_atribut( dptr, "Name" ) ))
			continue;
		else if ( strcmp( vptr, nptr ) )
			continue;
		
		else
		{
			rest_log_message("cn_contract found existing security group");
			rest_log_message(vptr);
			return( allocate_string( vptr ) );
		}			
	}
	
	return( build_computenext_securitygroup(config, cptr, pptr) );
}

/*	-----------------------------------------------------------------  */
/*      b u i l d _ c o m p u t e n e x t _ s e c u r i t y g r o u p  */
/*	-----------------------------------------------------------------  */
private	char *	build_computenext_securitygroup(struct cn_config * config, struct cords_cn_contract * cptr, struct computenext * pptr )
{
	char *	filename;
	struct  cn_response *   cnptr;
	struct  occi_element *  eptr;
	struct  occi_response * rptr;
	char * rulefrom=(char *) 0;
	char * ruleto=(char *) 0;
	char * ruleproto=(char *) 0;
	char * sptr;
	char * nptr=(char *) 0;
	char * zptr=(char *) 0;
	
	/* -------------------------- */
	/* create the security group  */
	/* -------------------------- */
	rest_log_message("cn_contract building new security group");
	if(!( nptr = occi_extract_atribut( cptr->network.message, "occi", _CORDS_NETWORK, _CORDS_NAME ) ))
		return( allocate_string("") );
	if (!( filename = cn_create_securitygroup_request( 
				nptr, "CNPROCCI auto-generated securitygroup", 
				cptr->region, cptr->providerId )))
		return( allocate_string(nptr) );
	else if (!( cnptr = cn_create_securitygroup( config,filename ) ))
		return( allocate_string(nptr) );
	else if (!( sptr = json_atribut( cnptr->jsonroot, "SecurityGroupId") ))
	{
		cnptr = cn_liberate_response( cnptr );
		return( allocate_string(nptr) );
	}

	rest_log_message("cn_contract securitygroup built");
	rest_log_message(sptr);
	
	for (	eptr = cords_first_link( cptr->network.message );
		eptr != (struct occi_element *) 0;
		eptr = cords_next_link( eptr ) )
	{	

		/* ---------------------------------- */
		/* retrieve the port rule information */
		/* ---------------------------------- */
		if(!( zptr = occi_unquoted_value(eptr->value) ))
			return( allocate_string(nptr) );
		else if(!( rptr = occi_simple_get(zptr, _CORDS_CN_AGENT, default_tls()) ))
			return( allocate_string(nptr) );
		else if ((!( ruleproto = occi_extract_atribut( rptr, "occi", 
			_CORDS_PORT, _CORDS_PROTOCOL ) ))
		||  (!( rulefrom = occi_extract_atribut( rptr, "occi", 
			_CORDS_PORT, _CORDS_FROM ) ))
		||  (!( ruleto = occi_extract_atribut( rptr, "occi", 
			_CORDS_PORT, _CORDS_TO   ) )) )
		{
			occi_remove_response(rptr);
			return( allocate_string(nptr) );
		}
		else occi_remove_response(rptr);
		/* ---------------------------------- */
		/* add the port to the security group */
		/* ---------------------------------- */
		if (!( filename = cn_add_port_request( ruleproto, rulefrom, ruleto, "0.0.0.0/0" ) ))
			return( allocate_string(nptr) );
		else if (!( cnptr = cn_add_port( config, sptr, filename ) ))
			return( allocate_string(nptr) );
		else if (!( sptr = json_atribut( cnptr->jsonroot, "SecurityGroupId") ))
		{
			cnptr = cn_liberate_response( cnptr );
			return( allocate_string(nptr) );
		}
	}
	
	cnptr = cn_liberate_response( cnptr );
	rest_log_message("cn_contract ports added successfully");
	return( allocate_string(nptr) );
}


/*	-----------------------------------------------------------------  */
/*      b u i l d _ c o m p u t e n e x t _ w o r k l o a d            	   */
/*	-----------------------------------------------------------------  */
private	char *	build_computenext_workload(struct cn_config * config, struct computenext * pptr )
{
	struct cn_response * cnptr;
	struct standard_message node;
	char *  vptr;
	char *  sptr;
	char *  filename;
	char    wlname[256];
	
	rest_log_message("*** CN PROCCI Building New Workload ***");
	
	memset( &node, 0, sizeof(struct standard_message));
	
	/* --------------------------- */
	/* create a new empty workload */
	/* --------------------------- */
	if( get_standard_message( &node, pptr->node, _CORDS_CONTRACT_AGENT, default_tls() ) )
		return ( (char *) 0);
	else if(!( vptr = occi_extract_atribut( node.message, "occi", _CORDS_NODE, _CORDS_NAME ) ))
		return ( (char *) 0);
	else release_standard_message( &node );
		
	sprintf(wlname,"CO-%ld-%s",time((long *) 0),vptr);
	
	rest_log_message("*** CN PROCCI Workload Name ***");
	rest_log_message(wlname);
	
	if(!(filename = cn_create_workload_request(wlname) ))
		return ( (char *) 0); 
	else if(!(cnptr = cn_create_workload( config, filename ) ))
		return ( (char *) 0);  
	else if(cnptr->response->status >= 300)
		return ( (char *) 0);	
	else if (!( sptr = json_atribut( cnptr->jsonroot, "WorkloadId" ) ))
	{
		cnptr = cn_liberate_response( cnptr );
		return ( (char *) 0);
	}
	
	rest_log_message("*** CN PROCCI Workload Built Successfully ***");
	rest_log_message(sptr);
	
	/* ------------------------------- */
	/* add resolved vm to new workload */
	/* ------------------------------- */
	if(!(filename = cn_add_vm_request( vptr, 1, pptr->image,
		pptr->instancetype, pptr->securitygroup, "MyKeyPair", "" ) ))
		return ( (char *) 0);
	else if(!(cnptr = cn_add_vm_to_workload( config, sptr, filename ) ))
		return ( (char *) 0);  
	else if(cnptr->response->status >= 300)
		return ( (char *) 0);
	else cnptr = cn_liberate_response( cnptr );
	
	rest_log_message("*** CN PROCCI VM Added Successfully ***");
	
	return( allocate_string(sptr) );

}

/*	-----------------------------------------------------------------  */
/* 	    u s e _ c o m p u t e n e x t _ c o n f i g u r a t i o n      */
/*	-----------------------------------------------------------------  */
private	struct cn_config * use_computenext_configuration( char * nptr )
{
	struct	cn_config * sptr;
	if (( sptr = resolve_cn_configuration( nptr )) != (struct cn_config *) 0)
		return( sptr );
	else if (!( nptr = get_operator_profile() ))
		return( sptr );
	else 	return( resolve_cn_configuration( nptr ) );
}

/*  -----------------------------------------------------------------  */
/* 	    r e s o l v e _ c n  _ c o n f i g u r a t i o n               */
/*  -----------------------------------------------------------------  */
private	struct	cn_config * resolve_cn_configuration( char * sptr )
{
	struct	occi_kind_node * nptr;
	struct	cn_config * pptr=(struct cn_config *) 0;
	struct	occi_kind_node  * occi_first_cn_config_node();
	struct	occi_kind_node  * occi_next_cn_config_node();

	rest_log_message("resolve_cn_configuration");
	rest_log_message( sptr );
	
	for (	nptr = occi_first_cn_config_node();
		nptr != (struct occi_kind_node *) 0;
		nptr = occi_next_cn_config_node(nptr) )
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
			return( cn_initialise_client( 
				pptr->apikey, pptr->apisec, pptr->host, 
				_CORDS_CN_AGENT, pptr->version, pptr->tls ));
	}
	return((struct cn_config *) 0);
}


/*  -----------------------------------------------------------------  */
/* 	    c o m p u t e n e x t _ b u i l d _ f a i l u r e              */
/*  -----------------------------------------------------------------  */
private	int	computenext_build_failure( struct computenext * pptr, int status, char * message )
{
	struct	cn_response * zptr;
	rest_log_message( message );
	if ((zptr = stop_computenext_provisioning( pptr )) != (struct cn_response *) 0)
		zptr = cn_liberate_response( zptr );

	return( status );
}

/*  -----------------------------------------------------------------  */
/*      s t o p  _ c o m p u t e n e x t _ p r o v i s i o n i n g     */
/*  -----------------------------------------------------------------  */
private	struct cn_response * stop_computenext_provisioning( struct computenext * pptr )
{
	int	   status;
	struct cn_response * cnptr;
	struct cn_config * config=(struct cn_config *) 0;
	struct	data_element * fptr;
	char   reference[512];
	char *	vptr;

	rest_log_message("*** CNPROCCI Stop Provisioning ****");
	if (!( config = use_computenext_configuration( pptr->profile )))
		return((struct cn_response *) 0);
	else
	{
		/* ------------------------------------------- */
		/* perform pre-release actions for destruction */
		/* ------------------------------------------- */
		sprintf(reference,"%s/%s/%s",Configuration.identity,_CORDS_COMPUTENEXT,pptr->id);

		if ( use_cosacs_agent( pptr->agent ) )
		{
			if ( cosacs_test_interface( pptr->hostname, _COSACS_STOP_TIMEOUT, _COSACS_STOP_RETRY ) )
			{
				cosacs_metadata_instructions( 
					pptr->hostname, _CORDS_RELEASE,
					reference, Configuration.publisher, pptr->account );
			}
		}

		/* ------------------------------------------ */
		/* delete workload if there is no transaction */
		/* ------------------------------------------ */
		if(!(pptr->transaction))
		{
			if(!(pptr->workload))
				cnptr = cn_liberate_response( cnptr );
			else if(!(cnptr=cn_delete_workload(config, pptr->workload) )) {
				cnptr = cn_liberate_response( cnptr );
			}
		}
		
		/* -------------------------------------- */
		/* launch the deletion of the transaction */
		/* -------------------------------------- */
		else if ((cnptr=cn_delete_transaction( config,pptr->transaction )) != (struct cn_response *) 0)
		{
			/* -------------------------- */
			/* await transaction deletion */
			/* -------------------------- */
			if(cnptr->response->status >= 300)
				cnptr = cn_liberate_response( cnptr );
			else
			{
				cnptr = cn_liberate_response( cnptr );
				while ((cnptr=cn_get_transaction( config, pptr->transaction )) != (struct cn_response *) 0)
				{
					if (!(fptr = cnptr->jsonroot))
						break;
					else if (!( vptr = json_atribut( fptr, "StatusCode" ) ))
						break;
					else if (!( strcmp( vptr, _CN_STATUS_DELETED ) ))
						break;
					else
					{
						sleep( _CN_DESTROY_SLEEP );
						cnptr = cn_liberate_response( cnptr );
					}
				}	
			}
		}
		pptr->state = _OCCI_IDLE;
		if ( pptr->hostname )
			pptr->hostname = liberate(pptr->hostname);
		if ( pptr->ipaddress )
			pptr->ipaddress = liberate(pptr->ipaddress);
		return( cnptr );
	}
}

/*	-----------------------------------------------------------------  */
/*	    t e r m i n a t e _ c o m p u t e n e x t _ c o n t r a c t    */
/*	-----------------------------------------------------------------  */
private	int	terminate_computenext_contract( int status, struct cords_cn_contract * cptr )
{
	char sptr[256];
	
	sprintf(sptr, "Status: %d", status);
	rest_log_message("*** CNPROCCI Contract Terminated ***");
	rest_log_message(sptr);
	
	if ( cptr->node.message )
		cptr->node.message = occi_remove_response( cptr->node.message );
	if ( cptr->infrastructure.message )
		cptr->infrastructure.message = occi_remove_response( cptr->infrastructure.message );
	if ( cptr->compute.message )
		cptr->compute.message = occi_remove_response( cptr->compute.message );
	if ( cptr->network.message )
		cptr->network.message = occi_remove_response( cptr->network.message );
	if ( cptr->firewall.message )
		cptr->firewall.message = occi_remove_response( cptr->firewall.message );
	if ( cptr->storage.message )
		cptr->storage.message = occi_remove_response( cptr->storage.message );
	if ( cptr->image.message )
		cptr->image.message = occi_remove_response( cptr->image.message );
	if ( cptr->system.message )
		cptr->system.message = occi_remove_response( cptr->system.message );
	if ( cptr->package.message )
		cptr->package.message = occi_remove_response( cptr->package.message );
	if ( cptr->instancetypes )
		cptr->instancetypes = cn_liberate_response( cptr->instancetypes );
	if ( cptr->images  )
		cptr->images  = cn_liberate_response( cptr->images  );
	if ( cptr->config )
		cptr->config = cn_liberate_config( cptr->config );

	return( status );
}

	    /* ----------- */
#endif	/* _contract_c */
	    /* ----------- */


