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
#ifndef	_occi_os_c
#define	_occi_os_c

#include "occiclient.h"
#include "stdnode.h"

/*	------------------------------------------	*/
/*	    o c c i _ o s _ l o c a t i o n		*/
/*	------------------------------------------	*/
private	char *	occi_os_location( struct rest_response * zptr )
{
	struct	rest_header * hptr;
	char	buffer[1024];
	char *	host;
	if ((!( hptr = rest_resolve_header( zptr->first, _OCCI_LOCATION ) ))
	&&  (!( hptr = rest_resolve_header( zptr->first, _HTTP_LOCATION ) )))
		return(0);
	else if (!( host = hptr->value ))
		return(0);
	else
	{
		if (!( strncmp( host, "http://", strlen( "http://" ) ) ))
			strcpy( buffer, host );
		else if (!( strncmp( host, "https://", strlen( "https://" ) ) ))
			strcpy( buffer, host );
		else	sprintf(buffer,"http://%s",host);
		return( allocate_string( buffer ) );
	}
}

/*	------------------------------------------	*/
/*	    o c c i _ o s _ r e f e r e n c e		*/
/*	------------------------------------------	*/
private	int	occi_os_reference( struct rest_response * zptr, struct openstack * pptr )
{
	char *	location;
	if (!( location = occi_os_location( zptr )))
		return( 0 );
	else
	{
		if ( pptr->reference ) pptr->reference = liberate( pptr->reference );
		pptr->reference = location;
		return(1);
	}
}

/*	------------------------------------------------------------	*/
/*		o c c i _ o s _ l o c a t e _ a t t r i b u t e		*/
/*	------------------------------------------------------------	*/
private	struct	rest_header * occi_os_locate_attribute( struct rest_response * zptr, char * atbname )
{
	struct	rest_header * hptr;
	if (!( zptr )) 	return((struct rest_header *) 0);

	for (	hptr = rest_resolve_header( zptr->first, _OCCI_ATTRIBUTE );
		hptr != (struct rest_header *) 0;
		hptr = rest_resolve_header( hptr->next, _OCCI_ATTRIBUTE ) )
	{
		if (!( hptr->value ))
			continue;
		else if ( strncmp( 	hptr->value, 
					atbname, strlen( atbname ) ) )
			continue;
		else	break;
	}
	return( hptr );
}

/*	------------------------------------------------------------	*/
/*		o c c i _ o s _ a t t r i b u t e _ v a l u e 		*/
/*	------------------------------------------------------------	*/
private	char *	occi_os_attribute_value( struct rest_header * hptr )
{
	char *	vptr;
	char *	rptr;
	char *	wptr;
	if (!( hptr )) 	
		return((char *) 0);
	else if (!( hptr->value ))
	 	return((char *) 0);
	else if (!( vptr = allocate_string( hptr->value ) ))
	 	return((char *) 0);
	{
		wptr = vptr;
		while (( *vptr ) && ( *vptr != '=' ) ) vptr++;
		if ( *vptr != '=' ) 
			return((char *) 0);
		*(vptr++) = 0;
		rptr = allocate_string( vptr );
		liberate( wptr );
		if (!( rptr ))
			return( rptr );
		else 	return( occi_unquoted_value( rptr ) );
	}
}

/*	------------------------------------------	*/
/*	    o c c i _ o s _ h o s t n a m e 		*/
/*	------------------------------------------	*/
private	int	occi_os_hostname( struct rest_response * zptr, struct openstack * pptr )
{
	struct	rest_header * hptr;
	char	buffer[1024];
	char *	host;

	if (!( hptr = occi_os_locate_attribute( zptr, "org.openstack.network.floating.ip" ) ))
	{
		if ( pptr->hostname ) pptr->hostname = liberate( pptr->hostname );
		if (!( pptr->hostname = occi_unquoted_value( "hostname" ) ))
			return(0);
		else	return(1);
	}
	else
	{
		sprintf(buffer,"http://%s",(hptr->value+strlen("org.openstack.network.floating.ip")+1));
		if ( pptr->hostname ) pptr->hostname = liberate( pptr->hostname );
		if (!( pptr->hostname = allocate_string( buffer ) ))
			return( 0 );
		strcpy( buffer, pptr->hostname );
		if ( pptr->hostname ) pptr->hostname = liberate( pptr->hostname );
		if (!( pptr->hostname = occi_unquoted_value( buffer ) ))
			return( 0 );
		else	return(1);
	}

}

/*	-----------------------------------------------------------	*/
/*		o s _ o c c i _ c l e a n _ s t r i n g			*/
/*	-----------------------------------------------------------	*/
private	void	os_occi_clean_string( char * sptr )
{
	if ( sptr )
	{
		while ( *sptr )
		{
			if ( *sptr == '\t' )
				*(sptr++) = ' ';
			else if ( *sptr == '\r' )
				*sptr = 0;
			else if ( *sptr == '\n' )
				*sptr = 0;
			else	sptr++;
		}
	}
	return;
}

/*	---------------------------------------------------------	*/
/* 	 p r o c e s s _ p l a i n _ t e x t a t t r i b u t e s 	*/
/*	---------------------------------------------------------	*/
private	struct	rest_response * process_plain_text_attributes( struct rest_response * zptr )
{
	struct	rest_header * hptr;
	char *	sptr;
	char *	nptr;
	char *	vptr;
	FILE * h;
	char	buffer[8192];
	if (!( zptr ))
		return( zptr );
	else if (!( zptr->body ))
		return( zptr );
	else if (!( h = fopen( zptr->body, "r" ) ))
		return( zptr );
	else
	{
		while ((nptr = fgets(buffer, 8000, h)) != (char *) 0)
		{
			os_occi_clean_string( nptr );
			while ( *nptr == ' ' ) nptr++;
			if (!( strncasecmp( nptr, _OCCI_ATTRIBUTE, strlen( _OCCI_ATTRIBUTE ) ) ))
			{
				nptr+= strlen( _OCCI_ATTRIBUTE );
				while ( *nptr == ' ' ) nptr++;
				if ( *nptr == ':' ) nptr++;
				while ( *nptr == ' ' ) nptr++;
				if (!( hptr = rest_response_header( zptr, _OCCI_ATTRIBUTE, nptr ) ))
					break;
				else	continue;
			}
		}
		return( zptr );
	}
}

/*	---------------------------------------------------	*/
/*	p r o c e s s _ o c c i _ o s _ a t t r i b u t e s	*/
/*	---------------------------------------------------	*/
private	struct	rest_response * process_occi_os_attributes( struct rest_response * zptr )
{
	struct	rest_header * hptr;
	if (!( zptr ))
		return( zptr );
	else if (!( hptr = rest_resolve_header( zptr->first, _HTTP_CONTENT_LENGTH ) ))
		return( zptr );
	else if (!( hptr->value ))
		return( zptr );
	else if (!( atoi( hptr->value ) ))
		return( zptr );
	else if (!( hptr = rest_resolve_header( zptr->first, _HTTP_CONTENT_TYPE ) ))
		return( zptr );
	else if (!( hptr->value ))
		return( zptr );
	else if (!( strncasecmp( hptr->value, "text/plain", strlen("text/plain") ) ))
		return( process_plain_text_attributes( zptr ) );
	else	return( zptr );
}

/*	----------------------------------------------------------------	*/
/*		b u i l d _ o c c i _ o s _ f i r e w a l l			*/
/*	----------------------------------------------------------------	*/
private	int	build_occi_os_firewall( struct openstack * pptr )
{
	struct	rest_response *	qptr;
	struct	standard_message firewall;
	struct	standard_message port;
	struct	occi_element * eptr;
	char * rulegroup=(char *) 0;
	char * rulefrom=(char *) 0;
	char * rulename=(char *) 0;
	char * ruleto=(char *) 0;
	char * ruleproto=(char *) 0;
	int	started=0;
	int	status=0;
	char *	sptr;

	memset( &firewall, 0, sizeof(struct standard_message));
	memset( &port, 0, sizeof(struct standard_message));

	if (!( pptr ))
		return(0);
	else if (!( pptr->firewall ))
		return(0);
	else if ((status = get_standard_message( &firewall, pptr->firewall, _CORDS_CONTRACT_AGENT, default_tls() )) != 0)
		return( 0 );
	else
	{
		for (	eptr = first_standard_message_link( firewall.message );
			eptr != (struct occi_element *) 0;
			eptr = next_standard_message_link( eptr ) )
		{
			/* ------------------------ */
			/* retrieve the port record */
			/* ------------------------ */
			if (!( sptr = standard_message_link_value( eptr->value )))
				continue;
			else if ((status = get_standard_message( &port, sptr, _CORDS_CONTRACT_AGENT, default_tls() )) != 0)
				continue;

			/* --------------------------------------- */
			/* build the security group if not started */
			/* --------------------------------------- */
			if (!( started++ ))
			{
				if (!( qptr = create_occi_os_security_group( pptr->id ) ))
					return(0);
				else if (!( rulegroup = occi_os_location( qptr ) ))
					qptr = liberate_rest_response( qptr );
				else
				{
					/* ---------------------------------- */
					/* save the security group identifier */
					/* ---------------------------------- */
					if ( pptr->group )
						pptr->group = liberate( pptr->group );
					pptr->group = rulegroup;
					qptr = liberate_rest_response( qptr );
				}
			}			

			/* ---------------------------------- */
			/* retrieve the port rule information */
			/* ---------------------------------- */
			if ((!( rulename = occi_extract_atribut( port.message, "occi", 
				_CORDS_PORT, _CORDS_NAME ) ))
			||  (!( ruleproto = occi_extract_atribut( port.message, "occi", 
				_CORDS_PORT, _CORDS_PROTOCOL ) ))
			||  (!( rulefrom = occi_extract_atribut( port.message, "occi", 
				_CORDS_PORT, _CORDS_FROM ) ))
			||  (!( ruleto = occi_extract_atribut( port.message, "occi", 
				_CORDS_PORT, _CORDS_TO   ) )) )
			{
				release_standard_message( &port );
				release_standard_message( &firewall );
				return(0);
			}
			/* ---------------------------------- */
			/* add the rule to the security group */
			/* ---------------------------------- */
			if (!( qptr = create_occi_os_security_rule( 
					pptr->id, ruleproto, rulefrom, ruleto, "0.0.0.0/0" ) ))
				return(0);
			else	qptr = liberate_rest_response( qptr );
		}
		release_standard_message( &port );
		release_standard_message( &firewall );
		return(0);
	}
}


/*	-------------------------------------------	*/
/* 	  s t a r t  _ o c c i_ o p e n s t a c k  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * start_occi_openstack(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	rest_header * hptr;
	char	reference[1024];
	int	status=0;
	int	startcompute=0;
	struct	openstack * pptr;
	struct	os_config * kptr;
	struct	rest_response * qptr;
	char	flavor[1024];
	char *	fscheme;
	char	image[1024];
	char *	ischeme;

	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->state != _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if (!(kptr = use_occi_openstack_configuration( pptr->profile )))
		return( rest_html_response( aptr, 800, "Configuration Not Found" ) );
	else
	{
		/* ---------------------------------------- */
		/* Prepare the Flavor and Image information */
		/* ---------------------------------------- */
		strcpy(flavor,pptr->flavor);

		fscheme = flavor;
		while (( *fscheme ) && ( *fscheme != ';' ))
			fscheme++;
		if ( *fscheme == ';' )
			*(fscheme++) = 0;

		strcpy(image,pptr->image);

		ischeme = image;
		while (( *ischeme ) && ( *ischeme != ';' ))
			ischeme++;
		if ( *ischeme == ';' )
			*(ischeme++) = 0;

		/* ------------------------------ */
		/* build the application firewall */
		/* ------------------------------ */
		if ((status = build_occi_os_firewall( pptr )) != 0)
			return( rest_html_response( aptr, status + 4000, "Server Failure : Firewall Preparation" ) );
	
		/* ----------------------------- */
		/* Create a new compute instance */
		/* ----------------------------- */
		else if (!( qptr = create_occi_os_secure_compute(flavor,image,fscheme,ischeme, pptr->group, pptr->zone) ))
		 	return( rest_html_response( aptr, 801, "Bad Request (POST COMPUTE)" ) );

		else if ((status = qptr->status) > 299 )
		{
			qptr = liberate_rest_response( qptr );
		 	return( rest_html_response( aptr, status + 4000, "Bad Request (CREATE COMPUTE FAILURE)" ) );
		}

		/* ------------------------------------------- */
		/* recover the compute instance identifier now */
		/* ------------------------------------------- */
		else if (!( occi_os_reference( qptr, pptr ) ))
		{
			qptr = liberate_rest_response( qptr );
		 	return( rest_html_response( aptr, 801, "Bad Request (NO LOCATION)" ) );
		}
		else if (!( pptr->reference ))
		{
			qptr = liberate_rest_response( qptr );
		 	return( rest_html_response( aptr, 801, "Bad Request (NO REFERENCE)" ) );
		}

		/* ------------------------------------ */
		/* GET the Compute Instance information */
		/* ------------------------------------ */
		if (!( qptr = get_occi_os_compute(pptr->reference )))
		 	return( rest_html_response( aptr, 801, "Bad Request (START COMPUTE)" ) );
		else if (!( qptr = process_occi_os_attributes( qptr )))
		 	return( rest_html_response( aptr, 801, "Bad Request (PROCESSING ATTRIBUTS)" ) );
		else if (!( hptr = occi_os_locate_attribute( qptr, "occi.compute.state" ) ))
			startcompute=1;
		else if (!( vptr = occi_os_attribute_value( hptr ) ))
			startcompute=1;
		else
		{ 
			if (!( strcmp( vptr, "active" ) ))
				startcompute=0;
			else	startcompute=1;
			liberate( vptr );
		}

		qptr = liberate_rest_response( qptr );

		/* -------------------------------------------------- */
		/* The POST COMPUTE seems to start the VM automaticly */
		/* -------------------------------------------------- */
		if ( startcompute )
		{
			/* ----------------------- */
			/* send OCCI START message */
			/* ----------------------- */
			if (!( qptr = start_occi_os_compute(pptr->reference )))
			 	return( rest_html_response( aptr, 801, "Bad Request (START COMPUTE)" ) );
			else if ((status = qptr->status) > 299 )
			{
				qptr = liberate_rest_response( qptr );
			 	return( rest_html_response( aptr, status + 4000, "Bad Request (START FAILURE)" ) );
			}
			else	qptr = liberate_rest_response( qptr );
		}

		/* ---------------------- */
		/* Allocate a Floating IP */
		/* ---------------------- */
		if (!( qptr = allocate_occi_os_floating_ip(pptr->reference)))
		 	return( rest_html_response( aptr, 801, "Bad Request (ALLOCATE FLOATING IP)" ) );
		else if ((status = qptr->status) > 299 )
		{
			qptr = liberate_rest_response( qptr );
		 	return( rest_html_response( aptr, status + 4000, "Bad Request (ALLOCATE IP FAILURE)" ) );
		}
		else	qptr = liberate_rest_response( qptr );

		/* -------------------------------- */
		/* Retrieve the Floating IP address */
		/* -------------------------------- */
		if (!( qptr = get_occi_os_compute(pptr->reference)))
		 	return( rest_html_response( aptr, 801, "Bad Request (RETRIEVE COMPUTE)" ) );
		else if (!( qptr = process_occi_os_attributes( qptr )))
		 	return( rest_html_response( aptr, 801, "Bad Request (PROCESSING ATTRIBUTS)" ) );
		{
			
			/* --------------------------------------- */
			/* establish the resulting host name value */
			/* --------------------------------------- */
			occi_os_hostname( qptr, pptr );
			qptr = liberate_rest_response( qptr );
		}

		/* ---------------------------- */
		/* launch the COSACS operations */
		/* ---------------------------- */
		cosacs_metadata_instructions( 
			pptr->hostname, _CORDS_CONFIGURATION,
			pptr->reference, OsOcciProcci.publisher,
			pptr->account );

		/* ------------------------------------- */
		/* release the public IP if not required */
		/* ------------------------------------- */
		if (!( strcasecmp( pptr->access , _CORDS_PRIVATE ) ))
		{
			if (!( qptr = release_occi_os_floating_ip(pptr->reference)))
			 	return( rest_html_response( aptr, 801, "Bad Request (RELEASE FLOATING IP)" ) );
			else	qptr = liberate_rest_response( qptr );
		}

		/* ------------------------------------------------------- */
		/* indicate that the machine is running and set start time */
		/* ------------------------------------------------------- */
		pptr->when = time((long *) 0);
		pptr->state = _OCCI_RUNNING;
		status = 0;
		if (!( status ))
		{
			sprintf(reference,"%s/%s/%s",OsOcciProcci.identity,_CORDS_OPENSTACK,pptr->id);
			if (!( rest_valid_string( pptr->price ) ))
				return( rest_html_response( aptr, 200, "OK" ) );
			else if ( occi_send_transaction( _CORDS_OPENSTACK, pptr->price, "action=start", pptr->account, reference ) )
				return( rest_html_response( aptr, 200, "OK" ) );
			else	return( rest_html_response( aptr, 200, "OK" ) );
		}
		else  	return( rest_html_response( aptr, 4256, "Server Failure : OCCI Start Open Stack" ) );
	}
}

/*	-------------------------------------------	*/
/* 	  s a v e  _ o c c i_ o p e n s t a c k  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * save_occi_openstack(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	openstack * pptr;
	struct	os_config * kptr;
	char	reference[1024];
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->state == _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if (!(kptr = use_occi_openstack_configuration( pptr->profile )))
		return( rest_html_response( aptr, 800, "Configuration Not Found" ) );
	else
	{
		/* ------------------------- */
		/* TODO : add OCCI save code */
		/* ------------------------- */
		sprintf(reference,"%s/%s/%s",OsOcciProcci.identity,_CORDS_OPENSTACK,pptr->id);
		if (!( rest_valid_string( pptr->price ) ))
			return( rest_html_response( aptr, 200, "OK" ) );
		else if ( occi_send_transaction( _CORDS_OPENSTACK, pptr->price, "action=save", pptr->account, reference ) )
			return( rest_html_response( aptr, 200, "OK" ) );
		else	return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	--------------------------------------------	*/
/* 	s n a p s h o t _ o c c i_ o p e n s t a c k  	*/
/*	--------------------------------------------	*/
private	struct	rest_response * snapshot_occi_openstack(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	openstack * pptr;
	char	reference[1024];
	struct	os_config * kptr;
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->state == _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if (!(kptr = use_occi_openstack_configuration( pptr->profile )))
		return( rest_html_response( aptr, 800, "Configuration Not Found" ) );
	else
	{
		/* ----------------------------- */
		/* TODO : add OCCI snapshot code */
		/* ----------------------------- */
		sprintf(reference,"%s/%s/%s",OsOcciProcci.identity,_CORDS_OPENSTACK,pptr->id);
		if (!( rest_valid_string( pptr->price ) ))
			return( rest_html_response( aptr, 200, "OK" ) );
		else if ( occi_send_transaction( _CORDS_OPENSTACK, pptr->price, "action=snapshot", pptr->account, reference ) )
			return( rest_html_response( aptr, 200, "OK" ) );
		else	return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	  s t o p _ o c c i_ o p e n s t a c k  	*/
/*	-------------------------------------------	*/
private	struct	rest_response * stop_occi_openstack(
		struct occi_category * optr, 
		struct rest_client * cptr, 
		struct rest_request * rptr, 
		struct rest_response * aptr, 
		void * vptr )
{
	struct	openstack * pptr;
	struct	os_config * kptr;
	struct	rest_response * qptr;
	char	reference[1024];
	if (!( pptr = vptr ))
	 	return( rest_html_response( aptr, 404, "Invalid Action" ) );
	else if ( pptr->state == _OCCI_IDLE )
		return( rest_html_response( aptr, 200, "OK" ) );
	else if (!(kptr = use_occi_openstack_configuration( pptr->profile )))
		return( rest_html_response( aptr, 800, "Configuration Not Found" ) );
	else
	{
		/* ------------------------------ */
		/* TODO: send OCCI stop   message */
		/* ------------------------------ */
		if (!( qptr = stop_occi_os_compute(pptr->reference)))
		 	return( rest_html_response( aptr, 801, "Bad Request (STOP COMPUTE)" ) );
		else	qptr = liberate_rest_response( qptr );

		/* ----------------------- */
		/* Release the Floating IP */
		/* ----------------------- */
		if (!( qptr = release_occi_os_floating_ip(pptr->reference)))
		 	return( rest_html_response( aptr, 801, "Bad Request (ALLOCATE FLOATING IP)" ) );
		else	qptr = liberate_rest_response( qptr );

		/* ------------------------------ */
		/* TODO: send OCCI DELETE request */
		/* ------------------------------ */
		if (!( qptr = delete_occi_os_compute(pptr->reference) ))
		 	return( rest_html_response( aptr, 801, "Bad Request (DELETE COMPUTE)" ) );
		else	qptr = liberate_rest_response( qptr );

		pptr->state = _OCCI_IDLE;
		pptr->when = time((long *) 0);
		sprintf(reference,"%s/%s/%s",OsOcciProcci.identity,_CORDS_OPENSTACK,pptr->id);

		if (!( rest_valid_string( pptr->price ) ))
			return( rest_html_response( aptr, 200, "OK" ) );
		else if ( occi_send_transaction( _CORDS_OPENSTACK, pptr->price, "action=stop", pptr->account, reference ) )
			return( rest_html_response( aptr, 200, "OK" ) );
		else	return( rest_html_response( aptr, 200, "OK" ) );
	}
}

/*	-------------------------------------------	*/
/* 	      c r e a t e _ o p e n s t a c k  		*/
/*	-------------------------------------------	*/
private	int	create_openstack(struct occi_category * optr, void * vptr,struct rest_request *rptr)
{
	struct	occi_kind_node * nptr;
	struct	openstack * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if (!( pptr->node ))
		return( 0 ); 
	else	return(create_openstack_contract( optr, pptr, _CORDS_CONTRACT_AGENT, default_tls()));
}

/*	-------------------------------------------	*/
/* 	      d e l e t e _ o p e n s t a c k  		*/
/*	-------------------------------------------	*/
private	int	delete_openstack(struct occi_category * optr, void * vptr,struct rest_request *rptr)
{
	struct	occi_kind_node * nptr;
	struct	openstack * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(delete_openstack_contract(optr, pptr, _CORDS_CONTRACT_AGENT, default_tls()));
}

private	struct	occi_interface	openstack_interface = {
	create_openstack,
	NULL,
	NULL,
	delete_openstack
	};


	/* ---------- */
#endif	/* _occi_os_c */
	/* ---------- */

