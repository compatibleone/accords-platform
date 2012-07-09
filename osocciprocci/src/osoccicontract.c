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
	struct	rest_header *	flavors;
	struct	rest_header *	images;
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
			return( pptr );
	}
	return((struct os_config *) 0);
}

/*	--------------------------------------------------------	*/
/* 	 u s e _ o p e n s t a c k _ c o n f i g u r a t i o n 		*/
/*	--------------------------------------------------------	*/
private	struct	os_config * use_occi_openstack_configuration( char * sptr )
{
	struct	os_config * pptr;

	if (!( pptr = resolve_os_occi_configuration( sptr )))
	 	return( pptr );
	else if (!( 
	 	os_occi_initialise_client( 
			pptr->user, pptr->password, 
			pptr->host, pptr->namespace, _CORDS_OS_AGENT, pptr->tls ) ))
			return((struct os_config *) 0);
	else
	{
		check_keystone_authorization();
		return( pptr );
	}
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
	if ( cptr->flavors )
		cptr->flavors = liberate_rest_header( cptr->flavors );
	if ( cptr->images  )
		cptr->images  = liberate_rest_header( cptr->images  );
	return( status );
}

/*	----------------------------------------------------------	*/
/*		o c c i _ l i s t _ m i x i n s 			*/
/*	----------------------------------------------------------	*/
private struct rest_header * occi_list_mixins( char * scheme, char * host )
{
	struct	rest_header * root=(struct rest_header *) 0;
	struct	rest_header * hptr=(struct rest_header *) 0;
	struct	rest_header * foot=(struct rest_header *) 0;
	struct	occi_client * cptr;
	struct	occi_category * kptr;

	if (!( hptr = keystone_credentials() ))
		return( hptr );
	else 	occi_add_default_header( hptr );

	if (!( cptr = occi_create_client( host, _CORDS_OS_AGENT, default_tls() ) ))
	{
		occi_drop_default_headers();
		return( root );
	}
	else
	{
		occi_drop_default_headers();
		for (	kptr=cptr->firstcat;
			kptr != (struct occi_category *) 0;
			kptr = kptr->next )
		{
			if (!( kptr->scheme ))
				continue;
			else if (!( kptr->rel ))
				continue;
			else if ( strcmp( kptr->rel, scheme ) )
				continue;
			else if (!( hptr = allocate_rest_header() ))
				continue;
			else if (!( hptr->value = allocate_string( kptr->scheme ) ))
			{
				hptr = liberate_rest_header( hptr );
				continue;
			}			
			else if (!( hptr->name = allocate_string( kptr->id ) ))
			{
				hptr = liberate_rest_header( hptr );
				continue;
			}			
			else
			{
				if (!( hptr->previous = foot ))
					root = hptr;
				else	hptr->previous->next = hptr;
				foot = hptr;
			}
		}
	}
	return( root );
}

/*	----------------------------------------------------------	*/
/*		o c c i _ l i s t _ o s _ t e m p l a t e s		*/
/*	----------------------------------------------------------	*/
private struct rest_header * occi_list_os_templates(char * host)
{
	return( occi_list_mixins( _OCCI_OS_TEMPLATE, host ) );
}

/*	----------------------------------------------------------	*/
/*		  r e s o l v e _ o s _ t e m p l a t e s		*/
/*	----------------------------------------------------------	*/
private char * resolve_os_template( struct cords_os_contract * cptr )
{
	struct	rest_header * hptr;
	char *	vptr;
	char *	sysname;
	char *	bestcase=(char *) 0;
	char 	buffer[2048];
	/* ---------------------------------------------------------- */
	/* retrieve appropriate parameters from node image components */
	/* ---------------------------------------------------------- */
	if (!( vptr = occi_extract_atribut( cptr->system.message, "occi", 
		_CORDS_SYSTEM, _CORDS_NAME ) ))
		return((char *) 0);
	else	sysname = vptr;

	/* ----------------------------------------- */
	/* scan the list of os templates for a match */
	/* ----------------------------------------- */
	for ( 	hptr=cptr->images;
		hptr != (struct rest_header * ) 0;
		hptr = hptr->next )
	{
		if (!( hptr->name ))
			continue;
		if (!( hptr->value ))
			continue;
		if  (!( strncasecmp( sysname,  hptr->name, strlen( sysname  ) )))
		{
			sprintf(buffer,"%s;%s",hptr->name,hptr->value);
			return( allocate_string( buffer ) );
		}
		else 
		{
			if  (!( strncasecmp( hptr->name, sysname,  strlen(hptr->name)  ) ))
			{
				sprintf(buffer,"%s;%s",hptr->name,hptr->value);
				return( allocate_string( buffer ) );
			}
			else	continue;
		}
	}
	if (!( bestcase ))
		return( allocate_string("(null)") );
	else	return( allocate_string( bestcase ) );
}

/*	----------------------------------------------------------	*/
/*	 o c c i _ l i s t _ r e s o u r c e _ t e m p l a t e s	*/
/*	----------------------------------------------------------	*/
private struct rest_header * occi_list_resource_templates(char * host)
{
	return( occi_list_mixins( _OCCI_RESOURCE_TEMPLATE, host ) );
}

/*	----------------------------------------------------------	*/
/*	   r e s o l v e _ r e s o u r c e _ t e m p l a t e s		*/
/*	----------------------------------------------------------	*/
private char * resolve_resource_template( struct cords_os_contract * cptr )
{
	struct	rest_header * hptr;
	char *	vptr;
	char *	machname;
	char 	buffer[2048];
	if (!( vptr = occi_extract_atribut( cptr->compute.message, "occi", 
		_CORDS_COMPUTE, _CORDS_NAME ) ))
		machname = "none";
	else	machname = vptr;

	for ( 	hptr=cptr->flavors;
		hptr != (struct rest_header * ) 0;
		hptr = hptr->next )
	{
		if (!( hptr->name ))
			continue;
		else if (!( hptr->value ))
			continue;
		else if (!( strcmp( hptr->name, machname ) ))
		{
			sprintf(buffer,"%s;%s",hptr->name,hptr->value);
			return( allocate_string( buffer ) );
		}
		else	continue;
	}
	return(allocate_string("(null)"));
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
	struct	os_config * kptr;
	struct	cords_os_contract contract;
	struct	os_response * flavors=(struct os_response *) 0;
	struct	os_response * images =(struct os_response *) 0;
	int	status;
	char *	vptr;

	if (!( kptr = use_occi_openstack_configuration( pptr->profile )))
		return( 777 );
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
	else if (!( contract.flavors = occi_list_resource_templates( kptr->host) ))
		return( terminate_openstack_contract( 1180, &contract ) );
	else if (!( pptr->flavor = resolve_resource_template( &contract ) ))
		return( terminate_openstack_contract( 1181, &contract ) );

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
	else if (!( contract.images = occi_list_os_templates( kptr->host) ))
		return( terminate_openstack_contract( 1186, &contract ) );
	else if (!( pptr->image = resolve_os_template( &contract ) ))
		return( terminate_openstack_contract( 1187, &contract ) );

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
	return( 0 );
/*
TODO
	if ((osptr = stop_openstack_provisioning( pptr )) != (struct os_response *) 0)
		osptr = liberate_os_response( osptr );
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
*/
}


	/* ------------------- */
#endif	/* _os_occi_contract_c */
	/* ------------------- */

