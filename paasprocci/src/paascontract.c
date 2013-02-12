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
	struct	cords_vector	environment;
};

#define	_CORDS_PAAS_MANIFEST		"paas_manifest"
#define	_CORDS_PAAS_APPLICATION		"paas_application"
#define	_CORDS_PAAS_ENVIRONMENT		"paas_environment"

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
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		/* ----------------------------------- */
        	/*	Deploy paas application        */
		/* ----------------------------------- */
        	if (!( prptr = deploy_paas_application( pptr->environment, pptr->application ) ))
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
	        else   prptr = paas_remove_response( prptr );
        
		return( rest_html_response( aptr, 200, "OK" ) );
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
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
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

	if ( cptr->environment.id )
		cptr->environment.id = liberate( cptr->environment.id );
	if ( cptr->environment.message )
		cptr->environment.message = occi_remove_response( cptr->environment.message );

	return( status );
}

/*	-------------------------------------------	*/
/*	p a a s _ s e r i a l i s e _ m e s s a g e	*/
/*	-------------------------------------------	*/
private	int	paas_serialise_message( FILE * h, struct occi_response * message )
{ 
	struct	occi_element * eptr;
	struct	occi_category * cptr;
	char *	vptr;
	struct	occi_response * zptr;

	if (!( cptr = message->category ))
		return( 118 );
	else
	{
		fprintf(h,"<%s",cptr->id);

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
				fprintf(h," %s=%s",eptr->name, eptr->value );
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
			if (!( eptr->value ))
				continue;
			else if (!( vptr = allocate_string( eptr->value ) ))
				continue;
			else if (!( vptr = occi_unquoted_value( vptr )))
				continue;
			else if ( strncmp( vptr, "http", strlen( "http" ) ) != 0)
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
				paas_serialise_message( h, zptr );
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
/*	p a a s _ s e r i a l i s e _ e l e m e n t	*/
/*	---------------------------------------------	*/
private	char *	paas_serialise_element( struct cords_vector * root )
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
		paas_serialise_message( h, root->message );
		fclose(h);
		return( filename );
	}
}

/*	---------------------------------------------	*/
/* 	   c r e a t e _ p a a s _ c o n t r a c t	*/
/*	---------------------------------------------	*/
public	int	create_paas_contract(
		struct occi_category * optr,
		struct paas * pptr)
{
	char 	*	requestfile;
     	struct  paas_response * rptr;
	struct	occi_response * node;
	char 	*	vptr;
	char	*	agent=_CORDS_CONTRACT_AGENT;
	char 	*	tls=default_tls();
	struct	cords_paas_contract	contract;
	memset( &contract, 0, sizeof( struct cords_paas_contract ));

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
		return( terminate_paas_contract( 1170, &contract ) );

	/* ---------------------------------------------------- */
	/* recover the paas application linkage and description */
	/* ---------------------------------------------------- */
	else if (!( vptr = occi_extract_atribut( contract.manifest.message, "occi", 
		_CORDS_PAAS_MANIFEST, _CORDS_PAAS_APPLICATION ) ))
		return( terminate_paas_contract( 1127, &contract ) );
	else if (!( contract.application.id = allocate_string( vptr ) ))
		return(terminate_paas_contract( 118, &contract ));
	else if (!( contract.application.message = occi_simple_get( contract.application.id, agent, tls ) ))
		return( terminate_paas_contract( 1170, &contract ) );

	/* ---------------------------------------------------- */
	/* recover the paas environment linkage and description */
	/* ---------------------------------------------------- */
	else if (!( vptr = occi_extract_atribut( contract.application.message, "occi", 
		_CORDS_PAAS_APPLICATION, _CORDS_PAAS_ENVIRONMENT ) ))
		return( terminate_paas_contract( 1127, &contract ) );
	else if (!( contract.environment.id = allocate_string( vptr ) ))
		return(terminate_paas_contract( 118, &contract ));
	else if (!( contract.environment.message = occi_simple_get( contract.environment.id, agent, tls ) ))
		return( terminate_paas_contract( 1170, &contract ) );

	/* ------------------------------------------------------- */
	/* generate the XML request file that will be used for EBV */
	/* API request calls to the intermediary "PAAS Interface"  */
	/* ------------------------------------------------------- */
	else if (!( pptr->envfile = paas_serialise_element( &contract.environment ) ))
		return( terminate_paas_contract( 1171, &contract ) );

	/* ------------------------------------------------------- */
	/* generate the XML request file that will be used for EBV */
	/* API request calls to the intermediary "PAAS Interface"  */
	/* ------------------------------------------------------- */
	else if (!( pptr->appfile = paas_serialise_element( &contract.application ) ))
		return( terminate_paas_contract( 1171, &contract ) );

	/* -------------------------------------------- */
	/*	Create paas environment                 */
	/* -------------------------------------------- */
	if (!( rptr = create_paas_environment( pptr->envfile ) ))
	{
          	return( terminate_paas_contract( 501, &contract ) );
	}
	else   
	{
		/* ----------------------------------------- */
		/* retrieve the environment id from response */
		/* ----------------------------------------- */
		if (!( pptr->environment = allocate_string( "#envID#" ) ))
          		return( terminate_paas_contract( 527, &contract ) );
		else	rptr = paas_remove_response( rptr );
   	}

	/* ------------------------------------------- 	*/
	/*	Create paas application	       		*/
   	/* ------------------------------------------- 	*/
   	if (!( rptr = create_paas_application( pptr->environment, pptr->appfile ) ))
   	{
		return( terminate_paas_contract( 502, &contract ) );
   	}
	else   
	{
		/* ----------------------------------------- */
		/* retrieve the application id from response */
		/* ----------------------------------------- */
		if (!( pptr->application = allocate_string( "#appID#" ) ))
          		return( terminate_paas_contract( 527, &contract ) );
		else	rptr = paas_remove_response( rptr );
   	}
      
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

   	/* ----------------------------------------------- */
   	/*	Delete paas application                    */
   	/* ----------------------------------------------- */
   	if (!( rptr = delete_paas_application( pptr->application )))
   	{
          	return( 504 );
   	}   
   	else   rptr = paas_remove_response( rptr );
   
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

	if ( pptr->appfile )
		pptr->appfile = liberate( pptr->appfile );

   	return(0);
}

        /* ---------------- */
#endif	/* _paas_contract_c */
	/* ---------------- */


