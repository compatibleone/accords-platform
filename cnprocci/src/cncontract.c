#ifndef	_cncontract_c	
#define	_cncontract_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"

/*	-------------------------------------------	*/
/* 	   	s t a r t _ c o m p u t e n e x t     		*/
/*	-------------------------------------------	*/
public	struct	rest_response * start_computenext(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_contract * pptr;
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
/* 	   	s a v e _ c o m p u t e n e x t     		*/
/*	-------------------------------------------	*/
public	struct	rest_response * save_computenext(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_contract * pptr;
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
/* 	   	s t o p _ c o m p u t e n e x t     		*/
/*	-------------------------------------------	*/
public	struct	rest_response * stop_computenext(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_contract * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		/* ---------------------------------- */
		/* add code here to release resources */	
		/* ---------------------------------- */
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	---------------------------------------------	*/
/* 	c r e a t e _ c o m p u t e n e x t _ c o n t r a c t	*/
/*	---------------------------------------------	*/
public	int	create_computenext_contract(
		struct occi_category * optr,
		struct computenext * pptr,
		struct rest_request  * rptr)
{
	/* ----------------------------------- */
	/* add code here for contract creation */
	/* ----------------------------------- */
	return(0);
}

/*	---------------------------------------------	*/
/* 	d e l e t e _ c o m p u t e n e x t _ c o n t r a c t	*/
/*	---------------------------------------------	*/
public	int	delete_computenext_contract(
		struct occi_category * optr,
		struct computenext * pptr,
		struct rest_request  * rptr)
{
	/* ----------------------------------- */
	/* add code here for contract deletion */
	/* ----------------------------------- */
	return(0);
}

	/* ----------- */
#endif	/* _contract_c */
	/* ----------- */


