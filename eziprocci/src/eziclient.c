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
#ifndef	_eziclient_c	
#define	_eziclient_c

#include "eziclient.h"

/*	------------------------------------------------------------	*/
/*		l i b e r a t e _ e z i _ r e s p o n s e			*/
/*	------------------------------------------------------------	*/
public	struct ezi_response * liberate_ezi_response( struct ezi_response * rptr )	
{
	if ( rptr )
	{
		if ( rptr->response )
			rptr->response = liberate_rest_response( rptr->response );
		if ( rptr->jsonroot )
			rptr->jsonroot = drop_data_element( rptr->jsonroot );
		if ( rptr->xmlroot )
			rptr->xmlroot = document_drop( rptr->xmlroot );
		if ( rptr->content )
			liberate( rptr->content );
		liberate( rptr );
	}
	return((struct ezi_response *) 0);
}

/*	------------------------------------------------------------	*/
/*		 		e z i _ c h e c k 			*/
/*	------------------------------------------------------------	*/
private	struct	ezi_response * ezi_check( struct rest_response * aptr )
{
	struct	ezi_response * rptr=(struct ezi_response *) 0;
	struct	rest_header * hptr=(struct rest_header *) 0;;
	if ( aptr )
	{
		if ( check_verbose() )
		{
			printf("   OS Client Response : %s %u %s\n",aptr->version,aptr->status,aptr->message);

			if ( check_debug() )
			{
				for (	hptr=aptr->first;
					hptr != (struct rest_header *) 0;
					hptr = hptr->next )
				{
					if (!( hptr->name )) 
						continue;
					else	printf("   %s: %s\n",hptr->name,hptr->value);
				}
				if ( aptr->body )
				{
					printf("   [ %s ] \n",aptr->body );
				}
			}
		}
	}
	if (!( rptr = allocate( sizeof( struct ezi_response ) ) ))
		return( rptr );
	else
	{
		rptr->nature = _TEXT_NONE;
		rptr->content= (char *) 0;
		rptr->xmlroot = (struct xml_element *) 0;
		rptr->jsonroot = (struct data_element *) 0;
		rptr->response = aptr;
		if (!( aptr->body ))
			return(rptr);
		if (!( hptr = rest_resolve_header( aptr->first, "Content-Type" ) ))
			return(rptr);
		else if (!( rptr->content = allocate_string( hptr->value ) ))
			return( rptr );
		else if ((!( strcmp( rptr->content, "text/json" 	)))
		     ||  (!( strcmp( rptr->content, "application/json" 	)))
		     ||  (!( strcmp( rptr->content, "x-application/json"))))
		{
			rptr->nature = _TEXT_JSON;
			rptr->jsonroot = json_parse_file( aptr->body );
			return( rptr );
		}
		else if ((!( strcmp( rptr->content, "text/xml" 		)))
		     ||  (!( strcmp( rptr->content, "application/xml" 	)))
		     ||  (!( strcmp( rptr->content, "x-application/xml"))))
		{
			rptr->nature = _TEXT_XML;
			rptr->xmlroot = document_parse_file( aptr->body );
			return( rptr );
		}
		else	return( rptr );
		
	}
}

/*	---------------------------------------------------------	*/
/*		e z i _ c l i e n t _ r a t e _ l i m i t e d		*/
/*	---------------------------------------------------------	*/
private	int	ezi_client_rate_limited( struct rest_response * rptr )
{
	if (!( rptr ))
		return(0);
	else if ( rptr->status < 400 )
		return(0);
	else if ( rptr->status != 413 )
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------	*/
/*		 e z i _ c l i e n t _ g e t _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	ezi_response * 
	ezi_client_get_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	struct	rest_response * rptr;
	struct	rest_header   * copy=(struct rest_header *) 0;

	if (( hptr )
	&&  (!( copy = rest_duplicate_headers( hptr ) )))
		return((struct ezi_response *) 0);

	while ((rptr = rest_client_get_request( target, tls, nptr, hptr )) != (struct rest_response *) 0)
	{
		if (!( ezi_client_rate_limited( rptr ) ))
			break;
		else if (!( copy ))
			continue;
		else if (!( hptr = rest_duplicate_headers( copy ) ))
			return((struct ezi_response *) 0);
		else	continue;
	}			
	if (( hptr )
	&&  ( copy ))
		copy = liberate_rest_headers( copy );

	return( ezi_check( rptr ) );
}

/*	------------------------------------------------------------	*/
/*		 e z i _ c l i e n t _ h e a d _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	ezi_response * 
	ezi_client_head_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	struct	rest_response * rptr;
	struct	rest_header   * copy=(struct rest_header *) 0;

	if (( hptr )
	&&  (!( copy = rest_duplicate_headers( hptr ) )))
		return((struct ezi_response *) 0);

	while (( rptr = rest_client_head_request( target, tls, nptr, hptr )) != (struct rest_response *) 0)
	{
		if (!( ezi_client_rate_limited( rptr ) ))
			break;
		else if (!( copy ))
			continue;
		else if (!( hptr = rest_duplicate_headers( copy ) ))
			return((struct ezi_response *) 0);
		else	continue;
	}			
	if (( hptr )
	&&  ( copy ))
		copy = liberate_rest_headers( copy );

	return( ezi_check( rptr ) );
}

/*	------------------------------------------------------------	*/
/*	      e z i _ c l i e n t _ d e l e t e _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	ezi_response * 
	ezi_client_delete_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	struct	rest_response * rptr;
	struct	rest_header   * copy=(struct rest_header *) 0;

	if (( hptr )
	&&  (!( copy = rest_duplicate_headers( hptr ) )))
		return((struct ezi_response *) 0);

	while (( rptr = rest_client_delete_request( target, tls, nptr, hptr )) != (struct rest_response *) 0)
	{
		if (!( ezi_client_rate_limited( rptr ) ))
			break;
		else if (!( copy ))
			continue;
		else if (!( hptr = rest_duplicate_headers( copy ) ))
			return((struct ezi_response *) 0);
		else	continue;
	}			

	if (( hptr )
	&&  ( copy ))
		copy = liberate_rest_headers( copy );

	return( ezi_check( rptr ) );
}

/*	------------------------------------------------------------	*/
/*		 e z i _ c l i e n t _ p o s t _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	ezi_response * 
	ezi_client_post_request(
		char * target, char * tls, char * nptr, char * filename, struct rest_header * hptr )
{
	struct	rest_response * rptr;
	struct	rest_header   * copy=(struct rest_header *) 0;
	char		      * body=(char *) 0;

	if (( hptr )
	&&  (!( copy = rest_duplicate_headers( hptr ) )))
		return((struct ezi_response *) 0);

	if (( filename )
	&&  (!( body = allocate_string( filename ) )))
	{
		copy = liberate_rest_headers( copy );
		return((struct ezi_response *) 0);
	}

	while (( rptr = rest_client_post_request( target, tls, nptr, filename, hptr )) != (struct rest_response *) 0)
	{
		if (!( ezi_client_rate_limited( rptr ) ))
			break;
		if ( copy  )
			if (!( hptr = rest_duplicate_headers( copy ) ))
				return((struct ezi_response *) 0);
		if ( body )
			if (!( filename = allocate_string( body ) ))
				return((struct ezi_response *) 0);

	}			

	if (( filename ) &&  ( body ))
		body = liberate( body );

	if (( hptr ) &&  ( copy ))
		copy = liberate_rest_headers( copy );

	return( ezi_check( rptr ) );
}

/*	------------------------------------------------------------	*/
/*		 e z i _ c l i e n t _ p u t _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	ezi_response * 
	ezi_client_put_request(
		char * target, char * tls, char * nptr, char * filename, struct rest_header * hptr )
{
	struct	rest_response * rptr;
	struct	rest_header   * copy=(struct rest_header *) 0;
	char		      * body=(char *) 0;

	if (( hptr )
	&&  (!( copy = rest_duplicate_headers( hptr ) )))
		return((struct ezi_response *) 0);

	if (( filename )
	&&  (!( body = allocate_string( filename ) )))
	{
		copy = liberate_rest_headers( copy );
		return((struct ezi_response *) 0);
	}

	while (( rptr = rest_client_put_request( target, tls, nptr, filename, hptr )) != (struct rest_response *) 0)
	{
		if (!( ezi_client_rate_limited( rptr ) ))
			break;
		if ( copy  )
			if (!( hptr = rest_duplicate_headers( copy ) ))
				return((struct ezi_response *) 0);
		if ( body )
			if (!( filename = allocate_string( body ) ))
				return((struct ezi_response *) 0);
	}			

	if (( filename ) && ( body ))
		body = liberate( body );

	if (( hptr ) &&  ( copy ))
		copy = liberate_rest_headers( copy );

	return( ezi_check( rptr ) );
}

/*	-------------------------------------------------	*/
/*	e z i _ k e y s t o n e _ a u t h _ m e s s a g e 	*/
/*	-------------------------------------------------	*/
public	char *	ezi_keystone_auth_message( char * user, char * password, char * tenant )
{
	char *	filename;
	FILE *	h;
	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename, "wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",
			0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<auth xmlns:xsi=%c%s%c xmlns=%c%s%c tenantName=%c%s%c>\n",
				0x0022,"http://www.w3.org/2001/XMLSchema-instance",0x0022,
				0x0022,"http://docs.openstack.com/identity/api/v2.0",0x0022,
				0x0022,tenant,0x0022);
		fprintf(h,"<passwordCredentials username=%c%s%c password=%c%s%c/>\n",
				0x0022,user,0x0022,0x0022,password,0x0022);
		fprintf(h,"</auth>\n");
		fclose(h);
		return( filename );
	}
}

/*	---------------------------------------------------------	*/
/*	 c h e c k _ k e y s t o n e _ a u t h o r i z a t i o n	*/
/*	---------------------------------------------------------	*/
public	int	check_keystone_authorization(struct ezi_subscription * sptr)
{
	struct	xml_element * document;
	struct	xml_element * eptr;
	struct	xml_element * gptr;
	struct	xml_atribut * aptr;
	struct	rest_response * rptr;
	struct	rest_header * hptr;
	char *	tptr;
	char *	filename;
	char	buffer[1024];
	if (!( sptr->Ezi.authenticate ))
	{
		sprintf(buffer,"%s/tokens",sptr->Ezi.host);
		if (!( hptr = rest_create_header( _HTTP_CONTENT_TYPE, sptr->KeyStone.requestauth ) ))
			return( 0 );
		else if (!( hptr->next = rest_create_header( _HTTP_ACCEPT, sptr->KeyStone.acceptauth ) ))
		{
			liberate_rest_header( hptr );
			return( 0 );
		}
		else	hptr->next->previous = hptr;
		if (!( filename = ezi_keystone_auth_message( 
			sptr->Ezi.user, 
			sptr->Ezi.password,
			sptr->KeyStone.tenantname ) ))
		{
			liberate_rest_header( hptr );
			return( 0 );
		}
		else if (!( rptr = rest_client_post_request( 
			buffer, sptr->Ezi.tls, sptr->Ezi.agent, filename, hptr ) ))
		{
			return( 0 );
		}

		if (!( hptr = rest_resolve_header( rptr->first, _HTTP_CONTENT_TYPE ) ))
		{
			rptr = liberate_rest_response( rptr );
			return( 0 );
		}
		else if (!( hptr->value ))
		{
			rptr = liberate_rest_response( rptr );
			return( 0 );
		}
		else if ( strncasecmp( hptr->value, sptr->KeyStone.acceptauth, strlen(sptr->KeyStone.acceptauth) ) )
		{
			rptr = liberate_rest_response( rptr );
			return( 0 );
		}
		else if (!( rptr->body ))
		{
			rptr = liberate_rest_response( rptr );
			return( 0 );
		}
		else if (!( document = document_parse_file( rptr->body ) ))
		{
			rptr = liberate_rest_response( rptr );
			return( 0 );
		}
		else if (!( eptr = document_element( document, "token" ) ))
		{
			document = document_drop( document );
			rptr = liberate_rest_response( rptr );
			return( 0 );
		}
		else if (!( aptr = document_atribut( eptr, "id" ) ))
		{
			document = document_drop( document );
			rptr = liberate_rest_response( rptr );
			return( 0 );
		}
		else if (!( aptr->value ))
		{
			document = document_drop( document );
			rptr = liberate_rest_response( rptr );
			return( 0 );
		}
		else if (!( sptr->Ezi.authenticate = allocate_string( aptr->value )))
		{
			document = document_drop( document );
			rptr = liberate_rest_response( rptr );
			return( 0 );
		}
		else if (!( sptr->Ezi.authenticate = occi_unquoted_value( sptr->Ezi.authenticate ) ))
		{
			document = document_drop( document );
			rptr = liberate_rest_response( rptr );
			return( 0 );
		}
		else if (!( eptr = document_element( eptr, "tenant" ) ))
		{
			document = document_drop( document );
			rptr = liberate_rest_response( rptr );
			return( 0 );
		}
		else if (!( aptr = document_atribut( eptr, "id" ) ))
		{
			document = document_drop( document );
			rptr = liberate_rest_response( rptr );
			return( 0 );
		}
		else if (!( aptr->value ))
		{
			document = document_drop( document );
			rptr = liberate_rest_response( rptr );
			return( 0 );
		}
		else if (!( sptr->KeyStone.tenantid = allocate_string( aptr->value )))
		{
			document = document_drop( document );
			rptr = liberate_rest_response( rptr );
			return( 0 );
		}
		else if (!( sptr->KeyStone.tenantid = occi_unquoted_value( sptr->KeyStone.tenantid ) ))
		{
			document = document_drop( document );
			rptr = liberate_rest_response( rptr );
			return( 0 );
		}
		else if (!( eptr = document_element( document, "serviceCatalog" ) ))
		{
			document = document_drop( document );
			rptr = liberate_rest_response( rptr );
			return( 0 );
		}
		else if (!( eptr = document_element( eptr, "service" ) ))
		{
			document = document_drop( document );
			rptr = liberate_rest_response( rptr );
			return( 0 );
		}
		else
		{
			while ( eptr )
			{
				if (!( aptr = document_atribut( eptr, "type" ) ))
				{
					document = document_drop( document );
					rptr = liberate_rest_response( rptr );
					return( 0 );
				}
				else if (!( tptr = aptr->value ))
				{
					document = document_drop( document );
					rptr = liberate_rest_response( rptr );
					return( 0 );
				}
				else if ((!( tptr = allocate_string( tptr ) ))
				     ||  (!( tptr = occi_unquoted_value( tptr ) )))
				{
					document = document_drop( document );
					rptr = liberate_rest_response( rptr );
					return( 0 );
				}
				else if (!( strcasecmp( tptr, "compute" ) ))
				{
					liberate( tptr );
					if (!( gptr = document_element( eptr, "endpoint" ) ))
					{
						document = document_drop( document );
						rptr = liberate_rest_response( rptr );
						return( 0 );
					}
					if (!( aptr = document_atribut( gptr, "publicURL" ) ))
					{
						document = document_drop( document );
						rptr = liberate_rest_response( rptr );
						return( 0 );
					}
					else if (!( aptr->value ))
					{
						document = document_drop( document );
						rptr = liberate_rest_response( rptr );
						return( 0 );
					}
					else if (!( sptr->Ezi.base = allocate_string( aptr->value ) ))
					{
						document = document_drop( document );
						rptr = liberate_rest_response( rptr );
						return( 0 );
					}
					else if (!( sptr->Ezi.base = occi_unquoted_value( sptr->Ezi.base ) ))
					{
						document = document_drop( document );
						rptr = liberate_rest_response( rptr );
						return( 0 );
					}
				}
				else if (!( strcasecmp( tptr, "image" ) ))
				{
					liberate( tptr );
					if (!( gptr = document_element( eptr, "endpoint" ) ))
					{
						document = document_drop( document );
						rptr = liberate_rest_response( rptr );
						return( 0 );
					}
					if (!( aptr = document_atribut( gptr, "publicURL" ) ))
					{
						document = document_drop( document );
						rptr = liberate_rest_response( rptr );
						return( 0 );
					}
					else if (!( aptr->value ))
					{
						document = document_drop( document );
						rptr = liberate_rest_response( rptr );
						return( 0 );
					}
					else if (!( sptr->KeyStone.glance = allocate_string( aptr->value ) ))
					{
						document = document_drop( document );
						rptr = liberate_rest_response( rptr );
						return( 0 );
					}
					else if (!( sptr->KeyStone.glance = occi_unquoted_value( sptr->KeyStone.glance ) ))
					{
						document = document_drop( document );
						rptr = liberate_rest_response( rptr );
						return( 0 );
					}
				}
				else if (!( strcasecmp( tptr, "volume" ) ))
				{
					liberate( tptr );
					if (!( gptr = document_element( eptr, "endpoint" ) ))
					{
						document = document_drop( document );
						rptr = liberate_rest_response( rptr );
						return( 0 );
					}
					if (!( aptr = document_atribut( gptr, "publicURL" ) ))
					{
						document = document_drop( document );
						rptr = liberate_rest_response( rptr );
						return( 0 );
					}
					else if (!( aptr->value ))
					{
						document = document_drop( document );
						rptr = liberate_rest_response( rptr );
						return( 0 );
					}
					else if (!( sptr->KeyStone.volume = allocate_string( aptr->value ) ))
					{
						document = document_drop( document );
						rptr = liberate_rest_response( rptr );
						return( 0 );
					}
					else if (!( sptr->KeyStone.volume = occi_unquoted_value( sptr->KeyStone.volume ) ))
					{
						document = document_drop( document );
						rptr = liberate_rest_response( rptr );
						return( 0 );
					}
				}
				else	liberate( tptr );
				eptr = eptr->next;
			}
			document = document_drop( document );
			rptr = liberate_rest_response( rptr );
			if (!( sptr->Ezi.base ))
				return( 0 );
			else if (!( sptr->Ezi.iaas ))
				return( 0 );
			else	return( 1 );
		}
	}
	return( 1 );
}

/*	------------------------------------------------------------	*/
/*			e z i _ a u t h e n t i c a t e ()		*/
/*	------------------------------------------------------------	*/
public	struct	rest_header   *	ezi_authenticate(struct ezi_subscription * sptr)
{
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	struct	ezi_response	*	rptr;
	struct	url		*	uptr;
	char 			*	nptr;
	int				status;
	char 			*	eptr;
	char	buffer[256];
	
	/* --------------------------------- */
	/* check if explicite Diablo Version */
	/* --------------------------------- */
	if (!( eptr = getenv("NOVADIABLO")))
		status = 0;
	else 	status = atoi(eptr);
	
	if (!( status ))
	{
		if (!( check_keystone_authorization(sptr) ))
			return((struct rest_header * )0);
		else if (!( sptr->Ezi.authenticate ))
			return((struct rest_header * )0);
	}
	else
	{
		/* ------------------------------ */
		/* Old Diablo Type Authentication */
		/* ------------------------------ */
		if (!( sptr->Ezi.user ))
			return( hptr );
		else if (!( sptr->Ezi.password ))
			return( hptr );
		else if (!( sptr->Ezi.version ))
			return( hptr );
		else if (!( sptr->Ezi.authenticate ))
		{
			sprintf(buffer,"/%s",sptr->Ezi.version);

			if (!( uptr = analyse_url( sptr->Ezi.host )))
				return( hptr );
			else if (!( uptr = validate_url( uptr ) ))
				return( hptr );
			else if (!( uptr->object = allocate_string( buffer ) ))
			{
				uptr = liberate_url( uptr );
				return( hptr );
			}
			else if (!( nptr = serialise_url( uptr,"" ) ))
			{
				uptr = liberate_url( uptr );
				return( hptr );
			}
			else	uptr = liberate_url( uptr );

			if (!( hptr = rest_create_header( "X-Auth-User", sptr->Ezi.user ) ))
			{
				liberate( nptr );
				return( hptr );
			}
			else if (!( hptr->next = rest_create_header( "X-Auth-Key", sptr->Ezi.password ) ))
			{
				liberate( nptr );
				return( liberate_rest_header( hptr ) );
			}
			else	hptr->next->previous = hptr;
	
			if (!( rptr = ezi_client_get_request( nptr, sptr->Ezi.tls, sptr->Ezi.agent, hptr ) ))
			{
				liberate( nptr );
				return( liberate_rest_header( hptr ) );
			}
			else if (!( hptr = rest_resolve_header(rptr->response->first,"X-Auth-Token") ))
			{
				if (!( sptr->Ezi.authenticate = allocate_string("abcde-1234-5678-fgh-ijklm") ))
				{
					liberate( nptr );
					return( (struct rest_header *) 0 );
				}
			}
			else if (!( sptr->Ezi.authenticate = allocate_string( hptr->value ) ))
			{
				liberate( nptr );
				return( (struct rest_header *) 0 );
			}
			else if (( hptr = rest_resolve_header(
					rptr->response->first,"X-Server-Management-Url")) 
					!= (struct rest_header *) 0)
			{
				if (!( sptr->Ezi.base = allocate_string( hptr->value ) ))
				{
					liberate( nptr );
					return( (struct rest_header *) 0 );
				}
			}
			else if (!( hptr = rest_resolve_header(rptr->response->first,"X-Identity") ))
			{
				if (!( sptr->Ezi.base = allocate( strlen( sptr->Ezi.host ) + strlen( sptr->Ezi.version ) + 16 ) ))
				{
					liberate( nptr );
					return( (struct rest_header *) 0 );
				}
				else	sprintf( sptr->Ezi.base, "%s/%s", sptr->Ezi.host, sptr->Ezi.version );
			}
			else if (!( sptr->Ezi.base = allocate_string( hptr->value ) ))
			{
				liberate( nptr );
				return( (struct rest_header *) 0 );
			}
		}
	}
	if (!( sptr->Ezi.authenticate ))
		return((struct rest_header *) 0);
	else if (!( hptr = rest_create_header( "X-Auth-Token", sptr->Ezi.authenticate ) ))
		return( hptr );
	else if (!( hptr->next = rest_create_header( _HTTP_ACCEPT, "text/xml" ) ))
		return( liberate_rest_header( hptr ) );
	else	return((hptr->next->previous = hptr));
}


/*	------------------------------------------------------------	*/
/*			e z i _ d e l e t e _ o p e r a t i o n		*/
/*	------------------------------------------------------------	*/
private	struct	ezi_response *	ezi_delete_operation(struct ezi_subscription * sptr,  char * buffer )
{
	struct	ezi_response	*	rptr=(struct ezi_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	if (!( hptr = ezi_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Ezi.iaas )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = ezi_client_delete_request( nptr, sptr->Ezi.tls, sptr->Ezi.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			e z i _ c r e a t e _  o p e r a t i o n		*/
/*	------------------------------------------------------------	*/
private	struct	ezi_response *	ezi_create_operation(struct ezi_subscription * sptr, char * buffer, char * filename )
{
	struct	ezi_response	*	rptr=(struct ezi_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	if (!( hptr = ezi_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Ezi.iaas )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = ezi_client_post_request( nptr, sptr->Ezi.tls, sptr->Ezi.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}


/*	------------------------------------------------------------	*/
/*			e z i _ c r e a t e _  s e r v e r 		*/
/*	------------------------------------------------------------	*/
public	struct	ezi_response *	ezi_create_server(struct ezi_subscription * sptr, char * filename )
{
	char	buffer[1024];
	sprintf(buffer,"/compound_app");
	return( ezi_create_operation( sptr,  buffer, filename ) );
}
/*	------------------------------------------------------------	*/
/*			e z i _ d e l e t e _ s e r v e r 		*/
/*	------------------------------------------------------------	*/
public	struct	ezi_response *	ezi_delete_server(struct ezi_subscription * sptr,  char * id )
{
	struct	ezi_response	*	rptr=(struct ezi_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	if (!( id ))
		sprintf(buffer,"/compound_app");
	else	sprintf(buffer,"/compound_app/%s",id);
	if (!( hptr = ezi_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Ezi.iaas )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = ezi_client_delete_request( nptr, sptr->Ezi.tls, sptr->Ezi.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			e z i _ g e t _ s e r v e r 			*/
/*	------------------------------------------------------------	*/
public	struct	ezi_response *	ezi_get_server	( struct ezi_subscription * sptr, char * id )
{
	struct	ezi_response	*	rptr=(struct ezi_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	if (!( id ))
		sprintf(buffer,"/compoud_app");
	else	sprintf(buffer,"/compoud_app/%s",id);
	if (!( hptr = ezi_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Ezi.iaas )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = ezi_client_get_request( nptr, sptr->Ezi.tls, sptr->Ezi.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		e z i _ l i b e r a t e _ s u b s c r i p t i o n		*/
/*	------------------------------------------------------------	*/
public	struct	ezi_subscription * ezi_liberate_subscription(struct ezi_subscription * sptr)
{
	if ( sptr )
	{
		liberate( sptr );
	}
	return((struct ezi_subscription *) 0);
}

/*	------------------------------------------------------------	*/
/*		e z i _ a l l o c a t e _ s u b s c r i p t i o n		*/
/*	------------------------------------------------------------	*/
public	struct	ezi_subscription * ezi_allocate_subscription()
{
	struct	ezi_subscription * sptr;
	if (!( sptr = (struct ezi_subscription *) allocate( sizeof( struct ezi_subscription ) ) ))
		return( sptr );
	else
	{
	 	memset( sptr, 0, sizeof( struct ezi_subscription ));
		if (!( sptr->KeyStone.requestauth = allocate_string( "application/xml" ) ))
			return( ezi_liberate_subscription( sptr ) );
		else if (!( sptr->KeyStone.acceptauth = allocate_string( "application/xml" ) ))
			return( ezi_liberate_subscription( sptr ) );
		else 	return( sptr );
	}
}

/*	------------------------------------------------------------	*/
/*		e z i _ i n i t i a l i s e _ c l i e n t 		*/
/*	------------------------------------------------------------	*/
public	struct ezi_subscription * ezi_initialise_client( 
		char * user, char * password, char * tenant, 
		char * host, char * iaas, char * agent, char * version, char * tls )
{
	struct ezi_subscription * sptr=(struct ezi_subscription *) 0;
	char	*	eptr;
	struct	url * 	url;
	char 	buffer[1024];

	if (!( sptr = ezi_allocate_subscription()))
		return( sptr );
	else if (!( url = analyse_url( host )))
		return(ezi_liberate_subscription( sptr ));
	else if (!( sptr->KeyStone.host = serialise_url_host_no_port( url ) ))
		return(ezi_liberate_subscription( sptr ));
	else	liberate_url( url );

	if (!( sptr->Ezi.user = allocate_string( user )))
		return(ezi_liberate_subscription( sptr ) );
	if (!( sptr->Ezi.password = allocate_string( password )))
		return(ezi_liberate_subscription( sptr ));
	if (!( sptr->Ezi.host = allocate_string( host )))
		return(ezi_liberate_subscription( sptr ));
	else if (!( sptr->Ezi.agent = allocate_string( agent )))
		return(ezi_liberate_subscription( sptr ));
	else if (!( sptr->Ezi.version = allocate_string( version )))
		return(ezi_liberate_subscription( sptr ));
	else if (!( sptr->KeyStone.tenantname = allocate_string( tenant )))
		return(ezi_liberate_subscription( sptr ));
	
	sprintf(buffer,"%s/%s",iaas,tenant);

	if (!( sptr->Ezi.iaas = allocate_string( buffer )))
		return(ezi_liberate_subscription( sptr ));

	/* namespace selection */
	if (!( strcmp( sptr->Ezi.version, "v1.0" ) ))
	{
		if (!( sptr->Ezi.namespace = allocate_string(  _OS_NS_COMPUTE_V10 ) ))
			return(ezi_liberate_subscription( sptr ));
	}
	else if (!( strcmp( sptr->Ezi.version, "v1.1" ) ))
	{
	     if (!( sptr->Ezi.namespace = allocate_string(  _OS_NS_COMPUTE_V11 ) ))
		return(ezi_liberate_subscription( sptr ));
	}
	else	return(ezi_liberate_subscription( sptr ));

	sptr->Ezi.authenticate= (char *) 0;
	if (!( tls ))
		sptr->Ezi.tls = (char *) 0;
	else if ((sptr->Ezi.tls = allocate_string(tls)) != (char *) 0)
		if ( (!( strlen( sptr->Ezi.tls ) )) || ( *(sptr->Ezi.tls) == '0' ) )
			sptr->Ezi.tls = liberate( sptr->Ezi.tls );

	return( sptr );
}



	/* ------------ */
#endif	/* _eziclient_c */
	/* ------------ */

