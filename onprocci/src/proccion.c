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
#include "cordslang.h"
#include "occiresolver.h"

/* ---------------------------------------------------------------------------- */
/* 		r e s o l v e _ o n _ c o n f i g u r a t i o n			*/
/* ---------------------------------------------------------------------------- */
private	struct	on_config * resolve_on_configuration( char * sptr )
{
	struct	occi_kind_node * nptr;
	struct	on_config * pptr=(struct on_config *) 0;
	struct	occi_kind_node  * occi_first_on_config_node();
	for (	nptr = occi_first_on_config_node();
		nptr != (struct occi_kind_node *) 0;
		nptr = nptr->next )
	{
		if (!( pptr = nptr->contents ))
			continue;
		else if (!( pptr->name ))
			continue;
		else if (!( strcmp( pptr->name, sptr ) ))
			break;
	}
	return( pptr );
}

/*	--------------------------------------------------------	*/
/* 	   r e s o l v e _ o p e n n e b u l a _ v e r s i o n 		*/
/*	--------------------------------------------------------	*/
private	char *	resolve_opennebula_version( char * sptr )
{
	struct	on_config * pptr;

	if (!( pptr = resolve_on_configuration( sptr )))
	 	return( "" );
	else	return( ( pptr->version ? pptr->version : "" ) );
}


/*	--------------------------------------------------------	*/
/* 	u s e _ o p e n n e b u l a _ c o n f i g u r a t i o n 	*/
/*	--------------------------------------------------------	*/
private	int	use_opennebula_configuration( char * sptr )
{
	struct	on_config * pptr;

	if (!( pptr = resolve_on_configuration( sptr )))
	 	return( 404 );

	else 	return( on_initialise_client( 
			pptr->user, pptr->password, 
			pptr->host, _CORDS_OS_AGENT, pptr->version, pptr->tls ));
}

/*	--------------------------------------------------------	*/
/* 	       r e s e t _ o p e n n e b u l a _ s e r v e r		*/
/*	--------------------------------------------------------	*/
private	int	reset_opennebula_server( struct opennebula * pptr )
{
	if ( pptr )
	{
		if ( pptr->number ) pptr->number = liberate( pptr->number );
		if ( pptr->hostname ) pptr->hostname = liberate( pptr->hostname );
		if ( pptr->publicaddr ) pptr->publicaddr = liberate( pptr->publicaddr );
		if ( pptr->privateaddr ) pptr->privateaddr = liberate( pptr->privateaddr );
		pptr->number = allocate_string("");
		pptr->hostname = allocate_string("");
		pptr->publicaddr = allocate_string("");
		pptr->privateaddr = allocate_string("");
		pptr->status = _OCCI_IDLE;
	}
	return(0);
}

/*	--------------------------------------------------------	*/
/* 	     c o n n e c t _ o p e n n e b u l a _ s e r v e r		*/
/*	--------------------------------------------------------	*/
private	int	connect_opennebula_server( struct on_response * rptr,struct opennebula * pptr )
{
	char *	version;
	struct	on_response * zptr;
	struct	on_response * yptr;
	char *	vptr;

	if (!( pptr ))
		return( 118 );
	else
	{
		/* ---------------------------------------------------- */
		/* we must now analyse the response from the open stack	*/
		/* and collect the valuable data : admin pass is only 	*/
		/* available at this point and cannot be retrieved by	*/
		/* any other means so it must not be lost.		*/ 
		/* ---------------------------------------------------- */
		if ( check_debug() )
		{
			rest_log_message("*** ON PROCCI connect_opennebula_server( entry ) ***");
		}

		if ( pptr->number ) 
			pptr->number = liberate( pptr->number );

		if (!( vptr = json_atribut( rptr->jsonroot, "id") ))
		{
			reset_opennebula_server( pptr );
			return( 27 );
		}
		else if (!( pptr->number = allocate_string(vptr)))
		{
			reset_opennebula_server( pptr );
			return( 27 );
		}
		if ( check_debug() )
		{
			rest_log_message("*** ON PROCCI Instance ID ***");
			rest_log_message( pptr->number );
		}

		autosave_opennebula_nodes();

		/* ----------------------------------------------------- */
		/* we must now await ACTIVE status to be able to collect */
		/* the final identification information to complete the  */
		/* opennebula provisioning request.			 */
		/* ----------------------------------------------------- */
		yptr = rptr;
		zptr = (struct on_response *) 0;
		while (1)
		{
			if (!( vptr = json_atribut( yptr->jsonroot, "status" )))
			{
				reset_opennebula_server( pptr );
				return( 27 );
			}
			if ( check_debug() )
			{
				rest_log_message("*** ON PROCCI Testing Build Status ***");
				rest_log_message( vptr );
			}
			if (!( strcmp( vptr, "BUILD" )))
			{
				sleep(1);
				if ( zptr )
					zptr = liberate_on_response( zptr );
				if (!( zptr = on_get_server( pptr->number )))
				{
					reset_opennebula_server( pptr );
					return( 555 );
				}
				else	yptr = zptr;
			}
			else if (!( strcmp( vptr, "ACTIVE" )))
				break;
		}

		if ( pptr->hostname ) pptr->hostname = liberate( pptr->hostname );
		if ( pptr->publicaddr ) pptr->publicaddr = liberate( pptr->publicaddr );
		if ( pptr->privateaddr ) pptr->privateaddr = liberate( pptr->privateaddr );

		/* ------------------------------------------------------------ */
		/* set the host name field now to the public or private address */
		/* ------------------------------------------------------------ */
		if (( pptr->publicaddr ) && ( strlen( pptr->publicaddr ) != 0))
		{
			if (!( pptr->hostname = allocate_string( pptr->publicaddr ) ))
			{
				reset_opennebula_server( pptr );
				return( 27 );
			}
		}
		else if (( pptr->privateaddr ) && ( strlen( pptr->privateaddr ) != 0))
		{
			if (!( pptr->hostname = allocate_string( pptr->privateaddr ) ))
			{
				reset_opennebula_server( pptr );
				return( 27 );
			}
		}

		/* ------------------------------------------------- */
		/* The instance is ready for use ( or more or less ) */
		/* ------------------------------------------------- */
		pptr->when = time((long *) 0);
		pptr->status = _OCCI_RUNNING;
		if ( check_debug() )
		{
			rest_log_message("*** ON PROCCI Instance is UP and RUNNING ***");
		}
		autosave_opennebula_nodes();
		if ( check_debug() )
		{
			rest_log_message("*** ON PROCCI connect_opennebula_server( exit ) ***");
		}
		return(0);
	}
}

/*	-------------------------------------------	*/
/* 	      s t a r t  _ o p e n n e b u l a	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * start_opennebula(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	char		* idptr;
	struct	on_response * osptr;
	struct	on_response * metaptr;
	struct	opennebula * pptr;
	int		status;
	char	*	filename;
	char	*	metafilename;
	char		buffer[512];
	char 	*	personality;
	char 	*	resource=_CORDS_LAUNCH_CFG;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->status != _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if ((status = use_opennebula_configuration( pptr->profile )) != 0)
		return( rest_html_response( aptr, status, "Configuration Not Found" ) );

#ifdef	_ON_PERSONALITY

	sprintf(buffer,"contract=%s/%s/%s\npublisher=%s\n",
		OnProcci.identity,_CORDS_OPENNEBULA,pptr->id,OnProcci.publisher);
	
	if (!( personality = allocate_string(buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure : Personality" ) );

	sprintf(buffer,"%s/%s/%s",OnProcci.identity,_CORDS_OPENNEBULA,pptr->id);

	if (!( personality = opennebula_instructions( buffer, personality ) ))
		return( rest_html_response( aptr, 500, "Server Failure : Configuration Instructions" ) );

#endif

	if (!( filename = on_create_compute_request( 
		pptr->name, pptr->image, pptr->flavor, pptr->publicnetwork, pptr->privatenetwork, personality, resource ) ))
	 	return( rest_html_response( aptr, 400, "Bad Request : Create Server Message" ) );
	else if (!( osptr = on_create_compute( filename )))
	 	return( rest_html_response( aptr, 400, "Bad Request : Create Server Request" ) );

	else
	{
		liberate( filename );
		/* --------------------------------- */
		/* retrieve crucial data from server */
		/* --------------------------------- */
		status = connect_opennebula_server( osptr, pptr );
#ifdef	_ON_PERSONALITY
		if (!( status ))
		{
			/* ----------------------- */
			/* create server meta data */
			/* ----------------------- */
			if (!( idptr = json_atribut( osptr->jsonroot, "id") ))
			 	return( rest_html_response( aptr, 400, "Bad Request : Missing Meta Data Server ID" ) );

			else if (!( metafilename = on_create_metadata_request( personality ) ))
			 	return( rest_html_response( aptr, 400, "Bad Request : Create MetaData Message" ) );
			else if (!( metaptr = on_create_metadata( idptr, metafilename )))
			 	return( rest_html_response( aptr, 400, "Bad Request : Create MetaData Request" ) );
			else
			{
				metaptr = liberate_on_response( metaptr );
				liberate( metafilename );
			}
		}
#endif
		osptr = liberate_on_response( osptr );
		if (!( status ))
			return( rest_html_response( aptr, 200, "OK" ) );
		else  	return( rest_html_response( aptr, 400, "Bad Request : Connect Open Stack" ) );
	}

}

/*	-------------------------------------------	*/
/* 	      s a v e  _ o p e n n e b u l a	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * save_opennebula(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	on_response * osptr;
	int		status;
	struct	opennebula * pptr;
	char	*	filename;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->status == _OCCI_IDLE )
		return( rest_html_response( aptr, 400, "Contract Not Active" ) );

#ifndef	_ON_PERSONALITY
	else 	return( rest_html_response( aptr, 200, "OK" ) );
#else
	else if ((status = use_opennebula_configuration( pptr->profile )) != 0)
		return( rest_html_response( aptr, status, "Not Found" ) );
	else if (!( filename = on_create_image_request( pptr->name, pptr->number ) ))
	 	return( rest_html_response( aptr, 400, "Bad Request" ) );
	else if (!( osptr = on_create_image( filename ) ))
	 	return( rest_html_response( aptr, 400, "Bad Request" ) );
	else
	{
		/* --------------------------------- */
		/* retrieve crucial data from server */
		/* --------------------------------- */
		status = connect_opennebula_image( osptr, pptr );
		osptr = liberate_on_response( osptr );
		if (!( status ))
			return( rest_html_response( aptr, 200, "OK" ) );
		else  	return( rest_html_response( aptr, 400, "Bad Request" ) );
	}
#endif
}

/*	-------------------------------------------	*/
/* 	      s t o p  _ o p e n n e b u l a	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * stop_opennebula(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	on_response * osptr;
	int		status;
	struct	opennebula * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->status == _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if ((status = use_opennebula_configuration( pptr->profile )) != 0)
		return( rest_html_response( aptr, status, "Not Found" ) );
	else if (!( osptr = on_delete_server( pptr->number )))
	 	return( rest_html_response( aptr, 400, "Bad Request" ) );
	else
	{
		if ( pptr->status != _OCCI_IDLE )
		{
			reset_opennebula_server( pptr );
			pptr->when = time((long *) 0);
			osptr = liberate_on_response( osptr );
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

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

		if (!( optr = occi_add_action( optr,"start","",start_opennebula)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"stop","",stop_opennebula)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"save","",save_opennebula)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"softboot","",softboot_opennebula)))
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


