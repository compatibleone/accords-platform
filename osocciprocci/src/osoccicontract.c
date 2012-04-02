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
#ifndef	_os_occi_contract_c
#define	_os_occi_contract_c

#include "occiclient.h"
#include "occiresolver.h"

struct	cords_vector
{
	char *	id;
	struct occi_response * message;
};

struct	cords_os_contract
{
	struct	cords_vector	node;
	struct	cords_vector	infrastructure;
	struct	cords_vector	compute;
	struct	cords_vector	network;
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
	char *	id;
};

struct	os_image_infos
{
	char *	id;
	char *	other;
	char *	name;
	char *	updated;
};

/* ---------------------------------------------------------------------------- */
/* 		r e s o l v e _ o s _ c o n f i g u r a t i o n			*/
/* ---------------------------------------------------------------------------- */
private	struct	os_config * resolve_os_occi_configuration( char * sptr )
{
	struct	occi_kind_node * nptr;
	struct	os_config * pptr=(struct os_config *) 0;
	struct	occi_kind_node  * occi_first_os_config_node();
	for (	nptr = occi_first_os_config_node();
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next )
	{
		if (!( pptr = nptr->contents ))
			continue;
		else if (!( pptr->name ))
			continue;
		else if (!( strcmp( pptr->name, sptr ) ))
			break;
	}
	return( pptr );
}

/*	--------------------------------------------------------	*/
/* 	 u s e _ o p e n s t a c k _ c o n f i g u r a t i o n 		*/
/*	--------------------------------------------------------	*/
private	int	use_occi_openstack_configuration( char * sptr )
{
	struct	os_config * pptr;

	if (!( pptr = resolve_os_occi_configuration( sptr )))
	 	return( 404 );
#ifdef	_OS_OCCI_PROCCI_TRUC
	TODO
	else 	return( os_initialise_client( 
			pptr->user, pptr->password, 
			pptr->host, _CORDS_OS_AGENT, pptr->version, pptr->tls ));
#else
	return(0);
#endif
}

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
	if ( cptr->storage.message )
		cptr->storage.message = occi_remove_response( cptr->storage.message );
	if ( cptr->image.message )
		cptr->image.message = occi_remove_response( cptr->image.message );
	if ( cptr->system.message )
		cptr->system.message = occi_remove_response( cptr->system.message );
	if ( cptr->package.message )
		cptr->package.message = occi_remove_response( cptr->package.message );
/*
 TODO
	if ( cptr->flavors )
		cptr->flavors = liberate_os_response( cptr->flavors );
	if ( cptr->images  )
		cptr->images  = liberate_os_response( cptr->images  );
 */
	return( status );
}

/*	-----------------------------------------------------------------	*/
/*		c r e a t e _ o p e n s t a c k _ c o n t r a c t		*/
/*	-----------------------------------------------------------------	*/
private	int	create_openstack_contract(
		struct occi_category * optr,
		struct openstack * pptr,
		char * agent,
		char * tls )
{
	struct	cords_os_contract contract;
	struct	os_response * flavors=(struct os_response *) 0;
	struct	os_response * images =(struct os_response *) 0;
	int	status;
	char *	vptr;

	if ((status = use_occi_openstack_configuration( pptr->profile )) != 0)
		return( status );
	else	memset( &contract, 0, sizeof( struct cords_os_contract ));

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
/*
TODO
	else if (!( contract.flavors = os_list_flavor_details() ))
		return( terminate_openstack_contract( 1180, &contract ) );
	else if (!( pptr->flavor = resolve_contract_flavor( &contract ) ))
		return( terminate_openstack_contract( 1181, &contract ) );
*/		

	/* ---------------------------------- */
	/* recover the node image description */
	/* ---------------------------------- */
	if (!( contract.image.id = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_IMAGE ) ))
		return( terminate_openstack_contract( 1182, &contract ) );
	else if (!( contract.image.message = occi_simple_get( contract.image.id, agent, tls ) ))
		return( terminate_openstack_contract( 1183, &contract ) );

	else if (!( contract.system.id = occi_extract_atribut( contract.image.message, "occi", 
		_CORDS_IMAGE, _CORDS_SYSTEM ) ))
		return( terminate_openstack_contract( 1184, &contract ) );
	else if (!( contract.system.message = occi_simple_get( contract.system.id, agent, tls ) ))
		return( terminate_openstack_contract( 1185, &contract ) );

	/* ------------------------------------------------------ */
	/* retrieve detailled list of images and resolve contract */
	/* ------------------------------------------------------ */
/*
 TODO
	else if (!( contract.images = os_list_image_details() ))
		return( terminate_openstack_contract( 1186, &contract ) );
	else if (!( pptr->image = resolve_contract_image( &contract ) ))
		return( terminate_openstack_contract( 1187, &contract ) );
 */
	else if (!( pptr->original = allocate_string( pptr->image ) ))
		return( terminate_openstack_contract( 1188, &contract ) );
	else
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
private	int	delete_openstack_contract(
		struct occi_category * optr,
		struct openstack * pptr,
		char * agent,
		char * tls )
{
	struct	os_response * osptr;
/*
 TODO
	if ((osptr = stop_openstack_provisioning( pptr )) != (struct os_response *) 0)
		osptr = liberate_os_response( osptr );
 */
	if (!( pptr->image ))
		return( 0 );
	else if (!( pptr->original ))
		return( 0 );
	else if (!( strcmp( pptr->original, pptr->image ) ))
		return( 0 );
	else
	{
		os_delete_image( pptr->image );
		return(0);
	}
}


	/* ------------------- */
#endif	/* _os_occi_contract_c */
	/* ------------------- */

