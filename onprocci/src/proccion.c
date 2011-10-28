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

#ifndef	_proccion_c
#define	_proccion_c

#include "onclient.h"

/*	-------------------------------------------	*/
/* 	   s o f t b o o t _ o p e n n e b u l a  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * softboot_opennebula(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	opennebula * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	   h a r d b o o t _ o p e n n e b u l a  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * hardboot_opennebula(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	opennebula * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	     r e b u i l d_ o p e n n e b u l a  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * rebuild_opennebula(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	opennebula * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	      r e s i z e _ o p e n n e b u l a  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * resize_opennebula(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	opennebula * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	    c o n f i r m _ o p e n n e b u l a  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * confirm_opennebula(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	opennebula * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	      r e v e r t _ o p e n n e b u l a  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * revert_opennebula(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	opennebula * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}


/*	-------------------------------------------	*/
/* 	      c r e a t e _ o p e n n e b u l a  	*/
/*	-------------------------------------------	*/
private	int	create_opennebula(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	opennebula * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ o p e n n e b u l a  	*/
/*	-------------------------------------------	*/
private	int	retrieve_opennebula(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	opennebula * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ o p e n n e b u l a  	*/
/*	-------------------------------------------	*/
private	int	update_opennebula(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	opennebula * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ o p e n n e b u l a  	*/
/*	-------------------------------------------	*/
private	int	delete_opennebula(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	opennebula * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

private	struct	occi_interface	opennebula_interface = {
	create_opennebula,
	retrieve_opennebula,
	update_opennebula,
	delete_opennebula
	};

/*	-------------------------------------------	*/
/* 	       b u i l d _ o p e n n e b u l a  	*/
/*	-------------------------------------------	*/
/*	this function is to be called to build the	*/
/*	complete opennebula occi category to offer	*/
/*	an opennebula compute provisioning service	*/
/*	through a compatible one published occi		*/
/*	service interface.				*/
/*	-------------------------------------------	*/
public	struct	occi_category * build_opennebula( char * domain )
{
	struct	occi_category * optr;
	if (!( optr = occi_opennebula_builder( domain,"opennebula" ) ))
		return( optr );
	else
	{
		optr->callback  = &opennebula_interface;

		if (!( optr = occi_add_action( optr,"softboot","",softboot_opennebula)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"hardboot","",hardboot_opennebula)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"rebuild","",rebuild_opennebula)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"resize","",resize_opennebula)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"confirm","",confirm_opennebula)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"revert","",revert_opennebula)))
			return( optr );
		else	return( optr );
	}
}

/*	-------------------------------------------	*/
/*	s e t _ d e f a u l t _ o p e n n e b u l a	*/
/*	-------------------------------------------	*/
private	int	set_default_opennebula(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	on_config * pptr;
	struct	on_config * wptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else
	{
		while ( nptr->previous )
			nptr = nptr->previous;
		while ( nptr )
		{
			if ((wptr = nptr->contents) != (struct on_config*) 0)
				wptr->current = 0,
			nptr = nptr->next;
		}
		pptr->current = 1;
		return(0);
	}
}

/*	-----------------------------------------------------------	*/
/* 	b u i l d _ o p e n n e b u l a _ c o n f i g u r a t i o n	*/
/*	-----------------------------------------------------------	*/
/*	this category handles the configuration of the interface	*/
/*	to the oepn stack nebula for the preceeding category of		*/
/*	provisioning instance requests.					*/
/*	---------------------------------------------------------	*/
public	struct	occi_category * build_opennebula_configuration( char * domain )
{
	struct	occi_category * optr;
	if (!( optr = occi_on_config_builder( domain, "opennebula_configuration" ) ))
		return( optr );
	else if (!( optr = occi_add_action( optr,"current","",set_default_opennebula)))
		return( optr );
	else	return( optr );
}


#endif	/* _proccion_c */
	/* ----------- */


