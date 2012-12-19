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
	struct	cords_vector	version;
	struct	cords_vector	instance;
	struct	cords_vector	deployable;
	struct	cords_vector	environment;
	struct	cords_vector	configuration;
};

#define	_CORDS_PAAS_MANIFEST		"paas_manifest"
#define	_CORDS_PAAS_APPLICATION		"paas_application"
#define	_CORDS_PAAS_VERSION		"paas_application_version"
#define	_CORDS_PAAS_INSTANCE		"paas_application_version_instance"
#define	_CORDS_PAAS_DEPLOYABLE		"paas_application_deployable"
#define	_CORDS_PAAS_ENVIRONMENT		"paas_application_environment"
#define	_CORDS_PAAS_CONFIGURATION	"paas_application_configuration_template"

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
        	if (!( prptr = deploy_paas_application( pptr->environment, pptr->application, pptr->version, pptr->instance )))
        	{
        	       return( rest_html_response( aptr, 601, "application deployment failure" ) );
        	}
        	else   prptr = paas_remove_response( prptr );
        
		/* ----------------------------------- */
	        /*	Start environment              */
		/* ----------------------------------- */
	        if (!( prptr = start_paas_environment( pptr->environment )))
		        {
	               return( rest_html_response( aptr, 602, "environment start failure" ) );
	        }
	        else   prptr = paas_remove_response( prptr );
		/* ----------------------------------- */
	        /*	Start paas application         */
		/* ----------------------------------- */
	        if (!( prptr = start_paas_application( pptr->application, pptr->version, pptr->instance )))
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
	        /*	UnDeploy paas application      */
		/* ----------------------------------- */
	        if (!( prptr = undeploy_paas_application( pptr->environment, pptr->application, pptr->version, pptr->instance )))
	        {
	               return( rest_html_response( aptr, 601, "application deployment failure" ) );
	        }
	        else   prptr = paas_remove_response( prptr );
        
		/* ----------------------------------- */
	        /*	Stop environment               */
		/* ----------------------------------- */
	        if (!( prptr = stop_paas_environment( pptr->environment )))
	        {
	               return( rest_html_response( aptr, 602, "environment start failure" ) );
	        }
	        else   prptr = paas_remove_response( prptr );
		/* ----------------------------------- */
	        /*	Stop paas application          */
		/* ----------------------------------- */
	        if (!( prptr = stop_paas_application( pptr->application, pptr->version, pptr->instance )))
	        {
	               return( rest_html_response( aptr, 603, "application start failure" ) );
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

	if ( cptr->version.id )
		cptr->version.id = liberate( cptr->version.id );
	if ( cptr->version.message )
		cptr->version.message = occi_remove_response( cptr->version.message );

	if ( cptr->environment.id )
		cptr->environment.id = liberate( cptr->environment.id );
	if ( cptr->environment.message )
		cptr->environment.message = occi_remove_response( cptr->environment.message );

	if ( cptr->instance.id )
		cptr->instance.id = liberate( cptr->instance.id );
	if ( cptr->instance.message )
		cptr->instance.message = occi_remove_response( cptr->instance.message );

	if ( cptr->configuration.id )
		cptr->configuration.id = liberate( cptr->configuration.id );
	if ( cptr->configuration.message )
		cptr->configuration.message = occi_remove_response( cptr->configuration.message );

	return( status );
}

/*	---------------------------------------------	*/
/* 	   c r e a t e _ p a a s _ c o n t r a c t	*/
/*	---------------------------------------------	*/
public	int	create_paas_contract(
		struct occi_category * optr,
		struct paas * pptr)
{

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
	else if (!( vptr = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_PAAS_MANIFEST, _CORDS_PAAS_APPLICATION ) ))
		return( terminate_paas_contract( 1127, &contract ) );
	else if (!( contract.application.id = allocate_string( vptr ) ))
		return(terminate_paas_contract( 118, &contract ));
	else if (!( contract.application.message = occi_simple_get( contract.manifest.id, agent, tls ) ))
		return( terminate_paas_contract( 1170, &contract ) );

	/* ------------------------------------------------ */
	/* recover the paas version linkage and description */
	/* ------------------------------------------------ */
	else if (!( vptr = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_PAAS_APPLICATION, _CORDS_PAAS_VERSION ) ))
		return( terminate_paas_contract( 1127, &contract ) );
	else if (!( contract.version.id = allocate_string( vptr ) ))
		return(terminate_paas_contract( 118, &contract ));
	else if (!( contract.version.message = occi_simple_get( contract.manifest.id, agent, tls ) ))
		return( terminate_paas_contract( 1170, &contract ) );

	/* ------------------------------------------------- */
	/* recover the paas instance linkage and description */
	/* ------------------------------------------------- */
	else if (!( vptr = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_PAAS_VERSION, _CORDS_PAAS_INSTANCE ) ))
		return( terminate_paas_contract( 1127, &contract ) );
	else if (!( contract.instance.id = allocate_string( vptr ) ))
		return(terminate_paas_contract( 118, &contract ));
	else if (!( contract.instance.message = occi_simple_get( contract.manifest.id, agent, tls ) ))
		return( terminate_paas_contract( 1170, &contract ) );

	/* --------------------------------------------------- */
	/* recover the paas deployable linkage and description */
	/* --------------------------------------------------- */
	else if (!( vptr = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_PAAS_INSTANCE, _CORDS_PAAS_DEPLOYABLE ) ))
		return( terminate_paas_contract( 1127, &contract ) );
	else if (!( contract.deployable.id = allocate_string( vptr ) ))
		return(terminate_paas_contract( 118, &contract ));
	else if (!( contract.deployable.message = occi_simple_get( contract.manifest.id, agent, tls ) ))
		return( terminate_paas_contract( 1170, &contract ) );

	/* ---------------------------------------------------- */
	/* recover the paas environment linkage and description */
	/* ---------------------------------------------------- */
	else if (!( vptr = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_PAAS_APPLICATION, _CORDS_PAAS_ENVIRONMENT ) ))
		return( terminate_paas_contract( 1127, &contract ) );
	else if (!( contract.environment.id = allocate_string( vptr ) ))
		return(terminate_paas_contract( 118, &contract ));
	else if (!( contract.environment.message = occi_simple_get( contract.manifest.id, agent, tls ) ))
		return( terminate_paas_contract( 1170, &contract ) );

	/* ------------------------------------------------------ */
	/* recover the paas configuration linkage and description */
	/* ------------------------------------------------------ */
	else if (!( vptr = occi_extract_atribut( contract.node.message, "occi", 
		_CORDS_PAAS_APPLICATION, _CORDS_PAAS_CONFIGURATION ) ))
		return( terminate_paas_contract( 1127, &contract ) );
	else if (!( contract.configuration.id = allocate_string( vptr ) ))
		return(terminate_paas_contract( 118, &contract ));
	else if (!( contract.configuration.message = occi_simple_get( contract.manifest.id, agent, tls ) ))
		return( terminate_paas_contract( 1170, &contract ) );

	/* -------------------------------------------- */
	/*	Create paas environment                 */
	/* -------------------------------------------- */
	if (!( rptr = create_paas_environment( pptr->environment )))
	{
          	return( terminate_paas_contract( 501, &contract ) );
	}
	else   rptr = paas_remove_response( rptr );
   
	/* ------------------------------------------- 	*/
	/*	Create paas application	       		*/
   	/* ------------------------------------------- 	*/
   	if (!( rptr = create_paas_application( pptr->artefact, pptr->description, pptr->version )))
   	{
		return( terminate_paas_contract( 502, &contract ) );
   	}
   	else   rptr = paas_remove_response( rptr );
      
   	/* ------------------------------------------- */
   	/* 	Create paas application version        */
   	/* ------------------------------------------- */
   	if (!( rptr = create_paas_application_version( pptr->application, pptr->artefact, pptr->description, pptr->version )))
   	{
         	 return( terminate_paas_contract( 503, &contract ) );
   	}
   	else   rptr = paas_remove_response( rptr );
      
	/* ------------------------------------------- */
	/*	Create paas application version instance  */
	/* ------------------------------------------- */
   	if (!( rptr = create_paas_application_instance( pptr->application, pptr->version, pptr->topology, pptr->environment )))
   	{
		return( terminate_paas_contract( 504, &contract ) );
   	}   
   	else   rptr = paas_remove_response( rptr );

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
   	/*	Delete paas application version instance   */
   	/* ----------------------------------------------- */
   	if (!( rptr = delete_paas_application_version_instance( pptr->application, pptr->version, pptr->instance )))
   	{
          	return( 501 );
   	}   
   	else   rptr = paas_remove_response( rptr );
   
   	/* ----------------------------------------------- */
   	/*	Delete paas application version            */
   	/* ----------------------------------------------- */
   	if (!( rptr = delete_paas_application_version( pptr->application, pptr->version )))
   	{
          	return( 502 );
   	}   
   	else   rptr = paas_remove_response( rptr );
   
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

   	return(0);
}

        /* ---------------- */
#endif	/* _paas_contract_c */
	/* ---------------- */


