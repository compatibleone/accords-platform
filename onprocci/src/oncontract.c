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
#ifndef	_oncontract_c
#define	_oncontract_c

#include "occiclient.h"

public	char *	occi_extract_atribut( 
	struct occi_response * zptr, char * domain,
	char * category, char * nptr );

struct	cords_vector
{
	char *	id;
	struct occi_response * message;
};

struct	cords_on_contract
{
	struct	cords_vector	node;
	struct	cords_vector	infrastructure;
	struct	cords_vector	compute;
	char *	architecture;
	char *	driver;
	struct	cords_vector	network;
	struct	cords_vector	firewall;
	struct	cords_vector	storage;
	struct	cords_vector	image;
	struct	cords_vector	system;
	struct	cords_vector	package;
	struct	on_response *	flavors;
	struct	on_response *	images;
	struct	on_response *	networks;
};

struct	on_compute_infos
{
	int	cores;
	int	speed;
	int	memory;
	int	storage;
	char *	id;
};

struct	on_image_infos
{
	char *	id;
	char *	other;
	char *	name;
	char *	updated;
};

struct	on_network_infos
{
	char *	name;
	char *	href;
};

/*	-----------------------------------------------------------------	*/
/*	     t e r m i n a t e _ o p e n n e b u l a _ c o n t r a c t		*/
/*	-----------------------------------------------------------------	*/
private	int	terminate_opennebula_contract( int status, struct cords_on_contract * cptr )
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
		cptr->flavors = liberate_on_response( cptr->flavors );
	if ( cptr->images  )
		cptr->images  = liberate_on_response( cptr->images  );
	if ( cptr->networks  )
		cptr->networks  = liberate_on_response( cptr->networks  );
	if ( cptr->architecture  )
		cptr->architecture  = liberate( cptr->architecture  );
	if ( cptr->driver  )
		cptr->driver  = liberate( cptr->driver  );
	
	return( status );
}

/*	-----------------------------------------------------------------	*/
/*		r e s o l v e _ o p e n n e b u l a _ f l a v o r 		*/
/*	-----------------------------------------------------------------	*/
private	char *	resolve_opennebula_flavor( struct cords_on_contract * cptr )
{
	struct	on_compute_infos	request;
	char *	vptr;
	char *	wptr;
	/* -------------------------------------------------------------- */
	/* retrieve appropriate parameters from infrastructure components */
	/* -------------------------------------------------------------- */
	memset( &request, 0, sizeof(struct on_compute_infos));
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

	if (!( wptr = occi_extract_atribut( cptr->compute.message, "occi", 
		_CORDS_COMPUTE, _CORDS_ARCHITECTURE ) ))
		vptr = allocate_string("x86_64");
	else 	vptr = allocate_string( wptr );

	/* ---------------------------- */
	/* scan for field separator ';' */
	/* ---------------------------- */
	wptr = vptr;
	while ( *wptr != 0 )
	{
		if ( *wptr == ';' )
			break;
		else	wptr++;
	}

	/* --------------------------------------- */
	/* detect the driver model field extension */
	/* --------------------------------------- */
	cptr->architecture = vptr;
	if ( *wptr == ';' )
	{
		*(wptr++) = 0;
		cptr->driver = allocate_string( wptr );
	}
	else	cptr->driver = (char *) 0;

	/* ---------------------------------------- */
	/* calculate the required memory model type */
	/* ---------------------------------------- */
	if (( request.storage <= 40000000 )
	&& ( request.memory  <= 2048000 ))
		return( allocate_string( "small" ) );
	else if (( request.storage > 80000000 )
	     && ( request.memory  > 4096000 ))
		return( allocate_string( "large" ) );
	else	return( allocate_string( "medium" ) );


}

/*	---------------------------------------------------	*/
/*		o p e n n e b u l a _ i m a g e _ i d		*/
/*	---------------------------------------------------	*/
private	char *	opennebula_image_id( char * href )
{
	struct	url * uptr;
	char *	result;
	char *	work;
	char *	number;
	if (!( href ))
		return( href );
	else if (!( uptr = analyse_url( href ) ))
		return( liberate( href ) );
	else if (!( uptr->object ))
	{
		uptr = liberate_url( uptr );
		return( liberate( href ) );
	}
	else if (!( result = allocate_string( uptr->object ) ))
	{
		uptr = liberate_url( uptr );
		return( liberate( href ) );
	}
	else
	{
		uptr = liberate_url( uptr );
		href = liberate( href );
		for (	number=work=result;
			*work != 0;
			work++)
		{
			if ( *work == '/' )
				number = (work+1);
		}
		work = allocate_string( number );
		liberate( result );
		return( work );
	}
}

/*	---------------------------------------------------	*/
/*		o p e n n e b u l a _ n e t w o r k _ i d	*/
/*	---------------------------------------------------	*/
private	char *	opennebula_network_id( char * href )
{
	struct	url * uptr;
	char *	result;
	char *	work;
	char *	number;
	if (!( href ))
		return( href );
	else if (!( uptr = analyse_url( href ) ))
		return( liberate( href ) );
	else if (!( uptr->object ))
	{
		uptr = liberate_url( uptr );
		return( liberate( href ) );
	}
	else if (!( result = allocate_string( uptr->object ) ))
	{
		uptr = liberate_url( uptr );
		return( liberate( href ) );
	}
	else
	{
		uptr = liberate_url( uptr );
		href = liberate( href );
		for (	number=work=result;
			*work != 0;
			work++)
		{
			if ( *work == '/' )
				number = (work+1);
		}
		work = allocate_string( number );
		liberate( result );
		return( work );
	}
}

/*	-----------------------------------------------------------------	*/
/*		r e s o l v e _ o p e n n e b u l a _ n e t w o r k 		*/
/*	-----------------------------------------------------------------	*/
private	char *	resolve_opennebula_network( struct cords_on_contract * cptr )
{
	struct	on_network_infos	request;
	struct	on_network_infos	infos;
	struct	on_network_infos	best;

	struct	xml_element * eptr;
	struct	xml_element * dptr;
	struct	xml_atribut * aptr;
	char 		    * vptr;
	char	* seekname;

	if (!( eptr = document_element( cptr->networks->xmlroot, "NETWORK_COLLECTION" )))
		return((char *) 0);

	/* ------------------------------------------------------------ */
	/* retrieve appropriate parameters from node network components */
	/* ------------------------------------------------------------ */

	else if (!( vptr = occi_extract_atribut( cptr->network.message, "occi", 
		_CORDS_NETWORK, _CORDS_NAME ) ))
		return((char *) 0);
	else if (!( request.name = occi_unquoted_value( vptr ) ))
		return((char *) 0);

	memset( &best, 0, sizeof( struct on_network_infos ));

	for ( 	dptr=eptr->first;
		dptr != (struct xml_element *) 0;
		dptr = dptr->next )
	{
		memset( &infos, 0, sizeof( struct on_network_infos ));
		if (!( aptr = document_atribut( dptr, "href" ) ))
			continue;
		else 	infos.href = occi_unquoted_value( aptr->value );

		if (!( aptr = document_atribut( dptr, "name" ) ))
			continue;
		else 	infos.name = occi_unquoted_value( aptr->value );

		if (!( strncasecmp( infos.name, request.name, strlen( request.name ) ) ))
		{
			best.href = infos.href;
			best.name = infos.name;
			break;
		}
		else
		{
			liberate( infos.href );
			liberate( infos.name );
		}
	}
	liberate( request.name );
	if ( best.name )
		best.name = liberate( best.name );
	if (!( best.href ))
		return( best.href );
	else 	return( opennebula_network_id( best.href ) );
	return((char *) 0);
}


/*	-----------------------------------------------------------------	*/
/*		r e s o l v e _ o p e n n e b u l a _ i m a g e   		*/
/*	-----------------------------------------------------------------	*/
private	char *	resolve_opennebula_image( struct cords_on_contract * cptr )
{
	struct	on_image_infos	request;
	struct	on_image_infos	image;
	struct	on_image_infos	best;
	char *			vptr;
	char *			iname=(char *) 0;

	struct	xml_element * eptr;
	struct	xml_element * dptr;
	struct	xml_atribut * aptr;

	if (!( eptr = document_element( cptr->images->xmlroot, "STORAGE_COLLECTION" )))
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

	memset( &best, 0, sizeof( struct on_image_infos ));

	if (!( iname = occi_category_id( cptr->image.id )))
		return((char *) 0);

	/* ------------------------------------------ */
	/* scan list for a perfect IPS produced match */
	/* ------------------------------------------ */
	for ( 	dptr=eptr->first;
		dptr != (struct xml_element *) 0;
		dptr = dptr->next )
	{
		if (!( aptr = document_atribut( dptr, "name" ) ))
		{
			image.id = liberate( image.id );
			continue;
		}

		else if (!( vptr = occi_unquoted_value(aptr->value)))
			continue;

		if ( strcmp( vptr, iname ) != 0 )
		{
			liberate( vptr );
			continue;
		}
		else
		{
			liberate( iname );
			liberate( vptr );
		}

		if (!( aptr = document_atribut( dptr, "href" ) ))
			return((char *) 0);
		else if (!( vptr = occi_unquoted_value(aptr->value)))
			return((char *) 0);
		else 	return( opennebula_image_id( vptr ) );
	
	}

	liberate( iname );

	/* --------------------------------------------------- */
	/* scan the image list for a system name partial match */
	/* --------------------------------------------------- */
	for ( 	dptr=eptr->first;
		dptr != (struct xml_element *) 0;
		dptr = dptr->next )
	{
		/* ----------------------------------------------- */
		/* collect the information from the flavor element */
		/* ----------------------------------------------- */
		memset( &image, 0, sizeof( struct on_image_infos ));
		if (!( aptr = document_atribut( dptr, "href" ) ))
			continue;
		else	image.id = occi_unquoted_value(aptr->value);
		if (!( aptr = document_atribut( dptr, "name" ) ))
		{
			image.id = liberate( image.id );
			continue;
		}
		else	image.name = occi_unquoted_value(aptr->value);

		if ( (!( strncasecmp( request.name,  image.name, strlen( request.name  ) )))
		||   (!( strncasecmp( request.other, image.name, strlen( request.other ) ))))
		{
			best.id = image.id;
			best.name = image.name;
			break;
		}		
		else
		{
			image.id = liberate( image.id );
			image.name = liberate( image.name );
			continue;
		}
	}
	if ( best.name )
		best.name = liberate( best.name );
	if (!( best.id ))
		return( best.id );
	else 	return( opennebula_image_id( best.id ) );
}

/*	-----------------------------------------------------------------	*/
/*		c r e a t e _ o p e n n e b u l a _ c o n t r a c t		*/
/*	-----------------------------------------------------------------	*/
public	int	create_opennebula_contract(
		struct occi_category * optr,
		struct opennebula * pptr,
		char * agent,
		char * tls )
{
	struct	cords_on_contract contract;
	struct	os_response * flavors=(struct os_response *) 0;
	struct	os_response * images =(struct os_response *) 0;
	int	status;

	if ((status = use_opennebula_configuration( pptr->profile )) != 0)
		return( status );
	else	memset( &contract, 0, sizeof( struct cords_on_contract ));

	/* ---------------------------- */
	/* recover the node description */
	/* ---------------------------- */
	if (!( contract.node.id = pptr->node ))
		return( 0 );
	else if (!( contract.node.message = occi_simple_get( contract.node.id, agent, tls ) ))
		return( terminate_opennebula_contract( 1570, &contract ) );

	/* -------------------------------------- */
	/* recover the infrastructure description */
	/* -------------------------------------- */
	else if (( contract.firewall.id = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_FIREWALL )) != (char *) 0)
	{
		if (!( contract.firewall.message = occi_simple_get( contract.firewall.id, agent, tls ) ))
			return( terminate_opennebula_contract( 1172, &contract ) );
	}

	if (!( contract.infrastructure.id = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_INFRASTRUCTURE ) ))
		return( terminate_opennebula_contract( 1571, &contract ) );
	else if (!( contract.infrastructure.message = occi_simple_get( contract.infrastructure.id, agent, tls ) ))
		return( terminate_opennebula_contract( 1572, &contract ) );

	else if (!( contract.compute.id = occi_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_COMPUTE ) ))
		return( terminate_opennebula_contract( 1573, &contract ) );
	else if (!( contract.compute.message = occi_simple_get( contract.compute.id, agent, tls ) ))
		return( terminate_opennebula_contract( 1574, &contract ) );

	else if (!( contract.network.id = occi_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_NETWORK ) ))
		return( terminate_opennebula_contract( 1575, &contract ) );
	else if (!( contract.network.message = occi_simple_get( contract.network.id, agent, tls ) ))
		return( terminate_opennebula_contract( 1576, &contract ) );

	else if (!( contract.storage.id = occi_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_STORAGE ) ))
		return( terminate_opennebula_contract( 1577, &contract ) );
	else if (!( contract.storage.message = occi_simple_get( contract.storage.id, agent, tls ) ))
		return( terminate_opennebula_contract( 1578, &contract ) );

	/* --------------------------------------------------------- */
	/* recover detailled list of OS Flavors and resolve contract */
	/* --------------------------------------------------------- */
	else if (!( contract.flavors = on_list_flavors() ))
		return( terminate_opennebula_contract( 1579, &contract ) );
	else if (!( pptr->flavor = resolve_opennebula_flavor( &contract ) ))
		return( terminate_opennebula_contract( 1580, &contract ) );
	else
	{
		if ( contract.architecture )
			pptr->architecture = allocate_string( contract.architecture );
		if ( contract.driver )
			pptr->driver = allocate_string( contract.driver );
	}
					
	/* ---------------------------------- */
	/* recover the node image description */
	/* ---------------------------------- */
	if (!( contract.image.id = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_IMAGE ) ))
		return( terminate_opennebula_contract( 1581, &contract ) );
	else if (!( contract.image.message = occi_simple_get( contract.image.id, agent, tls ) ))
		return( terminate_opennebula_contract( 1582, &contract ) );

	/* ------------------------ */
	/* retrieve the cosacs flag */
	/* ------------------------ */
	else if ((!( pptr->agent = occi_extract_atribut( contract.image.message, "occi", 
		_CORDS_IMAGE, "agent" ) ))
	     &&  (!( pptr->agent = allocate_string("cosacs") )))
		return( terminate_opennebula_contract( 1283, &contract ) );


	else if (!( contract.system.id = occi_extract_atribut( contract.image.message, "occi", 
		_CORDS_IMAGE, _CORDS_SYSTEM ) ))
		return( terminate_opennebula_contract( 1583, &contract ) );
	else if (!( contract.system.message = occi_simple_get( contract.system.id, agent, tls ) ))
		return( terminate_opennebula_contract( 1584, &contract ) );

	/* ------------------------------------------------------ */
	/* retrieve detailled list of images and resolve contract */
	/* ------------------------------------------------------ */
	else if (!( contract.images = on_list_images() ))
		return( terminate_opennebula_contract( 1585, &contract ) );
	else if (!( pptr->image = resolve_opennebula_image( &contract ) ))
		return( terminate_opennebula_contract( 1586, &contract ) );
	else if (!( pptr->original = allocate_string( pptr->image ) ))
		return( terminate_opennebula_contract( 1587, &contract ) );
		

	/* --------------------------------------- */
	/* retrieve the public network information */
	/* --------------------------------------- */
	else if (!( contract.networks = on_list_network_pool() ))
		return( terminate_opennebula_contract( 1588, &contract ) );
	else if (!( pptr->publicnetwork = resolve_opennebula_network( &contract ) ))
		return( terminate_opennebula_contract( 1589, &contract ) );


	else 
	{
		/* ----------------------------------------------- */
		/* resolve any price informatino for this category */
		/* ----------------------------------------------- */
		pptr->price = occi_resolve_category_price( _CORDS_OPENNEBULA, default_operator(), agent, tls );
	
		return( terminate_opennebula_contract( 0, &contract ) );
	}

}

/*	-----------------------------------------------------------------	*/
/*		d e l e t e _ o p e n n e b u l a _ c o n t r a c t		*/
/*	-----------------------------------------------------------------	*/
public	int	delete_opennebula_contract(
		struct occi_category * optr,
		struct opennebula * pptr,
		char * agent,
		char * tls )
{
	struct	on_response * osptr;
	if ((osptr = stop_opennebula_provisioning( pptr )) != (struct on_response *) 0)
		osptr = liberate_on_response( osptr );
	if (!( pptr->image ))
		return( 0 );
	else if (!( pptr->original ))
		return( 0 );
	else if (!( strcmp( pptr->original, pptr->image ) ))
		return( 0 );
	else
	{
		on_delete_image( pptr->image );
		return(0);
	}
	return(0);
}

	/* ------------- */
#endif	/* _oncontract_c */
	/* ------------- */

