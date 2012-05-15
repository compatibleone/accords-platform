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
#ifndef	_dcaction_c
#define	_dcaction_c

/*	-------------------------------------------	*/
/* 	      s t a r t  _ d e l t a c l o u d	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * start_deltacloud(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	deltacloud * pptr;
	struct	dc_config *  kptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 400, "Invalid Request" ) );
	else if (!( kptr = use_deltacloud_configuration( pptr->profile )))
	 	return( rest_html_response( aptr, 400, "Incorrect Configuration" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	      s t o p _ d e l t a c l o u d	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * stop_deltacloud(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	deltacloud * pptr;
	struct	dc_config *  kptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 400, "Invalid Request" ) );
	else if (!( kptr = use_deltacloud_configuration( pptr->profile )))
	 	return( rest_html_response( aptr, 400, "Incorrect Configuration" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	      s a v e  _ d e l t a c l o u d	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * save_deltacloud(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	deltacloud * pptr;
	struct	dc_config *  kptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 400, "Invalid Request" ) );
	else if (!( kptr = use_deltacloud_configuration( pptr->profile )))
	 	return( rest_html_response( aptr, 400, "Incorrect Configuration" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	    s n a p s h o t _ d e l t a c l o u d  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * snapshot_deltacloud(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	deltacloud * pptr;
	struct	dc_config *  kptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 400, "Invalid Request" ) );
	else if (!( kptr = use_deltacloud_configuration( pptr->profile )))
	 	return( rest_html_response( aptr, 400, "Incorrect Configuration" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}


	/* ----------- */
#endif	/* _dcaction_c */
	/* ----------- */

