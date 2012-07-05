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
#ifndef	_pacontract_c
#define	_pacontract_c

#include "occiclient.h"

struct	cords_vector
{
	char *	id;
	struct occi_response * message;
};

struct	cords_pa_contract
{
	struct	cords_vector	node;
	struct	cords_vector	infrastructure;
	struct	cords_vector	compute;
	struct	cords_vector	network;
	struct	cords_vector	firewall;
	struct	cords_vector	storage;
	struct	cords_vector	image;
	struct	cords_vector	system;
	struct	cords_vector	package;
	struct	pa_response *	flavors;
	struct	pa_response *	images;
};

struct	pa_compute_infos
{
	int	cores;
	int	speed;
	int	memory;
	int	storage;
	char *	id;
};

struct	pa_image_infos
{
	char *	id;
	char *	other;
	char *	name;
	char *	updated;
};

/*	-----------------------------------------------------------------	*/
/*	     t e r m i n a t e _ proactive _ c o n t r a c t		*/
/*	-----------------------------------------------------------------	*/
private	int	terminate_proactive_contract( int status, struct cords_pa_contract * cptr )
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
		cptr->flavors = liberate_pa_response( cptr->flavors );
	if ( cptr->images  )
		cptr->images  = liberate_pa_response( cptr->images  );
	return( status );
}


/*	---------------------------------------------------	*/
/*		o s _ n o r m a l i s e _ v a l u e 		*/
/*	---------------------------------------------------	*/
/*	this function takes a value string as its parameter	*/
/*	which may or not be terminated by a T,G,M,K which	*/
/*	indicates an explicite quantity. A default quantity	*/
/*	type is passed as the second parameter to be used	*/
/*	in case no explicite value is present.			*/
/*	The function returns the normalisation of the value	*/
/*	---------------------------------------------------	*/
private	int	pa_normalise_value( char * sptr, int normal )
{
	int	factor=1;
	int	value=0;
	if ( normal == 'T' )
		factor = 1000000000;
	else if ( normal == 'G' )
		factor = 1000000;
	else if ( normal == 'M' )
		factor = 1000;
	else if ( normal == 'K' )
		factor = 1;
	else	factor = 1;

	value = atoi(sptr);

	while (( *sptr >= '0' ) && ( *sptr <= '9' )) sptr++;

	if ( *sptr == 'T' )
		factor = 1000000000;
	else if ( *sptr == 'G' )
		factor = 1000000;
	else if ( *sptr == 'M' )
		factor = 1000;
	else if ( *sptr == 'K' )
		factor = 1;

	return( value * factor );
}


/*	-----------------------------------------------------------------	*/
/*		r e s o l v e _ c o n t r a c t _ f l a v o r 			*/
/*	-----------------------------------------------------------------	*/
private	char *	resolve_contract_flavor( struct cords_pa_contract * cptr )
{
	struct	pa_compute_infos	request;
	struct	pa_compute_infos	flavor;
	struct	pa_compute_infos	best;
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
	else	request.memory = pa_normalise_value( vptr,'G' );

	if (!( vptr = occi_extract_atribut( cptr->compute.message, "occi", 
		_CORDS_COMPUTE, _CORDS_CORES ) ))
		request.cores = 0;
	else	request.cores = pa_normalise_value( vptr,'U' );

	if (!( vptr = occi_extract_atribut( cptr->compute.message, "occi", 
		_CORDS_COMPUTE, _CORDS_SPEED ) ))
		request.speed = 0;
	else	request.speed = pa_normalise_value(vptr,'G');
	
	if (!( vptr = occi_extract_atribut( cptr->storage.message, "occi", 
		_CORDS_STORAGE, _CORDS_SIZE ) ))
		request.storage = 0;
	else	request.storage = pa_normalise_value(vptr,'G');
	
	/* ----------------------------------------- */
	/* for structures in flavor message response */
	/* ----------------------------------------- */
	memset( &best, 0, sizeof( struct pa_compute_infos ));
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
		else	flavor.storage = pa_normalise_value(vptr,'G');
		if (!( vptr = json_atribut( dptr, "ram" ) ))
			flavor.memory = 0;
		else	flavor.memory = pa_normalise_value(vptr,'M');
		if (!( vptr = json_atribut( dptr, "vcpus" ) ))
			flavor.cores = 0;
		else	flavor.cores = pa_normalise_value(vptr,'U');
		if (!( vptr = json_atribut( dptr, "speed" ) ))
			flavor.speed = 0;
		else	flavor.speed = pa_normalise_value(vptr,'G');
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
private	char *	resolve_contract_image( struct cords_pa_contract * cptr )
{
	struct	pa_image_infos	request;
	struct	pa_image_infos	image;
	struct	pa_image_infos	best;
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

	if (!( vptr = occi_extract_atribut( cptr->image.message, "occi", 
		_CORDS_IMAGE, _CORDS_NAME ) ))
		return((char *) 0);
	else	request.other = vptr;

	memset( &best, 0, sizeof( struct pa_image_infos ));

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
			break;
		}		
		else	continue;
	}
	if (!( best.id ))
		return( best.id );
	else 	return(allocate_string( best.id ));
}

/*	-----------------------------------------------------------------	*/
/*		c r e a t e _ proactive _ c o n t r a c t		*/
/*	-----------------------------------------------------------------	*/
public	int	create_proactive_contract(
		struct occi_category * optr,
		struct proactive * pptr,
		char * agent,
		char * tls )
{
	struct	cords_pa_contract contract;
	struct	pa_response * flavors=(struct pa_response *) 0;
	struct	pa_response * images =(struct pa_response *) 0;
	int	status;
	char *	vptr;

	if ((status = use_proactive_configuration( pptr->profile )) != 0)
		return( status );
	else	memset( &contract, 0, sizeof( struct cords_pa_contract ));

	/* ---------------------------- */
	/* recover the node description */
	/* ---------------------------- */
	if (!( contract.node.id = pptr->node ))
		return( 0 );
	else if (!( contract.node.message = occi_simple_get( contract.node.id, agent, tls ) ))
		return( terminate_proactive_contract( 1170, &contract ) );
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
			return( terminate_proactive_contract( 1127, &contract ) );
	}
	/* -------------------------------- */
	/* recover the firewall description */
	/* -------------------------------- */
	//if (( contract.firewall.id = occi_extract_atribut( contract.node.message, "occi", 
	//	_CORDS_NODE, _CORDS_FIREWALL )) != (char * ) 0)
	//{
	//	if (!( contract.firewall.message = occi_simple_get( contract.firewall.id, agent, tls ) ))
	//		return( terminate_proactive_contract( 1171, &contract ) );
	//	else if (!( pptr->firewall = allocate_string( contract.firewall.id ) ))
	//		return( terminate_proactive_contract( 1172, &contract ) );
	//}

	/* -------------------------------------- */
	/* recover the infrastructure description */
	/* -------------------------------------- */
	if (!( contract.infrastructure.id = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_INFRASTRUCTURE ) ))
		return( terminate_proactive_contract( 1171, &contract ) );
	else if (!( contract.infrastructure.message = occi_simple_get( contract.infrastructure.id, agent, tls ) ))
		return( terminate_proactive_contract( 1172, &contract ) );

	else if (!( contract.compute.id = occi_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_COMPUTE ) ))
		return( terminate_proactive_contract( 1173, &contract ) );
	else if (!( contract.compute.message = occi_simple_get( contract.compute.id, agent, tls ) ))
		return( terminate_proactive_contract( 1174, &contract ) );

	else if (!( contract.network.id = occi_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_NETWORK ) ))
		return( terminate_proactive_contract( 1175, &contract ) );
	else if (!( contract.network.message = occi_simple_get( contract.network.id, agent, tls ) ))
		return( terminate_proactive_contract( 1177, &contract ) );
	//else if (!( pptr->network = allocate_string( contract.network.id ) ))
	//	return( terminate_proactive_contract( 1176, &contract ) );

	else if (!( contract.storage.id = occi_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_STORAGE ) ))
		return( terminate_proactive_contract( 1178, &contract ) );
	else if (!( contract.storage.message = occi_simple_get( contract.storage.id, agent, tls ) ))
		return( terminate_proactive_contract( 1179, &contract ) );

	/* --------------------------------------------------------- */
	/* recover detailled list of pa Flavors and resolve contract */
	/* --------------------------------------------------------- */
	//else if (!( contract.flavors = pa_list_flavor_details() ))
	//	return( terminate_proactive_contract( 1180, &contract ) );
	//else if (!( pptr->flavor = resolve_contract_flavor( &contract ) ))
	//	return( terminate_proactive_contract( 1181, &contract ) );
		

	/* ---------------------------------- */
	/* recover the node image description */
	/* ---------------------------------- */
	if (!( contract.image.id = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_IMAGE ) ))
		return( terminate_proactive_contract( 1182, &contract ) );
	else if (!( contract.image.message = occi_simple_get( contract.image.id, agent, tls ) ))
		return( terminate_proactive_contract( 1183, &contract ) );

	else if (!( contract.system.id = occi_extract_atribut( contract.image.message, "occi", 
		_CORDS_IMAGE, _CORDS_SYSTEM ) ))
		return( terminate_proactive_contract( 1184, &contract ) );
	else if (!( contract.system.message = occi_simple_get( contract.system.id, agent, tls ) ))
		return( terminate_proactive_contract( 1185, &contract ) );

	/* ------------------------------------------------------ */
	/* retrieve detailled list of images and resolve contract */
	/* ------------------------------------------------------ */
	//else if (!( contract.images = pa_list_image_details() ))
	//	return( terminate_proactive_contract( 1186, &contract ) );
	//else if (!( pptr->image = resolve_contract_image( &contract ) ))
	//	return( terminate_proactive_contract( 1187, &contract ) );
	//else if (!( pptr->original = allocate_string( pptr->image ) ))
	//	return( terminate_proactive_contract( 1188, &contract ) );
	{
		/* ----------------------------------------------- */
		/* resolve any price informatino for this category */
		/* ----------------------------------------------- */
		pptr->price = occi_resolve_category_price( _CORDS_PROACTIVE, default_operator(), agent, tls );
	
		return( terminate_proactive_contract( 0, &contract ) );
	}
}

/*	-----------------------------------------------------------------	*/
/*		d e l e t e _ proactive _ c o n t r a c t		*/
/*	-----------------------------------------------------------------	*/
public	int	delete_proactive_contract(
		struct occi_category * optr,
		struct proactive * pptr,
		char * agent,
		char * tls )
{
	struct	pa_response * paptr;
	if ((paptr = stop_proactive_provisioning( pptr )) != (struct pa_response *) 0)
		paptr = liberate_pa_response( paptr );
	//if (!( pptr->image ))
	//	return( 0 );
	//else if (!( pptr->original ))
	//	return( 0 );
	//else if (!( strcmp( pptr->original, pptr->image ) ))
	//	return( 0 );
	else
	{
		//pa_delete_image( pptr->image );
		return(0);
	}
}

	/* ------------- */
#endif	/* _pacontract_c */
	/* ------------- */

