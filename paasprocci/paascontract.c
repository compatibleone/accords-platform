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
#ifndef	_paas_contract_c	
#define	_paas_contract_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"
#include "paasclient.h"
#include "paasconfig.h"

#include "kindnode.h" // TODO Remove all references to kindnodes directly from the procci

struct	cords_vector
{
	char *	id;
	struct occi_response * message;
};

struct	cords_paas_contract
{
	struct	cords_vector	node;
	struct	cords_vector	manifest;
	struct	cords_vector	application;
	struct	cords_vector	version;
	struct	cords_vector	deployable;
	struct	cords_vector	environment;
};

#define	_CORDS_PAAS_MANIFEST		"paas_application_manifest"
#define	_CORDS_PAAS_APPLICATION		"paas_application"
#define	_CORDS_PAAS_VERSION		"paas_application_version"
#define	_CORDS_PAAS_DEPLOYABLE		"paas_application_deployable"
#define	_CORDS_PAAS_ENVIRONMENT		"paas_environment"

/* ---------------------------------------------------------------------------- */
/* 	   l l _ r e s o l v e _ p a a s _ c o n f i g u r a t i o n		*/
/* ---------------------------------------------------------------------------- */
private	struct	paas_config * ll_resolve_paas_configuration( char * sptr )
{
	struct	occi_kind_node * nptr;
	struct	paas_config * pptr=(struct paas_config *) 0;
	struct	occi_kind_node  * occi_first_paas_config_node();
	rest_log_message("resolve_paas_configuration");
	rest_log_message( sptr );
	for (	nptr = occi_first_paas_config_node();
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
	return((struct paas_config *) 0);
}

/* ---------------------------------------------------------------------------- */
/* 		r e s o l v e _ p a a s _ c o n f i g u r a t i o n		*/
/* ---------------------------------------------------------------------------- */
private	struct	paas_config * resolve_paas_configuration( char * sptr )
{
	struct	paas_config * pptr=(struct paas_config *) 0;
	if (!( pptr = ll_resolve_paas_configuration( sptr )))
	{
		if (!( sptr = get_operator_profile() ))
			return( pptr );
		else if (!( pptr = ll_resolve_paas_configuration( sptr )))
			return( pptr );
	}	
	initialise_paas_client( 
		"COAPSClient/V1", pptr->tls, 
		pptr->host, pptr->port, 
		pptr->user, pptr->password );
	return( pptr );
}

/*	-----------------------------------------	*/
/*	p a a s _ r e s o l v e _ h o s t n a m e 	*/
/*	-----------------------------------------	*/
private	char * paas_resolve_hostname( struct paas_response  * zptr )
{
	struct xml_element * document;
	struct xml_element * eptr;
	if (!( zptr ))
		return((char *) 0);
	else if (!( document = zptr->xmlroot ))
		return((char *) 0);
	else if (!( eptr = document_element( document, "uris" ) ))
		return((char *) 0);
	else if (!( eptr->value ))
		return((char *) 0);
	else	return( allocate_string( eptr->value ) );
}

/*	-------------------------------------------	*/
/* 	   	   s t a r t _ p a a s     	        */
/*	-------------------------------------------	*/
public	struct	rest_response * start_paas(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	paas * pptr;
	struct	paas_response * prptr;
	struct	paas_config * cfgptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->state != 0 )
	 	return( rest_html_response( aptr, 200, "OK Already Started" ) );
	else if (!( cfgptr = resolve_paas_configuration( pptr->profile ) ))
	 	return( rest_html_response( aptr, 403, "Invalid Action" ) );
	else
	{
		/* ----------------------------------- */
        	/*	Deploy paas application        */
		/* ----------------------------------- */
        	if (!( prptr = deploy_paas_application( pptr->environment, pptr->application, allocate_string(pptr->wardata), pptr->boundary ) ))
        	{
        	       return( rest_html_response( aptr, 601, "application deployment failure" ) );
        	}
        	else   prptr = paas_remove_response( prptr );
        
		/* ----------------------------------- */
	        /*	Start paas application         */
		/* ----------------------------------- */
	        if (!( prptr = start_paas_application( pptr->application ) ))
	        {
	               return( rest_html_response( aptr, 603, "application start failure" ) );
	        }
	        else   
		{
			/* resolve the access URI */
			/* ---------------------- */
			if ( pptr->hostname ) pptr->hostname = liberate( pptr->hostname );
			pptr->hostname = paas_resolve_hostname( prptr );
			prptr = paas_remove_response( prptr );
			pptr->state = 1;
			autosave_paas_nodes();
			return( rest_html_response( aptr, 200, "OK" ) );
        	}
	}
}

/*	-------------------------------------------	*/
/* 	   	s a v e _ p a a s     		        */
/*	-------------------------------------------	*/
public	struct	rest_response * save_paas(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	paas * pptr;
	struct paas_response * prptr;
	struct	paas_config * cfgptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if (!( cfgptr = resolve_paas_configuration( pptr->profile ) ))
	 	return( rest_html_response( aptr, 403, "Invalid Action" ) );

	else
	{
		/* ----------------------------------- */
		/* add code here to allocate resources */	
		/* ----------------------------------- */
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	   	s t o p _ p a a s     		        */
/*	-------------------------------------------	*/
public	struct	rest_response * stop_paas(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	paas * pptr;
	struct paas_response * prptr;
	struct	paas_config * cfgptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if (!( pptr->state ))
	 	return( rest_html_response( aptr, 200,"OK Already Stopped" ) );
	else if (!( cfgptr = resolve_paas_configuration( pptr->profile ) ))
	 	return( rest_html_response( aptr, 403, "Invalid Action" ) );

	else
	{
		/* ----------------------------------- */
	        /*	Stop paas application          */
		/* ----------------------------------- */
	        if (!( prptr = stop_paas_application( pptr->application ) ))
	        {
	               return( rest_html_response( aptr, 603, "application stop failure" ) );
	        }
	        else   prptr = paas_remove_response( prptr );

		/* ----------------------------------- */
	        /*	UnDeploy paas application      */
		/* ----------------------------------- */
	        if (!( prptr = undeploy_paas_application(  pptr->environment, pptr->application ) ))
	        {
	               return( rest_html_response( aptr, 601, "application undeployment failure" ) );
	        }
	        else   prptr = paas_remove_response( prptr );
		if ( pptr->hostname ) pptr->hostname = liberate( pptr->hostname );
		pptr->state = 0;
		autosave_paas_nodes();
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------------------		*/
/*	     t e r m i n a t e _ p a a s _ c o n t r a c t		*/
/*	-------------------------------------------------------		*/
private	int	terminate_paas_contract(int status, struct cords_paas_contract * cptr )
{
	if ( cptr->node.id )
		cptr->node.id = liberate( cptr->node.id );
	if ( cptr->node.message )
		cptr->node.message = occi_remove_response( cptr->node.message );

	if ( cptr->manifest.id )
		cptr->manifest.id = liberate( cptr->manifest.id );
	if ( cptr->manifest.message )
		cptr->manifest.message = occi_remove_response( cptr->manifest.message );

	if ( cptr->application.id )
		cptr->application.id = liberate( cptr->application.id );
	if ( cptr->application.message )
		cptr->application.message = occi_remove_response( cptr->application.message );

	if ( cptr->version.id )
		cptr->version.id = liberate( cptr->version.id );
	if ( cptr->version.message )
		cptr->version.message = occi_remove_response( cptr->version.message );

	if ( cptr->deployable.id )
		cptr->deployable.id = liberate( cptr->deployable.id );
	if ( cptr->deployable.message )
		cptr->deployable.message = occi_remove_response( cptr->deployable.message );

	if ( cptr->environment.id )
		cptr->environment.id = liberate( cptr->environment.id );
	if ( cptr->environment.message )
		cptr->environment.message = occi_remove_response( cptr->environment.message );

	return( status );
}

/*	-------------------------------------------	*/
/*	p a a s _ s e r i a l i s e _ m e s s a g e	*/
/*	-------------------------------------------	*/
private	int	applicationcounter=0;
private	int	paas_serialise_message( FILE * h, struct occi_response * message,int appcounter )
{ 
	struct	occi_element * eptr;
	struct	occi_category * cptr;
	char *	vptr;
	char *	vvptr;
	char 	aname[1024];
	char *	nptr;
	int	i;
	int	isapplication;
	struct	occi_response * zptr;

	if (!( cptr = message->category ))
		return( 118 );
	else
	{
		fprintf(h,"<%s",cptr->id);
		if (!( strcmp( cptr->id, "paas_application") ))
			isapplication = appcounter;
		else	isapplication = 0;
	
		/* -------------------- */
		/* serialise attributes */
		/* -------------------- */

		for (	eptr = message->first;
			eptr != (struct occi_element *) 0;
			eptr = eptr->next )
		{
			if (!( eptr->value ))
				continue;
			else if (!( vptr = allocate_string( eptr->value ) ))
				continue;
			else if (!( vptr = occi_unquoted_value( vptr )))
				continue;
			else if (!( strncmp( vptr, "http", strlen( "http" ) )))
			{
				liberate( vptr );
				continue;
			}
			else
			{
				strcpy(nptr=aname,eptr->name);
				if (!( strcmp( nptr, "link" ) ))
					continue;			
				for ( i=0; aname[i] != 0; i++)
				{
					if ( aname[i] == '.' )
						nptr = (&aname[i+1]);
				}
				if (!( strcmp( nptr, "id" ) ))
					continue;			
				else if (!( strcmp( nptr, "state" ) ))
					continue;
				else if (!( rest_valid_string( vvptr = vptr ) ))
				{
					if ( vptr ) liberate( vptr );
					continue;
				}
				if (!( isapplication ))
					fprintf(h," %s=%c%s%c",nptr,0x0022,vvptr,0x0022);
				else if (!( strcmp( nptr, "name") ))
					fprintf(h," %s=%c%s%u%c",nptr,0x0022,vvptr,isapplication,0x0022);
				else	fprintf(h," %s=%c%s%c",nptr,0x0022,vvptr,0x0022);

				liberate( vptr );
				continue;
			}
		}

		fprintf(h,">\n");

		/* ----------------------------- */
		/* serialise linkage and content */
		/* ----------------------------- */
		for (	eptr = message->first;
			eptr != (struct occi_element *) 0;
			eptr = eptr->next )
		{
			if (!( eptr->name ))
				continue;
			else if (!( eptr->value ))
				continue;
			else if (!( vptr = allocate_string( eptr->value ) ))
				continue;
			else if (!( strcmp( eptr->name, "link" ) ))
			{
				if (!( vptr = occi_unquoted_link( vptr )))
					continue;
			}
			else if (!( vptr = occi_unquoted_value( vptr )))
				continue;
			if ( strncmp( vptr, "http", strlen( "http" ) ) != 0)
			{
				liberate( vptr );
				continue;
			}
			else if (!( zptr = occi_simple_get( vptr, _CORDS_CONTRACT_AGENT, default_tls() ) ))
			{
				liberate( vptr );
				continue;
			}
			else 
			{
				paas_serialise_message( h, zptr, appcounter );
				zptr = occi_remove_response( zptr );
				liberate( vptr );
				continue;
			}
		}


		fprintf(h,"</%s>\n",cptr->id);
		return( 0 );
	}
}

/*	---------------------------------------------	*/
/*		p a a s _ r e t r i e v e _ w a r 	*/
/*	---------------------------------------------	*/
private	char *	paas_retrieve_war( struct cords_vector * root )
{
	char *	nptr;
	char *	pptr;
	char 	buffer[2048];
	if (!( nptr = occi_extract_atribut( root->message, "occi", 
		_CORDS_PAAS_DEPLOYABLE, _CORDS_NAME) ))
		return((char *) 0);
	else if (!( pptr = occi_extract_atribut( root->message, "occi", 
		_CORDS_PAAS_DEPLOYABLE, "location") ))
		return((char *) 0);
	else
	{
		sprintf(buffer,"%s%s",pptr,nptr);
		return( allocate_string( buffer ) );
	}
}

/*	-------------------------------------------	*/
/*	p a a s _ m u l t i p a r t _ c o n t e n t 	*/
/*	-------------------------------------------	*/
private	char *	paas_multipart_content( char * control, char * type, char * content, char * boundary )
{
	FILE * h;
	FILE * sh;
	unsigned int	c;
	char 	buffer[256];
	int	now;
	char *	filename;
	if (!( sh = fopen(content,"rb") ))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename( "tmp" )))
	{
		fclose(sh);
		return( filename );
	}
	else if (!( h = fopen( filename, "wb" ) ))
	{
		fclose(sh);
		return( liberate( filename ) );
	}
	else
	{
		fprintf(h,"--%s\r\n",boundary);
		fprintf(h,"Content-Disposition: form-data; name=\"%s\"\r\n",control);
		fprintf(h,"Content-Type: %s\r\n\r\n",type);
		while ((c = fgetc(sh)) != -1) fputc(c,h);
		fprintf(h,"\r\n--%s--\r\n",boundary);
		fclose(h);
		fclose(sh);
		return( filename );
	}
}

/*	---------------------------------------------	*/
/*	p a a s _ s e r i a l i s e _ e l e m e n t	*/
/*	---------------------------------------------	*/
private	char *	paas_serialise_element( struct cords_vector * root, int appcount )
{
	FILE *	h;
	char *	vptr;
	char *	filename;

	if (!( filename = rest_temporary_filename( "xml" ) ))
		return( filename );
	else if (!( h = fopen( filename, "w" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=\"1.0\" encoding=\"UTF8\"?>\n");
		paas_serialise_message( h, root->message,appcount );
		fclose(h);
		return( filename );
	}
}

/*	---------------------------------------------	*/
/*		p a a s _ a p p l i ca t i o n _ i d		*/
/*	---------------------------------------------	*/
private char * paas_application_id( struct paas_response * rptr )
{
	struct	xml_element * eptr;
	struct	xml_atribut * aptr;
	char * vptr;
	if (!( rptr ))
		return( (char *) 0 );
	else if (!( rptr->xmlroot ))
		return( (char *) 0 );
	else if (!( eptr = document_element( rptr->xmlroot, "application" ) ))
		return( (char *) 0 );
	else if (!( aptr = document_atribut( eptr, "appId" ) ))
		return( (char *) 0 );
	else if (!( aptr->value ))
		return( (char *) 0 );
	else if (!( vptr = allocate_string( aptr->value ) ))
		return( vptr );
	else	return( occi_unquoted_value( vptr ) );
}

/*	---------------------------------------------	*/
/*		p a a s _ e nv i r o n m e n t _ i d	*/
/*	---------------------------------------------	*/
private char * paas_environment_id( struct paas_response * rptr )
{
	struct	xml_element * eptr;
	struct	xml_atribut * aptr;
	char * vptr;
	if (!( rptr ))
		return( (char *) 0 );
	else if (!( rptr->xmlroot ))
		return( (char *) 0 );
	else if (!( eptr = document_element( rptr->xmlroot, "environment" ) ))
		return( (char *) 0 );
	else if (!( aptr = document_atribut( eptr, "envId" ) ))
		return( (char *) 0 );
	else if (!( aptr->value ))
		return( (char *) 0 );
	else if (!( vptr = allocate_string( aptr->value ) ))
		return( vptr );
	else	return( occi_unquoted_value( vptr ) );
}

/*	---------------------------------------------	*/
/* 	   c r e a t e _ p a a s _ c o n t r a c t	*/
/*	---------------------------------------------	*/
public	int	create_paas_contract(
		struct occi_category * optr,
		struct paas * pptr)
{
	int	appcount=0;
	char 	*	requestfile;
     	struct  paas_response * rptr;
	struct	occi_response * node;
	struct	occi_response * yptr;
	char 	*	vptr;
	char	*	agent=_CORDS_CONTRACT_AGENT;
	char 	*	tls=default_tls();
	struct	cords_paas_contract	contract;
	struct	paas_config * cfgptr;

	memset( &contract, 0, sizeof( struct cords_paas_contract ));

	if (!( pptr ))
		return( 118 );
	else if (!( pptr->profile ))
		return( 118 );
	else if (!( cfgptr = resolve_paas_configuration( pptr->profile ) ))
		return( 118 );

	/* ---------------------------- */
	/* recover the node description */
	/* ---------------------------- */
	if (!( pptr->node ))
		return(terminate_paas_contract(118, &contract ));
	else if (!( contract.node.id = allocate_string( pptr->node ) ))
		return(terminate_paas_contract(118, &contract ));
	else if (!( contract.node.message = occi_simple_get( contract.node.id, agent, tls ) ))
		return( terminate_paas_contract( 1170, &contract ) );

	/* ------------------------------------------------- */
	/* recover the paas manifest linkage and description */
	/* ------------------------------------------------- */
	else if (!( vptr = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_NODE, _CORDS_TYPE ) ))
		return( terminate_paas_contract( 1127, &contract ) );
	else if (!( contract.manifest.id = allocate_string( vptr ) ))
		return(terminate_paas_contract( 118, &contract ));
	else if (!( contract.manifest.message = occi_simple_get( contract.manifest.id, agent, tls ) ))
	{
		if (!( yptr = cords_retrieve_named_instance_list( _CORDS_PAAS_MANIFEST, "occi.paas_application_manifest.name", contract.manifest.id, agent,tls ) ))
			return( terminate_paas_contract( 1404, &contract ) );
		else if (!( contract.manifest.message = cords_retrieve_named_instance( yptr, agent, tls )))
		{
			yptr = occi_remove_response( yptr );
			return( terminate_paas_contract( 1404, &contract ) );
		}
		else	yptr = occi_remove_response( yptr );
	}

	/* ---------------------------------------------------- */
	/* recover the paas application linkage and description */
	/* ---------------------------------------------------- */
	if (!( vptr = occi_extract_atribut( contract.manifest.message, "occi", 
		_CORDS_PAAS_MANIFEST, _CORDS_PAAS_APPLICATION ) ))
		return( terminate_paas_contract( 1127, &contract ) );
	else if (!( contract.application.id = allocate_string( vptr ) ))
		return(terminate_paas_contract( 118, &contract ));
	else if (!( contract.application.message = occi_simple_get( contract.application.id, agent, tls ) ))
		return( terminate_paas_contract( 1404, &contract ) );

	/* -------------------------------- */
	/* retrieve the application version */
	/* -------------------------------- */
	else if (!( vptr = occi_extract_atribut( contract.application.message, "occi", 
		_CORDS_PAAS_APPLICATION, _CORDS_PAAS_VERSION ) ))
		return( terminate_paas_contract( 1127, &contract ) );
	else if (!( contract.version.id = allocate_string( vptr ) ))
		return(terminate_paas_contract( 118, &contract ));
	else if (!( contract.version.message = occi_simple_get( contract.version.id, agent, tls ) ))
		return( terminate_paas_contract( 1404, &contract ) );

	/* ----------------------------- */
	/* and the application deploable */
	/* ----------------------------- */
	else if (!( vptr = occi_extract_atribut( contract.version.message, "occi", 
		_CORDS_PAAS_VERSION, _CORDS_PAAS_DEPLOYABLE ) ))
		return( terminate_paas_contract( 1127, &contract ) );
	else if (!( contract.deployable.id = allocate_string( vptr ) ))
		return(terminate_paas_contract( 118, &contract ));
	else if (!( contract.deployable.message = occi_simple_get( contract.deployable.id, agent, tls ) ))
		return( terminate_paas_contract( 1404, &contract ) );

	/* ---------------------------------------------------- */
	/* recover the paas environment linkage and description */
	/* ---------------------------------------------------- */
	else if (!( vptr = occi_extract_atribut( contract.manifest.message, "occi", 
		_CORDS_PAAS_MANIFEST, _CORDS_PAAS_ENVIRONMENT ) ))
		return( terminate_paas_contract( 1127, &contract ) );
	else if (!( contract.environment.id = allocate_string( vptr ) ))
		return(terminate_paas_contract( 118, &contract ));
	else if (!( contract.environment.message = occi_simple_get( contract.environment.id, agent, tls ) ))
		return( terminate_paas_contract( 1170, &contract ) );

	/* ------------------------------------------------------- */
	/* generate the XML request file that will be used for EBV */
	/* API request calls to the intermediary "PAAS Interface"  */
	/* ------------------------------------------------------- */
	else if (!( pptr->envfile = paas_serialise_element( &contract.manifest,(appcount=++applicationcounter) ) ))
		return( terminate_paas_contract( 1171, &contract ) );

	/* ------------------------------------------------------- */
	/* generate the XML request file that will be used for EBV */
	/* API request calls to the intermediary "PAAS Interface"  */
	/* ------------------------------------------------------- */
	else if (!( pptr->appfile = paas_serialise_element( &contract.manifest,appcount ) ))
		return( terminate_paas_contract( 1172, &contract ) );

	/* ----------------------- */
	/* build the WAR file name */
	/* ----------------------- */
	else if (!( pptr->warfile = paas_retrieve_war( &contract.deployable ) ))
		return( terminate_paas_contract( 1173, &contract ) );
	else if (!( pptr->boundary = occi_allocate_uuid() ))
		return( terminate_paas_contract( 1174, &contract ) );
	else if (!( pptr->wardata = paas_multipart_content( "file", "application/octet-stream", pptr->warfile, pptr->boundary ) ))
		return( terminate_paas_contract( 1175, &contract ) );

	/* -------------------------------------------- */
	/*	Create paas environment                 */
	/* -------------------------------------------- */
	if (!( rptr = create_paas_environment( allocate_string( pptr->envfile ) ) ))
          	return( terminate_paas_contract( 501, &contract ) );

	/* ----------------------------------------- */
	/* retrieve the environment id from response */
	/* ----------------------------------------- */
	else if (!( pptr->environment = paas_environment_id( rptr ) ))
	{
		rptr = paas_remove_response( rptr );
          	return( terminate_paas_contract( 502, &contract ) );
	}
	else	rptr = paas_remove_response( rptr );

	/* ------------------------------------------- 	*/
	/*	Create paas application	       		*/
   	/* ------------------------------------------- 	*/
   	if (!( rptr = create_paas_application( allocate_string( pptr->appfile ) ) ))
		return( terminate_paas_contract( 503, &contract ) );

	/* ----------------------------------------- */
	/* retrieve the application id from response */
	/* ----------------------------------------- */
	else if (!( pptr->application = paas_application_id( rptr ) ))
	{
		rptr = paas_remove_response( rptr );
      		return( terminate_paas_contract( 504, &contract ) );
   	}
	else	rptr = paas_remove_response( rptr );
      
	/* ----------------------------- */
	/* contract creation is complete */
	/* ----------------------------- */
   	return(terminate_paas_contract(0, &contract ));

}

/*	---------------------------------------------	*/
/* 	   d e l e t e _ p a a s _ c o n t r a c t	*/
/*	---------------------------------------------	*/
public	int	delete_paas_contract(
		struct occi_category * optr,
		struct paas * pptr)
{
   	struct paas_response * rptr;
	struct	paas_config * cfgptr;

   	/* ----------------------------------------------- */
   	/*	Delete paas application                    */
   	/* ----------------------------------------------- */
	if (!( cfgptr = resolve_paas_configuration( pptr->profile ) ))
		return( 118 );

   	if (!( rptr = delete_paas_application( pptr->application )))
   	{
          	return( 504 );
   	}   
   	else   rptr = paas_remove_response( rptr );
   
 	if ( pptr->appfile )
		pptr->appfile = liberate( pptr->appfile );

	if ( pptr->application )
		pptr->application = liberate( pptr->application );

  	/* ----------------------------------------------- */
   	/*	Delete paas environment                    */
   	/* ----------------------------------------------- */
   	if (!( rptr = delete_paas_environment( pptr->environment )))
   	{
          	return( 504 );
   	}   
   	else   rptr = paas_remove_response( rptr );

	if ( pptr->envfile )
		pptr->envfile = liberate( pptr->envfile );

	if ( pptr->environment )
		pptr->environment = liberate( pptr->environment );

   	return(0);
}

        /* ---------------- */
#endif	/* _paas_contract_c */
	/* ---------------- */


