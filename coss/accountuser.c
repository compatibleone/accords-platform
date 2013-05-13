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


