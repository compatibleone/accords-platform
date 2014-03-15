/*-------------------------------------------------------------------------------*/
/* accords platform                                                              */
/* copyright 2012 ,Hamid MEDJAHED (hmedjahed@prologue.fr) prologue    		 */
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
#include "occiclient.h"
#include "cordslang.h"
#include "ec2client.h"
#include "ec2contract.h"
#include "amazonEc2.h"
#include "ec2config.h"
#define BIGBUFF 4096
#define SMALLBUFF 1024


/* ---------------------------------------------------------------------------- */
/* 		r e s o l v e _ o s _ c o n f i g u r a t i o n			*/
/* ---------------------------------------------------------------------------- */
struct	ec2config * resolve_ec2_configuration( char * sptr )
{
	struct	occi_kind_node * nptr;
	struct	ec2config * pptr=(struct ec2config *) 0;
	struct  occi_kind_node * occi_first_ec2config_node(char * category,char * tptr, char * vptr, int type);
	rest_log_message("resolve_ec2_configuration");
	rest_log_message( sptr );
	for (	nptr = occi_first_ec2config_node("ec2config","name",sptr,0);
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
	return((struct ec2config *) 0);
}

private	struct	ec2config * solve_ec2_configuration( char * sptr )
{
	struct	ec2config * cptr;
	if ((cptr = resolve_ec2_configuration( sptr )) != (struct ec2config *) 0)
		return( cptr );
	else if (!( sptr = get_operator_profile() ))
		return( cptr );
	else 	return( resolve_ec2_configuration( sptr ) );
}


struct ec2config * resolve_ec2_configurationb(char * agent, char * tls, char * sptr)
{
   	struct	occi_element 	*	eptr=(struct occi_element*) 0;
	struct	occi_element 	*	eptr2=(struct occi_element*) 0;
	struct	occi_element 	*	eptr3=(struct occi_element*) 0;
	struct	occi_element 	*	eptr4=(struct occi_element*) 0;
	struct	occi_element 	*	eptr5=(struct occi_element*) 0;
	struct	occi_element 	*	eptr6=(struct occi_element*) 0;
	struct	occi_element 	*	eptr7=(struct occi_element*) 0;
	struct	occi_element 	*	eptr8=(struct occi_element*) 0;
	struct	occi_element 	*	eptr9=(struct occi_element*) 0;
	struct	occi_element 	*	eptr10=(struct occi_element*) 0;
	struct	occi_element 	*	eptr11=(struct occi_element*) 0;
	struct	occi_element 	*	eptr12=(struct occi_element*) 0;
	struct	occi_element 	*	eptr13=(struct occi_element*) 0;
	struct	occi_element 	*	eptr14=(struct occi_element*) 0;
	struct	occi_element 	*	eptr15=(struct occi_element*) 0;
	struct	occi_element 	*	eptr16=(struct occi_element*) 0;
	struct	occi_element 	*	eptr17=(struct occi_element*) 0;
	struct	occi_element 	*	eptr18=(struct occi_element*) 0;
	struct	occi_response	*	aptr=(struct occi_response*) 0;
	struct	occi_response	*	aaptr=(struct occi_response*) 0;
	struct	occi_response	*	aaptr2=(struct occi_response*) 0;
	
	struct  ec2config * cfptr = (struct ec2config *)malloc(sizeof(struct ec2config));
	
	char	buffer1[BIGBUFF];
        char	buffer2[BIGBUFF];
        
	
	char idbuff[SMALLBUFF];
	char namebuff[SMALLBUFF];
	char descriptionbuff[SMALLBUFF];
	char userbuff[SMALLBUFF];
	char passwordbuff[SMALLBUFF];
	char accesskeybuff[SMALLBUFF];
	char secretkeybuff[SMALLBUFF];
	char authenticatebuff[SMALLBUFF];
	char agentbuff[SMALLBUFF];
	char hostbuff[SMALLBUFF];
	char versionbuff[SMALLBUFF];
	char locationbuff[SMALLBUFF];
	char namespacebuff[SMALLBUFF];
	char basebuff[SMALLBUFF];
	char tlsbuff[SMALLBUFF];
	char currentbuff[SMALLBUFF];
     

	if ( check_verbose() )
		printf("   OCCI EC2Procci ( %s, %s )\n", "ec2config", agent);

        rest_log_message("resolve_ec2_configuration");
	initialise_occi_resolver( tls , (char *) 0, (char *) 0, (char *) 0 );

	if (!( aptr = occi_resolver("ec2config" , agent ) ))
	{ 
                 aptr = occi_remove_response( aptr );
 		 return ( (struct ec2config *) 0);
        }
	

	for (	eptr =aptr->first;
		eptr != (struct occi_element*) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;
		sprintf(buffer1,"%s/%s/",eptr->value,"ec2config");
		if (!( aaptr = occi_simple_get( buffer1, agent, tls ) ) )
		{ 
                 aaptr = occi_remove_response( aaptr );
 		 continue;	
	        }
                for (	eptr2 =aaptr->first;
		eptr2 != (struct occi_element*) 0;
		eptr2 = eptr2->next )
		{
			
		 	rest_add_http_prefix(buffer2,BIGBUFF,eptr2->value);
			if (!( aaptr2 = occi_simple_get( buffer2, agent, tls ) ) )
		 	{ 
                  		aaptr2 = occi_remove_response( aaptr2 );
 		  		continue;	
	         	}
			sprintf(idbuff,	         "%s%s%s", "occi.","core.", "id");
			sprintf(namebuff,        "%s%s%s", "occi.","ec2config.", "name");
			sprintf(descriptionbuff, "%s%s%s", "occi.","ec2config.", "description");
			sprintf(userbuff,        "%s%s%s", "occi.","ec2config.", "user");
			sprintf(passwordbuff,    "%s%s%s", "occi.","ec2config.", "password");
			sprintf(accesskeybuff,   "%s%s%s", "occi.","ec2config.", "accesskey");
			sprintf(secretkeybuff,   "%s%s%s", "occi.","ec2config.", "secretkey");
			sprintf(authenticatebuff,"%s%s%s", "occi.","ec2config.", "authenticate");
			sprintf(agentbuff,	 "%s%s%s", "occi.","ec2config.", "agent");
			sprintf(hostbuff,        "%s%s%s", "occi.","ec2config.", "host");
			sprintf(versionbuff,     "%s%s%s", "occi.","ec2config.", "version");
			sprintf(locationbuff,    "%s%s%s", "occi.","ec2config.", "location");
			sprintf(namespacebuff,   "%s%s%s", "occi.","ec2config.", "namespace");
			sprintf(basebuff,        "%s%s%s", "occi.","ec2config.", "base");
			sprintf(tlsbuff,	 "%s%s%s", "occi.","ec2config.", "tls");
			sprintf(currentbuff,	 "%s%s%s", "occi.","ec2config.", "current");


			if (    (eptr3 = occi_locate_element(aaptr2->first,  idbuff) )
			    &&  (eptr4 = occi_locate_element(aaptr2->first,  namebuff) ) 
			    &&  (eptr5 = occi_locate_element(aaptr2->first,  descriptionbuff) ) 
			    &&  (eptr6 = occi_locate_element(aaptr2->first,  userbuff) ) 
			    &&  (eptr7 = occi_locate_element(aaptr2->first,  passwordbuff) )
			    &&  (eptr8 = occi_locate_element(aaptr2->first,  accesskeybuff) ) 
			    &&  (eptr9 = occi_locate_element(aaptr2->first,  secretkeybuff) )
			    &&  (eptr10 = occi_locate_element(aaptr2->first,  authenticatebuff) ) 
			    &&  (eptr11 = occi_locate_element(aaptr2->first,  agentbuff) )
			    &&  (eptr12 = occi_locate_element(aaptr2->first, hostbuff) ) 
			    &&  (eptr13 = occi_locate_element(aaptr2->first, versionbuff) )
			    &&  (eptr14 = occi_locate_element(aaptr2->first, locationbuff) )
			    &&  (eptr15 = occi_locate_element(aaptr2->first, namespacebuff) ) 
			    &&  (eptr16 = occi_locate_element(aaptr2->first, basebuff) )
			    &&  (eptr17 = occi_locate_element(aaptr2->first, tlsbuff) ) 
			    &&  (eptr18 = occi_locate_element(aaptr2->first, currentbuff) ))
			{

				  cfptr->id     	= eptr3->value;
				  cfptr->name   	= eptr4->value;
				  cfptr->description	= eptr5->value;
				  cfptr->user		= eptr6->value;
				  cfptr->password	= eptr7->value;
				  cfptr->accesskey	= eptr8->value;
				  cfptr->secretkey	= eptr9->value;
				  cfptr->authenticate	= eptr10->value;
				  cfptr->agent		= eptr11->value;
				  cfptr->host	 	= eptr12->value;
				  cfptr->version	= eptr13->value;
				  cfptr->location	= eptr14->value;
				  cfptr->namespace	= eptr15->value;
				  cfptr->base		= eptr16->value;
				  cfptr->tls         	= eptr17->value;
				  cfptr->current	= eptr18->value;
			          
				  if(!(strcmp(cfptr->name, sptr)))
				  {
				  	return cfptr; 
				  }
				  else continue;
			}
			else continue;
	     	}     
         }               
	aaptr2 = occi_remove_response( aaptr2 );
        aaptr = occi_remove_response( aaptr );
	aptr = occi_remove_response( aaptr );
        return ( (struct ec2config *) 0);
}

struct ec2_subscription * initialise_ec2_subscription(char * accesskey, char * secretkey, char * zone, char * keypair)
{
	struct	ec2_subscription * subptr;

	if (!( subptr = allocate( sizeof( struct ec2_subscription ) )))
		return( subptr );
	else	memset( subptr, 0, sizeof( struct ec2_subscription ) );
       
	if (!( subptr->accesskey = allocate_string( accesskey )))
		return( liberate( subptr ) );
	if (!( subptr->secretkey = allocate_string( secretkey )))
		return( liberate( subptr ) );
	if (!( subptr->zone = allocate_string( zone )))
		return( liberate( subptr ) );
	if (!( subptr->keypair = allocate_string( keypair )))
		return( liberate( subptr ) );
	return subptr;
}

/*	------------------------------------------------------------	*/
/* 	l l _ u s e _ o p e n s t a c k _ c o n f i g u r a t i o n 	*/
/*	------------------------------------------------------------	*/
struct ec2_subscription * ll_use_ec2_configuration( char * sptr )
{
	struct	ec2config * pptr;

	if (!( pptr = resolve_ec2_configuration( sptr )))
	 	return((struct ec2_subscription *) 0);

	else 	return(initialise_ec2_subscription(pptr->accesskey, pptr->secretkey, pptr->location, pptr->authenticate));
}

struct ec2_subscription * use_ec2_configuration( char * nptr )
{
        struct  ec2_subscription * subptr = (struct ec2_subscription *) 0;
	if (( subptr = ll_use_ec2_configuration( nptr )) != (struct ec2_subscription *) 0)
		return( subptr );
	else if (!( nptr = get_operator_profile() ))
		return( subptr );
	else 	return( ll_use_ec2_configuration( nptr ) );

}

char * resolve_ec2_location(struct cords_ec2_contract * cptr,struct ec2config * cfptr,char * zone)
{
	char * ec2zone = (char *) 0;

 	/*----------------------------------------------------*/
	/* handle the configuration prefered geo-localisation */
	/*----------------------------------------------------*/
	if (!( rest_valid_string( zone ) ))
		zone = cfptr->location;

	if (!(rest_valid_string(cfptr->location)))
		cfptr->location = (char*) 0;

	/*------------------------------------------------------------------------------*/
	/* check usage and existance of the zone else take the default europe eu-west-1 */
	/*------------------------------------------------------------------------------*/
	else
	{
		ec2zone = get_ec2_zone(cptr->subscription,zone);
		cptr->location = allocate_string(ec2zone);
	}
     	return allocate_string(ec2zone);
}

/*	-----------------------------------------------------------------	*/
/*		r e s o l v e _ e c 2 _ i m a g e   			        */
/*	-----------------------------------------------------------------	*/
char *	resolve_ec2_image( struct ec2_subscription * subptr, struct cords_ec2_contract * cptr )
{
	struct	ec2_image_infos	request;
	struct  ec2_image_infos image;
	char			buffer[1024];
	char *			vptr;

	/* ---------------------------------------------------------- */
	/* retrieve appropriate parameters from node image components */
	/* ---------------------------------------------------------- */
	if (!( vptr = occi_extract_atribut( cptr->system.message, "occi", 
		_CORDS_SYSTEM, _CORDS_NAME ) ))
		return((char *) 0);
	else	request.name = vptr;

	rest_log_message( "ec2_contract system :");
	rest_log_message( request.name );

	if (!( vptr = occi_extract_atribut( cptr->image.message, "occi", 
		_CORDS_IMAGE, _CORDS_NAME ) ))
		return((char *) 0);
	else	request.other = vptr;

	rest_log_message( "ec2_contract image :");
	rest_log_message( request.other );

	/* --------------------------------------------------- */
	/* scan the image list for a system name partial match */
	/* --------------------------------------------------- */
	sprintf(buffer,"get_ec2_ami for %s in %s",request.name,cptr->location);
	rest_log_message( buffer );

	sprintf(buffer,"%s*",request.name);
	if (!( vptr = get_ec2_imgname(subptr,buffer,cptr->location)))
	{
		rest_log_message("get_ec2_ami failure");
		return((char*) 0);
	}
	else
	{
		rest_log_message("ec2_contract perfect match");
		rest_log_message( vptr );
		return( allocate_string( vptr ) );
	}
}


/*	-----------------------------------------------------------------	*/
/*		r e s o l v e _ e c 2 _ f l a v o r  			        */
/*	-----------------------------------------------------------------	*/
char *	resolve_ec2_flavor(struct cords_ec2_contract * cptr)
{

	struct	ec2_compute_infos	request;
	char *	vptr;
	char *	wptr;
	char *  flavor;

	/* -------------------------------------------------------------- */
	/* retrieve appropriate parameters from infrastructure components */
	/* -------------------------------------------------------------- */
	memset( &request, 0, sizeof(struct ec2_compute_infos));
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
	if(!(flavor = get_ec2_flavor(request.memory,request.cores,request.speed, request.storage, cptr->architecture)))
		return ((char*)0);
	else
		return (flavor);
}

int create_ec2_contract(struct occi_category * optr, struct amazonEc2 * pptr, char * agent, char * tls)
{
	struct ec2_subscription * subptr = (struct ec2_subscription *) 0;	
	struct	cords_ec2_contract contract;
	struct ec2config * cfptr;
	int	status = 0;
	char *	vptr;
	char * zptr;

	//resolve the user credentials and configuration
	if(!rest_valid_string(pptr->profile))
		return (777);
	if (!(subptr = use_ec2_configuration(pptr->profile)))
		return (778);
	else if (!(cfptr = solve_ec2_configuration(pptr->profile)))
		return (779);
	else
	{
        	memset( &contract, 0, sizeof(contract ));
		contract.subscription = subptr;
	}
	
	/*----------------------------------------------------*/
	/* resolve the required Geolocalisation               */
	/*----------------------------------------------------*/
	if (!( rest_valid_string(pptr->zone) )) 
		zptr = allocate_string(cfptr->location);
	else 	zptr = allocate_string( pptr->zone );

	/* ---------------------------------------------- */
	/* transform logical to ec2 region nameing scheme */
	/* ---------------------------------------------- */
	pptr->zone = ec2_resolve_region( subptr, zptr );
	rest_log_message(pptr->zone);

	if ( zptr ) zptr = liberate( zptr );

	/* -------------------------------------------- */
	/* ensure that the zone is valid or abandon now */
	/* -------------------------------------------- */
	if (!(pptr->zone = resolve_ec2_location(&contract,cfptr,pptr->zone)))
		return (terminate_ec2_contract(27,&contract));

	set_ec2_zone( subptr, pptr->zone );
	rest_log_message(pptr->zone);

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
        
	/* ---------------------------------------------------------- */
	/* recover detailled list of EC2 Flavors and resolve contract */
	/* ---------------------------------------------------------- */
	else if (!( pptr->flavor = resolve_ec2_flavor(&contract) ))
		return( terminate_ec2_contract( 1180, &contract ) );	

	/* ---------------------------------- */
	/* recover the node image description */
	/* ---------------------------------- */
	if (!( contract.image.id = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_IMAGE ) ))
		return( terminate_ec2_contract( 1182, &contract ) );
	else if (!( contract.image.message = occi_simple_get( contract.image.id, agent, tls ) ))
		return( terminate_ec2_contract( 1183, &contract ) );

	/* ------------------------ */
	/* retrieve the cosacs flag */
	/* ------------------------ */
	else if ((!( pptr->agent = occi_extract_atribut( contract.image.message, "occi", 
		_CORDS_IMAGE, "agent" ) ))
	     &&  (!( pptr->agent = allocate_string("cosacs") )))
		return( terminate_ec2_contract( 1283, &contract ) );

	else if (!( contract.system.id = occi_extract_atribut( contract.image.message, "occi", 
		_CORDS_IMAGE, _CORDS_SYSTEM ) ))
		return( terminate_ec2_contract( 1184, &contract ) );
	else if (!( contract.system.message = occi_simple_get( contract.system.id, agent, tls ) ))
		return( terminate_ec2_contract( 1185, &contract ) );

	/* ------------------------------------------------------ */
	/* retrieve detailled list of images and resolve contract */
	/* ------------------------------------------------------ */
	if (!( pptr->image = resolve_ec2_image(subptr,&contract) ))
		return( terminate_ec2_contract( 1187, &contract ) );
	else if (!( pptr->original = allocate_string( pptr->image ) ))
		return( terminate_ec2_contract( 1188, &contract ) );
	else
	{
		/* ----------------------------------------------- */
		/* resolve any price informatino for this category */
		/* ----------------------------------------------- */
		pptr->price = occi_resolve_category_price( _CORDS_EC2, default_operator(), agent, tls );
		//py_finalize();
		return( terminate_ec2_contract( 0, &contract ) );
	}
}


int terminate_ec2_contract( int status, struct cords_ec2_contract * cptr )
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
	if (cptr->architecture)
		cptr->architecture = liberate(cptr->architecture);
	if(cptr->driver)
		cptr->driver = liberate(cptr->driver);
	if(cptr->location)
		cptr->location = liberate(cptr->location);
	return( status );
}


/*	-----------------------------------------------------------------	*/
/*		d e l e t e _ e c 2 _ c o n t r a c t		  		*/
/*	-----------------------------------------------------------------	*/
public	int	delete_ec2_contract(
		struct occi_category * optr,
		struct amazonEc2 * pptr,
		char * agent,
		char * tls )
{
	int	status = 0;
	struct 	ec2_subscription * subptr=(struct ec2_subscription *) 0;

	if ( pptr->state != _OCCI_IDLE )
		status = stop_ec2_provisioning( pptr );

	if ( pptr->reference )
	{
		if ((subptr = use_ec2_configuration(pptr->profile )) != (struct ec2_subscription *) 0)
		{

			/* ------------------------- */
			/* delete the hosted service */
			/* ------------------------- */
			if (( status = stop_ec2_provisioning(pptr)) != 0)
			{
				liberate(subptr);
				return status;
			}
		}
	}
	liberate(subptr);
	return(status);
}


