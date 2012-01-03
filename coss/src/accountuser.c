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
#ifndef	_account_user_c
#define	_account_user_c

/*	-------------------------------------------	*/
/* 	   	l o g i n _ u s e r 			*/
/*	-------------------------------------------	*/
private	struct	rest_response * login_user(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_user * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		if ( pptr->state == _OCCI_IDLE )
		{
			/* cords_invoke_action( pptr->provider, "save", _CORDS_CONTRACT_AGENT ); */
			pptr->when  = time((long*) 0);
			pptr->state = _OCCI_ACTIVE;
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	   	l o g o u t _ u s e r 			*/
/*	-------------------------------------------	*/
private	struct	rest_response * logout_user(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	cords_user * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else
	{
		if ( pptr->state != _OCCI_IDLE )
		{
			/* cords_invoke_action( pptr->provider, "save", _CORDS_CONTRACT_AGENT ); */
			pptr->when  = time((long*) 0);
			pptr->state = _OCCI_IDLE;
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	------------------------------------------------------------------	*/
/*			a c c o u n t _ u s e r _ b u i l d e r			*/
/*	------------------------------------------------------------------	*/
private	struct occi_category *	account_user_builder( char * dptr, char * nptr )
{
	struct	occi_category * optr;

	if (!( optr = occi_cords_user_builder( dptr, nptr ) ))
		return( optr );
	else if (!( optr = occi_add_action( optr,"login","",login_user)))
		return( optr );
	else if (!( optr = occi_add_action( optr,"logout","",logout_user)))
		return( optr );
	else 	return( optr );
}

#endif	/* _account_user_c */
	/* --------------- */


