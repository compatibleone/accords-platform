#ifndef	_proccidc_c
#define	_proccidc_c

#include "cordslang.h"

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
 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
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
 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
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
 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
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
 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
}

/*	-------------------------------------------	*/
/* 	      c r e a t e _ d e l t a c l o u d  	*/
/*	-------------------------------------------	*/
private	int	create_deltacloud(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	deltacloud * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( pptr->node ))
		return( 0 ); 
	else	return( 0 );
	/* create_deltacloud_contract(optr,pptr, _CORDS_CONTRACT_AGENT, DcProcci.tls)); */
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ d e l t a c l o u d  	*/
/*	-------------------------------------------	*/
private	int	retrieve_deltacloud(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	deltacloud * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ d e l t a c l o u d  	*/
/*	-------------------------------------------	*/
private	int	update_deltacloud(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	deltacloud * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ d e l t a c l o u d  	*/
/*	-------------------------------------------	*/
private	int	delete_deltacloud(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	deltacloud * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
	/* delete_deltacloud_contract(optr, pptr, _CORDS_CONTRACT_AGENT, DcProcci.tls)); */
}

private	struct	occi_interface	deltacloud_interface = {
	create_deltacloud,
	retrieve_deltacloud,
	update_deltacloud,
	delete_deltacloud
	};

/*	-------------------------------------------	*/
/* 	       b u i l d _ d e l t a c l o u d  	*/
/*	-------------------------------------------	*/
/*	this function is to be called to build the	*/
/*	complete deltacloud occi category to offer	*/
/*	an deltacloud compute provisioning service	*/
/*	through a compatible one published occi		*/
/*	service interface.				*/
/*	-------------------------------------------	*/
public	struct	occi_category * build_deltacloud( char * domain )
{
	struct	occi_category * optr;
	if (!( optr = occi_deltacloud_builder( domain,_CORDS_DELTACLOUD ) ))
		return( optr );
	else
	{
		optr->callback  = &deltacloud_interface;
		optr->access |= _OCCI_PROVIDER;
		if (!( optr = occi_add_action( optr,_CORDS_START,"",start_deltacloud)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_SAVE,"",save_deltacloud)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_SNAPSHOT,"",snapshot_deltacloud)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_STOP,"",stop_deltacloud)))
			return( optr );
		else	return( optr );
	}
}

/*	-------------------------------------------	*/
/*	s e t _ d e f a u l t _ d e l t a c l o u d	*/
/*	-------------------------------------------	*/
private	int	set_default_deltacloud(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	os_config * pptr;
	struct	os_config * wptr;
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
			if ((wptr = nptr->contents) != (struct os_config*) 0)
				wptr->current = 0,
			nptr = nptr->next;
		}
		pptr->current = 1;
		return(0);
	}
}

/*	-----------------------------------------------------------	*/
/* 	b u i l d _ d e l t a c l o u d _ c o n f i g u r a t i o n	*/
/*	-----------------------------------------------------------	*/
/*	this category handles the configuration of the interface	*/
/*	to the oepn stack server for the preceeding category of		*/
/*	provisioning instance requests.					*/
/*	-----------------------------------------------------------	*/
public	struct	occi_category * build_deltacloud_configuration( char * domain )
{
	struct	occi_category * optr;
	if (!( optr = occi_cords_osconfig_builder( domain, "deltacloud_configuration" ) ))
		return( optr );
	else if (!( optr = occi_add_action( optr,"current","",set_default_deltacloud)))
		return( optr );
	else	return( optr );
}

#endif	/* _proccios_c */
	/* ----------- */


