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

/*	-------------------------------------------	*/
/* 	   	   s t a r t _ p a a s     	            */
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
        /*	Deploy paas application            */
		/* ----------------------------------- */
        if (!( prptr = deploy_paas_application( pptr->environment, pptr->application, pptr->version, pptr->instance )))
        {
               return( rest_html_response( aptr, 601, "application deployment failure" ) );
        }
        else   prptr = paas_remove_response( prptr );
        
		/* ----------------------------------- */
        /*	Start environment                  */
		/* ----------------------------------- */
        if (!( prptr = start_paas_environment( pptr->environment )))
        {
               return( rest_html_response( aptr, 602, "environment start failure" ) );
        }
        else   prptr = paas_remove_response( prptr );
		/* ----------------------------------- */
        /*	Start paas application             */
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
/* 	   	s a v e _ p a a s     		            */
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
/* 	   	s t o p _ p a a s     		            */
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
        /*	UnDeploy paas application          */
		/* ----------------------------------- */
        if (!( prptr = undeploy_paas_application( pptr->environment, pptr->application, pptr->version, pptr->instance )))
        {
               return( rest_html_response( aptr, 601, "application deployment failure" ) );
        }
        else   prptr = paas_remove_response( prptr );
        
	/* ----------------------------------- */
        /*	Stop environment                   */
	/* ----------------------------------- */
        if (!( prptr = stop_paas_environment( pptr->environment )))
        {
               return( rest_html_response( aptr, 602, "environment start failure" ) );
        }
        else   prptr = paas_remove_response( prptr );
		/* ----------------------------------- */
        /*	Stop paas application              */
		/* ----------------------------------- */
        if (!( prptr = stop_paas_application( pptr->application, pptr->version, pptr->instance )))
        {
               return( rest_html_response( aptr, 603, "application start failure" ) );
        }
        else   prptr = paas_remove_response( prptr );
 		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	---------------------------------------------	*/
/* 	   c r e a t e _ p a a s _ c o n t r a c t	    */
/*	---------------------------------------------	*/
public	int	create_paas_contract(
		struct occi_category * optr,
		struct paas * pptr)
{
     struct    paas_response * rptr;
   /* ------------------------------------------- */
   /*	Create paas environment                   */
   /* ------------------------------------------- */
   if (!( rptr = create_paas_environment( pptr->environment )))
   {
          return( 501 );
   }
   else   rptr = paas_remove_response( rptr );
   
   /* ------------------------------------------- */
   /*	Create paas application                   */
   /* ------------------------------------------- */
   if (!( rptr = create_paas_application( pptr->artefact, pptr->description, pptr->version )))
   {
          return( 502 );
   }
   else   rptr = paas_remove_response( rptr );
      
   /* ------------------------------------------- */
   /* 	Create paas application version           */
   /* ------------------------------------------- */
   if (!( rptr = create_paas_application_version( pptr->application,pptr->artefact, pptr->description, pptr->version )))
   {
          return( 503 );
   }
   else   rptr = paas_remove_response( rptr );
      
   /* ------------------------------------------- */
   /*	Create paas application version instance  */
   /* ------------------------------------------- */
   if (!( rptr = create_paas_application_instance( pptr->application, pptr->version, pptr->topology, pptr->environment )))
   {
          return( 504 );
   }   
   else   rptr = paas_remove_response( rptr );

   return(0);

}

/*	---------------------------------------------	*/
/* 	   d e l e t e _ p a a s _ c o n t r a c t	    */
/*	---------------------------------------------	*/
public	int	delete_paas_contract(
		struct occi_category * optr,
		struct paas * pptr)
{
   struct paas_response * rptr;
   /* ----------------------------------------------- */
   /*	Delete paas application version instance      */
   /* ----------------------------------------------- */
   if (!( rptr = delete_paas_application_version_instance( pptr->application, pptr->version, pptr->instance )))
   {
          return( 501 );
   }   
   else   rptr = paas_remove_response( rptr );
   
   /* ----------------------------------------------- */
   /*	Delete paas application version               */
   /* ----------------------------------------------- */
   if (!( rptr = delete_paas_application_version( pptr->application, pptr->version )))
   {
          return( 502 );
   }   
   else   rptr = paas_remove_response( rptr );
   
   /* ----------------------------------------------- */
   /*	Delete paas application                       */
   /* ----------------------------------------------- */
   if (!( rptr = delete_paas_application( pptr->application )))
   {
          return( 504 );
   }   
   else   rptr = paas_remove_response( rptr );
   
   /* ----------------------------------------------- */
   /*	Delete paas environment                       */
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


