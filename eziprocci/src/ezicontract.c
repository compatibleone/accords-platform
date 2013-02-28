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
#ifndef	_ezicontract_c	
#define	_ezicontract_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"

/*	-------------------------------------------	*/
/* 	   	s t a r t _ e a s i c l o u d s     		*/
/*	-------------------------------------------	*/
public	struct	rest_response * start_easiclouds(
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
/* 	   	s a v e _ e a s i c l o u d s     		*/
/*	-------------------------------------------	*/
public	struct	rest_response * save_easiclouds(
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
/* 	   	s t o p _ e a s i c l o u d s     		*/
/*	-------------------------------------------	*/
public	struct	rest_response * stop_easiclouds(
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
/* 	c r e a t e _ e a s i c l o u d s _ c o n t r a c t	*/
/*	---------------------------------------------	*/
public	int	create_easiclouds_contract(
		struct occi_category * optr,
		struct easiclouds * pptr)
{
	/* ----------------------------------- */
	/* add code here for contract creation */
	/* ----------------------------------- */
	return(0);
}

/*	---------------------------------------------	*/
/* 	d e l e t e _ e a s i c l o u d s _ c o n t r a c t	*/
/*	---------------------------------------------	*/
public	int	delete_easiclouds_contract(
		struct occi_category * optr,
		struct easiclouds * pptr)
{
	/* ----------------------------------- */
	/* add code here for contract deletion */
	/* ----------------------------------- */
	return(0);
}

		/* -------------- */
#endif	/* _ezicontract_c */
		/* -------------- */


