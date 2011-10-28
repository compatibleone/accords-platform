/* ------------------------------------------------------------------------------------	*/
/*				 CompatibleOne Cloudware				*/
/* ------------------------------------------------------------------------------------ */
/*											*/
/* Ce fichier fait partie de ce(tte) oeuvre de Iain James Marshall et est mise a 	*/
/* disposition selon les termes de la licence Creative Commons Paternit‚ : 		*/
/*											*/
/*			 	Pas d'Utilisation Commerciale 				*/
/*				Pas de Modification 					*/
/*				3.0 non transcrit.					*/
/*											*/
/* ------------------------------------------------------------------------------------ */
/* 			Copyright (c) 2011 Iain James Marshall for Prologue 		*/
/*				   All rights reserved					*/
/* ------------------------------------------------------------------------------------ */
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


