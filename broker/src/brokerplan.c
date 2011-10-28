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

#ifndef	_broker_plan_c
#define	_broker_plan_c


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
	int	status;
	struct	cords_plan * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if (!(status = cords_manifest_broker(
			_DEFAULT_PUBLISHER,
			pptr->id,
			pptr->name,
			pptr->manifest,
			_CORDS_BROKER_AGENT,
			default_tls() ) ))
	{
		pptr->services++;
		return( rest_html_response( aptr, 200, "OK" ) );
	}
	else 	return( rest_html_response( aptr, status, "whoops" ) );

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
	else if (!( optr = occi_add_action( optr,"instance","",instance_plan)))
		return( optr );
	else	return( optr );
}


#endif	/* _broker_plan_c */
	/* -------------- */










