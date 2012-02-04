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
#ifndef	_proccios_c
#define	_proccios_c

#include "azclient.h"
#include "cordslang.h"

/* ---------------------------------------------------------------------------------- */
/* if ((status = az_initialise_client( user, pass, host, agent, version, tls )) != 0) */
/* ---------------------------------------------------------------------------------- */
private	struct	az_config * resolve_az_configuration( char * sptr )
{
	struct	occi_kind_node * nptr;
	struct	az_config * pptr=(struct az_config *) 0;
	struct	occi_kind_node  * occi_first_az_config_node();
	for (	nptr = occi_first_az_config_node();
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
/* 	 u s e _ w i n d o w s a z u r e _ c o n f i g u r a t i o n 		*/
/*	--------------------------------------------------------	*/
private	int	use_windowsazure_configuration( char * sptr )
{
	struct	az_config * pptr;

	if (!( pptr = resolve_az_configuration( sptr )))
	 	return( 404 );

	else 	return( az_initialise_client( 
			pptr->user, pptr->password, 
			pptr->host, pptr->agent, pptr->version, pptr->tls,
			pptr->namespace, pptr->subscription ));
}

/*	--------------------------------------------------------	*/
/* 	       r e s e t _ w i n d o w s a z u r e _ s e r v e r		*/
/*	--------------------------------------------------------	*/
private	int	reset_windowsazure_server( struct windowsazure * pptr )
{
	if ( pptr )
	{
		if ( pptr->number ) pptr->number = liberate( pptr->number );
		if ( pptr->hostname ) pptr->hostname = liberate( pptr->hostname );
		if ( pptr->reference ) pptr->reference = liberate( pptr->reference );
		if ( pptr->rootpass ) pptr->rootpass = liberate( pptr->rootpass );
		if ( pptr->publicaddr ) pptr->publicaddr = liberate( pptr->publicaddr );
		if ( pptr->privateaddr ) pptr->privateaddr = liberate( pptr->privateaddr );
		pptr->number = allocate_string("");
		pptr->hostname = allocate_string("");
		pptr->rootpass  = allocate_string("");
		pptr->publicaddr = allocate_string("");
		pptr->privateaddr = allocate_string("");
		pptr->status = _OCCI_IDLE;
	}
	return(0);
}

/*	--------------------------------------------------------	*/
/* 	     c o n n e c t _ w i n d o w s a z u r e _ i m a g e  		*/
/*	--------------------------------------------------------	*/
private	int	connect_windowsazure_image( struct az_response * rptr,struct windowsazure * pptr )
{
	struct	az_response * zptr;
	struct	az_response * yptr;
	char *	vptr;
	if (!( pptr ))
		return( 118 );
	else if (!( vptr = json_atribut( rptr->jsonroot, "id") ))
	{
		reset_windowsazure_server( pptr );
		return( 27 );
	}
	else
	{
		if ( pptr->image )
			pptr->image = liberate( pptr->image );
		if (!( pptr->image = allocate_string( vptr ) ))
		{
			reset_windowsazure_server( pptr );
			return( 27 );
		}
		autosave_windowsazure_nodes();
		/* ----------------------------------------------------- */
		/* we must now await ACTIVE status to be able to collect */
		/* the final identification information to complete the  */
		/* windowsazure provisioning request.			 */
		/* ----------------------------------------------------- */
		yptr = rptr;
		zptr = (struct az_response *) 0;
		while (1)
		{
			if (!( vptr = json_atribut( yptr->jsonroot, "status" )))
			{
				reset_windowsazure_server( pptr );
				return( 27 );
			}
			else if (!( strcmp( vptr, "SAVING" )))
			{
				sleep(1);
				if ( zptr )
					zptr = liberate_az_response( zptr );
				if (!( zptr = az_get_image( pptr->image )))
				{
					reset_windowsazure_server( pptr );
					return( 555 );
				}
				else	yptr = zptr;
			}
			else if (!( strcmp( vptr, "ACTIVE" )))
				break;
		}
		return( 0 );
	}
}

/*	--------------------------------------------------------	*/
/* 	     c o n n e c t _ w i n d o w s a z u r e _ s e r v e r		*/
/*	--------------------------------------------------------	*/
private	int	connect_windowsazure_server( struct az_response * rptr,struct windowsazure * pptr )
{
	struct	az_response * zptr;
	struct	az_response * yptr;
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
		if ( pptr->number ) 
			pptr->number = liberate( pptr->number );

		if ( pptr->rootpass ) 
			pptr->rootpass = liberate( pptr->rootpass );

		if (!( vptr = json_atribut( rptr->jsonroot, "id") ))
		{
			reset_windowsazure_server( pptr );
			return( 27 );
		}
		else if (!( pptr->number = allocate_string(vptr)))
		{
			reset_windowsazure_server( pptr );
			return( 27 );
		}
		else if (!( vptr = json_atribut( rptr->jsonroot, "adminPass") ))
		{
			reset_windowsazure_server( pptr );
			return( 27 );
		}
		else if (!( pptr->rootpass  = allocate_string(vptr)))
		{
			reset_windowsazure_server( pptr );
			return( 27 );
		}
		autosave_windowsazure_nodes();
		/* ----------------------------------------------------- */
		/* we must now await ACTIVE status to be able to collect */
		/* the final identification information to complete the  */
		/* windowsazure provisioning request.			 */
		/* ----------------------------------------------------- */
		yptr = rptr;
		zptr = (struct az_response *) 0;
		while (1)
		{
			if (!( vptr = json_atribut( yptr->jsonroot, "status" )))
			{
				reset_windowsazure_server( pptr );
				return( 27 );
			}
			else if (!( strcmp( vptr, "BUILD" )))
			{
				sleep(1);
				if ( zptr )
					zptr = liberate_az_response( zptr );
				if (!( zptr = az_get_server( pptr->number )))
				{
					reset_windowsazure_server( pptr );
					return( 555 );
				}
				else	yptr = zptr;
			}
			else if (!( strcmp( vptr, "ACTIVE" )))
				break;
		}
		if ( pptr->hostname ) pptr->hostname = liberate( pptr->hostname );
		if ( pptr->reference ) pptr->reference = liberate( pptr->reference );
		if ( pptr->publicaddr ) pptr->publicaddr = liberate( pptr->publicaddr );
		if ( pptr->privateaddr ) pptr->privateaddr = liberate( pptr->privateaddr );
		if (!( vptr = json_atribut( yptr->jsonroot, "hostId") ))
		{
			reset_windowsazure_server( pptr );
			return( 27 );
		}
		else if (!( pptr->reference = allocate_string(vptr)))
		{
			reset_windowsazure_server( pptr );
			return( 27 );
		}
		if (( vptr = json_atribut( yptr->jsonroot, "private")) != (char *) 0)
		{
			if (!( pptr->privateaddr  = allocate_string(vptr)))
			{
				reset_windowsazure_server( pptr );
				return( 27 );
			}
		}
		if (( vptr = json_atribut( yptr->jsonroot, "public")) != (char *) 0)
		{
			if (!( pptr->publicaddr  = allocate_string(vptr)))
			{
				reset_windowsazure_server( pptr );
				return( 27 );
			}
		}
		if (( pptr->publicaddr ) && ( strlen( pptr->publicaddr ) != 0))
		{
			if (!( pptr->hostname = allocate_string( pptr->publicaddr ) ))
			{
				reset_windowsazure_server( pptr );
				return( 27 );
			}
		}
		else if (( pptr->privateaddr ) && ( strlen( pptr->privateaddr ) != 0))
		{
			if (!( pptr->hostname = allocate_string( pptr->privateaddr ) ))
			{
				reset_windowsazure_server( pptr );
				return( 27 );
			}
		}
		pptr->when = time((long *) 0);
		pptr->status = _OCCI_RUNNING;
		autosave_windowsazure_nodes();
		return(0);
	}
}

/*	-------------------------------------------	*/
/* 	      s t a r t  _ w i n d o w s a z u r e	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * start_windowsazure(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	az_response * osptr;
	struct	windowsazure * pptr;
	int		status;
	char	*	filename;
	char 	*	personality="";
	char 	*	resource=_CORDS_LAUNCH_CFG;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->status != _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if ((status = use_windowsazure_configuration( pptr->profile )) != 0)
		return( rest_html_response( aptr, status, "Not Found" ) );
	else if (!( filename = az_create_server_request( 
		pptr->id, pptr->image, pptr->flavor, personality, resource ) ))
	 	return( rest_html_response( aptr, 400, "Bad Request" ) );
	else if (!( osptr = az_create_server( filename )))
	 	return( rest_html_response( aptr, 400, "Bad Request" ) );
	else
	{
		/* --------------------------------- */
		/* retrieve crucial data from server */
		/* --------------------------------- */
		status = connect_windowsazure_server( osptr, pptr );
		osptr = liberate_az_response( osptr );
		if (!( status ))
			return( rest_html_response( aptr, 200, "OK" ) );
		else  	return( rest_html_response( aptr, 400, "Bad Request" ) );
	}

}

/*	-------------------------------------------	*/
/* 	      s a v e  _ w i n d o w s a z u r e	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * save_windowsazure(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	az_response * osptr;
	int		status;
	struct	windowsazure * pptr;
	char	*	filename;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->status == _OCCI_IDLE )
		return( rest_html_response( aptr, 400, "Contract Not Active" ) );
	else if ((status = use_windowsazure_configuration( pptr->profile )) != 0)
		return( rest_html_response( aptr, status, "Not Found" ) );
	else if (!( filename = az_create_image_request( pptr->name, pptr->number ) ))
	 	return( rest_html_response( aptr, 400, "Bad Request" ) );
	else if (!( osptr = az_create_image( filename ) ))
	 	return( rest_html_response( aptr, 400, "Bad Request" ) );
	else
	{
		/* --------------------------------- */
		/* retrieve crucial data from server */
		/* --------------------------------- */
		status = connect_windowsazure_image( osptr, pptr );
		osptr = liberate_az_response( osptr );
		if (!( status ))
			return( rest_html_response( aptr, 200, "OK" ) );
		else  	return( rest_html_response( aptr, 400, "Bad Request" ) );
	}
}

/*	-------------------------------------------	*/
/* 	      s t o p  _ w i n d o w s a z u r e	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * stop_windowsazure(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	az_response * osptr;
	int		status;
	struct	windowsazure * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->status == _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if ((status = use_windowsazure_configuration( pptr->profile )) != 0)
		return( rest_html_response( aptr, status, "Not Found" ) );
	else if (!( osptr = az_delete_server( pptr->number )))
	 	return( rest_html_response( aptr, 400, "Bad Request" ) );
	else
	{
		if ( pptr->status != _OCCI_IDLE )
		{
			reset_windowsazure_server( pptr );
			pptr->when = time((long *) 0);
			osptr = liberate_az_response( osptr );
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	      r e s t a r t  _ w i n d o w s a z u r e	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * restart_windowsazure(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	int	status;
	struct	windowsazure * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ((status = use_windowsazure_configuration( pptr->profile )) != 0)
		return( rest_html_response( aptr, status, "Not Found" ) );
	else
	{
		if ( pptr->status == _OCCI_SUSPENDED )
		{
			pptr->when = time((long *) 0);
			pptr->status = _OCCI_RUNNING;
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	     s u s p e n d  _ w i n d o w s a z u r e	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * suspend_windowsazure(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	int	status;
	struct	windowsazure * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ((status = use_windowsazure_configuration( pptr->profile )) != 0)
		return( rest_html_response( aptr, status, "Not Found" ) );
	{
		if ( pptr->status == _OCCI_RUNNING )
		{
			pptr->when = time((long *) 0);
			pptr->status = _OCCI_SUSPENDED;
		}
		return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	   s o f t b o o t _ w i n d o w s a z u r e  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * softboot_windowsazure(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	windowsazure * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	   h a r d b o o t _ w i n d o w s a z u r e  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * hardboot_windowsazure(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	windowsazure * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	     r e b u i l d_ w i n d o w s a z u r e  		*/
/*	-------------------------------------------	*/
private	struct	rest_response * rebuild_windowsazure(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	windowsazure * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	      r e s i z e _ w i n d o w s a z u r e  		*/
/*	-------------------------------------------	*/
private	struct	rest_response * resize_windowsazure(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	windowsazure * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	    c o n f i r m _ w i n d o w s a z u r e  		*/
/*	-------------------------------------------	*/
private	struct	rest_response * confirm_windowsazure(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	windowsazure * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	      r e v e r t _ w i n d o w s a z u r e  		*/
/*	-------------------------------------------	*/
private	struct	rest_response * revert_windowsazure(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	windowsazure * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}


/*	-------------------------------------------	*/
/* 	      c r e a t e _ w i n d o w s a z u r e  		*/
/*	-------------------------------------------	*/
private	int	create_windowsazure(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	windowsazure * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ w i n d o w s a z u r e  	*/
/*	-------------------------------------------	*/
private	int	retrieve_windowsazure(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	windowsazure * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ w i n d o w s a z u r e 	 	*/
/*	-------------------------------------------	*/
private	int	update_windowsazure(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	windowsazure * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ w i n d o w s a z u r e  		*/
/*	-------------------------------------------	*/
private	int	delete_windowsazure(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	windowsazure * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

private	struct	occi_interface	windowsazure_interface = {
	create_windowsazure,
	retrieve_windowsazure,
	update_windowsazure,
	delete_windowsazure
	};

/*	-------------------------------------------	*/
/* 	       b u i l d _ w i n d o w s a z u r e  		*/
/*	-------------------------------------------	*/
/*	this function is to be called to build the	*/
/*	complete windowsazure occi category to offer	*/
/*	an windowsazure compute provisioning service	*/
/*	through a compatible one published occi		*/
/*	service interface.				*/
/*	-------------------------------------------	*/
public	struct	occi_category * build_windowsazure( char * domain )
{
	struct	occi_category * optr;
	if (!( optr = occi_cords_windowsazure_builder( domain,"windowsazure" ) ))
		return( optr );
	else
	{
		optr->callback  = &windowsazure_interface;
		optr->access |= _OCCI_PROVIDER;

		if (!( optr = occi_add_action( optr,_CORDS_START,"",start_windowsazure)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_SAVE,"",save_windowsazure)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_STOP,"",stop_windowsazure)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_SUSPEND,"",suspend_windowsazure)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_RESTART,"",restart_windowsazure)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"softboot","",softboot_windowsazure)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"hardboot","",hardboot_windowsazure)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"rebuild","",rebuild_windowsazure)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"resize","",resize_windowsazure)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"confirm","",confirm_windowsazure)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"revert","",revert_windowsazure)))
			return( optr );
		else	return( optr );
	}
}

/*	-------------------------------------------	*/
/*	 s e t _ d e f a u l t _ w i n d o w s a z u r e	*/
/*	-------------------------------------------	*/
private	int	set_default_windowsazure(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	az_config * pptr;
	struct	az_config * wptr;
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
			if ((wptr = nptr->contents) != (struct az_config*) 0)
				wptr->current = 0,
			nptr = nptr->next;
		}
		pptr->current = 1;
		return(0);
	}
}

/*	---------------------------------------------------------	*/
/* 	b u i l d _ w i n d o w s a z u r e _ c o n f i g u r a t i o n	*/
/*	---------------------------------------------------------	*/
/*	this category handles the configuration of the interface	*/
/*	to the oepn stack server for the preceeding category of		*/
/*	provisioning instance requests.					*/
/*	---------------------------------------------------------	*/
public	struct	occi_category * build_windowsazure_configuration( char * domain )
{
	struct	occi_category * optr;
	if (!( optr = occi_cords_azconfig_builder( domain, "windowsazure_configuration" ) ))
		return( optr );
	else if (!( optr = occi_add_action( optr,"current","",set_default_windowsazure)))
		return( optr );
	else	return( optr );
}

#endif	/* _procciaz_c */
	/* ----------- */



