#ifndef	_example_contract_c	
#define	_example_contract_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"

/*	-------------------------------------------	*/
/* 	   	s t a r t _ e x a m p l e     		*/
/*	-------------------------------------------	*/
public	struct	rest_response * start_example(
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
/* 	   	s a v e _ e x a m p l e     		*/
/*	-------------------------------------------	*/
public	struct	rest_response * save_example(
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
/* 	   	s t o p _ e x a m p l e     		*/
/*	-------------------------------------------	*/
public	struct	rest_response * stop_example(
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
/* 	c r e a t e _ e x a m p l e _ c o n t r a c t	*/
/*	---------------------------------------------	*/
public	int	create_example_contract(
		struct occi_category * optr,
		struct exampleprocci * pptr)
{
	/* ----------------------------------- */
	/* add code here for contract creation */
	/* ----------------------------------- */
	return(0);
}

/*	---------------------------------------------	*/
/* 	d e l e t e _ e x a m p l e _ c o n t r a c t	*/
/*	---------------------------------------------	*/
public	int	delete_example_contract(
		struct occi_category * optr,
		struct exampleprocci * pptr)
{
	/* ----------------------------------- */
	/* add code here for contract deletion */
	/* ----------------------------------- */
	return(0);
}

	/* ------------------- */
#endif	/* _example_contract_c */
	/* ------------------- */


