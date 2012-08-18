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
#ifndef	_azcontract_c
#define	_azcontract_c

#include "occiclient.h"
#include "occiresolver.h"

public	char *	occi_extract_atribut( 
	struct occi_response * zptr, char * domain,
	char * category, char * nptr );

struct	cords_vector
{
	char *	id;
	struct occi_response * message;
};

struct	cords_az_contract
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
	struct	az_response *	flavors;
	struct	az_response *	images;
	struct	az_response *	networks;
};

struct	az_compute_infos
{
	int	cores;
	int	speed;
	int	memory;
	int	storage;
	char *	id;
};

struct	az_image_infos
{
	char *	id;
	char *	other;
	char *	name;
	char *	updated;
};

struct	az_network_infos
{
	char *	name;
	char *	href;
};

/*	-----------------------------------------------------------------	*/
/*	     t e r m i n a t e _ w i n d o w s a z u r e _ c o n t r a c t		*/
/*	-----------------------------------------------------------------	*/
private	int	terminate_windowsazure_contract( int status, struct cords_az_contract * cptr )
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
		cptr->flavors = liberate_az_response( cptr->flavors );
	if ( cptr->images  )
		cptr->images  = liberate_az_response( cptr->images  );
	if ( cptr->networks  )
		cptr->networks  = liberate_az_response( cptr->networks  );
	if ( cptr->architecture  )
		cptr->architecture  = liberate( cptr->architecture  );
	if ( cptr->driver  )
		cptr->driver  = liberate( cptr->driver  );
	
	return( status );
}

/*	---------------------------------	*/
/*	a z _ n o r m a l i s e _ n a m e 	*/
/*	---------------------------------	*/
private	char *	az_normalise_name( char * wptr, int maxlength )
{
	int	c;
	int	i;
	int	j;
	int	upper;
	char *	sptr;
	if (!( sptr = wptr ))
		return( sptr );
	else if (!( sptr = allocate_string( wptr )))
		return( sptr );
	else	wptr = sptr;

	for ( j=0,i=0; *(wptr+i) != 0; i++)
	{
		if ((c = *(wptr+i)) == ' ' )
			continue;
		else
		{
			if (( c >= 'A') && ( c <= 'Z'))
				*(wptr+j) = ((c - 'A') + 'a');
			else	*(wptr+j) = c;
			j++;
		}
	}
	*(wptr+j) = 0;
	if ( strlen( sptr ) > maxlength )
		*(sptr+ maxlength) = 0;
	return( sptr );
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
private	int	az_normalise_value( char * sptr, int normal )
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
/*		r e s o l v e _ w i n d o w s a z u r e _ f l a v o r 		*/
/*	-----------------------------------------------------------------	*/
private	char *	resolve_windowsazure_flavor( struct cords_az_contract * cptr )
{
	struct	az_compute_infos	request;
	char *	vptr;
	char *	wptr;
	/* -------------------------------------------------------------- */
	/* retrieve appropriate parameters from infrastructure components */
	/* -------------------------------------------------------------- */
	memset( &request, 0, sizeof(struct az_compute_infos));
	if (!( vptr = occi_extract_atribut( cptr->compute.message, "occi", 
		_CORDS_COMPUTE, _CORDS_MEMORY ) ))
		request.memory = 0;
	else	request.memory = az_normalise_value( vptr,'G' );

	if (!( vptr = occi_extract_atribut( cptr->compute.message, "occi", 
		_CORDS_COMPUTE, _CORDS_CORES ) ))
		request.cores = 0;
	else	request.cores = az_normalise_value( vptr,'U' );

	if (!( vptr = occi_extract_atribut( cptr->compute.message, "occi", 
		_CORDS_COMPUTE, _CORDS_SPEED ) ))
		request.speed = 0;
	else	request.speed = az_normalise_value(vptr,'G');
	
	if (!( vptr = occi_extract_atribut( cptr->storage.message, "occi", 
		_CORDS_STORAGE, _CORDS_SIZE ) ))
		request.storage = 0;
	else	request.storage = az_normalise_value(vptr,'G');

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
		return( allocate_string( "Small" ) );
	else if (( request.storage > 80000000 )
	     && ( request.memory  > 4096000 ))
		return( allocate_string( "Large" ) );
	else	return( allocate_string( "Medium" ) );


}

/*	-----------------------------------------------------------------	*/
/*		r e s o l v e _ w i n d o w s a z u r e _ n e t w o r k 		*/
/*	-----------------------------------------------------------------	*/
private	char *	resolve_windowsazure_network( struct cords_az_contract * cptr )
{
	struct	az_network_infos	request;
	struct	az_network_infos	infos;
	struct	az_network_infos	best;

	struct	xml_element * eptr;
	struct	xml_element * dptr;
	struct	xml_element * bptr;
	struct	xml_atribut * aptr;
	char 		    * vptr;
	char	* seekname;

	if (!( eptr = document_element( cptr->networks->xmlroot, "VirtualNetworkSites" )))
		return((char *) 0);

	/* ------------------------------------------------------------ */
	/* retrieve appropriate parameters from node network components */
	/* ------------------------------------------------------------ */

	else if (!( vptr = occi_extract_atribut( cptr->network.message, "occi", 
		_CORDS_NETWORK, _CORDS_NAME ) ))
		return((char *) 0);
	else if (!( request.name = occi_unquoted_value( vptr ) ))
		return((char *) 0);

	memset( &best, 0, sizeof( struct az_network_infos ));

	for ( 	dptr=eptr->first;
		dptr != (struct xml_element *) 0;
		dptr = dptr->next )
	{
		if (!( dptr->name ))
			continue;
		else if ( strcmp( dptr->name, "VirtualNetworkSite" ) != 0 )
			continue;

		memset( &infos, 0, sizeof( struct az_network_infos ));
		if (!( bptr = document_element( dptr, "Name" ) ))
			continue;
		else 	infos.name = occi_unquoted_value( bptr->value );

		if (!( strncasecmp( infos.name, request.name, strlen( request.name ) ) ))
		{
			best.name = infos.name;
			break;
		}
		else
		{
			liberate( infos.name );
		}
	}
	liberate( request.name );
	return ( best.name );
}

/*	-----------------------------------------------------------------	*/
/*		    b u i l d _ a z u r e _ m e d i a l i n k 			*/
/*	-----------------------------------------------------------------	*/
private	int	build_windowsazure_medialink( struct windowsazure * pptr )
{
	char 	buffer[2048];
	if (!( rest_valid_string( pptr->storageaccount ) ))
		return( 118 );
	else
	{
		sprintf(buffer,"https://%s.blob.core.windows.net/vhds/%s.vhd",
			pptr->storageaccount,
			pptr->id );
		if (!( pptr->media = allocate_string( buffer ) ))
			return( 27 );
		else	return( 0 );
	}
}

/*	-----------------------------------------------------------------	*/
/*		r e s o l v e _ w i n d o w s a z u r e _ i m a g e   		*/
/*	-----------------------------------------------------------------	*/
private	char *	resolve_windowsazure_image( struct cords_az_contract * cptr, struct windowsazure * pptr )
{
	struct	az_image_infos	request;
	struct	az_image_infos	image;
	struct	az_image_infos	best;
	char *			vptr;
	char *			iname=(char *) 0;

	struct	xml_element * eptr;
	struct	xml_element * dptr;
	struct	xml_element * bptr;
	struct	xml_atribut * aptr;

	if (!( eptr = document_element( cptr->images->xmlroot, "Images" )))
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

	memset( &best, 0, sizeof( struct az_image_infos ));

	if (!( iname = occi_category_id( cptr->image.id )))
		return((char *) 0);

	/* ------------------------------------------ */
	/* scan list for a perfect IPS produced match */
	/* ------------------------------------------ */
	for ( 	dptr=eptr->first;
		dptr != (struct xml_element *) 0;
		dptr = dptr->next )
	{
		if (!( dptr->name ))
			continue;
		else if ( strcmp( dptr->name, "OSImage" ) != 0 )
			continue;
		else if (!( bptr = document_element( dptr, "Name" ) ))
		{
			image.id = liberate( image.id );
			continue;
		}

		else if (!( vptr = occi_unquoted_value(bptr->value)))
			continue;

		if ( strcmp( vptr, iname ) != 0 )
		{
			liberate( vptr );
			continue;
		}

		else if (!( bptr = document_element( dptr, "Name" ) ))
			return((char *) 0);
		else if (!( vptr = occi_unquoted_value(bptr->value)))
			return((char *) 0);
		else	return(( pptr->image = vptr ));
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
		if (!( dptr->name ))
			continue;
		else if ( strcmp( dptr->name, "OSImage" ) != 0 )
			continue;

		memset( &image, 0, sizeof( struct az_image_infos ));

		if (!( bptr = document_element( dptr, "Label" ) ))
			continue;
		else	image.name = occi_unquoted_value(bptr->value);

		if (!( bptr = document_element( dptr, "Name" ) ))
		{
			image.name = liberate( image.name );
			continue;
		}
		else	image.id = occi_unquoted_value(bptr->value);

		if  (!( strncasecmp( request.name,  image.name, strlen( request.name  ) )))
		{
			best.id = image.id;
			best.name = image.name;
			break;
		}		
		else if  (!( strncasecmp( request.name,  image.id, strlen( request.name  ) )))
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
	return(( pptr->image = best.id ));
}

/*	-----------------------------------------------------------------	*/
/*	    r e s o l v e _ w i n d o w s a z u r e _ l o c a t i o n		*/
/*	-----------------------------------------------------------------	*/
/*	- retrieve location definition from account and use in contract		*/
/*	- retrieve affinity definition from account and use in contract		*/
/*	- if    affinity defined then check affinity existance			*/
/*	- else  check location existance					*/
/*	-----------------------------------------------------------------	*/
private	int 	resolve_windowsazure_location( 
		struct cords_az_contract * cptr, 
		struct windowsazure * pptr, 
		struct	az_config * cfptr )
{
	struct	xml_element * fptr;
	struct	xml_element * eptr;
	struct	az_response * zptr;
	int	status;

	/***************************************************/
	/* This is not yet SLA GEO Localisation Compliant  */
	/***************************************************/
	/*			TODO			   */
	/* ----------------------------------------------- */


	/* ----------------------------------------------- */
	/* handle the configured prefered geo-localisation */
	/* ----------------------------------------------- */
	if ( pptr->location ) pptr->location = liberate( pptr->location );

	if (!( rest_valid_string( cfptr->location ) ))
		cfptr->location = (char *) 0;
	else if (!( pptr->location = allocate_string( cfptr->location ) ))
		return( 27 );

	/* ----------------------------------------------- */
	/* handle the configured prefered "affinity group" */
	/* ----------------------------------------------- */
	if ( pptr->group ) pptr->group = liberate( pptr->group );

	if (!( rest_valid_string( cfptr->group ) ))
		cfptr->group = (char *) 0;
	else if (!( pptr->group = az_normalise_name( cfptr->group, 63 ) ))
		return( 27 );

	/* ----------------------------------------------- */
	/* check usage and existance of the affinity group */
	/* ----------------------------------------------- */
	if ( rest_valid_string( pptr->group ) )
	{
		if (!( zptr = az_retrieve_affinity_group( pptr->group ) ))
			return( 801 );
		else if ((status = check_windowsazure_operation( zptr )) != 200)
			return( status );
		else	return( 0 );
	}

	/* ------------------------------------------------ */
	/* check usage and existance of the geolocalisation */
	/* ------------------------------------------------ */
	else if (!( rest_valid_string( pptr->location )))
		return( 56 );
	{
		if (!( zptr = az_list_locations() ))
			return( 801 );
		else if (!( zptr->response ))
			return( 802 );
		else if ((status = zptr->response->status) != 200)
		{
			zptr = liberate_az_response( zptr );
			return( status );
		}
		else if ((!( eptr = zptr->xmlroot ))
		     ||  (!( eptr->name )))
		{
			zptr = liberate_az_response( zptr );
			return( 803 );
		}
		else if ( strcmp( eptr->name, "Locations" ) )
		{
			zptr = liberate_az_response( zptr );
			return( 804 );
		}
		else
		{
			for (	eptr = eptr->first;
				eptr != (struct xml_element *) 0;
				eptr = eptr->next )
			{
				if (!( eptr->name ))
					continue;
				else if ( strcmp( eptr->name, "Location") != 0)
					continue;
				else if (!( fptr = document_element( eptr->first, "Name" ) ))
					continue;
				else if (!( fptr->value ))
					continue;
				else if (!( strcmp( fptr->value, pptr->location ) ))
				{
					zptr = liberate_az_response( zptr );
					return( 0 );
				}
			}
			zptr = liberate_az_response( zptr );
			return( 804 );
		}
	}
	
}

/*	-----------------------------------------------------------------	*/
/*	    r e s o l v e _ w i n d o w s a z u r e _ s t o r a g e		*/
/*	-----------------------------------------------------------------	*/
private	int 	resolve_windowsazure_storage( 
		struct cords_az_contract * cptr, 
		struct windowsazure * pptr, 
		struct	az_config * cfptr )
{
	char	buffer[2048];
	struct	az_response * zptr;
	int	i;
	int	j;
	int	upper=0;
	int	status;
	char *	filename;

	/* ---------------------------------------------------- */
	/* recover the root of the subscription storage account */
	/* ---------------------------------------------------- */
	if ( pptr->storageaccount ) pptr->storageaccount = liberate( pptr->storageaccount );

	if (!( rest_valid_string( cfptr->storageaccount ) ))
		return( 818 );
	else	strcpy(buffer,cfptr->storageaccount);

	/* ------------------------------- */
	/* create the storage account name */
	/* ------------------------------- */
	strcat( buffer," ");

	if ( rest_valid_string( pptr->group) )	
		strcat(buffer,pptr->group);
	else if (rest_valid_string(  pptr->location ))
		strcat(buffer,pptr->location);
	else	return( 818 );

	/* ----------------------------------------------- */
	/* ensure the geo localised storage account exists */
	/* ----------------------------------------------- */
	if (!( pptr->storageaccount = az_normalise_name( buffer, 24 )))
		return( 817 );
	else if (!( zptr = az_retrieve_storage_service( pptr->storageaccount )))
		return( 500 );
	else if (( status = check_windowsazure_operation( zptr )) == 200 )
		return( 0 );

	/* ------------------------------ */
	/* create the new storage account */
	/* ------------------------------ */
	if (!( filename = az_create_storage_service_request( 
				pptr->storageaccount,
				pptr->storageaccount,
				pptr->storageaccount,
				pptr->location,
				pptr->group )))
		return( 820 );
	else if (!( zptr = az_create_storage_service( filename )))
		return( 500 );

	/* ------------------------------------------ */
	/* ensure creation of the appropriate Account */
	/* ------------------------------------------ */
	else if (( status = check_windowsazure_operation( zptr )) == 200 )
		return( 0 );
	else	return( status );

}

/*	-----------------------------------------------------------------	*/
/*	    r e s o l v e _ w i n d o w s a z u r e _ s e r v i c e 		*/
/*	-----------------------------------------------------------------	*/
private	int 	resolve_windowsazure_service( 
		struct cords_az_contract * cptr, 
		struct windowsazure * pptr, 
		struct	az_config * cfptr )
{
	char	buffer[2048];
	struct	az_response * zptr;
	int	i;
	int	j;
	int	upper=0;
	int	status;
	char *	filename;
	/* --------------------------------------------------- */
	/* recover the root of the subscription hosted service */
	/* --------------------------------------------------- */
	if ( pptr->hostedservice ) pptr->hostedservice = liberate( pptr->hostedservice );

	if (!( rest_valid_string( cfptr->hostedservice )))
		return( 818 );
	else	strcpy(buffer,cfptr->hostedservice);

	/* ------------------------ */
	/* append the required name */
	/* ------------------------ */
	sprintf(buffer+strlen(buffer)," %s%u",pptr->name,++cfptr->services);

	/* ---------------------------------------------- */
	/* ensure the geo localised hosted service exists */
	/* ---------------------------------------------- */
	if (!( pptr->hostedservice = az_normalise_name( buffer, 24 )))
		return( 817 );
	else if (!( zptr = az_get_hosted_service( pptr->hostedservice )))
		return( 500 );
	else if (( status = check_windowsazure_operation( zptr )) == 200 )
		return( 0 );

	/* ----------------------------- */
	/* create the new hosted service */
	/* ----------------------------- */
	if (!( filename = az_create_hosted_service_request( 
				pptr->hostedservice,
				pptr->hostedservice,
				pptr->hostedservice,
				pptr->location,
				pptr->group )))
		return( 820 );

	else if (!( zptr = az_create_hosted_service( filename )))
		return( 500 );

	/* ------------------------------------------ */
	/* ensure creation of the appropriate Account */
	/* ------------------------------------------ */
	else if (( status = check_windowsazure_operation( zptr )) == 200 )
		return( 0 );
	else	return( status );
}

/*	-----------------------------------------------------------------	*/
/*		c r e a t e _ w i n d o w s a z u r e _ c o n t r a c t		*/
/*	-----------------------------------------------------------------	*/
public	int	create_windowsazure_contract(
		struct occi_category * optr,
		struct windowsazure * pptr,
		char * agent,
		char * tls )
{
	struct	az_config * cfptr;
	struct	az_response * azptr;
	struct	cords_az_contract contract;
	struct	os_response * flavors=(struct os_response *) 0;
	struct	os_response * images =(struct os_response *) 0;
	char *	filename;
	int	status;
	char	buffer[1024];

	/* ----------------------------------------- */
	/* resolve the user or operator subscription */
	/* ----------------------------------------- */
	if ((status = use_windowsazure_configuration( pptr->profile )) != 0)
		return( terminate_windowsazure_contract( status, &contract ) );

	else if (!( cfptr = resolve_az_configuration( pptr->profile )))
		return( terminate_windowsazure_contract( 404, &contract ) );

	else	memset( &contract, 0, sizeof( struct cords_az_contract ));

	/* ------------------------------------------ */
	/* Resolve the required Geographical Location */
	/* ------------------------------------------ */
	if (( status = resolve_windowsazure_location( &contract, pptr, cfptr )) != 0)
		return( terminate_windowsazure_contract( status, &contract ) );

	/* ------------------------------------------ */
	/* Resolve the required GeoLo Storage Account */
	/* ------------------------------------------ */
	else if ((status = resolve_windowsazure_storage( &contract, pptr, cfptr )) != 0)
		return( terminate_windowsazure_contract( status, &contract ) );

	/* ------------------------------------------ */
	/* Build the Media Link for the Image Storage */
	/* ------------------------------------------ */
	else if ((status = build_windowsazure_medialink( pptr )) != 0)
		return( terminate_windowsazure_contract( status, &contract ) );

	/* ------------------------------------------ */
	/* Resolve the required Hosted Service Entity */
	/* ------------------------------------------ */
	else if ((status = resolve_windowsazure_service( &contract, pptr, cfptr )) != 0)
		return( terminate_windowsazure_contract( status, &contract ) );

	/* ---------------------------- */
	/* recover the node description */
	/* ---------------------------- */
	if (!( contract.node.id = pptr->node ))
		return( 0 );
	else if (!( contract.node.message = occi_simple_get( contract.node.id, agent, tls ) ))
		return( terminate_windowsazure_contract( 1570, &contract ) );

	/* -------------------------------------- */
	/* recover the firewall ports description */
	/* -------------------------------------- */
	else if (( contract.firewall.id = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_FIREWALL )) != (char *) 0)
	{
		if (!( contract.firewall.message = occi_simple_get( contract.firewall.id, agent, tls ) ))
			return( terminate_windowsazure_contract( 1172, &contract ) );
		else if (!( pptr->firewall = allocate_string( contract.firewall.id ) ))
			return( terminate_windowsazure_contract( 1172, &contract ) );
	}

	/* -------------------------------------- */
	/* recover the infrastructure description */
	/* -------------------------------------- */
	if (!( contract.infrastructure.id = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_INFRASTRUCTURE ) ))
		return( terminate_windowsazure_contract( 1571, &contract ) );
	else if (!( contract.infrastructure.message = occi_simple_get( contract.infrastructure.id, agent, tls ) ))
		return( terminate_windowsazure_contract( 1572, &contract ) );

	else if (!( contract.compute.id = occi_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_COMPUTE ) ))
		return( terminate_windowsazure_contract( 1573, &contract ) );
	else if (!( contract.compute.message = occi_simple_get( contract.compute.id, agent, tls ) ))
		return( terminate_windowsazure_contract( 1574, &contract ) );

	else if (!( contract.network.id = occi_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_NETWORK ) ))
		return( terminate_windowsazure_contract( 1575, &contract ) );
	else if (!( contract.network.message = occi_simple_get( contract.network.id, agent, tls ) ))
		return( terminate_windowsazure_contract( 1576, &contract ) );

	else if (!( contract.storage.id = occi_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_STORAGE ) ))
		return( terminate_windowsazure_contract( 1577, &contract ) );
	else if (!( contract.storage.message = occi_simple_get( contract.storage.id, agent, tls ) ))
		return( terminate_windowsazure_contract( 1578, &contract ) );

	/* --------------------------------------------------------- */
	/* recover detailled list of OS Flavors and resolve contract */
	/* --------------------------------------------------------- */
	else if (!( contract.flavors = az_list_flavors() ))
		return( terminate_windowsazure_contract( 1579, &contract ) );
	else if (!( pptr->flavor = resolve_windowsazure_flavor( &contract ) ))
		return( terminate_windowsazure_contract( 1580, &contract ) );
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
		return( terminate_windowsazure_contract( 1581, &contract ) );
	else if (!( contract.image.message = occi_simple_get( contract.image.id, agent, tls ) ))
		return( terminate_windowsazure_contract( 1582, &contract ) );

	else if (!( contract.system.id = occi_extract_atribut( contract.image.message, "occi", 
		_CORDS_IMAGE, _CORDS_SYSTEM ) ))
		return( terminate_windowsazure_contract( 1583, &contract ) );
	else if (!( contract.system.message = occi_simple_get( contract.system.id, agent, tls ) ))
		return( terminate_windowsazure_contract( 1584, &contract ) );

	/* ------------------------------------------------------ */
	/* retrieve detailled list of images and resolve contract */
	/* ------------------------------------------------------ */
	else if (!( contract.images = az_list_os_images() ))
		return( terminate_windowsazure_contract( 1585, &contract ) );
	else if (!( pptr->image = resolve_windowsazure_image( &contract, pptr ) ))
		return( terminate_windowsazure_contract( 1586, &contract ) );
	else if (!( pptr->original = allocate_string( pptr->image ) ))
		return( terminate_windowsazure_contract( 1587, &contract ) );
		
	/* --------------------------------------- */
	/* retrieve the public network information */
	/* --------------------------------------- */
	else if (!( contract.networks = az_list_network() ))
		return( terminate_windowsazure_contract( 1588, &contract ) );
	else if (!( pptr->publicnetwork = resolve_windowsazure_network( &contract ) ))
		return( terminate_windowsazure_contract( 1589, &contract ) );

	else 
	{
		/* ----------------------------------------------- */
		/* resolve any price informatino for this category */
		/* ----------------------------------------------- */
		pptr->price = occi_resolve_category_price( _CORDS_WINDOWSAZURE, default_operator(), agent, tls );
	
		return( terminate_windowsazure_contract( 0, &contract ) );
	}

}

/*	-----------------------------------------------------------------	*/
/*		d e l e t e _ w i n d o w s a z u r e _ c o n t r a c t		*/
/*	-----------------------------------------------------------------	*/
public	int	delete_windowsazure_contract(
		struct occi_category * optr,
		struct windowsazure * pptr,
		char * agent,
		char * tls )
{
	int	status;
	struct	az_response * azptr;
	struct	xml_element * eptr;
	struct	xml_element * dptr;

	status = stop_windowsazure_provisioning( pptr );

	if ( pptr->hostedservice )
	{
		/* ------------------------- */
		/* delete the hosted service */
		/* ------------------------- */
		if (( azptr = az_delete_hosted_service( pptr->hostedservice )) != (struct az_response *) 0)
			check_windowsazure_operation( azptr );
		pptr->hostedservice = liberate( pptr->hostedservice );

		/* ------------------------------------------------------------ */
		/* scan list of disks to locate the OS storage disk for removal */
		/* ------------------------------------------------------------ */
		if ((( azptr = az_list_os_disks()) != (struct az_response *) 0)
		&&  ( azptr->response )
		&&  ( azptr->response->status == 200 )
		&&  ((eptr = azptr->xmlroot) != (struct xml_element *) 0)
		&&  ( eptr->name )
		&&  (!( strcmp( eptr->name, "Disks" ) )))
		{
			for (	eptr = eptr->first;
				eptr != (struct xml_element *) 0;
				eptr = eptr->next )
			{
				if (!( dptr = document_element( eptr, "Name" ) ))
					continue;
				else if (!( dptr->value ))
					continue;
				else if ( strncmp( dptr->value, pptr->id, strlen( pptr->id ) ) != 0 )
					continue;
				else
				{
					if (( azptr = az_delete_os_disk( dptr->value )) != (struct az_response *) 0)
						check_windowsazure_operation( azptr );
					break;
				}
			}
		}
	}



	if ( pptr->storageaccount ) 
		pptr->storageaccount = liberate( pptr->storageaccount );
	if ( pptr->group ) 
		pptr->group = liberate( pptr->group );
	if ( pptr->location ) 
		pptr->location = liberate( pptr->location );
	if ( pptr->media ) 
		pptr->media = liberate( pptr->media );

	return(status);
}

	/* ------------- */
#endif	/* _azcontract_c */
	/* ------------- */

