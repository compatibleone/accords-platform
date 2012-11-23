/*-------------------------------------------------------------------------------*/
/* accords platform                                                              */
/* copyright 2012 ,Hamid MEDJAHED & Elyes ZEKRI (hmedjahed@prologue.fr) prologue */
/*-------------------------------------------------------------------------------*/
/* licensed under the apache license, version 2.0 (the "license");               */
/* you may not use this file except in compliance with the license.              */
/* you may obtain a copy of the license at                                       */
/*                                                                               */
/*       http://www.apache.org/licenses/license-2.0                              */
/*                                                                               */
/* unless required by applicable law or agreed to in writing, software           */
/* distributed under the license is distributed on an "as is" basis,             */
/* without warranties or conditions of any kind, either express or implied.      */
/* see the license for the specific language governing permissions and           */
/* limitations under the license.                                                */
/*-------------------------------------------------------------------------------*/
#ifndef	_ec2procci_tools_c
#define	_ec2procci_tools_c

#include "occiclient.h"
#include "cordslang.h"
#include "ec2_procci_boto.c"
#include "occiresolver.c"


 char accesskey[256]=" ";
 char secretkey[256]="";
 char zone[256]=" ";




struct	cords_vector
{
	char *	id;
	struct occi_response * message;
};

struct	cords_ec2_contract
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
};

struct	ec2_compute_infos
{
	int	cores;
	int	speed;
	int	memory;
	int	storage;
	char *	id;
};

struct	ec2_image_infos
{
	char *	id;
	char *	other;
	char *	name;
	char *	updated;
};





struct	ec2config * resolve_ec2_configuration( char * sptr )
{
	struct	occi_kind_node * nptr;
	struct	ec2config * pptr=(struct ec2config *) 0;
	struct	occi_kind_node  * occi_first_ec2config_node();
	for (	nptr = occi_first_ec2config_node();
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next )
	{
		if (!( pptr = nptr->contents ))
			continue;
		else if (!( pptr->name ))
			continue;
		else if (!( strcmp( pptr->name, sptr ) ))
                {
			return( pptr );
                }
	}
	return((struct ec2config *) 0);
}



int use_ec2_configuration( char * sptr )
{
        struct	ec2config * pptr;
	if (!( pptr = resolve_ec2_configuration(sptr)))
	 	return( 404 );
        else
        {
          strcpy(accesskey,pptr->accesskey);
	  strcpy(secretkey,pptr->secretkey);
        }
        return 0;
}




int create_ec2_contract(
		struct occi_category * optr,
		struct amazonEc2 * pptr,
		char * agent,
		char * tls )
{
	
	struct	cords_ec2_contract contract;
	int	status;
	char *	vptr;

	if ((status = use_ec2_configuration( pptr->name )) != 0)
	     return( status );
	else	
        memset( &contract, 0, sizeof( struct cords_ec2_contract ));
        

        
	/* ---------------------------- */
	/* recover the node description */
	/* ---------------------------- */
	if (!( contract.node.id = pptr->node ))
		return( 0 );
	else if (!( contract.node.message = occi_simple_get( contract.node.id, agent, tls ) ))
		return( terminate_ec2_contract( 1170, &contract ) );
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
			return( terminate_ec2_contract( 1127, &contract ) );
	}
	/* -------------------------------- */
	/* recover the firewall description */
	/* -------------------------------- */
	if (( contract.firewall.id = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_FIREWALL )) != (char * ) 0)
	{
		if (!( contract.firewall.message = occi_simple_get( contract.firewall.id, agent, tls ) ))
			return( terminate_ec2_contract( 1171, &contract ) );
		else if (!( pptr->firewall = allocate_string( contract.firewall.id ) ))
			return( terminate_ec2_contract( 1172, &contract ) );
	}

	/* -------------------------------------- */
	/* recover the infrastructure description */
	/* -------------------------------------- */
	if (!( contract.infrastructure.id = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_INFRASTRUCTURE ) ))
		return( terminate_ec2_contract( 1171, &contract ) );
	else if (!( contract.infrastructure.message = occi_simple_get( contract.infrastructure.id, agent, tls ) ))
		return( terminate_ec2_contract( 1172, &contract ) );

	else if (!( contract.compute.id = occi_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_COMPUTE ) ))
		return( terminate_ec2_contract( 1173, &contract ) );
	else if (!( contract.compute.message = occi_simple_get( contract.compute.id, agent, tls ) ))
		return( terminate_ec2_contract( 1174, &contract ) );

	else if (!( contract.network.id = occi_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_NETWORK ) ))
		return( terminate_ec2_contract( 1175, &contract ) );
	else if (!( contract.network.message = occi_simple_get( contract.network.id, agent, tls ) ))
		return( terminate_ec2_contract( 1177, &contract ) );
	else if (!( pptr->network = allocate_string( contract.network.id ) ))
		return( terminate_ec2_contract( 1176, &contract ) );

	else if (!( contract.storage.id = occi_extract_atribut( contract.infrastructure.message, "occi", 
		_CORDS_INFRASTRUCTURE, _CORDS_STORAGE ) ))
		return( terminate_ec2_contract( 1178, &contract ) );
	else if (!( contract.storage.message = occi_simple_get( contract.storage.id, agent, tls ) ))
		return( terminate_ec2_contract( 1179, &contract ) );
        
	/* --------------------------------------------------------- */
	/* recover detailled list of OS Flavors and resolve contract */
	/* --------------------------------------------------------- */
	/*else if (!( contract.flavors = ec2_list_flavor_details() ))
		return( terminate_ec2_contract( 1180, &contract ) ); */
	if (!( pptr->flavor = resolve_contract_flavor() ))
		return( terminate_ec2_contract( 1181, &contract ) );
	

	/* ---------------------------------- */
	/* recover the node image description */
	/* ---------------------------------- */
	if (!( contract.image.id = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_IMAGE ) ))
		return( terminate_ec2_contract( 1182, &contract ) );
	else if (!( contract.image.message = occi_simple_get( contract.image.id, agent, tls ) ))
		return( terminate_ec2_contract( 1183, &contract ) );

	else if (!( contract.system.id = occi_extract_atribut( contract.image.message, "occi", 
		_CORDS_IMAGE, _CORDS_SYSTEM ) ))
		return( terminate_ec2_contract( 1184, &contract ) );
	else if (!( contract.system.message = occi_simple_get( contract.system.id, agent, tls ) ))
		return( terminate_ec2_contract( 1185, &contract ) );

	/* ------------------------------------------------------ */
	/* retrieve detailled list of images and resolve contract */
	/* ------------------------------------------------------ */
	/*else if (!( contract.images = ec2_list_image_details() ))
		return( terminate_ec2_contract( 1186, &contract ) );*/
        
	if (!( pptr->image = resolve_contract_image(accesskey, secretkey,zone, pptr->image ) ))
		return( terminate_ec2_contract( 1187, &contract ) );
	else if (!( pptr->original = allocate_string( pptr->image ) ))
		return( terminate_ec2_contract( 1188, &contract ) );
	
	{
		/* ----------------------------------------------- */
		/* resolve any price informatino for this category */
		/* ----------------------------------------------- */
		pptr->price = occi_resolve_category_price( _CORDS_EC2, default_operator(), agent, tls );
	
		return( terminate_ec2_contract( 0, &contract ) );
	}
}


int	terminate_ec2_contract( int status, struct cords_ec2_contract * cptr )
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
	return( status );
}


#endif


