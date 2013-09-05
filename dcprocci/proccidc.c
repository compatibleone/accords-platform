#ifndef	_proccidc_c
#define	_proccidc_c

#include "cordslang.h"
#include "dcclient.h"

/* -------------------------------------------------------------------- */
/*   r e s o l v e _ d e l t a c l o u d _ c o n f i g u r a t i o n	*/
/* -------------------------------------------------------------------- */
private	struct	dc_config * resolve_deltacloud_configuration( char * sptr )
{
	struct	occi_kind_node * nptr;
	struct	dc_config * pptr=(struct dc_config *) 0;
	struct	occi_kind_node  * occi_first_dc_config_node();
	for (	nptr = occi_first_dc_config_node();
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next )
	{
		if (!( pptr = nptr->contents ))
			continue;
		else if (!( pptr->name ))
			continue;
		else if (!( strcmp( pptr->name, sptr ) ))
			return( pptr );
	}
	return((struct dc_config *) 0);
}

/*	-------------------------------------------------------------	*/
/* 	l l _ u s e _ d e l t a c l o u d _ c o n f i g u r a t i o n 	*/
/*	-------------------------------------------------------------	*/
private	struct	dc_config * ll_use_deltacloud_configuration( char * sptr )
{
	struct	dc_config * pptr;

	if (!( pptr = resolve_deltacloud_configuration( sptr )))
	 	return( pptr );
	else if (!( 
	 	dc_api_configuration(
			pptr->host,  
			pptr->user, pptr->password, 
			pptr->namespace, _CORDS_OS_AGENT, pptr->tls ) ))
		return( pptr );
	else	return( pptr );
}

/*	--------------------------------------------------------	*/
/* 	u s e _ d e l t a c l o u d _ c o n f i g u r a t i o n 	*/
/*	--------------------------------------------------------	*/
private	struct	dc_config * use_deltacloud_configuration( char * sptr )
{
	struct	dc_config * pptr;
	if (( pptr = ll_use_deltacloud_configuration( sptr )) != (struct dc_config *) 0)
		return( pptr );
	else if (!( sptr = get_operator_profile() ))
		return( pptr );
	else	return( use_deltacloud_configuration( sptr ) );
}


#include "dcaction.c"
#include "dccontract.c"
#include "dcmethod.c"

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
	struct	dc_config * pptr;
	struct	dc_config * wptr;
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
			if ((wptr = nptr->contents) != (struct dc_config*) 0)
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
	if (!( optr = occi_dc_config_builder( domain, "deltacloud_configuration" ) ))
		return( optr );
	else if (!( optr = occi_add_action( optr,"current","",set_default_deltacloud)))
		return( optr );
	else	return( optr );
}

#endif	/* _proccios_c */
	/* ----------- */


