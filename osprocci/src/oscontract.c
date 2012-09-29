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
#ifndef	_oscontract_c
#define	_oscontract_c

#include "occiclient.h"

struct	cords_vector
{
	char *	id;
	struct occi_response * message;
};

struct	cords_os_contract
{
	struct	os_subscription * subscription;
	struct	cords_vector	node;
	struct	cords_vector	infrastructure;
	struct	cords_vector	compute;
	struct	cords_vector	network;
	struct	cords_vector	firewall;
	struct	cords_vector	storage;
	struct	cords_vector	image;
	struct	cords_vector	system;
	struct	cords_vector	package;
	struct	os_response *	flavors;
	struct	os_response *	images;
};

struct	os_compute_infos
{
	int	cores;
	int	speed;
	int	memory;
	int	storage;
	char 	architecture[256];
	char *	id;
};

struct	os_image_infos
{
	char *	id;
	char *	other;
	char *	name;
	char *	updated;
};

/*	-----------------------------------------------------------------	*/
/*	     t e r m i n a t e _ o p e n s t a c k _ c o n t r a c t		*/
/*	-----------------------------------------------------------------	*/
private	int	terminate_openstack_contract( int status, struct cords_os_contract * cptr )
{
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
	if ( cptr->flavors )
		cptr->flavors = liberate_os_response( cptr->flavors );
	if ( cptr->images  )
		cptr->images  = liberate_os_response( cptr->images  );
	if ( cptr->subscription )
		cptr->subscription = os_liberate_subscription( cptr->subscription );

	return( status );
}

/*	-----------------------------------------------------------------	*/
/*		r e s o l v e _ c o n t r a c t _ f l a v o r 			*/
/*	-----------------------------------------------------------------	*/
private	char *	resolve_contract_flavor( struct	os_subscription * subptr, struct cords_os_contract * cptr )
{
	struct	os_compute_infos	request;
	struct	os_compute_infos	flavor;
	struct	os_compute_infos	best;
	char *			vptr;

	struct	data_element * eptr=(struct data_element *) 0;
	struct	data_element * dptr=(struct data_element *) 0;

	if (!( eptr = json_element( cptr->flavors->jsonroot, "flavors" )))
		return((char *) 0);

	/* -------------------------------------------------------------- */
	/* retrieve appropriate parameters from infrastructure components */
	/* -------------------------------------------------------------- */
	if (!( vptr = occi_extract_atribut( cptr->compute.message, "occi", 
		_CORDS_COMPUTE, _CORDS_MEMORY ) ))
		request.memory = 0;
	else	request.memory = rest_normalise_value( vptr,'G' );

	if (!( vptr = occi_extract_atribut( cptr->compute.message, "occi", 
		_CORDS_COMPUTE, _CORDS_CORES ) ))
		request.cores = 0;
	else	request.cores = rest_normalise_value( vptr,'U' );

	if (!( vptr = occi_extract_atribut( cptr->compute.message, "occi", 
		_CORDS_COMPUTE, _CORDS_SPEED ) ))
		request.speed = 0;
	else	request.speed = rest_normalise_value(vptr,'G');
	
	if (!( vptr = occi_extract_atribut( cptr->storage.message, "occi", 
		_CORDS_STORAGE, _CORDS_SIZE ) ))
		request.storage = 0;
	else	request.storage = rest_normalise_value(vptr,'G');

	/* --------------------------------------------- */
	/* collect and control the architecture required */
	/* --------------------------------------------- */
	if (!( vptr = occi_extract_atribut( cptr->storage.message, "occi", 
		_CORDS_COMPUTE, _CORDS_ARCHITECTURE ) ))
		strcpy(request.architecture,"blank" );
	else if ((!(strcasecmp( vptr, "x86"    		) ))
	     ||  (!(strcasecmp( vptr, "x86_32" 		) ))
	     ||  (!(strcasecmp( vptr, "x86_64" 		) )))
		strcpy(request.architecture,"untrusted" );
	else if ((!(strcasecmp( vptr, "txt86"  		) ))
	     ||  (!(strcasecmp( vptr, "txt86_32" 	) ))
	     ||  (!(strcasecmp( vptr, "txt86_64"  	) )))
		strcpy(request.architecture,"trusted" );
	else	strcpy(request.architecture,"untrusted" );
	
	/* ----------------------------------------- */
	/* for structures in flavor message response */
	/* ----------------------------------------- */
	memset( &best, 0, sizeof( struct os_compute_infos ));
	for ( 	dptr=eptr->first;
		dptr != (struct data_element *) 0;
		dptr = dptr->next )
	{
		/* ----------------------------------------------- */
		/* collect the information from the flavor element */
		/* ----------------------------------------------- */
		if (!( vptr = json_atribut( dptr, "id" ) ))
			continue;
		else	flavor.id = vptr;
		if (!( vptr = json_atribut( dptr, "disk" ) ))
			flavor.storage = 0;
		else	flavor.storage = rest_normalise_value(vptr,'G');
		if (!( vptr = json_atribut( dptr, "ram" ) ))
			flavor.memory = 0;
		else	flavor.memory = rest_normalise_value(vptr,'M');
		if (!( vptr = json_atribut( dptr, "vcpus" ) ))
			flavor.cores = 0;
		else	flavor.cores = rest_normalise_value(vptr,'U');

		if (!( vptr = json_atribut( dptr, "speed" ) ))
			flavor.speed = 0;
		else	flavor.speed = rest_normalise_value(vptr,'G');

		if (!( vptr = json_atribut( dptr, "trust_lvl" ) ))
			strcpy(flavor.architecture,"untrusted" );

		else	strcpy(flavor.architecture,vptr  );

		/* ------------------------------------ */
		/* handle flavour architecture types    */
		/* ------------------------------------ */
		if (!( strncasecmp( request.architecture, "trusted", strlen("trusted") ) ))
		{
			if ( strncasecmp( flavor.architecture, "trusted",strlen("trusted") ) != 0 )
				continue; 
		}
		else if (!( strncasecmp( flavor.architecture, "trusted",strlen("trusted") ) ))
			continue; 
	
		/* ------------------------------------ */
		/* compare the request and the response */
		/* ------------------------------------ */
		if (( request.storage ) && ( flavor.storage < request.storage ))
			continue;
		else if (( request.memory  ) && ( flavor.memory < request.memory ))
			continue;
		else if (( request.cores ) && ( flavor.cores ) && ( flavor.cores < request.cores ))
			continue;
		else if (( request.speed ) && ( flavor.speed ) && ( flavor.speed < request.speed ))
			continue;
		/* --------------------- */
		/* ok so its good enough */
		/* --------------------- */
		if (( best.cores ) && ( flavor.cores ) && ( best.cores < flavor.cores ))
			continue;
		if (( best.speed ) && ( flavor.speed ) && ( best.speed < flavor.speed ))
			continue;
		if (( best.memory ) && ( best.memory < flavor.memory ))
			continue;
		if (( best.storage ) && ( best.storage < flavor.storage ))
			continue;
		/* -------------------- */
		/* in fact it is better */
		/* -------------------- */
		best.cores = flavor.cores;
		best.speed = flavor.speed;
		best.memory = flavor.memory;
		best.storage = flavor.storage;
		best.id = flavor.id;
	}
	if (!( best.id ))
		return( best.id );
	else	return(allocate_string( best.id ) );
}

/*	-----------------------------------------------------------------	*/
/*		r e s o l v e _ c o n t r a c t _ i m a g e   			*/
/*	-----------------------------------------------------------------	*/
private	char *	resolve_contract_image( struct	os_subscription * subptr, struct cords_os_contract * cptr )
{
	struct	os_image_infos	request;
	struct	os_image_infos	image;
	struct	os_image_infos	best;
	char *			vptr;
	char *			iname=(char*) 0;
	struct	data_element * eptr=(struct data_element *) 0;
	struct	data_element * dptr=(struct data_element *) 0;


	if (!( eptr = json_element( cptr->images->jsonroot, "images" )))
		return((char *) 0);

	/* ---------------------------------------------------------- */
	/* retrieve appropriate parameters from node image components */
	/* ---------------------------------------------------------- */
	if (!( vptr = occi_extract_atribut( cptr->system.message, "occi", 
		_CORDS_SYSTEM, _CORDS_NAME ) ))
		return((char *) 0);
	else	request.name = vptr;

	rest_log_message( "os_contract system :");
	rest_log_message( request.name );

	if (!( vptr = occi_extract_atribut( cptr->image.message, "occi", 
		_CORDS_IMAGE, _CORDS_NAME ) ))
		return((char *) 0);
	else	request.other = vptr;

	rest_log_message( "os_contract image :");
	rest_log_message( request.other );

	memset( &best, 0, sizeof( struct os_image_infos ));

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
		{
			liberate( iname );
			return((char *) 0);
		}
		else
		{
			liberate( iname );
			rest_log_message("os_contract perfect match");
			rest_log_message( vptr );
			return( allocate_string( vptr ) );
		}
	}
	liberate( iname );

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

		if ( (!( strncasecmp( request.name,  image.name, strlen( request.name  ) )))
		||   (!( strncasecmp( request.other, image.name, strlen( request.other ) ))))
		{
			best.id = image.id;
			best.name = image.name;
			rest_log_message("os_contract found match");
			rest_log_message( image.name );
			rest_log_message( image.id );
			break;
		}		
		else	continue;
	}
	if (!( best.id ))
		return( best.id );
	else
	{
		rest_log_message("os_contract best match");
		rest_log_message( best.name );
		rest_log_message( best.id );
		return(allocate_string( best.id ));
	}
}

/*	-----------------------------------------------------------------	*/
/*		c r e a t e _ o p e n s t a c k _ c o n t r a c t		*/
/*	-----------------------------------------------------------------	*/
public	int	create_openstack_contract(
		struct occi_category * optr,
		struct openstack * pptr,
		char * agent,
		char * tls )
{
	struct	os_subscription * subptr=(struct os_subscription *) 0;
	struct	cords_os_contract contract;
	struct	os_response * flavors=(struct os_response *) 0;
	struct	os_response * images =(struct os_response *) 0;
	int	status;
	char *	vptr;

	if (!(subptr = use_openstack_configuration( pptr->profile )))
		return( status );
	else
	{
		memset( &contract, 0, sizeof( struct cords_os_contract ));
		contract.subscription = subptr;
	}

	/* ---------------------------- */
	/* recover the node description */
	/* ---------------------------- */
	if (!( contract.node.id = pptr->node ))
		return( 0 );
	else if (!( contract.node.message = occi_simple_get( contract.node.id, agent, tls ) ))
		return( terminate_openstack_contract( 1170, &contract ) );
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
			return( terminate_openstack_contract( 1127, &contract ) );
	}
	/* -------------------------------- */
	/* recover the firewall description */
	/* -------------------------------- */
	if (( contract.firewall.id = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_FIREWALL )) != (char * ) 0)
	{
		if (!( contract.firewall.message = occi_simple_get( contract.firewall.id, agent, tls ) ))
			return( terminate_openstack_contract( 1171, &contract ) );
		else if (!( pptr->firewall = allocate_string( contract.firewall.id ) ))
			return( terminate_openstack_contract( 1172, &contract ) );
	}

	/* -------------------------------------- */
	/* recover the infrastructure description */
	/* -------------------------------------- */
	if (!( contract.infrastructure.id = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_INFRASTRUCTURE ) ))
		return( terminate_openstack_contract( 1171, &contract ) );
	else if (!( contract.infrastructure.message = occi_simple_get( contract.infrastructure.id, agent, tls ) ))
		return( terminate_openstack_contract( 1172, &contract ) );

	else if (!( contract.compute.id = occi_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_COMPUTE ) ))
		return( terminate_openstack_contract( 1173, &contract ) );
	else if (!( contract.compute.message = occi_simple_get( contract.compute.id, agent, tls ) ))
		return( terminate_openstack_contract( 1174, &contract ) );

	else if (!( contract.network.id = occi_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_NETWORK ) ))
		return( terminate_openstack_contract( 1175, &contract ) );
	else if (!( contract.network.message = occi_simple_get( contract.network.id, agent, tls ) ))
		return( terminate_openstack_contract( 1177, &contract ) );
	else if (!( pptr->network = allocate_string( contract.network.id ) ))
		return( terminate_openstack_contract( 1176, &contract ) );

	else if (!( contract.storage.id = occi_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_STORAGE ) ))
		return( terminate_openstack_contract( 1178, &contract ) );
	else if (!( contract.storage.message = occi_simple_get( contract.storage.id, agent, tls ) ))
		return( terminate_openstack_contract( 1179, &contract ) );

	/* --------------------------------------------------------- */
	/* recover detailled list of OS Flavors and resolve contract */
	/* --------------------------------------------------------- */
	else if (!( contract.flavors = os_list_flavor_details(subptr) ))
		return( terminate_openstack_contract( 1180, &contract ) );
	else if (!( pptr->flavor = resolve_contract_flavor(subptr, &contract ) ))
		return( terminate_openstack_contract( 1181, &contract ) );
		

	/* ---------------------------------- */
	/* recover the node image description */
	/* ---------------------------------- */
	if (!( contract.image.id = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_IMAGE ) ))
		return( terminate_openstack_contract( 1182, &contract ) );
	else if (!( contract.image.message = occi_simple_get( contract.image.id, agent, tls ) ))
		return( terminate_openstack_contract( 1183, &contract ) );

	/* ------------------------ */
	/* retrieve the cosacs flag */
	/* ------------------------ */
	else if ((!( pptr->agent = occi_extract_atribut( contract.image.message, "occi", 
		_CORDS_IMAGE, "agent" ) ))
	     &&  (!( pptr->agent = allocate_string("cosacs") )))
		return( terminate_openstack_contract( 1283, &contract ) );

	else if (!( contract.system.id = occi_extract_atribut( contract.image.message, "occi", 
		_CORDS_IMAGE, _CORDS_SYSTEM ) ))
		return( terminate_openstack_contract( 1184, &contract ) );
	else if (!( contract.system.message = occi_simple_get( contract.system.id, agent, tls ) ))
		return( terminate_openstack_contract( 1185, &contract ) );

	/* ------------------------------------------------------ */
	/* retrieve detailled list of images and resolve contract */
	/* ------------------------------------------------------ */
	else if (!( contract.images = os_list_image_details(subptr) ))
		return( terminate_openstack_contract( 1186, &contract ) );
	else if (!( pptr->image = resolve_contract_image( subptr, &contract ) ))
		return( terminate_openstack_contract( 1187, &contract ) );
	else if (!( pptr->original = allocate_string( pptr->image ) ))
		return( terminate_openstack_contract( 1188, &contract ) );
	{
		/* ----------------------------------------------- */
		/* resolve any price informatino for this category */
		/* ----------------------------------------------- */
		pptr->price = occi_resolve_category_price( _CORDS_OPENSTACK, default_operator(), agent, tls );
	
		return( terminate_openstack_contract( 0, &contract ) );
	}
}

/*	-----------------------------------------------------------------	*/
/*		d e l e t e _ o p e n s t a c k _ c o n t r a c t		*/
/*	-----------------------------------------------------------------	*/
public	int	delete_openstack_contract(
		struct occi_category * optr,
		struct openstack * pptr,
		char * agent,
		char * tls )
{
	struct	os_response * osptr;
	struct	os_subscription * subptr;
	if ( pptr->state == _OCCI_IDLE )
		return(0);
	else if (!(subptr = use_openstack_configuration( pptr->profile )))
		return(0);
	else if ((osptr = stop_openstack_provisioning( pptr )) != (struct os_response *) 0)
		osptr = liberate_os_response( osptr );
	if (!( pptr->image ))
		return( 0 );
	else if (!( pptr->original ))
		return( 0 );
	else if (!( strcmp( pptr->original, pptr->image ) ))
		return( 0 );
	else
	{
		os_delete_image( subptr, pptr->image );
		subptr = os_liberate_subscription( subptr );
		return(0);
	}
}

	/* ------------- */
#endif	/* _oscontract_c */
	/* ------------- */

