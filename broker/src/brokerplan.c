/* ------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                   */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>    */
/* --------------------------------------------------------------------*/
/*  This is free software; you can redistribute it and/or modify it    */
/*  under the terms of the GNU Lesser General Public License as        */
/*  published by the Free Software Foundation; either version 2.1 of   */
/*  the License, or (at your option) any later version.                */
/*                                                                     */
/*  This software is distributed in the hope that it will be useful,   */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of     */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   */
/*  Lesser General Public License for more details.                    */
/*                                                                     */
/*  You should have received a copy of the GNU Lesser General Public   */
/*  License along with this software; if not, write to the Free        */
/*  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA */
/*  02110-1301 USA, or see the FSF site: http://www.fsf.org.           */
/* --------------------------------------------------------------------*/
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
			_CORDS_BROKER_AGENT,
			default_tls(), (char *) 0, (struct xml_element **) 0 ) ))
	{
	 	return( rest_html_response( aptr, get_provisioning_status(), "PROVISIONING FAILURE" ) );
	}

	/* -------------------------------------------------------- */
	/* invoke the start action for the new service of this plan */
	/* -------------------------------------------------------- */
	else if (!( cords_invoke_action( service, _CORDS_START, _CORDS_BROKER_AGENT, default_tls() ) ))
	{
		service = liberate( service );
	 	return( rest_html_response( aptr, 517, "START FAILURE" ) );
	}
	else
	{
		/* ------------------------------------------------ */
		/* TODO: need to link the service to the plan still */
		/* ------------------------------------------------ */
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










