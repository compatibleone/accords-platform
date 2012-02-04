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

#include "osclient.h"
#include "cordslang.h"
#include "occiresolver.h"
#include "cosacsctrl.h"
#include "cosacsctrl.c"

/*	------------------------------------------	*/
/*		o s _ v a l i d _ p r i c e		*/
/*	------------------------------------------	*/
private	int	os_valid_price( char * price )
{
	if (!( price ))
		return(0);
	else if (!( strlen( price )))
		return( 0 );
	else if (!( strcmp( price, _CORDS_NULL ) ))
		return( 0 );
	else	return( 1 );
}	

/* ---------------------------------------------------------------------------- */
/* 		r e s o l v e _ o s _ c o n f i g u r a t i o n			*/
/* ---------------------------------------------------------------------------- */
private	struct	os_config * resolve_os_configuration( char * sptr )
{
	struct	occi_kind_node * nptr;
	struct	os_config * pptr=(struct os_config *) 0;
	struct	occi_kind_node  * occi_first_os_config_node();
	for (	nptr = occi_first_os_config_node();
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
/* 	   r e s o l v e _ o p e n s t a c k _ v e r s i o n 		*/
/*	--------------------------------------------------------	*/
private	char *	resolve_openstack_version( char * sptr )
{
	struct	os_config * pptr;

	if (!( pptr = resolve_os_configuration( sptr )))
	 	return( "" );
	else	return( ( pptr->version ? pptr->version : "" ) );
}

/*	-----------------------------------------------------------	*/
/* 		o p e n s t a c k _ i n s t r u c t i o n s		*/
/*	-----------------------------------------------------------	*/
/*	Collect the instruction values which apply to this provider	*/
/*	contract. These values will be the result of configuration	*/
/*	actions complied to instructions during instance creation	*/
/*	and the values will have been collected and prepared by the 	*/
/*	from the corresponding provisioned contracts.			*/
/*	The values collected will be used to build the META DATA to	*/
/*	be made available as the provisioned instance personality	*/
/*	-----------------------------------------------------------	*/
private	char *	openstack_instructions( char * contract, char * result, char * nature )
{

	char	*	ihost;
	char 	*	vptr;

	struct	occi_response * zptr=(struct occi_response *) 0;
	struct	occi_response * zzptr=(struct occi_response *) 0;
	struct	occi_response * yptr=(struct occi_response *) 0;
	struct	occi_element  * fptr=(struct occi_element  *) 0;
	struct	occi_element  * gptr=(struct occi_element  *) 0;
	struct	occi_element  * jptr=(struct occi_element  *) 0;
	struct	occi_element  * eptr=(struct occi_element  *) 0;
	struct	occi_client   * kptr=(struct occi_client   *) 0;
	struct	occi_request  * qptr=(struct occi_request  *) 0;
	char	buffer[4096];
	char	tempname[4096];
	int	length=0;

	/* ------------------------------------------------------------------------- */
	/* select / resolve the instruction category service provider identification */
	/* ------------------------------------------------------------------------- */
	if (!( ihost = occi_resolve_category_provider( _CORDS_INSTRUCTION, _CORDS_CONTRACT_AGENT, OsProcci.tls ) ))
	 	return( result );

	sprintf(buffer,"%s/%s/",ihost,_CORDS_INSTRUCTION);
	liberate( ihost );
	length = strlen(buffer);

	/* --------------------------------------------------------------------------- */
	/* retrieve the collection of instruction category instances for this contract */
	/* --------------------------------------------------------------------------- */
	if (!( kptr = occi_create_client( buffer, _CORDS_CONTRACT_AGENT, OsProcci.tls ) ))
		return( result );

	else if (!(qptr = occi_create_request( 
			kptr, 
			kptr->target->object, 
			_OCCI_NORMAL )))
		return(result);
	else if (!( fptr = occi_request_element( 
			qptr,
			"occi.instruction.provision", contract ) ))
	{
		qptr = occi_remove_request( qptr );
		return( result );
	}
	else if (!( yptr = occi_client_get( kptr, qptr ) ))
	{
		qptr = occi_remove_request( qptr );
		return( result );
	}

	qptr = occi_remove_request ( qptr );

	/* -------------------------------------------------- */
	/* for each instruction category instance in the list */
	/* -------------------------------------------------- */
	for (	eptr = yptr->first;
		eptr != (struct occi_element*) 0;
		eptr = eptr->next )
	{
		if (!( eptr->name ))
			continue;
		else if (!( eptr->value ))
			continue;
		else if (!( vptr = allocate_string( eptr->value )))
			continue;
		else if (!( vptr = occi_category_id( vptr ) ))
			continue;
		else
		{
			strcat( buffer, vptr );
			liberate( vptr );
		}

		/* -------------------------------------------------- */
		/* retrieve the current instruction category instance */
		/* -------------------------------------------------- */
		if (( zptr = occi_simple_get( buffer, _CORDS_CONTRACT_AGENT, OsProcci.tls )) != (struct occi_response *) 0)
		{
			/* ensure its a configuration method */
			/* --------------------------------- */
			if (!(fptr = occi_locate_element( zptr->first, "occi.instruction.nature" )))
				zptr = occi_remove_response ( zptr );
			else if ( strcasecmp( fptr->value, nature ) )
				zptr = occi_remove_response ( zptr );
			else if (!(fptr = occi_locate_element( zptr->first, "occi.instruction.type" )))
				zptr = occi_remove_response ( zptr );
			else if ( strcasecmp( fptr->value, "method" ) )
				zptr = occi_remove_response ( zptr );
			else if (!(fptr = occi_locate_element( zptr->first, "occi.instruction.method" )))
				zptr = occi_remove_response ( zptr );
			else if ( strcasecmp( fptr->value, "configure" ) )
				zptr = occi_remove_response ( zptr );

			/* collect the configuration details */
			/* --------------------------------- */
			else if (!(fptr = occi_locate_element( zptr->first, "occi.instruction.symbol" )))
				zptr = occi_remove_response ( zptr );
			else if (!(gptr = occi_locate_element( zptr->first, "occi.instruction.property" )))
				zptr = occi_remove_response ( zptr );
			else if (!(jptr = occi_locate_element( zptr->first, "occi.instruction.value" )))
				zptr = occi_remove_response ( zptr );
			else
			{
				/* ------------------------------------------------------- */
				/* create the corresponding META DATA type NAME=VALUE pair */
				/* ------------------------------------------------------- */
				sprintf(tempname,"%s_%s=%s\n",fptr->value,gptr->value,jptr->value);
				zzptr = occi_remove_response ( zzptr );
				zptr = occi_remove_response ( zptr );
				if (!( vptr = allocate( strlen( result ) + strlen( tempname ) + 8 ) ))
					break;
				else	
				{
					sprintf(vptr, "%s%s",result,tempname);
					liberate( result );
					result = vptr;
				}
			}
		}

		/* ----------------------- */
		/* quick reset of base url */
		/* ----------------------- */
		buffer[length] = 0;
	}

	yptr = occi_remove_response ( yptr );

	return(result);
}



/*	--------------------------------------------------------	*/
/* 	 u s e _ o p e n s t a c k _ c o n f i g u r a t i o n 		*/
/*	--------------------------------------------------------	*/
private	int	use_openstack_configuration( char * sptr )
{
	struct	os_config * pptr;

	if (!( pptr = resolve_os_configuration( sptr )))
	 	return( 404 );

	else 	return( os_initialise_client( 
			pptr->user, pptr->password, 
			pptr->host, _CORDS_OS_AGENT, pptr->version, pptr->tls ));
}

/*	--------------------------------------------------------	*/
/* 	       r e s e t _ o p e n s t a c k _ s e r v e r		*/
/*	--------------------------------------------------------	*/
private	int	reset_openstack_server( struct openstack * pptr )
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
/* 	     c o n n e c t _ o p e n s t a c k _ i m a g e  		*/
/*	--------------------------------------------------------	*/
private	int	connect_openstack_image( struct os_response * rptr,struct openstack * pptr )
{
	struct	os_response * zptr;
	struct	os_response * yptr;
	char *	vptr;
	if (!( pptr ))
		return( 118 );
	else if (!( vptr = json_atribut( rptr->jsonroot, "id") ))
	{
		reset_openstack_server( pptr );
		return( 27 );
	}
	else
	{
		if ( pptr->image )
			pptr->image = liberate( pptr->image );
		if (!( pptr->image = allocate_string( vptr ) ))
		{
			reset_openstack_server( pptr );
			return( 27 );
		}
		autosave_openstack_nodes();
		/* ----------------------------------------------------- */
		/* we must now await ACTIVE status to be able to collect */
		/* the final identification information to complete the  */
		/* openstack provisioning request.			 */
		/* ----------------------------------------------------- */
		yptr = rptr;
		zptr = (struct os_response *) 0;
		while (1)
		{
			if (!( vptr = json_atribut( yptr->jsonroot, "status" )))
			{
				reset_openstack_server( pptr );
				return( 27 );
			}
			else if (!( strcmp( vptr, "SAVING" )))
			{
				sleep(1);
				if ( zptr )
					zptr = liberate_os_response( zptr );
				if (!( zptr = os_get_image( pptr->image )))
				{
					reset_openstack_server( pptr );
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
/*	    r e s o l v e _ o s _ v 1 0 _ a d d r e s s e s		*/
/*	--------------------------------------------------------	*/
private	int	resolve_os_v10_addresses( struct os_response * yptr, struct openstack * pptr )
{
	char *	vptr;
	if (( vptr = json_atribut( yptr->jsonroot, "private")) != (char *) 0)
	{
		if (!( pptr->privateaddr  = allocate_string(vptr)))
		{
			reset_openstack_server( pptr );
			return( 27 );
		}
		if ( check_debug() )
		{
			rest_log_message("*** OS PROCCI Instance PRIVATE IP ***");
			rest_log_message( pptr->privateaddr );
		}
	}

	if (( vptr = json_atribut( yptr->jsonroot, "public")) != (char *) 0)
	{
		if (!( pptr->publicaddr  = allocate_string(vptr)))
		{
			reset_openstack_server( pptr );
			return( 27 );
		}
		if ( check_debug() )
		{
			rest_log_message("*** OS PROCCI Instance PUBLIC IP ***");
			rest_log_message( pptr->publicaddr );
		}
	}
	return(0);
}

/*	--------------------------------------------------------	*/
/*	    r e s o l v e _ o s _ v 1 1 _ a d d r e s s e s		*/
/*	--------------------------------------------------------	*/
private	int	resolve_os_v11_addresses( struct os_response * yptr, struct openstack * pptr )
{
	struct	data_element * eptr;
	struct	data_element * aptr;
	char *	vptr;
	int	addresses=0;
	if (!( aptr = json_element( yptr->jsonroot, "addresses" )))
		return(0);

	else 
	{
		if (( eptr = json_element( aptr, "private" )) != (struct data_element *) 0)
		{
			if (( vptr = json_atribut( eptr, "addr" )) != (char *) 0)
			{
				if (!( pptr->privateaddr  = allocate_string(vptr)))
				{
					reset_openstack_server( pptr );
					return( 27 );
				}
				if ( check_debug() )
				{
					rest_log_message("*** OS PROCCI Instance PRIVATE IP ***");
					rest_log_message( pptr->privateaddr );
				}
				addresses |= 1;
			}
		}

		if (( eptr = json_element( aptr, "public" )) != (struct data_element *) 0)
		{
			if (( vptr = json_atribut( eptr, "addr" )) != (char *) 0)
			{
				if (!( pptr->publicaddr  = allocate_string(vptr)))
				{
					reset_openstack_server( pptr );
					return( 27 );
				}
				if ( check_debug() )
				{
					rest_log_message("*** OS PROCCI Instance PUBLIC IP ***");
					rest_log_message( pptr->publicaddr );
				}
				addresses |= 2;
			}
		}
		if (!( addresses ))
		{
			if (( vptr = json_atribut( aptr, "addr" )) != (char *) 0)
			{
				if (!( pptr->publicaddr  = allocate_string(vptr)))
				{
					reset_openstack_server( pptr );
					return( 27 );
				}
				if ( check_debug() )
				{
					rest_log_message("*** OS PROCCI Instance PUBLIC IP ***");
					rest_log_message( pptr->publicaddr );
				}
				addresses |= 2;
			}
		}

	}
	return(0);
}

/*	--------------------------------------------------------	*/
/* 	     c o n n e c t _ o p e n s t a c k _ s e r v e r		*/
/*	--------------------------------------------------------	*/
private	int	connect_openstack_server( struct os_response * rptr,struct openstack * pptr )
{
	char *	version;
	struct	os_response * zptr;
	struct	os_response * yptr;
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
			rest_log_message("*** OS PROCCI connect_openstack_server( entry ) ***");
		}

		if ( pptr->number ) 
			pptr->number = liberate( pptr->number );

		if ( pptr->rootpass ) 
			pptr->rootpass = liberate( pptr->rootpass );

		if (!( vptr = json_atribut( rptr->jsonroot, "id") ))
		{
			reset_openstack_server( pptr );
			return( 27 );
		}
		else if (!( pptr->number = allocate_string(vptr)))
		{
			reset_openstack_server( pptr );
			return( 27 );
		}
		if ( check_debug() )
		{
			rest_log_message("*** OS PROCCI Instance ID ***");
			rest_log_message( pptr->number );
		}
		if (!( vptr = json_atribut( rptr->jsonroot, "adminPass") ))
		{
			reset_openstack_server( pptr );
			return( 27 );
		}
		else if (!( pptr->rootpass  = allocate_string(vptr)))
		{
			reset_openstack_server( pptr );
			return( 27 );
		}

		if ( check_debug() )
		{
			rest_log_message("*** OS PROCCI Instance ADMIN PASS ***");
			rest_log_message( pptr->rootpass );
		}

		autosave_openstack_nodes();

		/* ----------------------------------------------------- */
		/* we must now await ACTIVE status to be able to collect */
		/* the final identification information to complete the  */
		/* openstack provisioning request.			 */
		/* ----------------------------------------------------- */
		yptr = rptr;
		zptr = (struct os_response *) 0;
		while (1)
		{
			if (!( vptr = json_atribut( yptr->jsonroot, "status" )))
			{
				reset_openstack_server( pptr );
				return( 27 );
			}
			if ( check_debug() )
			{
				rest_log_message("*** OS PROCCI Testing Build Status ***");
				rest_log_message( vptr );
			}
			if (!( strcmp( vptr, "BUILD" )))
			{
				sleep(1);
				if ( zptr )
					zptr = liberate_os_response( zptr );
				if (!( zptr = os_get_server( pptr->number )))
				{
					reset_openstack_server( pptr );
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
			reset_openstack_server( pptr );
			return( 27 );
		}
		else if (!( pptr->reference = allocate_string(vptr)))
		{
			reset_openstack_server( pptr );
			return( 27 );
		}

		if ( check_debug() )
		{
			rest_log_message("*** OS PROCCI Instance HOST ID ***");
			rest_log_message( pptr->reference );
		}

		/* ------------------------------------------------------------- */
		/* determine the openstack message version and collect addresses */
		/* ------------------------------------------------------------- */
		if (!( version = resolve_openstack_version( pptr->profile ) ))
			resolve_os_v10_addresses( yptr, pptr );

		else if (!( strcmp( version, "v1.0" ) ))
			resolve_os_v10_addresses( yptr, pptr );

		else if (!( strcmp( version, "v1.1" ) ))
			resolve_os_v11_addresses( yptr, pptr );

		/* ------------------------------------------------------------ */
		/* set the host name field now to the public or private address */
		/* ------------------------------------------------------------ */
		if (( pptr->publicaddr ) && ( strlen( pptr->publicaddr ) != 0))
		{
			if (!( pptr->hostname = allocate_string( pptr->publicaddr ) ))
			{
				reset_openstack_server( pptr );
				return( 27 );
			}
		}
		else if (( pptr->privateaddr ) && ( strlen( pptr->privateaddr ) != 0))
		{
			if (!( pptr->hostname = allocate_string( pptr->privateaddr ) ))
			{
				reset_openstack_server( pptr );
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
			rest_log_message("*** OS PROCCI Instance is UP and RUNNING ***");
		}
		autosave_openstack_nodes();
		if ( check_debug() )
		{
			rest_log_message("*** OS PROCCI connect_openstack_server( exit ) ***");
		}
		return(0);
	}
}

/*	-------------------------------------------	*/
/* 	      s t a r t  _ o p e n s t a c k	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * start_openstack(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	char		* idptr;
	struct	os_response * osptr;
	struct	os_response * metaptr;
	struct	openstack * pptr;
	int		status;
	char	*	filename;
	char	*	metafilename;
	char		buffer[512];
	char		reference[512];
	char 	*	personality;
	char 	*	resource=_CORDS_LAUNCH_CFG;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->status != _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if ((status = use_openstack_configuration( pptr->profile )) != 0)
		return( rest_html_response( aptr, status, "Configuration Not Found" ) );

	sprintf(buffer,"contract=%s/%s/%s\npublisher=%s\n",
		OsProcci.identity,_CORDS_OPENSTACK,pptr->id,OsProcci.publisher);
	
	if (!( personality = allocate_string(buffer) ))
		return( rest_html_response( aptr, 500, "Server Failure : Personality" ) );

	sprintf(reference,"%s/%s/%s",OsProcci.identity,_CORDS_OPENSTACK,pptr->id);

	if (!( personality = openstack_instructions( reference, personality, _CORDS_CONFIGURATION ) ))
		return( rest_html_response( aptr, 500, "Server Failure : Configuration Instructions" ) );

	if (!( filename = os_create_server_request( 
		pptr->name, pptr->image, pptr->flavor, personality, resource ) ))
	 	return( rest_html_response( aptr, 400, "Bad Request : Create Server Message" ) );
	else if (!( osptr = os_create_server( filename )))
	 	return( rest_html_response( aptr, 400, "Bad Request : Create Server Request" ) );

	else
	{
		liberate( filename );
		/* --------------------------------- */
		/* retrieve crucial data from server */
		/* --------------------------------- */
		if (!( status = connect_openstack_server( osptr, pptr ) ))
		{
			/* ---------------------------- */
			/* launch the COSACS operations */
			/* ---------------------------- */
			cosacs_metadata_instructions( 
				pptr->hostname, _CORDS_CONFIGURATION,
				reference, OsProcci.publisher );

			/* ----------------------- */
			/* create server meta data */
			/* ----------------------- */
			if (!( idptr = json_atribut( osptr->jsonroot, "id") ))
			 	return( rest_html_response( aptr, 400, "Bad Request : Missing Meta Data Server ID" ) );

			else if (!( metafilename = os_create_metadata_request( personality ) ))
			 	return( rest_html_response( aptr, 400, "Bad Request : Create MetaData Message" ) );
			else if (!( metaptr = os_create_metadata( idptr, metafilename )))
			 	return( rest_html_response( aptr, 400, "Bad Request : Create MetaData Request" ) );
			else
			{
				metaptr = liberate_os_response( metaptr );
				liberate( metafilename );
			}
		}
		osptr = liberate_os_response( osptr );
		if (!( status ))
		{
			if (!( os_valid_price( pptr->price ) ))
				return( rest_html_response( aptr, 200, "OK" ) );
			else if ( occi_send_transaction( _CORDS_OPENSTACK, pptr->price, "action=start", pptr->account, reference ) )
				return( rest_html_response( aptr, 200, "OK" ) );
			else	return( rest_html_response( aptr, 200, "OK" ) );
		}
		else  	return( rest_html_response( aptr, 400, "Bad Request : Connect Open Stack" ) );
	}

}

/*	-------------------------------------------	*/
/* 	      s a v e  _ o p e n s t a c k	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * save_openstack(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	char	reference[512];
	struct	os_response * osptr;
	int		status;
	struct	openstack * pptr;
	char	*	filename;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->status == _OCCI_IDLE )
		return( rest_html_response( aptr, 400, "Contract Not Active" ) );
	else if ((status = use_openstack_configuration( pptr->profile )) != 0)
		return( rest_html_response( aptr, status, "Not Found" ) );
	else if (!( filename = os_create_image_request( pptr->name, pptr->number ) ))
	 	return( rest_html_response( aptr, 400, "Bad Request" ) );
	else if (!( osptr = os_create_image( filename, pptr->number ) ))
	 	return( rest_html_response( aptr, 400, "Bad Request" ) );
	else
	{
		/* --------------------------------- */
		/* retrieve crucial data from server */
		/* --------------------------------- */
		status = connect_openstack_image( osptr, pptr );
		osptr = liberate_os_response( osptr );
		if (!( status ))
		{
			sprintf(reference,"%s/%s/%s",OsProcci.identity,_CORDS_OPENSTACK,pptr->id);
			if (!( os_valid_price( pptr->price ) ))
				return( rest_html_response( aptr, 200, "OK" ) );
			else if ( occi_send_transaction( _CORDS_OPENSTACK, pptr->price, "action=save", pptr->account, reference ) )
				return( rest_html_response( aptr, 200, "OK" ) );
			else	return( rest_html_response( aptr, 200, "OK" ) );
		}
		else  	return( rest_html_response( aptr, 400, "Bad Request" ) );
	}
}

/*	--------------------------------------------------------	*/
/* 	 s t o p  _ o p e n s t a c k _ p r o v i s i o n i n g  	*/
/*	--------------------------------------------------------	*/
private	struct os_response *	stop_openstack_provisioning( struct openstack * pptr )
{
	int	status;
	struct	os_response * osptr;
	if ((status = use_openstack_configuration( pptr->profile )) != 0)
		return((struct os_response *) 0);
	else 	return( os_delete_server( pptr->number ) );
}

/*	-------------------------------------------	*/
/* 	      s t o p  _ o p e n s t a c k	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * stop_openstack(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	char	reference[512];
	struct	os_response * osptr;
	int		status;
	struct	openstack * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->status == _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if (!(osptr = stop_openstack_provisioning( pptr )))
		return( rest_html_response( aptr, status, "Not Found" ) );
	else
	{
		if ( pptr->status == _OCCI_IDLE )
			return( rest_html_response( aptr, 200, "OK" ) );
		else
		{
			reset_openstack_server( pptr );
			pptr->when = time((long *) 0);
			osptr = liberate_os_response( osptr );
			sprintf(reference,"%s/%s/%s",OsProcci.identity,_CORDS_OPENSTACK,pptr->id);
			if (!( os_valid_price( pptr->price ) ))
				return( rest_html_response( aptr, 200, "OK" ) );
			else if ( occi_send_transaction( _CORDS_OPENSTACK, pptr->price, "action=stop", pptr->account, reference ) )
				return( rest_html_response( aptr, 200, "OK" ) );
			else	return( rest_html_response( aptr, 200, "OK" ) );
		}
	}
}

/*	-------------------------------------------	*/
/* 	      r e s t a r t  _ o p e n s t a c k	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * restart_openstack(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	int	status;
	struct	openstack * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ((status = use_openstack_configuration( pptr->profile )) != 0)
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
/* 	     s u s p e n d  _ o p e n s t a c k	  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * suspend_openstack(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	int	status;
	struct	openstack * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ((status = use_openstack_configuration( pptr->profile )) != 0)
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
/* 	   s o f t b o o t _ o p e n s t a c k  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * softboot_openstack(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	openstack * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	   h a r d b o o t _ o p e n s t a c k  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * hardboot_openstack(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	openstack * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	     r e b u i l d_ o p e n s t a c k  		*/
/*	-------------------------------------------	*/
private	struct	rest_response * rebuild_openstack(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	openstack * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	      r e s i z e _ o p e n s t a c k  		*/
/*	-------------------------------------------	*/
private	struct	rest_response * resize_openstack(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	openstack * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	    c o n f i r m _ o p e n s t a c k  		*/
/*	-------------------------------------------	*/
private	struct	rest_response * confirm_openstack(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	openstack * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

/*	-------------------------------------------	*/
/* 	      r e v e r t _ o p e n s t a c k  		*/
/*	-------------------------------------------	*/
private	struct	rest_response * revert_openstack(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	openstack * pptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else	return( rest_html_response( aptr, 200, "OK" ) );
}

#include "oscontract.c"

/*	-------------------------------------------	*/
/* 	      c r e a t e _ o p e n s t a c k  		*/
/*	-------------------------------------------	*/
private	int	create_openstack(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	openstack * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( pptr->node ))
		return( 0 ); 
	else	return(create_openstack_contract(optr,pptr, _CORDS_CONTRACT_AGENT, OsProcci.tls));
}

/*	-------------------------------------------	*/
/* 	    r e t r i e v e _ o p e n s t a c k  	*/
/*	-------------------------------------------	*/
private	int	retrieve_openstack(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	openstack * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      u p d a t e _ o p e n s t a c k 	 	*/
/*	-------------------------------------------	*/
private	int	update_openstack(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	openstack * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ o p e n s t a c k  		*/
/*	-------------------------------------------	*/
private	int	delete_openstack(struct occi_category * optr, void * vptr)
{
	struct	occi_kind_node * nptr;
	struct	openstack * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(delete_openstack_contract(optr, pptr, _CORDS_CONTRACT_AGENT, OsProcci.tls));
}

private	struct	occi_interface	openstack_interface = {
	create_openstack,
	retrieve_openstack,
	update_openstack,
	delete_openstack
	};

/*	-------------------------------------------	*/
/* 	       b u i l d _ o p e n s t a c k  		*/
/*	-------------------------------------------	*/
/*	this function is to be called to build the	*/
/*	complete openstack occi category to offer	*/
/*	an openstack compute provisioning service	*/
/*	through a compatible one published occi		*/
/*	service interface.				*/
/*	-------------------------------------------	*/
public	struct	occi_category * build_openstack( char * domain )
{
	struct	occi_category * optr;
	if (!( optr = occi_cords_openstack_builder( domain,_CORDS_OPENSTACK ) ))
		return( optr );
	else
	{
		optr->callback  = &openstack_interface;
		optr->access |= _OCCI_PROVIDER;
		if (!( optr = occi_add_action( optr,_CORDS_START,"",start_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_SAVE,"",save_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_STOP,"",stop_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_SUSPEND,"",suspend_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,_CORDS_RESTART,"",restart_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"softboot","",softboot_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"hardboot","",hardboot_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"rebuild","",rebuild_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"resize","",resize_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"confirm","",confirm_openstack)))
			return( optr );
		else if (!( optr = occi_add_action( optr,"revert","",revert_openstack)))
			return( optr );
		else	return( optr );
	}
}

/*	-------------------------------------------	*/
/*	 s e t _ d e f a u l t _ o p e n s t a c k	*/
/*	-------------------------------------------	*/
private	int	set_default_openstack(struct occi_category * optr, void * vptr)
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

/*	---------------------------------------------------------	*/
/* 	b u i l d _ o p e n s t a c k _ c o n f i g u r a t i o n	*/
/*	---------------------------------------------------------	*/
/*	this category handles the configuration of the interface	*/
/*	to the oepn stack server for the preceeding category of		*/
/*	provisioning instance requests.					*/
/*	---------------------------------------------------------	*/
public	struct	occi_category * build_openstack_configuration( char * domain )
{
	struct	occi_category * optr;
	if (!( optr = occi_cords_osconfig_builder( domain, "openstack_configuration" ) ))
		return( optr );
	else if (!( optr = occi_add_action( optr,"current","",set_default_openstack)))
		return( optr );
	else	return( optr );
}

#endif	/* _proccios_c */
	/* ----------- */


