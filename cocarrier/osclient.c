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
#ifndef _os_client_c
#define _os_client_c

#include "osclient.h"

char *                  occi_unquoted_value( char * sptr );

private	int	rate_recovery=120;	/* time to wait for rate limiting recovery	*/
private	int	hack=0;			/* forces the use of the EUCA scripts		*/
private	int	usejson=0;		/* forces use of json message for server create	*/
private	int	quantum_xml=0;		/* use xml for quantum otherwise json		*/
private	int    	use_personality_file=1;	/* forces the use of PERSONALITY FILE in XML	*/

public	void	os_use_json( int v )
{
	usejson = v;
	return;
}

/*	------------------------------------------------------------	*/
/*		l i b e r a t e _ o s _ r e s p o n s e			*/
/*	------------------------------------------------------------	*/
public	struct os_response * liberate_os_response( struct os_response * rptr )	
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
	return((struct os_response *) 0);
}

/*	------------------------------------------------------------	*/
/*		 		o s _ c h e c k 			*/
/*	------------------------------------------------------------	*/
private	struct	os_response * os_check( struct rest_response * aptr )
{
	struct	os_response * rptr=(struct os_response *) 0;
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
	if (!( rptr = allocate( sizeof( struct os_response ) ) ))
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
		else if ((!( strncmp( rptr->content, "text/json" , strlen("text/json")	)))
		     ||  (!( strncmp( rptr->content, "application/json" , strlen("application/json")	)))
		     ||  (!( strncmp( rptr->content, "x-application/json", strlen("x-application/json")))))
		{
			rptr->nature = _TEXT_JSON;
			rptr->jsonroot = json_parse_file( aptr->body );
			return( rptr );
		}
		else if ((!( strncmp( rptr->content, "text/xml" , strlen("text/xml")		)))
		     ||  (!( strncmp( rptr->content, "application/xml", strlen("application/xml") 	)))
		     ||  (!( strncmp( rptr->content, "x-application/xml", strlen("x-application/xml")))))
		{
			rptr->nature = _TEXT_XML;
			rptr->xmlroot = document_parse_file( aptr->body );
			return( rptr );
		}
		else	return( rptr );
		
	}
}

/*	---------------------------------------------------------	*/
/*		o s _ c l i e n t _ r a t e _ l i m i t e d		*/
/*	---------------------------------------------------------	*/
private	int	os_client_rate_limited( struct rest_response * rptr )
{
	if (!( rptr ))
		return(0);
	else if ( rptr->status < 400 )
		return(0);
	else if ( rptr->status != 413 )
		return(0);
	{
		/* -------------------------- */
		/* rate limiting is in effect */
		/* -------------------------- */
		rptr = liberate_rest_response( rptr );
		sleep(rate_recovery);
		return( 1 );
	}
}

/*	------------------------------------------------------------	*/
/*		 o s _ c l i e n t _ g e t _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	os_response * 
	os_client_get_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	struct	rest_response * rptr;
	struct	rest_header   * copy=(struct rest_header *) 0;

	if (( hptr )
	&&  (!( copy = rest_duplicate_headers( hptr ) )))
		return((struct os_response *) 0);

	while ((rptr = rest_client_get_request( target, tls, nptr, hptr )) != (struct rest_response *) 0)
	{
		if (!( os_client_rate_limited( rptr ) ))
			break;
		else if (!( copy ))
			continue;
		else if (!( hptr = rest_duplicate_headers( copy ) ))
			return((struct os_response *) 0);
		else	continue;
	}			
	if (( hptr )
	&&  ( copy ))
		copy = liberate_rest_headers( copy );

	return( os_check( rptr ) );
}

/*	------------------------------------------------------------	*/
/*		 o s _ c l i e n t _ h e a d _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	os_response * 
	os_client_head_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	struct	rest_response * rptr;
	struct	rest_header   * copy=(struct rest_header *) 0;

	if (( hptr )
	&&  (!( copy = rest_duplicate_headers( hptr ) )))
		return((struct os_response *) 0);

	while (( rptr = rest_client_head_request( target, tls, nptr, hptr )) != (struct rest_response *) 0)
	{
		if (!( os_client_rate_limited( rptr ) ))
			break;
		else if (!( copy ))
			continue;
		else if (!( hptr = rest_duplicate_headers( copy ) ))
			return((struct os_response *) 0);
		else	continue;
	}			
	if (( hptr )
	&&  ( copy ))
		copy = liberate_rest_headers( copy );

	return( os_check( rptr ) );
}

/*	------------------------------------------------------------	*/
/*		 o s _ c l i e n t _ c o p y _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	os_response * 
	os_client_copy_request(
		char * target1, char * tls1, char * nptr1, struct rest_header * hptr1,
		char * target2, char * tls2, char * nptr2, struct rest_header * hptr2 )
{
	struct	rest_response * rptr;
	struct	rest_header   * copy1=(struct rest_header *) 0;
	struct	rest_header   * copy2=(struct rest_header *) 0;

	if (( hptr1 )
	&&  (!( copy1 = rest_duplicate_headers( hptr1 ) )))
		return((struct os_response *) 0);

	if (( hptr2 )
	&&  (!( copy2 = rest_duplicate_headers( hptr2 ) )))
		return((struct os_response *) 0);

	while (( rptr = rest_client_copy_request( 
			target1, tls1, nptr1, hptr1, 
			target2, tls2, nptr2, hptr2
			)) != (struct rest_response *) 0)
	{
		if (!( os_client_rate_limited( rptr ) ))
			break;
		if ( copy1 )
		{
			if (!( hptr1 = rest_duplicate_headers( copy1 ) ))
				return((struct os_response *) 0);
		}
		if ( copy2 )
		{
			if (!( hptr2 = rest_duplicate_headers( copy2 ) ))
				return((struct os_response *) 0);
		}
	}			

	if (( hptr1 )
	&&  ( copy1 ))
		copy1 = liberate_rest_headers( copy1 );

	if (( hptr2 )
	&&  ( copy2 ))
		copy2 = liberate_rest_headers( copy2 );

	return( os_check( rptr ) );
}

/*	------------------------------------------------------------	*/
/*	      o s _ c l i e n t _ d e l e t e _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	os_response * 
	os_client_delete_request(
		char * target, char * tls, char * nptr, struct rest_header * hptr )
{
	struct	rest_response * rptr;
	struct	rest_header   * copy=(struct rest_header *) 0;

	if (( hptr )
	&&  (!( copy = rest_duplicate_headers( hptr ) )))
		return((struct os_response *) 0);

	while (( rptr = rest_client_delete_request( target, tls, nptr, hptr )) != (struct rest_response *) 0)
	{
		if (!( os_client_rate_limited( rptr ) ))
			break;
		else if (!( copy ))
			continue;
		else if (!( hptr = rest_duplicate_headers( copy ) ))
			return((struct os_response *) 0);
		else	continue;
	}			

	if (( hptr )
	&&  ( copy ))
		copy = liberate_rest_headers( copy );

	return( os_check( rptr ) );
}

/*	------------------------------------------------------------	*/
/*		 o s _ c l i e n t _ p o s t _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	os_response * 
	os_client_post_request(
		char * target, char * tls, char * nptr, char * filename, struct rest_header * hptr )
{
	struct	rest_response * rptr;
	struct	rest_header   * copy=(struct rest_header *) 0;
	char		      * body=(char *) 0;

	if (( hptr )
	&&  (!( copy = rest_duplicate_headers( hptr ) )))
		return((struct os_response *) 0);

	if (( filename )
	&&  (!( body = allocate_string( filename ) )))
	{
		copy = liberate_rest_headers( copy );
		return((struct os_response *) 0);
	}

	while (( rptr = rest_client_post_request( target, tls, nptr, filename, hptr )) != (struct rest_response *) 0)
	{
		if (!( os_client_rate_limited( rptr ) ))
			break;
		if ( copy  )
			if (!( hptr = rest_duplicate_headers( copy ) ))
				return((struct os_response *) 0);
		if ( body )
			if (!( filename = allocate_string( body ) ))
				return((struct os_response *) 0);

	}			

	if (( filename ) &&  ( body ))
		body = liberate( body );

	if (( hptr ) &&  ( copy ))
		copy = liberate_rest_headers( copy );

	return( os_check( rptr ) );
}

/*	------------------------------------------------------------	*/
/*		 o s _ c l i e n t _ p u t _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	struct	os_response * 
	os_client_put_request(
		char * target, char * tls, char * nptr, char * filename, struct rest_header * hptr )
{
	struct	rest_response * rptr;
	struct	rest_header   * copy=(struct rest_header *) 0;
	char		      * body=(char *) 0;

	if (( hptr )
	&&  (!( copy = rest_duplicate_headers( hptr ) )))
		return((struct os_response *) 0);

	if (( filename )
	&&  (!( body = allocate_string( filename ) )))
	{
		copy = liberate_rest_headers( copy );
		return((struct os_response *) 0);
	}

	while (( rptr = rest_client_put_request( target, tls, nptr, filename, hptr )) != (struct rest_response *) 0)
	{
		if (!( os_client_rate_limited( rptr ) ))
			break;
		if ( copy  )
			if (!( hptr = rest_duplicate_headers( copy ) ))
				return((struct os_response *) 0);
		if ( body )
			if (!( filename = allocate_string( body ) ))
				return((struct os_response *) 0);
	}			

	if (( filename ) && ( body ))
		body = liberate( body );

	if (( hptr ) &&  ( copy ))
		copy = liberate_rest_headers( copy );

	return( os_check( rptr ) );
}


/*	-------------------------------------------	*/
/*	 k e y s t o n e _ a u t h _ m e s s a g e 	*/
/*	-------------------------------------------	*/
public	char *	keystone_auth_message( char * user, char * password, char * tenant )
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
/*		o s _ a d d _ m e t a _ h e a d e r			*/
/*	---------------------------------------------------------	*/
public	struct	rest_header * 	os_add_meta_header( 
		struct rest_header * root,
		char * name,
		char * value 
		)
{
	struct	rest_header * hptr;
	struct	rest_header * foot;
	if (!( foot = root ))
		return( foot );
	else
	{ 	while ( foot->next )
			foot = foot->next;
		if (!( hptr = rest_create_header( name, value ) ))
			return( root );
		else
		{
			foot->next = hptr;
			hptr->previous = foot;
			return( root );
		}
	}
}

/*	---------------------------------------------------------	*/
/*	 c h e c k _ k e y s t o n e _ a u t h o r i z a t i o n	*/
/*	---------------------------------------------------------	*/
public	int	check_keystone_authorization(struct os_subscription * sptr)
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
	if (!( sptr->Os.authenticate ))
	{
		sprintf(buffer,"%s/tokens",sptr->Os.host);
		if (!( hptr = rest_create_header( _HTTP_CONTENT_TYPE, sptr->KeyStone.requestauth ) ))
			return( 0 );
		else if (!( hptr->next = rest_create_header( _HTTP_ACCEPT, sptr->KeyStone.acceptauth ) ))
		{
			liberate_rest_header( hptr );
			return( 0 );
		}
		else	hptr->next->previous = hptr;
		if (!( filename = keystone_auth_message( 
			sptr->Os.user, 
			sptr->Os.password,
			sptr->KeyStone.tenantname ) ))
		{
			liberate_rest_header( hptr );
			return( 0 );
		}
		else if (!( rptr = rest_client_post_request( 
			buffer, sptr->Os.tls, sptr->Os.agent, filename, hptr ) ))
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
		else if (!( sptr->Os.authenticate = allocate_string( aptr->value )))
		{
			document = document_drop( document );
			rptr = liberate_rest_response( rptr );
			return( 0 );
		}
		else if (!( sptr->Os.authenticate = occi_unquoted_value( sptr->Os.authenticate ) ))
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
					else if (!( sptr->Os.base = allocate_string( aptr->value ) ))
					{
						document = document_drop( document );
						rptr = liberate_rest_response( rptr );
						return( 0 );
					}
					else if (!( sptr->Os.base = occi_unquoted_value( sptr->Os.base ) ))
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
				else if (!( strcasecmp( tptr, "network" ) ))
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
					else if (!( sptr->KeyStone.network = allocate_string( aptr->value ) ))
					{
						document = document_drop( document );
						rptr = liberate_rest_response( rptr );
						return( 0 );
					}
					else if (!( sptr->KeyStone.network = occi_unquoted_value( sptr->KeyStone.network ) ))
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
			if (!( sptr->Os.base ))
				return( 0 );
			else	return( 1 );
		}
	}
	return( 1 );
}

/*	------------------------------------------------------------	*/
/*			o s _ a u t h e n t i c a t e ()		*/
/*	------------------------------------------------------------	*/
public	struct	rest_header   *	os_authenticate	(struct os_subscription * sptr)
{
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	struct	os_response	*	rptr;
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
		else if (!( sptr->Os.authenticate ))
			return((struct rest_header * )0);
	}
	else
	{
		/* ------------------------------ */
		/* Old Diablo Type Authentication */
		/* ------------------------------ */
		if (!( sptr->Os.user ))
			return( hptr );
		else if (!( sptr->Os.password ))
			return( hptr );
		else if (!( sptr->Os.version ))
			return( hptr );
		else if (!( sptr->Os.authenticate ))
		{
			sprintf(buffer,"/%s",sptr->Os.version);

			if (!( uptr = analyse_url( sptr->Os.host )))
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



			if (!( hptr = rest_create_header( "X-Auth-User", sptr->Os.user ) ))
			{
				liberate( nptr );
				return( hptr );
			}
			else if (!( hptr->next = rest_create_header( "X-Auth-Key", sptr->Os.password ) ))
			{
				liberate( nptr );
				return( liberate_rest_header( hptr ) );
			}
			else	hptr->next->previous = hptr;
	
			if (!( rptr = os_client_get_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
			{
				liberate( nptr );
				return( liberate_rest_header( hptr ) );
			}
			else if (!( hptr = rest_resolve_header(rptr->response->first,"X-Auth-Token") ))
			{
				if (!( sptr->Os.authenticate = allocate_string("abcde-1234-5678-fgh-ijklm") ))
				{
					liberate( nptr );
					return( (struct rest_header *) 0 );
				}
			}
			else if (!( sptr->Os.authenticate = allocate_string( hptr->value ) ))
			{
				liberate( nptr );
				return( (struct rest_header *) 0 );
			}
			else if (( hptr = rest_resolve_header(
					rptr->response->first,"X-Server-Management-Url")) 
					!= (struct rest_header *) 0)
			{
				if (!( sptr->Os.base = allocate_string( hptr->value ) ))
				{
					liberate( nptr );
					return( (struct rest_header *) 0 );
				}
			}
			else if (!( hptr = rest_resolve_header(rptr->response->first,"X-Identity") ))
			{
				if (!( sptr->Os.base = allocate( strlen( sptr->Os.host ) + strlen( sptr->Os.version ) + 16 ) ))
				{
					liberate( nptr );
					return( (struct rest_header *) 0 );
				}
				else	sprintf( sptr->Os.base, "%s/%s", sptr->Os.host, sptr->Os.version );
			}
			else if (!( sptr->Os.base = allocate_string( hptr->value ) ))
			{
				liberate( nptr );
				return( (struct rest_header *) 0 );
			}
		}
	}
	if (!( sptr->Os.authenticate ))
		return((struct rest_header *) 0);
	else if (!( hptr = rest_create_header( "X-Auth-Token", sptr->Os.authenticate ) ))
		return( hptr );
	else if (!( hptr->next = rest_create_header( _HTTP_ACCEPT, "text/xml" ) ))
		return( liberate_rest_header( hptr ) );
	else	return((hptr->next->previous = hptr));
}

/*	------------------------------------------------------------	*/
/*			o s _ l i s t _ s e r v e r s			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_list_servers	(struct os_subscription * sptr )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	sprintf(buffer,"/servers");

	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		o s _ l i s t _ s e r v e r _ d e t a i l s 		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_list_server_details	(struct os_subscription * sptr )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	sprintf(buffer,"/servers/detail");

	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			o s _ l i s t _ o p e r a t i o n		*/
/*	------------------------------------------------------------	*/
private struct	os_response *	os_list_operation(struct os_subscription * sptr, char * buffer )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*	     o s _ n e t w o r k _ l i s t _ o p e r a t i o n		*/
/*	------------------------------------------------------------	*/
private struct	os_response *	os_network_list_operation(struct os_subscription * sptr, char * buffer )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	char 	url[1024];

	sprintf(url,"v2.0%s",buffer);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->KeyStone.network )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, url ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	---------------------------------------------------------	*/
/*	o s _ n e t w o r k _ r e t r i e v e _ o p e r a t i o n	*/
/*	---------------------------------------------------------	*/
public	struct	os_response *	os_network_retrieve_operation(struct os_subscription *  sptr, char * what )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char				buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	sprintf(buffer,"v2.0%s",what);

	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->KeyStone.network )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}


/*	------------------------------------------------------------	*/
/*			o s _ l i s t _ f l o a t i n g _ i p s		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_list_floating_ips(struct os_subscription * sptr )
{
	return( os_list_operation(sptr, "/os-floating-ips" ) );
}

/*	------------------------------------------------------------	*/
/*			o s _ l i s t _ p o o l s			*/ 
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_list_pools(struct os_subscription * sptr )
{
	return( os_list_operation(sptr, "/os-floating-ip-pools" ) );
}

/*	------------------------------------------------------------	*/
/*	   o s _ l i s t _ f l o a t i n g _ i p _ d e t a i l s	*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_list_floating_ip_details(struct os_subscription * sptr )
{
	return( os_list_operation(sptr, "/os-floating-ips/detail" ) );
}

/*	------------------------------------------------------------	*/
/*		o s _ l i s t _ s e c u r i t y _ g r o u p		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_list_security_groups(struct os_subscription * sptr )
{
	return( os_list_operation(sptr, "/os-security-groups" ) );
}

/*	------------------------------------------------------------	*/
/*	o s _ l i s t _ s e c u r i t y _ g r o u p _ d e t a i l s	*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_list_security_group_details(struct os_subscription * sptr )
{
	return( os_list_operation(sptr, "/os-security-groups/detail" ) );
}

/*	------------------------------------------------------------	*/
/*			o s _ l i s t _ f l a v o u r s			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_list_flavors(struct os_subscription * sptr )
{
	return( os_list_operation(sptr, "/flavors" ) );
}

/*	------------------------------------------------------------	*/
/*		o s _ l i s t _ f l a v o u r _ d e t a i l s		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_list_flavor_details(struct os_subscription * sptr )
{
	return( os_list_operation(sptr, "/flavors/detail" ) );
}

/*	------------------------------------------------------------	*/
/*			o s _ l i s t _ i m a g e s 			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_list_images(struct os_subscription * sptr )
{
	return( os_list_operation(sptr, "/images" ) );
}

/*	------------------------------------------------------------	*/
/*		o s _ l i s t _ i m a g e _ d e t a i l s 		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_list_image_details(struct os_subscription * sptr )
{
	return( os_list_operation(sptr, "/images/detail" ) );
}

/*	------------------------------------------------------------	*/
/*		    o s _ p a r s e _  m e t a d a t a 			*/
/*	------------------------------------------------------------	*/
private	int	os_parse_metadata( char ** rptr, char ** kptr, char ** dptr )
{
	char *	sptr;
	int	c;
	if (!( rptr ))
		return( 0 );
	else if (!( sptr = *rptr ))
		return( 0 );

	while ( ( *sptr == ' ' ) 
	|| 	( *sptr == '\r') 
	|| 	( *sptr == '\n' ) 
	|| 	( *sptr == '\t' ) )
		sptr++;

	if (!( *sptr ))		return(0);
	*kptr = sptr;
	while ( *sptr )
	{
		if ((( *sptr >= 'a' ) && ( *sptr <= 'z' ))
		||  (( *sptr >= 'A' ) && ( *sptr <= 'Z' ))
		||  (( *sptr >= '0' ) && ( *sptr <= '9' ))
		||  (( *sptr == '-' ) || ( *sptr == '_' )))
			sptr++;
	else	break;
	}
	c = *sptr;
	*(sptr++) = 0;
	if ( c != '=' )
	{
		while ( *sptr )
		{
			if ( *sptr != '=' )
				sptr++;
			else
			{
				*(sptr++) = 0;
				break;
			}
		}
		if (!( *sptr )) return( 0 );
	}
	*dptr = sptr;
	while ( ( *sptr == ' ' ) 
	|| 	( *sptr == '\r') 
	|| 	( *sptr == '\n' ) 
	|| 	( *sptr == '\t' ) )
		sptr++;
	if (!( *sptr ))	
	{
		*rptr = sptr;
		return(1);
	}
	while ( *sptr )
	{
		if ( *sptr == ',' )
			break;
		else if ( *sptr == ';' )
			break;
		else if ( *sptr == '\n' )
			break;
		else if ( *sptr == '\r' )
			break;
		else 	sptr++;
	}
	if ( *sptr ) *(sptr++) = 0;
	*rptr = sptr;
	return(1);	
}

/*	-------------------------------------------------------------------	*/
/*	    o s _ c r e a t e _ s e c u r i t y _ g r o u p _ r e q u e s t	*/
/*	-------------------------------------------------------------------	*/
public	char * os_create_security_group_request(struct os_subscription * sptr, char * nptr )
{
	char *	filename;
	FILE *	h;
	struct	rest_header * hptr;
	if (!( hptr = os_authenticate(sptr) ))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<security_group name=%c%s%c>\n",0x0022,nptr,0x0022);
		fprintf(h,"<description>Security Group For %s</description>\n",nptr);
		fprintf(h,"</security_group>\n");
		fclose(h);
		return( filename );
	}
}

/*	-------------------------------------------------------------------	*/
/*	    o s _ c r e a t e _ s e c u r i t y _ r u l e _ r e q u e s t	*/
/*	-------------------------------------------------------------------	*/
public	char * os_create_security_rule_request(struct os_subscription * sptr, char * group, char * protocol, char * from, char * to, char * cidr )
{
	char *	filename;
	FILE *	h;
	struct	rest_header * hptr;
	if (!( hptr = os_authenticate(sptr) ))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<security_group_rule>\n");
		fprintf(h,"<ip_protocol>%s</ip_protocol>\n",protocol);
		fprintf(h,"<from_port>%s</from_port>\n",from);
		fprintf(h,"<to_port>%s</to_port>\n",to);
		fprintf(h,"<cidr>%s</cidr>\n",cidr);
		fprintf(h,"<group/>\n");
		fprintf(h,"<parent_group_id>%s</parent_group_id>\n",group);
		fprintf(h,"</security_group_rule>\n");
		fclose(h);
		return( filename );
	}
}

/*	------------------------------------------------------------	*/
/*	     o s _ c r e a t e _  a d d r e s s _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	char * os_create_address_request(struct os_subscription * sptr, char * address )
{
	char *	filename;
	FILE *	h;
	struct	rest_header * hptr;
	if (!( hptr = os_authenticate(sptr) ))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<addFloatingIp xmlns=%c%s%c>\n",0x0022,sptr->Os.namespace,0x0022);
		fprintf(h,"<address>%s</address>\n",address);
		fprintf(h,"</addFloatingIp>\n");
		fclose(h);
		return( filename );
	}
}
			
/*	------------------------------------------------------------	*/
/*	     o s _ r e m o v e _  a d d r e s s _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	char * os_remove_address_request(struct os_subscription * sptr,char * address )
{
	char *	filename;
	FILE *	h;
	struct	rest_header * hptr;
	if (!( hptr = os_authenticate(sptr) ))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<removeFloatingIp xmlns=%c%s%c>\n",0x0022,sptr->Os.namespace,0x0022);
		fprintf(h,"<address>%s</address>\n",address);
		fprintf(h,"</addFloatingIp>\n");
		fclose(h);
		return( filename );
	}
}
			
/*	------------------------------------------------------------	*/
/*	   o s _ c r e a t e _  m e t a d a t a _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	char * 	os_create_metadata_request(struct os_subscription * subptr, char * personality )
{
	char *	filename;
	FILE *	h;
	int	bytes;
	char *	kptr;
	char *	dptr;
	char *	sptr;
	char *	tptr;
	struct	rest_header * hptr;
	if (!( hptr = os_authenticate(subptr) ))
		return((char *) 0);
	if (!( personality ))
		return( personality );
	else if (!( sptr = allocate_string( personality ) ))
		return( sptr );
	else	tptr = sptr;
	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<metadata xmlns=%c%s%c>\n",0x0022,subptr->Os.namespace,0x0022);
		while ( os_parse_metadata( &sptr, &kptr, &dptr ) != 0)
			fprintf(h,"<meta key=\"%s\">%s</meta>\n",kptr,dptr);
		fprintf(h,"</metadata>\n");
		fclose(h);
		liberate( tptr );
		return( filename );
	}


}

/*	------------------------------------------------------------	*/
/*	   	o s _ c r e a t e _  m e t a _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	char * os_create_meta_request(struct os_subscription * sptr, char * key, char * value )
{
	char *	filename;
	FILE *	h;
	int	bytes;

	struct	rest_header * hptr;
	if (!( hptr = os_authenticate(sptr) ))
		return((char *) 0);
	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<meta xmlns=%c%s%c\n",0x0022,sptr->Os.namespace,0x0022);
		fprintf(h," key=%c%s%c>%s</meta>\n",0x0022,key,0x0022,value);
		fclose(h);
		return( filename );
	}


}

/*	------------------------------------------------------------	*/
/*			o s _ g e t _ a d d r e s s			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_get_address(struct os_subscription * sptr, char * id )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/os-floating-ips/%s",id);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		o s _ g e t _ s e c u r i t y _ g r o u p		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_get_security_group(struct os_subscription * sptr, char * id )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/os-security-groups/%s",id);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		o s _ g e t _ s e r v e r _ m e t a d a t a		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_get_server_metadata(struct os_subscription * sptr, char * id, char * name )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/servers/%s/metadata/%s",id,name);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		o s _ g e t _ i m a g e _ m e t a d a t a		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_get_image_metadata(struct os_subscription * sptr, char * id, char * name )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/images/%s/metadata/%s",id,name);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*	    o s _ u p d a t e _ s e r v e r _ m e t a d a t a		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_update_server_metadata(struct os_subscription * sptr, char * id, char * name, char * value )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	char 			*	filename;
	sprintf(buffer,"/servers/%s/metadata/%s",id,name);
	if (!( filename = os_create_meta_request(sptr, name, value )))
		return( rptr );
	else if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_put_request( nptr, sptr->Os.tls, sptr->Os.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*	    o s _ u p d a t e _ i m a g e _ m e t a d a t a		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_update_image_metadata(struct os_subscription * sptr, char * id, char * name, char * value )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	char 			*	filename;
	sprintf(buffer,"/images/%s/metadata/%s",id,name);
	if (!( filename = os_create_meta_request(sptr, name, value )))
		return( rptr );
	else if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_put_request( nptr, sptr->Os.tls, sptr->Os.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*	    o s _ d e l e t e _ s e r v e r _ m e t a d a t a		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_delete_server_metadata(struct os_subscription * sptr, char * id, char * name )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/servers/%s/metadata/%s",id,name);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_delete_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*	    o s _ d e l e t e _ i m a g e _ m e t a d a t a		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_delete_image_metadata(struct os_subscription * sptr, char * id, char * name )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/images/%s/metadata/%s",id,name);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_delete_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

			/* -------------------------------------- */
#include "eucahack.c"	/* this was necessary for the CO POC 1 V1 */
			/* -------------------------------------- */

/*	------------------------------------------------------------	*/
/*		o s _ b u i l d _ i m a g e _ r e f e r e n c e		*/
/*	------------------------------------------------------------	*/
public	char *	os_build_image_reference(struct os_subscription * sptr, char * nptr )
{
	char	buffer[4096];
	if (!( nptr ))
		return( nptr );
	else if (!( strncmp( nptr, sptr->Os.base, strlen( sptr->Os.base ) ) ))
		return( allocate_string( nptr ) );
	else if (!( strncmp( nptr, "http", strlen( "http" ) ) ))
		return( allocate_string( nptr ) );
	else
	{
		sprintf(buffer,"%s/images/%s",sptr->Os.base,nptr);
		return( allocate_string( buffer ) );
	}
}

/*	------------------------------------------------------------	*/
/*	    o s _ b u i l d _ g l a n c e _ r e f e r e n c e		*/
/*	------------------------------------------------------------	*/
public	char *	os_build_glance_reference(struct os_subscription * sptr, char * nptr )
{
	char	buffer[4096];
	if (!( nptr ))
		return( nptr );
	else if (!( strncmp( nptr, sptr->Os.base, strlen( sptr->Os.base ) ) ))
		return( allocate_string( nptr ) );
	else if (!( strncmp( nptr, "http", strlen( "http" ) ) ))
		return( allocate_string( nptr ) );
	else
	{
		sprintf(buffer,"%s/images/%s",sptr->KeyStone.glance,nptr);
		return( allocate_string( buffer ) );
	}
}

/*	------------------------------------------------------------	*/
/*	    o s _ b u i l d _ f l a v o r _ r e f e r e n c e		*/
/*	------------------------------------------------------------	*/
public	char *	os_build_flavor_reference(struct os_subscription * sptr, char * nptr )
{
	char	buffer[4096];
	if (!( nptr ))
		return( nptr );
	else if (!( strncmp( nptr, sptr->Os.base, strlen( sptr->Os.base ) ) ))
		return( allocate_string( nptr ) );
	else
	{
		sprintf(buffer,"%s/flavors/%s",sptr->Os.base,nptr);
		return( allocate_string( buffer ) );
	}
}

/*	------------------------------------------------------------	*/
/*		o s _ c r e a te _  s e r v e r _ r e q u e s t		*/
/*	------------------------------------------------------------	*/

private	char * os_create_server_xml_request(
		struct os_subscription * subptr,
		char * identity,	/* the identity of the server 	*/
		char * image,		/* the server image identifier  */
		char * flavor,		/* the server machine flavour	*/
		char * address,		/* the public IP address 	*/
		char * personality,	/* the source personality data	*/
		char * resource,	/* the target personality file  */
		char * group,		/* an eventual security group	*/
		char * zone,		/* an eventual locality zone	*/
		char * keyname )	/* an eventual key pair name	*/
{
	char *	sptr=(char *) 0;
	char *	tptr=(char *) 0;
	char *	kptr=(char *) 0;
	char *	dptr=(char *) 0;

	char	encoded[8192];
	char *	eptr=encoded;
	char *	filename;
	FILE *	h;
	int	bytes;
	struct	rest_header * hptr;
	if (!( hptr = os_authenticate(subptr) ))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		/* ---------------------------------------- */
		/* generate server creation request element */
		/* ---------------------------------------- */
		fprintf(h,"<server xmlns=%c%s%c\n",0x0022,subptr->Os.namespace,0x0022);
		if (!( strcmp( subptr->Os.version, "v1.0" ) ))
		{
			fprintf(h,"\timageId=%c%s%c\n",0x0022,image,0x0022);
			fprintf(h,"\tflavorId=%c%s%c\n",0x0022,flavor,0x0022);
		}
		else if (!( strcmp( subptr->Os.version, "v1.1" ) ))
		{
			/* ----------------------------------- */
			/* check if full uri has been provided */
			/* ----------------------------------- */
			if (!( strncmp( image, "http", strlen("http") ) ))
				fprintf(h,"\timageRef=%c%s%c\n",0x0022,image,0x0022);
			else	fprintf(h,"\timageRef=%c%s/images/%s%c\n",0x0022,subptr->Os.base,image,0x0022);

			if (( rest_valid_string( keyname ) )
			&&  (strcmp( keyname, "none" ) != 0 ))
			{
				fprintf(h,"\tkey_name=%c%s%c\n",0x0022,keyname,0x0022);
			}
			fprintf(h,"\tflavorRef=%c%s/flavors/%s%c\n",0x0022,subptr->Os.base,flavor,0x0022);
		}
		else if (!( strcmp( subptr->Os.version, "v2" ) ))
		{
			/* ----------------------------------- */
			/* check if full uri has been provided */
			/* ----------------------------------- */
			if (!( strncmp( image, "http", strlen("http") ) ))
				fprintf(h,"\timageRef=%c%s%c\n",0x0022,image,0x0022);
			else	fprintf(h,"\timageRef=%c%s/images/%s%c\n",0x0022,subptr->Os.base,image,0x0022);


			if (( rest_valid_string( keyname ) )
			&&  ( strcmp( keyname, "none" ) != 0 ))
			{
				fprintf(h,"\tkey_name=%c%s%c\n",0x0022,keyname,0x0022);
			}

			fprintf(h,"\tflavorRef=%c%s/flavors/%s%c\n",0x0022,subptr->Os.base,flavor,0x0022);
		}

		if ( rest_valid_string( address ) )
		{
			fprintf(h,"\taccessIPv4=%c%s%c\n",0x0022,address,0x0022);
		}
		if ( rest_valid_string( zone ) )
		{
			fprintf(h,"\tavailability_zone=%c%s%c\n",0x0022,zone,0x0022);
		}
			
		fprintf(h,"\tname=%c%s%c >\n",0x0022,identity,0x0022);

		if ( rest_valid_string( group ) )
		{
			fprintf(h,"\t<security_groups>\n");
			fprintf(h,"\t<security_group name=%c%s%c/>\n",0x0022,group,0x0022);
			fprintf(h,"\t</security_groups>\n");
		}			

		/* ----------------------------- */
		/* generate meta data statements */
		/* ----------------------------- */
		fprintf(h,"\t<metadata>\n");
		fprintf(h,"\t\t<meta key='ServerName'>%s</meta>\n",identity);
		if ( personality )
		{
			if (( sptr = allocate_string( personality )) != (char *) 0)
			{
				tptr = sptr;
				while ( os_parse_metadata( &sptr, &kptr, &dptr ) != 0)
				{
					fprintf(h,"\t\t<meta key=\"%s\">%s</meta>\n",kptr,dptr);
				}
				liberate(tptr );
			}
		}
		fprintf(h,"\t</metadata>\n");

		/* ---------------------------- */
		/* generate personality section */
		/* ---------------------------- */
		if (( personality ) && ( resource ) && ( use_personality_file ))
		{
			if (( strlen( personality ) )
			&&  ( EncodeBase64( eptr, personality, strlen(personality) ) > 0 ))
			{
				fprintf(h,"<personality>\n");
				fprintf(h,"<file path=%c%s%c>\n",0x0022,resource,0x0022);
				bytes = 0;
				while ( *eptr != 0 )
				{
					if (( bytes ) && (!( bytes % 48 )))
					{
						fprintf(h,"\n");
						bytes = 0;
					}
					fprintf(h,"%c",*(eptr++));
					bytes++;
				}
				if ( bytes ) fprintf(h,"\n");
				fprintf(h,"</file>\n");
				fprintf(h,"</personality>\n");
			}
		}
		fprintf(h,"</server>\n");
		fclose(h);
		return( filename );
	}
}

/*	------------------------------------------------------------	*/
/*		o s _ c r e a te _  s e r v e r _ r e q u e s t		*/
/*	------------------------------------------------------------	*/

private	char * os_create_server_json_request(
		struct os_subscription * subptr,
		char * identity,	/* the identity of the server 	*/
		char * image,		/* the server image identifier  */
		char * flavor,		/* the server machine flavour	*/
		char * address,		/* the public IP address 	*/
		char * personality,	/* the source personality data	*/
		char * resource,	/* the target personality file  */
		char * group,		/* an eventual security group	*/
		char * zone,		/* an eventual locality zone	*/
		char * keyname )	/* an eventual key pair name	*/
{
	char *	sptr=(char *) 0;
	char *	tptr=(char *) 0;
	char *	kptr=(char *) 0;
	char *	dptr=(char *) 0;

	char	encoded[8192];
	char *	eptr=encoded;
	char *	filename;
	FILE *	h;
	int	bytes;
	struct	rest_header * hptr;
	if (!( hptr = os_authenticate(subptr) ))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename("json")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		/* ---------------------------------------- */
		/* generate server creation request element */
		/* ---------------------------------------- */
		fprintf(h,"{\"server\":{\"name\":\"%s\"",identity);

		/* ----------------------------------- */
		/* check if full uri has been provided */
		/* ----------------------------------- */
		if (!( strncmp( image, "http", strlen("http") ) ))
			fprintf(h,",\"imageRef\":%c%s%c",0x0022,image,0x0022);
		else	fprintf(h,",\"imageRef\":%c%s/images/%s%c",0x0022,subptr->Os.base,image,0x0022);

		if (( rest_valid_string( keyname ) )
		&&  (strcmp( keyname, "none" ) != 0 ))
		{
			fprintf(h,",\"key_name\":%c%s%c",0x0022,keyname,0x0022);
		}

		fprintf(h,",\"flavorRef\":%c%s/flavors/%s%c",0x0022,subptr->Os.base,flavor,0x0022);

		if ( rest_valid_string( address ) )
		{
			fprintf(h,",\"accessIPv4\":%c%s%c",0x0022,address,0x0022);
		}
		if ( 1 > 2 )
		{
		if ( rest_valid_string( zone ) )
		{
			fprintf(h,",\"availability_zone\":%c%s%c\n",0x0022,zone,0x0022);
		}
		}
		if ( rest_valid_string( group ) )
		{
			fprintf(h,",\"security_groups\":[");
			fprintf(h,"{\"name\":%c%s%c}]\n",0x0022,group,0x0022);
		}			

		/* ----------------------------- */
		/* generate meta data statements */
		/* ----------------------------- */
		fprintf(h,",\"metadata\": {");
		fprintf(h,"\"ServerName\":%c%s%c",0x0022,identity,0x0022);
		if ( personality )
		{
			if (( sptr = allocate_string( personality )) != (char *) 0)
			{
				tptr = sptr;
				while ( os_parse_metadata( &sptr, &kptr, &dptr ) != 0)
				{
					fprintf(h,",\"%s\":\"%s\"\n",kptr,dptr);
				}
				liberate(tptr );
			}
		}
		fprintf(h,"}\n");

		/* ---------------------------- */
		/* generate personality section */
		/* ---------------------------- */
		if (( personality ) && ( resource ) && ( use_personality_file ))
		{
			if (( strlen( personality ) )
			&&  ( EncodeBase64( eptr, personality, strlen(personality) ) > 0 ))
			{
				fprintf(h,",\"personality\":[\n");
				fprintf(h,"{ \"path\":%c%s%c, \"contents\":\"",0x0022,resource,0x0022);
				bytes = 0;
				while ( *eptr != 0 )
				{
					if (( bytes ) && (!( bytes % 48 )))
					{
						fprintf(h,"\n");
						bytes = 0;
					}
					fprintf(h,"%c",*(eptr++));
					bytes++;
				}
				fprintf(h,"\"}]\n");
			}
		}
		fprintf(h,"}}\n");
		fclose(h);
		return( filename );
	}
}

/*	------------------------------------------------------------	*/
/*		o s _ c r e a te _  s e r v e r _ r e q u e s t		*/
/*	------------------------------------------------------------	*/

public	char * os_create_server_request(
		struct os_subscription * subptr,
		char * identity,	/* the identity of the server 	*/
		char * image,		/* the server image identifier  */
		char * flavor,		/* the server machine flavour	*/
		char * address,		/* the public IP address 	*/
		char * personality,	/* the source personality data	*/
		char * resource,	/* the target personality file  */
		char * group,		/* an eventual security group	*/
		char * zone,		/* an eventual locality zone	*/
		char * keyname )	/* an eventual key pair name	*/
{
	if ( hack )
		return( euca_data_hack( image, flavor, personality ) );
	else if (!( strcmp( subptr->Os.version, "v2" ) ))
		return( os_create_server_json_request( subptr, identity, image, flavor, address, personality, resource, group, zone, keyname ));
	else if ( usejson )
		return( os_create_server_json_request( subptr, identity, image, flavor, address, personality, resource, group, zone, keyname ));
	else	return( os_create_server_xml_request( subptr, identity, image, flavor, address, personality, resource, group, zone, keyname ));
}

/*	------------------------------------------------------------	*/
/*		o s _ c r e a te _ f l a v o r _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	char * os_create_flavor_request(struct os_subscription * sptr, char * identity, char * ram, char * cpus, char * disk )
{
	char *	filename;
	FILE *	h;

	struct	rest_header * hptr;
	if (!( hptr = os_authenticate(sptr) ))
		return((char *) 0);
	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<flavor name=%c%s%c>\n",0x0022,identity,0x0022);
		fprintf(h,"<ram>%s</ram>\n",ram);
		fprintf(h,"<vcpus>%s</vcpus>\n",cpus);
		fprintf(h,"<disk>%s</disk>\n",disk);
		fprintf(h,"</flavor>\n");
		fclose(h);
		return( filename );
	}
}

/*	------------------------------------------------------------	*/
/*		o s _ c r e a te _  i m a g e _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	char * os_create_image_request(struct os_subscription * sptr,char * identity, char * server )
{
	char *	filename;
	FILE *	h;

	struct	rest_header * hptr;
	if (!( hptr = os_authenticate(sptr) ))
		return((char *) 0);
	if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		if (!( strcmp( sptr->Os.version, "v1.0" ) ))
		{
			fprintf(h,"<image xmlns=%c%s%c\n",0x0022,sptr->Os.namespace,0x0022);
			fprintf(h,"\tname=%c%s%c\n",0x0022,identity,0x0022);
			fprintf(h,"\tserverId=%c%s%c />\n",0x0022,server,0x0022);
		}
		else if (!( strcmp( sptr->Os.version, "v1.1" ) ))
		{
			fprintf(h,"<createImage xmlns=%c%s%c\n",0x0022,sptr->Os.namespace,0x0022);
			fprintf(h,"\tname=%c%s%c\n>",0x0022,identity,0x0022);
			fprintf(h,"\t<metadata>\n");
			fprintf(h,"\t\t<meta key=%cactionAgent%c>OS Client</meta>\n",0x0022,0x0022);
			fprintf(h,"\t\t<meta key=%ctimeStamp%c>%u</meta>\n",0x0022,0x0022,time((long *) 0));
			fprintf(h,"\t\t<meta key=%coriginServer%c>%s</meta>\n",0x0022,0x0022,server);
			fprintf(h,"\t</metadata>\n");
			fprintf(h,"</createImage>\n");
		}
		else if (!( strcmp( sptr->Os.version, "v2" ) ))
		{
			fprintf(h,"<createImage xmlns=%c%s%c\n",0x0022,sptr->Os.namespace,0x0022);
			fprintf(h,"\tname=%c%s%c\n>",0x0022,identity,0x0022);
			fprintf(h,"\t<metadata>\n");
			fprintf(h,"\t\t<meta key=%cactionAgent%c>OS Client</meta>\n",0x0022,0x0022);
			fprintf(h,"\t\t<meta key=%ctimeStamp%c>%u</meta>\n",0x0022,0x0022,time((long *) 0));
			fprintf(h,"\t\t<meta key=%coriginServer%c>%s</meta>\n",0x0022,0x0022,server);
			fprintf(h,"\t</metadata>\n");
			fprintf(h,"</createImage>\n");
		}
		fclose(h);
		return( filename );
	}
}

/*	------------------------------------------------------------	*/
/*			o s _ g l a n c e _ a c c e s s   		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_glance_access(struct os_subscription * sptr, char * id, int ispublic )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	sprintf(buffer,"/images/%s",id);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->KeyStone.glance )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	/* ----------------------- */
	/* test for a public image */
	/* ----------------------- */
	if ( ispublic )
	{
		if (!( hptr = os_add_meta_header( hptr, "X-Image-Meta-Is-Public", "True" ) ))
			return( rptr );
	}
	else if (!( hptr = os_add_meta_header( hptr, "X-Image-Meta-Is-Public", "False" ) ))
		return( rptr );
	
	if (!( rptr = os_client_put_request( nptr, sptr->Os.tls, sptr->Os.agent, (char *) 0, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			o s _ c r e a t e _  i m a g e   		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_create_image(struct os_subscription * sptr, char * filename, char * id, int ispublic )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	if (!( strcmp( sptr->Os.version, "v1.0" ) ))
		sprintf(buffer,"/images");
	else if (!( strcmp( sptr->Os.version, "v1.1" ) ))
		sprintf(buffer,"/servers/%s/action",id);
	else if (!( strcmp( sptr->Os.version, "v2" ) ))
		sprintf(buffer,"/servers/%s/action",id);
	else	return( rptr );

	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	/* ----------------------- */
	/* test for a public image */
	/* ----------------------- */
	if (( ispublic )
	&&  (!( hptr = os_add_meta_header( hptr, "x-image-meta-is-public", "true" ) )))
		return( rptr );

	if (!( rptr = os_client_post_request( nptr, sptr->Os.tls, sptr->Os.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			o s _ p o o l _ m e s s a g e 			*/
/*	------------------------------------------------------------	*/
public	char * 	os_pool_message(struct os_subscription * sptr,char * poolname)
{
	FILE * h;
	char *	filename;
	if (!( filename = rest_temporary_filename( "xml" ) ))
		return( filename );
	else if (!( h = fopen( filename,"w") ))
		return( (char *) 0 );
	else
	{
		fprintf(h,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
		fprintf(h,"<pool>%s</pool>\n",(poolname? poolname : "nova"));
		fclose(h);
		return( filename );
	}
}

/*	------------------------------------------------------------	*/
/*			o s _ c r e a t e _  a d d r e s s 		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_create_address(struct os_subscription * sptr)
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	char 			*	filename=(char *) 0;
	char			*	poolname=(char *) 0;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/os-floating-ips");

	if (!( rptr = os_list_pools( sptr ) ))
		return( rptr );
	else if (!( rptr->jsonroot ))
		poolname="nova";
	else if (!( poolname = json_atribut( rptr->jsonroot, "name" ) ))
		poolname = "nova";

	if ( poolname )
		poolname = allocate_string( poolname );

	rest_log_message( "poolname" );
	rest_log_message( poolname );
	rptr = liberate_os_response( rptr );

		
	if (!( filename = os_pool_message( sptr, poolname ) ))
		return( rptr );
	else if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_post_request( nptr, sptr->Os.tls, sptr->Os.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		o s _ c r e a t e _  s e c u r i t y _ g r o u p	*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_create_security_group(struct os_subscription * sptr, char * filename )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/os-security-groups");

	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_post_request( nptr, sptr->Os.tls, sptr->Os.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			o s _ c r e a t e _ f l a v o r 		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_create_flavor(struct os_subscription * sptr, char * filename )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/flavors");

	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_post_request( nptr, sptr->Os.tls, sptr->Os.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		o s _ c r e a t e _  s e c u r i t y _ r u l e 		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_create_security_rule(struct os_subscription * sptr, char * filename )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/os-security-group-rules");

	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_post_request( nptr, sptr->Os.tls, sptr->Os.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			o s _ s e r v e r _ a d d r e s s 		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_server_address(struct os_subscription * sptr, char * filename, char * serverid )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/servers/%s/action",serverid);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_post_request( nptr, sptr->Os.tls, sptr->Os.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			o s _ d e l e t e _ o p e r a t i o n		*/
/*	------------------------------------------------------------	*/
private	struct	os_response *	os_delete_operation(struct os_subscription * sptr,  char * buffer )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_delete_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			o s _ c r e a t e _  o p e r a t i o n		*/
/*	------------------------------------------------------------	*/
private	struct	os_response *	os_create_operation(struct os_subscription * sptr, char * buffer, char * filename )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_post_request( nptr, sptr->Os.tls, sptr->Os.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			o s _ u p d a t e _  o p e r a t i o n		*/
/*	------------------------------------------------------------	*/
private	struct	os_response *	os_update_operation(struct os_subscription * sptr, char * buffer, char * filename )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_put_request( nptr, sptr->Os.tls, sptr->Os.agent,filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*	   o s _ n e t w o r k _ d e l e t e _ o p e r a t i o n	*/
/*	------------------------------------------------------------	*/
private	struct	os_response *	os_network_delete_operation(struct os_subscription * sptr,  char * what )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	char				buffer[1024];
	sprintf(buffer,"v2.0%s",what);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->KeyStone.network )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_delete_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*	   o s _ n e t w o r k _ c r e a t e _  o p e r a t i o n	*/
/*	------------------------------------------------------------	*/
private	struct	os_response *	os_network_create_operation(struct os_subscription * sptr, char * what, char * filename )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	char				buffer[1024];
	sprintf(buffer,"v2.0%s",what);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->KeyStone.network )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_post_request( nptr, sptr->Os.tls, sptr->Os.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*	  o s _ n e t w o r k _ u p d a t e _  o p e r a t i o n	*/
/*	------------------------------------------------------------	*/
private	struct	os_response *	os_network_update_operation(struct os_subscription * sptr, char * what, char * filename )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	char				buffer[1024];
	sprintf(buffer,"v2.0%s",what);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->KeyStone.network )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_put_request( nptr, sptr->Os.tls, sptr->Os.agent,filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}


/*	------------------------------------------------------------	*/
/*			o s _ c r e a t e _ v o l u m e  		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_create_volume(struct os_subscription *  sptr, char * name, char * size, char * type, char * zone )
{
	char	buffer[1024];
	char *	filename;
	FILE *	h;

	if (!( filename = rest_temporary_filename("json")))
		return((struct	os_response *) 0);
	else if (!( h = fopen( filename,"wa" ) ))
		return((struct	os_response *) 0);
	else
	{
		fprintf(h,"{ \"volume\": {\n");
		fprintf(h,"\t\"display_name\":\"%s\",\n",name);
		fprintf(h,"\t\"display_description\":\"%s\",\n","compatibleone volume");
		fprintf(h,"\t\"size\":\"%s\",\n",size);
		fprintf(h,"\t\"volume_type\":\"%s\",\n",type);
		fprintf(h,"\t\"availability_zone\":\"%s\"\n",zone);
		fprintf(h,"}}\n");
		fclose(h);
		sprintf(buffer,"/os-volumes");
		return( os_create_operation( sptr,  buffer, filename ) );
	}
}

/*	------------------------------------------------------------	*/
/*			o s _ d e l e t e _ v o l u m e  		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_delete_volume(struct os_subscription *  sptr, char * id )
{
	char	buffer[1024];
	sprintf(buffer,"/os-volumes/%s",id);
	return( os_delete_operation( sptr,  buffer ) );
}

/*	------------------------------------------------------------	*/
/*			o s _ l i s t _ v o l u m e  			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_list_volumes(struct os_subscription *  sptr, char * id )
{
	char	buffer[1024];
	sprintf(buffer,"/servers/%s/os-volume_attachments", id );
	return( os_list_operation( sptr, buffer ) );
}

/*	------------------------------------------------------------	*/
/*		  o s _ l i s t _ v o l u m e _ t y p e s		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_list_volume_types(struct os_subscription *  sptr )
{
	char	buffer[1024];
	sprintf(buffer,"/os-volume-types" );
	return( os_list_operation( sptr, buffer ) );
}

/*	------------------------------------------------------------	*/
/*		   o s _ g e t _ v o l u m e _ t y p e s		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_get_volume_type(struct os_subscription *  sptr, char * id )
{
	char	buffer[1024];
	sprintf(buffer,"/os-volume-types/%s",id );
	return( os_list_operation( sptr, buffer ) );
}

/*	------------------------------------------------------------	*/
/*			o s _ d e t a i l _ v o l u m e  		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_list_volume_details(struct os_subscription *  sptr, char * id )
{
	char	buffer[1024];
	sprintf(buffer,"/servers/%s/os-volume_attachments/details", id );
	return( os_list_operation( sptr, buffer ) );
}

/*	------------------------------------------------------------	*/
/*			o s _ a t t a c h _ v o l u m e  		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_attach_volume(struct os_subscription *  sptr, char * server, char * vid, char * device )
{
	char	buffer[1024];
	char *	filename;
	FILE *	h;
	sprintf(buffer,"/servers/%s/os-volume_attachments", server );

	if (!( filename = rest_temporary_filename("json")))
		return((struct	os_response *) 0);
	else if (!( h = fopen( filename,"wa" ) ))
		return((struct	os_response *) 0);
	else
	{
		fprintf(h,"{ \"volumeAttachment\": { \"volumeId\":\"%s\", \"device\":\"%s\" } }",vid,device);
		fclose(h);
		return( os_create_operation( sptr,  buffer, filename ) );
	}
}

/*	------------------------------------------------------------	*/
/*			o s _ d e t a c h _  v o l u m e 		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_detach_volume(struct os_subscription *  sptr, char * server, char * volume )
{
	char	buffer[1024];
	sprintf(buffer,"/servers/%s/os-volume_attachments/%s", server, volume );
	return( os_delete_operation(sptr, buffer ) );
}

/*	------------------------------------------------------------	*/
/*			o s _ c r e a t e _  s e r v e r 		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_create_server(struct os_subscription * sptr, char * filename )
{
	char	buffer[1024];
	if ( hack )
		return( os_check( euca_command_hack( filename ) ) );
	else
	{
		sprintf(buffer,"/servers");
		return( os_create_operation( sptr,  buffer, filename ) );
	}
}

/*	------------------------------------------------------------	*/
/*		o s _ c r e a t e _ s e r v e r _ m e t a d a t a	*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_create_server_metadata(struct os_subscription * sptr, char * id, char * filename )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/servers/%s/metadata",id);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_post_request( nptr, sptr->Os.tls, sptr->Os.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

	
/*	------------------------------------------------------------	*/
/*		o s _ c r e a t e _ i m a ge _ m e t a d a t a		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_create_image_metadata(struct os_subscription * sptr, char * id, char * filename )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/images/%s/metadata",id);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_post_request( nptr, sptr->Os.tls, sptr->Os.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		o s _ l i s t  _ s e r v e r _ m e t a d a t a		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_list_server_metadata	(struct os_subscription * sptr,  char * id )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/servers/%s/metadata",id);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*		o s _ l i s t  _ i m a g e _ m e t a d a t a		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_list_image_metadata	(struct os_subscription * sptr,  char * id )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/images/%s/metadata",id);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			o s _ g e t _ s e r v e r 			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_get_server	( struct os_subscription * sptr, char * id )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/servers/%s",id);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}


/*	------------------------------------------------------------	*/
/*			o s _ g e t _ l i m i t s 			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_get_limits( struct os_subscription * sptr )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/limits");
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}


/*	------------------------------------------------------------	*/
/*			o s _ g e t _ f l av o u r			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_get_flavor(struct os_subscription * sptr,  char * id )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/flavors/%s",id);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}


/*	------------------------------------------------------------	*/
/*			o s _ g e t _ i m a g e 			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_get_image 	(struct os_subscription * sptr,  char * id )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/images/%s",id);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			o s _ g e t _ g l an c e 			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_get_glance	(struct os_subscription * sptr,  char * id )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/images/%s",id);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->KeyStone.glance )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			o s _ p o s t _ g l an c e 			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_post_glance	(struct os_subscription * sptr,  char * name, char * format, char * filename )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	struct	rest_header 	*	mptr=(struct rest_header * ) 0;
	struct	rest_header 	*	lptr=(struct rest_header * ) 0;
	strcat(buffer,"/images/");

	if (!( mptr = rest_create_header( "x-image-meta-name", name )))
		return( rptr );
	else if (!( mptr->next = rest_create_header( "x-image-meta-disk-format", format )))
		return( rptr );
	else	mptr->next->previous = mptr;

	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	for (	lptr=hptr;
		lptr->next;
		lptr = lptr->next);
	lptr->next = mptr;
	mptr->previous = lptr;

	if (!( uptr = analyse_url( sptr->KeyStone.glance )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_post_request( nptr, sptr->Os.tls, sptr->Os.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			o s _ c o p y _ g l an c e 			*/
/*	------------------------------------------------------------	*/
/*	performs a GLANCE to GLANCE copy of an OpenStack Images from	*/
/*	one platform as defined by the first subscription to another 	*/
/*	that is defined by the second subscription			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_copy_glance	(struct os_subscription * sptr,  char * id, struct os_subscription * tptr )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	suptr;
	char	sbuffer[1024];
	char 			*	snptr;
	struct	rest_header 	*	shptr=(struct rest_header * ) 0;
	struct	url		*	tuptr;
	char	tbuffer[1024];
	char 			*	tnptr;
	struct	rest_header 	*	thptr=(struct rest_header * ) 0;
	sprintf(sbuffer,"/images/%s",id);
	sprintf(tbuffer,"/images/");
	if (!( shptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( suptr = analyse_url( sptr->KeyStone.glance )))
		return( rptr );
	else if (!( suptr = validate_url( suptr ) ))
		return( rptr );
	else if (!( snptr = serialise_url( suptr, sbuffer ) ))
	{
		suptr = liberate_url( suptr );
		return( rptr );
	}
	if (!( thptr = os_authenticate(tptr) ))
		return( rptr );
	else if (!( tuptr = analyse_url( tptr->KeyStone.glance )))
		return( rptr );
	else if (!( tuptr = validate_url( tuptr ) ))
		return( rptr );
	else if (!( tnptr = serialise_url( tuptr, sbuffer ) ))
	{
		tuptr = liberate_url( tuptr );
		return( rptr );
	}
	else if (!( rptr = os_client_copy_request( 
		snptr, sptr->Os.tls, sptr->Os.agent, shptr,
		tnptr, tptr->Os.tls, tptr->Os.agent, thptr ) ))
	{
		suptr = liberate_url( suptr );
		liberate( snptr );
		tuptr = liberate_url( tuptr );
		liberate( tnptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			o s _ h e a d _ g l an c e 			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_head_glance	(struct os_subscription * sptr,  char * id )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/images/%s",id);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->KeyStone.glance )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_head_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}


/*	------------------------------------------------------------	*/
/*			o s _ u p d a t e _ s e r v e r 		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_update_server(struct os_subscription * sptr,  char * id, char * filename )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/servers/%s",id);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_put_request( nptr, sptr->Os.tls, sptr->Os.agent,filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}


/*	------------------------------------------------------------	*/
/*			o s _ d e l e t e _ s e r v e r 		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_delete_server(struct os_subscription * sptr,  char * id )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/servers/%s",id);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_delete_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			o s _ k e y p a i r _ r e q u e s t		*/
/*	------------------------------------------------------------	*/
public	char *	os_keypair_request( struct os_subscription *  sptr, char * name, char * value )
{
	char *	filename;
	FILE *	h;
	struct	rest_header * hptr;
	if (!( hptr = os_authenticate(sptr) ))
		return((char *) 0);
	else if (!( filename = rest_temporary_filename("xml")))
		return( filename );
	else if (!( h = fopen( filename,"wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",0x0022,0x0022,0x0022,0x0022);
		fprintf(h,"<keypair>\n<name>%s</name>\n",name);
		if ( rest_valid_string( value ) )
		{
			fprintf(h,"<public_key>%s</public_key>\n",value);
		}
		fprintf(h,"</keypair>\n");
		fclose(h);
		return( filename );
	}
}

/*	------------------------------------------------------------	*/
/*			o s _ c r e at e _ k e y p a i r 		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_create_keypair(struct os_subscription *  sptr, char * filename )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/os-keypairs");

	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr,buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_post_request( nptr, sptr->Os.tls, sptr->Os.agent, filename, hptr ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			    o s _ g e t _ k e y p a i r 		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_get_keypair(struct os_subscription *  sptr, char * name )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/os-keypairs/%s",name);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			o s _ d e l e t e _ k e y p a i r 		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_delete_keypair(struct os_subscription *  sptr, char * name )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/os-keypairs/%s",name);
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_delete_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			    o s _ l i s t _ k e y p a i r s 		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_list_keypairs(struct os_subscription *  sptr )
{
	struct	os_response	*	rptr=(struct os_response *) 0;
	struct	url		*	uptr;
	char	buffer[1024];
	char 			*	nptr;
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;
	sprintf(buffer,"/os-keypairs");
	if (!( hptr = os_authenticate(sptr) ))
		return( rptr );
	else if (!( uptr = analyse_url( sptr->Os.base )))
		return( rptr );
	else if (!( uptr = validate_url( uptr ) ))
		return( rptr );
	else if (!( nptr = serialise_url( uptr, buffer ) ))
	{
		uptr = liberate_url( uptr );
		return( rptr );
	}
	else if (!( rptr = os_client_get_request( nptr, sptr->Os.tls, sptr->Os.agent, hptr ) ))
	{
		uptr = liberate_url( uptr );
		liberate( nptr );
		return( rptr );
	}
	else	return( rptr );
}

/*	------------------------------------------------------------	*/
/*			o s _ d e l e t e _ i m a g e 			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_delete_image	(struct os_subscription * sptr,  char * id )
{
	char	buffer[1024];
	sprintf(buffer,"/images/%s",id);
	return( os_delete_operation(sptr, buffer ) );
}

/*	------------------------------------------------------------	*/
/*			o s _ d e l e t e _ f l a v o r			*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_delete_flavor( struct os_subscription * sptr, char * id )
{
	char	buffer[1024];
	sprintf(buffer,"/flavors/%s",id);
	return( os_delete_operation(sptr, buffer ) );
}

/*	------------------------------------------------------------	*/
/*			o s _ d e l e t e _ a d d r e s s		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_delete_address(struct os_subscription * sptr,  char * id )
{
	char	buffer[1024];
	sprintf(buffer,"/os-floating-ips/%s",id);
	return( os_delete_operation(sptr, buffer ) );
}

/*	------------------------------------------------------------	*/
/*	 	o s _ d e l e t e _ s e c u r i t y _ g r o u p		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_delete_security_group(struct os_subscription * sptr,  char * id )
{
	char	buffer[1024];
	sprintf(buffer,"/os-security-groups/%s",id);
	return( os_delete_operation(sptr, buffer ) );
}

/*	------------------------------------------------------------	*/
/*	 	o s _ d e l e t e _ s e c u r i t y _ r u l e 		*/
/*	------------------------------------------------------------	*/
public	struct	os_response *	os_delete_security_rule( struct os_subscription * sptr, char * id )
{
	char	buffer[1024];
	sprintf(buffer,"/os-security-group-rules/%s",id);
	return( os_delete_operation(sptr, buffer ) );
}


/* ----------- */
/* QUANTUM API */
/* ----------- */

/*	-----------------------------------		*/
/*	o s _ n e t w o r k _ m e s s a g e 		*/
/*	-----------------------------------		*/
private	char *	os_network_message( struct os_subscription *  sptr, char * name, int state, int external )
{
	char *	filename;
	FILE *	h;
	if ( quantum_xml )
	{
		if (!( filename = rest_temporary_filename( "xml" ) ))
			return( filename );
		else if (!( h = fopen( filename, "w" ) ))
			return((char *) 0);
		else
		{
			fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",
			0x0022,0x0022,0x0022,0x0022);
			fprintf(h,"<network name=\"%s\" router_external=\"%s\" admin_state_up=\"%s\"/>\n",name,(external?"true":"false"),(state ? "true" : "false"));
			fclose(h);
			return( filename );
		}
	}
	else
	{
		if (!( filename = rest_temporary_filename( "json" ) ))
			return( filename );
		else if (!( h = fopen( filename, "w" ) ))
			return((char *) 0);
		else
		{
			if ( external )
				fprintf(h,"{\"network\":{\"name\":\"%s\",\"router:external\":\"%s\",\"admin_state_up\":\"%s\"}}\n",name,(external ? "true" : "false"),(state ? "true" : "false"));
			else	fprintf(h,"{\"network\":{\"name\":\"%s\",\"admin_state_up\":\"%s\"}}\n",name,(state ? "true" : "false"));
			fclose(h);
			return( filename );
		}
	}
}

/*	-----------------------------------------	*/
/*	o s _ f l o a t i n g i p _ m e s s a g e 	*/
/*	-----------------------------------------	*/
private	char *	os_floatingip_message( struct os_subscription *  sptr, char * netid, char * portid )
{
	char *	filename;
	FILE *	h;
	if ( quantum_xml )
	{
		if (!( filename = rest_temporary_filename( "xml" ) ))
			return( filename );
		else if (!( h = fopen( filename, "w" ) ))
			return((char *) 0);
		else
		{
			fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",
			0x0022,0x0022,0x0022,0x0022);
			fprintf(h,"<floatingips/>");
			fclose(h);
			return( filename );
		}
	}
	else
	{
		if (!( filename = rest_temporary_filename( "json" ) ))
			return( filename );
		else if (!( h = fopen( filename, "w" ) ))
			return((char *) 0);
		else
		{
			if ( rest_valid_string(portid) )
				fprintf(h,"{\"floatingip\":{\"floating_network_id\":\"%s\",\"port_id\":\"%s\"}}\n",netid,portid);
			else	fprintf(h,"{\"floatingip\":{\"floating_network_id\":\"%s\"}}\n",netid);
			fclose(h);
			return( filename );
		}
	}
}

/*	---------------------------------		*/
/*	o s _ s u b n e t _ m e s s a g e 		*/
/*	---------------------------------		*/
private	char *	os_subnet_message( struct os_subscription *  sptr, char * netid, int version, char * cidr )
{
	char *	filename;
	FILE *	h;
	if ( quantum_xml )
	{
		if (!( filename = rest_temporary_filename( "xml" ) ))
			return( filename );
		else if (!( h = fopen( filename, "w" ) ))
			return((char *) 0);
		else
		{
			fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",
				0x0022,0x0022,0x0022,0x0022);
			fprintf(h,"<subnet network_id=\"%s\" version=\"%u\" cidr=\"%s\"/>\n",netid,version,cidr);
			fclose(h);
			return( filename );
		}
	}
	else
	{
		if (!( filename = rest_temporary_filename( "json" ) ))
			return( filename );
		else if (!( h = fopen( filename, "w" ) ))
			return((char *) 0);
		else
		{
			fprintf(h,"{\"subnet\":{\"network_id\":\"%s\",\"version\":\"%u\",\"cidr\":\"%s\"}}\n",netid,version,cidr);
			fclose(h);
			return( filename );
		}
	}
}

/*	-----------------------------		*/
/*	o s _ p o r t _ m e s s a g e 		*/
/*	-----------------------------		*/
private	char *	os_port_message( struct os_subscription *  sptr, char * name, char * netid, char * devid, char * grpid, int state )
{
	char *	filename;
	FILE *	h;
	if ( quantum_xml )
	{
		if (!( filename = rest_temporary_filename( "xml" ) ))
			return( filename );
		else if (!( h = fopen( filename, "w" ) ))
			return((char *) 0);	
		else
		{
			fprintf(h,"<?xml version=%c1.0%c encoding=%cUTF-8%c?>\n",
				0x0022,0x0022,0x0022,0x0022);
			fprintf(h,"<port ");
			if ( rest_valid_string( name ) != 0 )
			{
				fprintf(h," name=\"%s\"",name);
			}
			if ( rest_valid_string( netid ) != 0 )
			{
				fprintf(h," network_id=\"%s\"",netid);
			}
			if ( rest_valid_string( devid ) != 0 )
			{
				fprintf(h," device_id=\"%s\"",devid);
			}
			fprintf(h," admin_state_up=\"%s\">\n",(state ? "true" : "false"));
			if ( rest_valid_string( grpid ) != 0 )
			{
				fprintf(h,"<security_group>%s</security_group>\n",grpid);
			}
			fprintf(h,"</port>\n");
			fclose(h);
			return( filename );
		}
	}
	else
	{
		if (!( filename = rest_temporary_filename( "json" ) ))
			return( filename );
		else if (!( h = fopen( filename, "w" ) ))
			return((char *) 0);	
		else
		{
			fprintf(h,"{\"port\":{");
			fprintf(h,"\"name\":\"%s\"",(name?name:"portname"));
			if ( rest_valid_string( netid ) != 0 )
			{
				fprintf(h,",\"network_id\":\"%s\"",netid);
			}
			if ( rest_valid_string( devid ) != 0 )
			{
				fprintf(h,",\"device_id\":\"%s\"",devid);
			}
			fprintf(h,",\"admin_state_up\":\"%s\"\n",(state ? "true" : "false"));
			if ( rest_valid_string( grpid ) != 0 )
			{
				fprintf(h,",\"security_group\":[\"%s\"]\n",grpid);
			}
			fprintf(h,"}}\n");
			fclose(h);
			return( filename );
		}
	}
}

public	struct	os_response *	os_list_floatingips(struct os_subscription *  sptr)
{
	return( os_network_list_operation( sptr, "/floatingips" ) );
}

public	struct	os_response *	os_create_floatingip(struct os_subscription *  sptr, char * netid, char * portid )
{
	char *	filename;
	if (!( filename = os_floatingip_message( sptr, netid, portid ) ))
		return((struct os_response *) 0);
	else	return( os_network_create_operation( sptr, "/floatingips", filename ) );
}

public	struct	os_response *	os_delete_floatingip(struct os_subscription *  sptr, char * id )
{
	char	buffer[1024];
	sprintf(buffer,"/floatingips/%s",id);
	return( os_network_delete_operation(sptr, buffer ) );
}

public	struct	os_response *	os_retrieve_floatingip(struct os_subscription *  sptr, char * id )
{
	char	buffer[1024];
	sprintf(buffer,"/floatingips/%s",id);
	return( os_network_retrieve_operation(sptr, buffer ) );
}

public	struct	os_response *	os_update_floatingip(struct os_subscription *  sptr, char * id, char * netid, char * portid )
{
	char	buffer[1024];
	sprintf(buffer,"/floatingips/%s",id);
	char *	filename;
	if (!( filename = os_floatingip_message( sptr, netid, portid ) ))
		return((struct os_response *) 0);
	else	return( os_network_update_operation( sptr, buffer, filename ) );
}

/*	---------------------------------------	*/
/*	o s _ l i s t _ n e t w o r k s		*/
/*	---------------------------------------	*/
public	struct	os_response *	os_list_networks(struct os_subscription *  sptr)
{
	return( os_network_list_operation( sptr, "/networks" ) );
}

/*	---------------------------------------	*/
/*	o s _ c r e a t e _ n e t w o r k s 	*/
/*	---------------------------------------	*/
public	struct	os_response *	os_create_network(struct os_subscription *  sptr, char * name, int state, int external )
{
	char *	filename;
	if (!( filename = os_network_message( sptr, name, state, external ) ))
		return((struct os_response *) 0);
	else	return( os_network_create_operation( sptr, "/networks", filename ) );
}

/*	---------------------------------------	*/
/*	o s _ u p d a t e _ n e t w o r k s	*/
/*	---------------------------------------	*/
public	struct	os_response *	os_update_network(struct os_subscription *  sptr, char * netid, char * name, int state, int external )
{
	char	buffer[1024];
	sprintf(buffer,"/networks/%s",netid);
	char *	filename;
	if (!( filename = os_network_message( sptr, name, state, external ) ))
		return((struct os_response *) 0);
	else	return( os_network_update_operation( sptr, buffer, filename ) );
}

/*	---------------------------------------	*/
/*	o s _ d e l e t e _ n e t w o r k s	*/
/*	---------------------------------------	*/
public	struct	os_response *	os_delete_network(struct os_subscription *  sptr, char * netid )
{
	char	buffer[1024];
	sprintf(buffer,"/networks/%s",netid);
	return( os_network_delete_operation(sptr, buffer ) );
}

/*	---------------------------------------	*/
/*	o s _ r e t r i e v e _ n e t w o r k s	*/
/*	---------------------------------------	*/
public	struct	os_response *	os_retrieve_network(struct os_subscription *  sptr, char * netid )
{
	char	buffer[1024];
	sprintf(buffer,"/networks/%s",netid);
	return( os_network_retrieve_operation( sptr, buffer ) );
}


/*	---------------------------------------	*/
/*	o s _ l i s t _ s u b n e t s		*/
/*	---------------------------------------	*/
public	struct	os_response *	os_list_subnets(struct os_subscription *  sptr)
{
	return( os_network_list_operation( sptr, "/subnets" ) );
}

/*	---------------------------------------	*/
/*	o s _ c r e a t e _ s u b n e t s	*/
/*	---------------------------------------	*/
public	struct	os_response *	os_create_subnet(struct os_subscription *  sptr, char * netid, int version, char * cidr )
{
	char *	filename;
	if (!( filename = os_subnet_message( sptr, netid, version, cidr )))
		return((struct os_response *) 0);
	else	return( os_network_create_operation( sptr, "/subnets", filename ) );
}

/*	---------------------------------------	*/
/*	o s _ d e l e t e _ s u b n e t s	*/
/*	---------------------------------------	*/
public	struct	os_response *	os_delete_subnet(struct os_subscription *  sptr, char * subid )
{
	char	buffer[1024];
	sprintf(buffer,"/subnets/%s",subid);
	return( os_network_delete_operation(sptr, buffer ) );
}

/*	---------------------------------------	*/
/*	o s _ r e t r i e v e _ s u b n e t s	*/
/*	---------------------------------------	*/
public	struct	os_response *	os_retrieve_subnet(struct os_subscription *  sptr, char * subid )
{
	char	buffer[1024];
	sprintf(buffer,"/subnets/%s",subid);
	return( os_network_retrieve_operation( sptr, buffer ) );
}

/*	---------------------------------------	*/
/*	o s _ u p d a t e _ s u b n e t s 	*/
/*	---------------------------------------	*/
public	struct	os_response *	os_update_subnet(struct os_subscription *  sptr, char * subid, char * netid, int version, char * cidr )
{
	char	buffer[1024];
	sprintf(buffer,"/subnets/%s",subid);
	char *	filename;
	if (!( filename = os_subnet_message( sptr, netid, version, cidr )))
		return((struct os_response *) 0);
	else	return( os_network_update_operation( sptr, buffer, filename ) );
}



/*	---------------------------------------	*/
/*	o s _ l i s t _ p o r t s 		*/
/*	---------------------------------------	*/
public	struct	os_response *	os_list_ports(struct os_subscription *  sptr)
{
	return( os_network_list_operation( sptr, "/ports" ) );
}

/*	---------------------------------------	*/
/*	o s _ c r e a t e _ p o r t s		*/
/*	---------------------------------------	*/
public	struct	os_response *	os_create_port(struct os_subscription *  sptr, char * name, char * netid, char * devid, char * grpid, int state )
{
	char *	filename;
	if (!( filename = os_port_message( sptr, name, netid, devid, grpid, state ) ))
		return((struct os_response *) 0);
	else	return( os_network_create_operation( sptr, "/ports", filename ) );
}

/*	---------------------------------------	*/
/*	o s _ d e l e t e _ p o r t s 		*/
/*	---------------------------------------	*/
public	struct	os_response *	os_delete_port(struct os_subscription *  sptr, char * portid )
{
	char	buffer[1024];
	sprintf(buffer,"/ports/%s",portid);
	return( os_network_delete_operation(sptr, buffer ) );
}

/*	---------------------------------------	*/
/*	o s _ r e t r i e v e _ p o r t s 	*/
/*	---------------------------------------	*/
public	struct	os_response *	os_retrieve_port(struct os_subscription *  sptr, char * portid )
{
	char	buffer[1024];
	sprintf(buffer,"/ports/%s",portid);
	return( os_network_retrieve_operation( sptr, buffer ) );
}

/*	---------------------------------------	*/
/*	o s _ u p d a t e _ p o r t s		*/
/*	---------------------------------------	*/
public	struct	os_response *	os_update_port(struct os_subscription *  sptr, char * portid, char * name, char * netid, char * devid, char * grpid, int state )
{
	char	buffer[1024];
	sprintf(buffer,"/ports/%s",portid);
	char *	filename;
	if (!( filename = os_port_message( sptr, name, netid, devid, grpid, state ) ))
		return((struct os_response *) 0);
	else	return( os_network_update_operation( sptr, buffer, filename ) );
}



/*	------------------------------------------------------------	*/
/*		o s _ l i b e r a t e _ s u b s c r i p t i o n		*/
/*	------------------------------------------------------------	*/
public	struct	os_subscription * os_liberate_subscription(struct os_subscription * sptr)
{
	if ( sptr )
	{
		liberate( sptr );
	}
	return((struct os_subscription *) 0);
}

/*	------------------------------------------------------------	*/
/*		o s _ a l l o c a t e _ s u b s c r i p t i o n		*/
/*	------------------------------------------------------------	*/
public	struct	os_subscription * os_allocate_subscription()
{
	struct	os_subscription * sptr;
	if (!( sptr = (struct os_subscription *) allocate( sizeof( struct os_subscription ) ) ))
		return( sptr );
	else
	{
	 	memset( sptr, 0, sizeof( struct os_subscription ));
		if (!( sptr->KeyStone.requestauth = allocate_string( "application/xml" ) ))
			return( os_liberate_subscription( sptr ) );
		else if (!( sptr->KeyStone.acceptauth = allocate_string( "application/xml" ) ))
			return( os_liberate_subscription( sptr ) );
		else 	return( sptr );
	}
}

/*	------------------------------------------------------------	*/
/*		o s _ i n i t i a l i s e _ c l i e n t 		*/
/*	------------------------------------------------------------	*/
public	struct os_subscription * os_initialise_client( 
		char * user, char * password, char * tenant, 
		char * host, char * agent, char * version, char * tls )
{
	struct os_subscription * sptr=(struct os_subscription *) 0;
	char	*	eptr;
	struct	url * 	url;

	if (!( sptr = os_allocate_subscription()))
		return( sptr );
	else if ((eptr = getenv("EUCAHACK")) != (char *) 0)
		hack = atoi(eptr);
	else	hack = 0;

	if (!( url = analyse_url( host )))
		return(os_liberate_subscription( sptr ));
	else if (!( sptr->KeyStone.host = serialise_url_host_no_port( url ) ))
		return(os_liberate_subscription( sptr ));
	else	liberate_url( url );

	if (!( sptr->Os.user = allocate_string( user )))
		return(os_liberate_subscription( sptr ) );
	if (!( sptr->Os.password = allocate_string( password )))
		return(os_liberate_subscription( sptr ));
	if (!( sptr->Os.host = allocate_string( host )))
		return(os_liberate_subscription( sptr ));
	else if (!( sptr->Os.agent = allocate_string( agent )))
		return(os_liberate_subscription( sptr ));
	else if (!( sptr->Os.version = allocate_string( version )))
		return(os_liberate_subscription( sptr ));
	else if (!( sptr->KeyStone.tenantname = allocate_string( tenant )))
		return(os_liberate_subscription( sptr ));

	/* namespace selection */
	if (!( strcmp( sptr->Os.version, "v1.0" ) ))
	{
		if (!( sptr->Os.namespace = allocate_string(  _OS_NS_COMPUTE_V10 ) ))
			return(os_liberate_subscription( sptr ));
	}
	else if (!( strcmp( sptr->Os.version, "v1.1" ) ))
	{
	     if (!( sptr->Os.namespace = allocate_string(  _OS_NS_COMPUTE_V11 ) ))
		return(os_liberate_subscription( sptr ));
	}
	else if (!( strcmp( sptr->Os.version, "v2" ) ))
	{
	     if (!( sptr->Os.namespace = allocate_string(  _OS_NS_COMPUTE_V2 ) ))
		return(os_liberate_subscription( sptr ));
	}
	else	return(os_liberate_subscription( sptr ));

	sptr->Os.authenticate= (char *) 0;
	if (!( tls ))
		sptr->Os.tls = (char *) 0;
	else if ((sptr->Os.tls = allocate_string(tls)) != (char *) 0)
		if ( (!( strlen( sptr->Os.tls ) )) || ( *(sptr->Os.tls) == '0' ) )
			sptr->Os.tls = liberate( sptr->Os.tls );

	return( sptr );
}

#endif	/* _os_client_c */
	/* ------------ */



