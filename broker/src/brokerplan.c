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
#ifndef	_broker_plan_c
#define	_broker_plan_c

#include "cb.h"
#include "cordslang.h"

/*	-------------------------------------------	*/
/* 	        i n s t a n c e _ p l a n 		*/
/*	-------------------------------------------	*/
private	struct	rest_response * instance_plan(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	occi_response * zptr;
	char *	service=(char *) 0;
	struct	cords_plan * pptr;

	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );

	/* ----------------------------------------------------------- */
	/* attempt to build a new service instance graph for this plan */
	/* ----------------------------------------------------------- */
	else if (!(service = cords_manifest_broker(
			_DEFAULT_PUBLISHER,
			pptr->id,
			pptr->name,
			pptr->manifest,
			_CORDS_CONTRACT_AGENT,
			default_tls(), (char *) 0, (struct xml_element **) 0 ) ))
	{
	 	return( rest_html_response( aptr, get_provisioning_status(), "PROVISIONING FAILURE" ) );
	}

	/* -------------------------------------------------------- */
	/* invoke the start action for the new service of this plan */
	/* -------------------------------------------------------- */
	else if (!( zptr = cords_invoke_action( service, _CORDS_START, _CORDS_CONTRACT_AGENT, default_tls() ) ))
	{
		service = liberate( service );
	 	return( rest_html_response( aptr, 517, "START FAILURE" ) );
	}
	else
	{
		/* ------------------------------------------------ */
		/* TODO: need to link the service to the plan still */
		/* ------------------------------------------------ */
		zptr = occi_remove_response( zptr );
		service = liberate( service );
		pptr->services++;
		return( rest_html_response( aptr, 200, "OK" ) );
	}

}

/*	------------------------------------------------------------------	*/
/*			b r o k e r _ p l a n _ b u i l d e r			*/
/*	------------------------------------------------------------------	*/
private	struct	occi_category *	broker_plan_builder( char * domain, char * category )
{
	struct	occi_category * optr;
	initialise_occi_resolver( _DEFAULT_PUBLISHER, (char *) 0, (char *) 0, (char *) 0 );
	if (!( optr = occi_cords_plan_builder( domain ,category ) ))
		return( optr );
	else if (!( optr = occi_add_action( optr,_CORDS_INSTANCE,"",instance_plan)))
		return( optr );
	else	return( optr );
}

#endif	/* _broker_plan_c */
	/* -------------- */










