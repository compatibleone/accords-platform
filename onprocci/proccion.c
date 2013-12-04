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
#ifndef	_proccion_c
#define	_proccion_c

#include "onclient.h"
#include "cordslang.h"
#include "occiresolver.h"
#include "cosacsctrl.h"

/* ---------------------------------------------------------------------------- */
/*			x m l _ a t r i b u t _ v a l u e			*/
/* ---------------------------------------------------------------------------- */
private	char *	xml_element_value( struct xml_element * eptr, char * nptr )
{
	struct	xml_element * aptr;
	if (!( aptr = document_element( eptr, nptr ) ))
		return( (char *) 0);
	else	return( aptr->value );
}

/* ---------------------------------------------------------------------------- */
/* 		r e s o l v e _ o n _ c o n f i g u r a t i o n			*/
/* ---------------------------------------------------------------------------- */
private	struct	on_config * resolve_on_configuration( char * sptr )
{
	struct	occi_kind_node * nptr;
	struct	on_config * pptr=(struct on_config *) 0;
	struct	occi_kind_node  * occi_first_on_config_node(char *,char *, char *, int);
	struct	occi_kind_node  * occi_next_on_config_node(struct occi_kind_node  *);
	for (	nptr = occi_first_on_config_node("opennebula_configuration","name",sptr,0);
		nptr != (struct occi_kind_node *) 0;
		nptr = occi_next_on_config_node(nptr) )
	{
		if (!( pptr = nptr->contents ))
			continue;
		else if (!( pptr->name ))
			continue;
		else if (!( strcmp( pptr->name, sptr ) ))
			return( pptr );
	}
	return( (struct on_config *) 0);
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
/*		r e s o l v e _ o n _ a d d r e s s e s			*/
/*	--------------------------------------------------------	*/
private	int	resolve_on_addresses( struct on_response * yptr, struct opennebula * pptr )
{
	char	*	vptr;
	struct	xml_element * eptr;
	struct	xml_element * iptr;
	char	*	firstadr=(char *) 0;
	char	*	nextadr=(char *) 0;

	/* ---------------------------------- */
	/* NB: OpenNebula will attempt to set */
	/* up its gateway on the 1st address  */
	/* so it has to be the public one if  */
	/* a public one is to be used.	      */
	/* ---------------------------------- */
	/* attempt to resolve a first address */
	/* ---------------------------------- */
	if (!( eptr = document_element( yptr->xmlroot, "NIC")))
		return( 0 );
	else if (!( iptr = document_element( eptr, "IP")))
		return( 0 );
	else if (!( firstadr = allocate_string(iptr->value)))
		return( 27 );


	/* ----------------------------------- */
	/* attempt to resolve a second address */
	/* ----------------------------------- */
	if (!( eptr = document_element( eptr->next, "NIC")))
		return( 0 );
	else if (!( iptr = document_element( eptr, "IP")))
		return( 0 );
	else if (!( nextadr  = allocate_string(iptr->value)))
		return( 27 );

	if ( nextadr )
	{
		pptr->privateaddr = nextadr;
		pptr->publicaddr  = firstadr;
	}
	else
	{
		pptr->privateaddr = firstadr;		
		pptr->publicaddr = nextadr;
	}

	if ( check_debug() )
	{
		rest_log_message("*** ON PROCCI Instance PRIVATE IP ***");
		rest_log_message( pptr->publicaddr );
	}

	if ( check_debug() )
	{
		rest_log_message("*** ON PROCCI Instance PUBLIC IP ***");
		rest_log_message( pptr->publicaddr );
	}
	return(0);
}


/*	-------------------------------------------------------------	*/
/* 	l l _ u s e _ o p e n n e b u l a _ c o n f i g u r a t i o n 	*/
/*	-------------------------------------------------------------	*/
private	struct	on_subscription * ll_use_opennebula_configuration( char * sptr )
{
	struct	on_config * pptr;

	if (!( pptr = resolve_on_configuration( sptr )))
	 	return((struct on_subscription *) 0);

	else 	return( on_initialise_client( 
			pptr->user, pptr->password, 
			pptr->host, _CORDS_OS_AGENT, pptr->version, pptr->tls ));
}

/*	--------------------------------------------------------	*/
/* 	u s e _ o p e n n e b u l a _ c o n f i g u r a t i o n 	*/
/*	--------------------------------------------------------	*/
private	struct	on_subscription * use_opennebula_configuration( char * nptr )
{
	struct	on_subscription * sptr;
	if ((sptr = ll_use_opennebula_configuration( nptr )) != (struct on_subscription *) 0)
		return( sptr );
	else if (!( nptr = get_operator_profile()))
		return( sptr );
	else	return( use_opennebula_configuration( nptr ) );
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
		pptr->state = _OCCI_IDLE;
	}
	return(0);
}

/*	--------------------------------------------------------	*/
/* 	     c o n n e c t _ o p e n n e b u l a _ s e r v e r		*/
/*	--------------------------------------------------------	*/
private	int	connect_opennebula_server( struct on_subscription * subptr, struct on_response * rptr,struct opennebula * pptr )
{
	int	pending=60*60;
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

		if (!( vptr = xml_element_value( rptr->xmlroot, "ID") ))
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

		autosave_opennebula_node(pptr);

		/* ----------------------------------------------------- */
		/* we must now await ACTIVE status to be able to collect */
		/* the final identification information to complete the  */
		/* opennebula provisioning request.			 */
		/* ----------------------------------------------------- */
		yptr = rptr;
		zptr = (struct on_response *) 0;
		while (1)
		{
			if (!( vptr = xml_element_value( yptr->xmlroot, "STATE" )))
			{
				reset_opennebula_server( pptr );
				return( 27 );
			}
			if ( check_debug() )
			{
				rest_log_message("*** ON PROCCI Testing Build Status ***");
				rest_log_message( vptr );
			}
			if (!( strcmp( vptr, "PENDING" )))
			{
				if (!( pending ))
					return( 444 );
				else	pending--;
				sleep(1);
				if ( zptr )
					zptr = liberate_on_response( zptr );
				if (!( zptr = on_get_server( subptr, pptr->number )))
				{
					reset_opennebula_server( pptr );
					return( 555 );
				}
				else	yptr = zptr;
			}
			else if (!( strcmp( vptr, "ACTIVE" )))
				break;
			else if (!( strcmp( vptr, "FAILED" )))
				return( 666 );
			else if (!( strcmp( vptr, "DONE" )))
				return( 777 );
		}

		if ( pptr->hostname ) pptr->hostname = liberate( pptr->hostname );
		if ( pptr->publicaddr ) pptr->publicaddr = liberate( pptr->publicaddr );
		if ( pptr->privateaddr ) pptr->privateaddr = liberate( pptr->privateaddr );

		if ( resolve_on_addresses(yptr,pptr ) != 0 )
		{
			reset_opennebula_server( pptr );
			return( 27 );
		}


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
		pptr->stamp = time((long *) 0);
		pptr->state = _OCCI_RUNNING;
		autosave_opennebula_node(pptr);
		return(0);
	}
}

/*	-------------------------------------------------	*/
/*	  o n _ l a u n c h _ u s i n g _ k e y p a i r		*/
/*	-------------------------------------------------	*/
public	int	on_launch_using_keypair( struct opennebula * pptr, char * username, char * command )
{
	if (!( pptr ))
		return( 118 );
	else if (!( command ))
		return( 118 );
	else if (!( pptr->hostname ))
		return( 118 );
	else if (!( pptr->keyfile ))
		return( 118 );
	else	return( ssh_launch_using_keypair( username, pptr->keyfile, pptr->hostname, command ) );
}

/*	-----------------------------------	*/
/*	 o n _ i n s t a l l _ c o s a c s	*/
/*	-----------------------------------	*/
private	int	on_install_cosacs( struct on_subscription * subptr, struct opennebula * pptr )
{
	int	status;
	char *	hostdepot=(char *) 0;
	char *	buffer=(char *) 0;
	char *	syntax=(char *) 0;
	char *	username=(char *) 0;
	char *	version=(char *) 0;
	char *	package=(char *) 0;
	char *	sptr;

	if (!( hostdepot = get_default_depot()))
		return( _NO_COSACS );

	/* ---------------------------------- */
	/* detect cosacs installation request */
	/* ---------------------------------- */
	if (!( sptr = pptr->agent ))
		return( _NO_COSACS );
	else if (( strncasecmp( sptr, "cosacs:install", strlen("cosacs:install") ) != 0 )
	     &&  ( strncasecmp( sptr, "install:cosacs", strlen("install:cosacs") ) != 0 ))
		return( _NO_COSACS );
	else	sptr += strlen( "cosacs:install" );

	if ( *sptr != ':' )
		return( _NO_COSACS );
	else	sptr++;

	/* -------------------------------------- */
	/* extract user, version and package info */
	/* -------------------------------------- */
	if (!( buffer = allocate_string( sptr ) ))
		return( _NO_COSACS );

	username = sptr = strcpy( buffer, sptr );

	while ( *sptr )
	{
		if ( *sptr == ':' )
		{
			*(sptr++) = 0;
			if (!( version ))
				version = sptr;
			else if (!( package ))
				package = sptr;
			else	break;
		}
		else	sptr++;
	}
	if ((!( package)) || (!( version )) || (!( username )))
	{
		liberate( buffer ) ;
		return( _NO_COSACS );
	}

	/* ---------------------------------------- */
	/* build installation command syntax string */
	/* ---------------------------------------- */
	else if (!( syntax = allocate( strlen( hostdepot ) + strlen( version ) + ( strlen( package ) * 2) + 64 ) ))
	{
		liberate( buffer ) ;
		return( _NO_COSACS );
	}
	sprintf(syntax,"wget %s/%s/%s",hostdepot,version,package);
	status = on_launch_using_keypair( pptr, username, syntax );
	sprintf(syntax,"bash ./%s",package);
	status = on_launch_using_keypair( pptr, username, syntax );

	buffer = liberate( buffer );
	syntax = liberate( syntax );

	if ( status )
		return( _NO_COSACS  );
	else	return( _USE_COSACS );
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
	struct	on_subscription * subptr;
	char		* idptr;
	struct	on_response * onptr;
	struct	on_response * metaptr;
	struct	opennebula * pptr;
	int		status;
	char	*	filename;
	char	*	metafilename;
	char		buffer[512];
	char		reference[512];
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->state != _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if (!(subptr = use_opennebula_configuration( pptr->profile )))
		return( rest_html_response( aptr, status, "Configuration Not Found" ) );

	sprintf(reference,"%s/%s/%s",OnProcci.identity,_CORDS_OPENNEBULA,pptr->id);

	if (!( filename = on_create_compute_request(
		subptr, 
		pptr->name, pptr->flavor, pptr->image, pptr->publicnetwork, pptr->privatenetwork, pptr->architecture, pptr->driver ) ))
	 	return( rest_html_response( aptr, 2400, "Bad Request : Create Server Message" ) );
	else if (!( onptr = on_create_compute( subptr, filename )))
	 	return( rest_html_response( aptr, 2401, "Bad Request : Create Server Request" ) );
	else if (!( onptr->response ))
	 	return( rest_html_response( aptr, 2402, "Bad Request : Create Server No Response" ) );
	else if ( onptr->response->status >= 400 )
	{
		aptr = rest_html_response( aptr, onptr->response->status + 2000, "Bad Request : Create Server No Response" );
		onptr = liberate_on_response( onptr );
		return( aptr );
	}
	else
	{
		if (!(status = connect_opennebula_server( subptr,onptr, pptr ) ))
		{
			/* ---------------------------- */
			/* launch the COSACS operations */
			/* ---------------------------- */
			switch ((pptr->agentstatus = use_cosacs_agent( pptr->agent )))
			{
			case	_INSTALL_COSACS	:
				if (!( pptr->agentstatus = on_install_cosacs( subptr, pptr ) ))
					break;
			case	_USE_COSACS	:
				if ( cosacs_test_interface( pptr->hostname, _COSACS_START_TIMEOUT, _COSACS_START_RETRY ) )
				{
					cosacs_metadata_instructions( 
						pptr->hostname, _CORDS_CONFIGURATION,
						reference, default_publisher(), pptr->account );
				}
			}
		}
		onptr = liberate_on_response( onptr );
		if (!( status ))
		{
			if (!( rest_valid_string( pptr->price ) ))
				return( rest_html_response( aptr, 200, "OK" ) );
			else if ( occi_send_transaction( _CORDS_OPENNEBULA, pptr->price, "action=start", pptr->account, reference ) )
				return( rest_html_response( aptr, 200, "OK" ) );
			else	return( rest_html_response( aptr, 200, "OK" ) );
		}
		else  	return( rest_html_response( aptr, 400, "Bad Request : Connect Open Nebula" ) );
	}

}

/*	--------------------------------------------	*/
/*	      o n _ c o n n e c t _ i m a g e		*/
/*	--------------------------------------------	*/
private	struct on_response * on_connect_image( struct on_subscription * subptr, struct opennebula * pptr, struct on_response * onptr )
{
	char *	vptr;
	char * 	wptr=(char *) 0;
	if (!( pptr ))
		return( onptr );
	else if (!( onptr ))
		return( onptr );
	else if (!( onptr->xmlroot ))
		return( onptr );
	else if (!( vptr = xml_element_value( onptr->xmlroot, "SAVE_AS" )))
		return( onptr );
	else 
	{
		while ( *vptr )
			if ( *(vptr++) == '/' )
				wptr = vptr;

		if (!( wptr ))
			return(onptr);
		else
		{
			if ( pptr->image )
				pptr->image = liberate( pptr->image );
			pptr->image = allocate_string( wptr );
			return( onptr );
		}
	}
}

/*	--------------------------------------------	*/
/*	      o n _ s h u t d o w n _ i m a g e		*/
/*	--------------------------------------------	*/
private	int 	on_shutdown_image( struct on_subscription * subptr, struct opennebula * pptr  )
{
	char * filename;
	struct	on_response * rptr;
	char *	vptr;
	if (!( filename = on_shutdown_compute_request( subptr, pptr->number ) ))
		return(118);
	else if (!( rptr = on_shutdown_compute( subptr, pptr->number, filename ) ))
		return(119);
	else
	{
		rptr = liberate_on_response( rptr );
		while (1)
		{
			if (!( rptr = on_get_server( subptr, pptr->number )))
			{
				return(120);
			}
			else if (!( vptr = xml_element_value( rptr->xmlroot, "STATE" )))
			{
				rptr = liberate_on_response( rptr );
				return(121);
			}
			else if (!( strcmp( vptr, "DONE" )))
			{
				sleep(1);
				rptr = liberate_on_response( rptr );
				return(0);
			}
			else if (!( strcmp( vptr, "ACTIVE" )))
			{
				sleep(1);
				rptr = liberate_on_response( rptr );
				continue;
			}
			else if (!( strcmp( vptr, "FAILED" )))
			{
				rptr = liberate_on_response( rptr );
				return( 666 );
			}
		}
	}
}

/*	-------------------------------------------	*/
/*		o n _ i m a g e _ a c c e s s		*/
/*	-------------------------------------------	*/
private	int	on_image_access( struct on_subscription * subptr, char * image, int mode )
{
	struct on_response * onptr;
	char *	filename;
	if ( mode )
	{
		if (!( filename = on_public_image_request( subptr, image ) ))
			return(0);
		else if (!( onptr = on_public_image( subptr, image, filename ) ))
			return( 0 );
		else	
		{
			onptr = liberate_on_response( onptr );
			return(1);
		}
	}
	else
	{
		if (!( filename = on_private_image_request( subptr, image ) ))
			return(0);
		else if (!( onptr = on_private_image( subptr, image, filename ) ))
			return( 0 );
		else	
		{
			onptr = liberate_on_response( onptr );
			return(1);
		}
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
	struct	on_subscription * subptr;
	char		reference[512];
	struct	on_response * onptr;
	int		status;
	struct	opennebula * pptr;
	char	*	filename;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 1401, "Invalid Action" ) );
	else if ( pptr->state == _OCCI_IDLE )
		return( rest_html_response( aptr, 1402, "Contract Not Active" ) );

	else if (!(subptr = use_opennebula_configuration( pptr->profile )))
		return( rest_html_response( aptr, status, "configuration not found" ) );
	else if (!( filename = on_create_image_request( subptr,pptr->number, pptr->image, pptr->name, pptr->driver ) ))
	 	return( rest_html_response( aptr, 1403, "image message failure" ) );
	else if (!( onptr = on_create_image( subptr, pptr->number, filename ) ))
	 	return( rest_html_response( aptr, 1404, "create image failure" ) );
	else if (!( onptr->response ))
	 	return( rest_html_response( aptr, 2402, "create image no response" ) );
	else if ( onptr->response->status >= 400 )
	{
		aptr = rest_html_response( aptr, onptr->response->status + 1000, "create image request" );
		onptr = liberate_on_response( onptr );
		return( aptr );
	}
	else if (!( onptr = on_connect_image( subptr, pptr, onptr ) ))
	 	return( rest_html_response( aptr, 1405, "connect image failure" ) );
	else if ((status = on_shutdown_image( subptr, pptr )) != 0)
	 	return( rest_html_response( aptr, 1400 + status, "save shutdown failure" ) );
	else
	{
		/* ----------------------------- */
		/* save must update the original */
		/* to ensure image preservation  */
		/* ----------------------------- */
		if ( pptr->image )
		{ 
			if ( pptr->original ) 
				pptr->original = liberate( pptr->original );

			pptr->original = allocate_string( pptr->image );

			/* -------------------------------------------- */
			/* its a save operation and MUST be made public */
			/* -------------------------------------------- */
			on_image_access( subptr, pptr->image, 1 );
		}
		onptr = liberate_on_response( onptr );
		sprintf(reference,"%s/%s/%s",OnProcci.identity,_CORDS_OPENNEBULA,pptr->id);
		autosave_opennebula_node(pptr);
		if (!( rest_valid_string( pptr->price ) ))
			return( rest_html_response( aptr, 200, "OK" ) );
		else if ( occi_send_transaction( _CORDS_OPENNEBULA, pptr->price, "action=save", pptr->account, reference ) )
			return( rest_html_response( aptr, 200, "OK" ) );
		else	return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	      s a v e  _ o p e n n e b u l a	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * snapshot_opennebula(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	on_subscription * subptr;
	char		reference[512];
	struct	on_response * onptr;
	int		status;
	struct	opennebula * pptr;
	char	*	filename;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 1401, "Invalid Action" ) );
	else if ( pptr->state == _OCCI_IDLE )
		return( rest_html_response( aptr, 1402, "Contract Not Active" ) );

	else if (( subptr = use_opennebula_configuration( pptr->profile )))
		return( rest_html_response( aptr, status, "configuration not found" ) );
	else if (!( filename = on_create_image_request(subptr,  pptr->number, pptr->image, pptr->id, pptr->driver ) ))
	 	return( rest_html_response( aptr, 1403, "image request failure" ) );
	else if (!( onptr = on_create_image( subptr, pptr->number, filename ) ))
	 	return( rest_html_response( aptr, 1404, "create image failure" ) );
	else if (!( onptr->response ))
	 	return( rest_html_response( aptr, 2402, "create image no response" ) );
	else if ( onptr->response->status >= 400 )
	{
		aptr = rest_html_response( aptr, onptr->response->status + 1000, "create image request" );
		onptr = liberate_on_response( onptr );
		return( aptr );
	}
	else if (!( onptr = on_connect_image( subptr, pptr, onptr ) ))
	 	return( rest_html_response( aptr, 1405, "connect image failure" ) );
	else if ((status = on_shutdown_image( subptr, pptr )) != 0)
	 	return( rest_html_response( aptr, 1400+status, "snapshot shutdown failure" ) );
	{
		onptr = liberate_on_response( onptr );
		autosave_opennebula_node(pptr);
		sprintf(reference,"%s/%s/%s",OnProcci.identity,_CORDS_OPENNEBULA,pptr->id);
		if (!( rest_valid_string( pptr->price ) ))
			return( rest_html_response( aptr, 200, "OK" ) );
		else if ( occi_send_transaction( _CORDS_OPENNEBULA, pptr->price, "action=save", pptr->account, reference ) )
			return( rest_html_response( aptr, 200, "OK" ) );
		else	return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	--------------------------------------------------------	*/
/* 	s t o p  _ o p e n n e b u l a _ p r o v i s i o n i n g  	*/
/*	--------------------------------------------------------	*/
private	struct on_response * stop_opennebula_provisioning( struct opennebula * pptr )
{
	struct	on_subscription * subptr;
	int	status;
	struct	on_response * onptr;
	char		reference[512];
	if (!(subptr = use_opennebula_configuration( pptr->profile )))
		return((struct on_response *) 0);
	else
	{
		sprintf(reference,"%s/%s/%s",OnProcci.identity,_CORDS_OPENNEBULA,pptr->id);
		if ( pptr->agentstatus == _USE_COSACS )
		{
			if ( cosacs_test_interface( pptr->hostname, _COSACS_STOP_TIMEOUT, _COSACS_STOP_RETRY ) )
			{
				cosacs_metadata_instructions( 
					pptr->hostname, 
					_CORDS_RELEASE,
					reference, 
					default_publisher(),
					pptr->account );
			}
		}
		return( on_delete_server( subptr, pptr->number ) );
	}
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
	struct	on_subscription * subptr;
	char		reference[512];
	struct	on_response * onptr;
	int		status;
	struct	opennebula * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->state == _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if (!(onptr = stop_opennebula_provisioning( pptr )))
		return( rest_html_response( aptr, status, "Not Found" ) );
	else
	{
		reset_opennebula_server( pptr );
		pptr->stamp = time((long *) 0);
		onptr = liberate_on_response( onptr );
		autosave_opennebula_node(pptr);
		sprintf(reference,"%s/%s/%s",OnProcci.identity,_CORDS_OPENNEBULA,pptr->id);
		if (!( rest_valid_string( pptr->price ) ))
			return( rest_html_response( aptr, 200, "OK" ) );
		else if ( occi_send_transaction( _CORDS_OPENNEBULA, pptr->price, "action=stop", pptr->account, reference ) )
			return( rest_html_response( aptr, 200, "OK" ) );
		else	return( rest_html_response( aptr, 200, "OK" ) );
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
	struct	on_subscription * subptr;
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
	struct	on_subscription * subptr;
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
	struct	on_subscription * subptr;
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
	struct	on_subscription * subptr;
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
	struct	on_subscription * subptr;
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
	struct	on_subscription * subptr;
	struct	opennebula * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

#include "oncontract.c"

/*	-------------------------------------------	*/
/* 	      c r e a t e _ o p e n n e b u l a  	*/
/*	-------------------------------------------	*/
private	int	create_opennebula(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	opennebula * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( pptr->node ))
		return( 0 ); 
	else	return(create_opennebula_contract(optr, pptr, _CORDS_CONTRACT_AGENT, OnProcci.tls));
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ o p e n n e b u l a  	*/
/*	-------------------------------------------	*/
private	int	retrieve_opennebula(struct occi_category * optr, void * vptr,struct rest_request * rptr)
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
private	int	update_opennebula(struct occi_category * optr, void * vptr,struct rest_request * rptr)
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
private	int	delete_opennebula(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	opennebula * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(delete_opennebula_contract(optr, pptr, _CORDS_CONTRACT_AGENT, OnProcci.tls));
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
		optr->access |= _OCCI_PROVIDER;

		if (!( optr = occi_add_action( optr,_CORDS_START,"",start_opennebula)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_STOP,"",stop_opennebula)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_SAVE,"",save_opennebula)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_SNAPSHOT,"",snapshot_opennebula)))
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


