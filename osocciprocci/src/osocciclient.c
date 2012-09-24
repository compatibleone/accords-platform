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
#ifndef	_occi_os_client_c
#define	_occi_os_client_c

#include "osocciclient.h"

struct	occi_os_configuration
{
	char *	requestocci;
	char *	acceptocci;
	char *	requestauth;
	char *	acceptauth;
	char *	authorization;
	char *	tenantid;
	char *	tls;
	char *	agent;
	char *	tenantname;
	char *	user;
	char *	password;
	char *	host;
	int	port;
};

private struct occi_os_configuration OcciConfig = 
{
	"text/occi",
	"text/plain",
	"application/xml",
	"application/xml",
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,
	0
};

private	struct	rest_response * occi_os_failure( struct rest_header * hptr )
{
	hptr = liberate_rest_header( hptr );
	return(( struct rest_response * ) 0 );
}

private	struct	rest_header * occi_os_header(char* nptr, char * vptr)
{
	struct	rest_header * hptr=(struct rest_header *) 0;

	if (!( hptr = allocate_rest_header()))
		return( hptr );
	else if (!( hptr->name = allocate_string( nptr )))
		return( liberate_rest_header( hptr ) );
	else if (!( hptr->value = allocate_string( vptr )))
		return( liberate_rest_header( hptr ) );
	else	return( hptr );
}

/*	-------------------------------------------	*/
/*	 k e y s t o n e _ a u t h _ m e s s a g e 	*/
/*	-------------------------------------------	*/
public	char *	keystone_auth_message( char * user, char * password, char * tenant )
{
	char *	filename;
	FILE *	h;
	if (!( filename = rest_temporary_filename(".xml")))
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
public	int	check_keystone_authorization()
{
	struct	xml_element * document;
	struct	xml_element * eptr;
	struct	xml_atribut * aptr;
	struct	rest_response * rptr;
	struct	rest_header * hptr;
	char *	filename;
	char	buffer[1024];
	if (!( OcciConfig.authorization ))
	{
		sprintf(buffer,"%s:35357/v2.0/tokens",OcciConfig.host);
		if (!( hptr = occi_os_header( _HTTP_CONTENT_TYPE, OcciConfig.requestauth ) ))
			return( 0 );
		else if (!( hptr->next = occi_os_header( _HTTP_ACCEPT, OcciConfig.acceptauth ) ))
		{
			liberate_rest_header( hptr );
			return( 0 );
		}
		else	hptr->next->previous = hptr;
		if (!( filename = keystone_auth_message( 
			OcciConfig.user, 
			OcciConfig.password,
			OcciConfig.tenantname ) ))
		{
			liberate_rest_header( hptr );
			return( 0 );
		}
		else if (!( rptr = rest_client_post_request( 
			buffer, OcciConfig.tls, OcciConfig.agent, filename, hptr ) ))
		{
			liberate_rest_header( hptr );
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
		else if ( strncasecmp( hptr->value, OcciConfig.acceptauth, strlen(OcciConfig.acceptauth) ) )
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
		else if (!( OcciConfig.authorization = allocate_string( aptr->value )))
		{
			document = document_drop( document );
			rptr = liberate_rest_response( rptr );
			return( 0 );
		}
		else if (!( OcciConfig.authorization = occi_unquoted_value( OcciConfig.authorization ) ))
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
		else if (!( OcciConfig.tenantid = allocate_string( aptr->value )))
		{
			document = document_drop( document );
			rptr = liberate_rest_response( rptr );
			return( 0 );
		}
		else if (!( OcciConfig.tenantid = occi_unquoted_value( OcciConfig.tenantid ) ))
		{
			document = document_drop( document );
			rptr = liberate_rest_response( rptr );
			return( 0 );
		}
		else
		{
			document = document_drop( document );
			rptr = liberate_rest_response( rptr );
		}
	}
	return( 1 );
}

/*	-------------------------------------------	*/
/*	k e y s t o n e _ a u t h o r i z a t i o n	*/
/*	-------------------------------------------	*/
public	struct	rest_header * keystone_authorization()
{
	check_keystone_authorization();
	if (!( OcciConfig.authorization ))
		return((struct rest_header *) 0);
	else 	return( occi_os_header( "X-Auth-Token", OcciConfig.authorization ) );
}

/*	-------------------------------------------	*/
/*	k e y s t o n e _ a u t h o r i z a t i o n	*/
/*	-------------------------------------------	*/
public	struct	rest_header * keystone_credentials()
{
	struct	rest_header * hptr;
	struct	rest_header * root=(struct rest_header *) 0;
	struct	rest_header * foot=(struct rest_header *) 0;

	if (!( hptr = keystone_authorization() ))
		return( hptr );
	else	root = foot = hptr;

	/* ---------------------------------------------- */
	/* then the subscription  header if its available */
	/* ---------------------------------------------- */
	if ( OcciConfig.tenantid )
	{
		if (!( hptr = occi_os_header( "X-Auth-Tenant-Id", OcciConfig.tenantid ) ))
			return( liberate_rest_header( root ) );
		else if (!( hptr->previous = foot ))
			root = hptr;
		else	hptr->previous->next = hptr;
		foot = hptr;
	}

	/* ---------------------------------------------- */
	/* then the user logname  header if its available */
	/* ---------------------------------------------- */
	if ( OcciConfig.tenantname )
	{
		if (!( hptr = occi_os_header( "X-Auth-User", OcciConfig.tenantname ) ))
			return( liberate_rest_header( root ) );
		else if (!( hptr->previous = foot ))
			root = hptr;
		else	hptr->previous->next = hptr;
		foot = hptr;
	}

	if ( OcciConfig.acceptocci )
	{
		if (!( hptr = occi_os_header( "Accept", OcciConfig.acceptocci ) ))
			return( liberate_rest_header( root ) );
		else if (!( hptr->previous = foot ))
			root = hptr;
		else	hptr->previous->next = hptr;
		foot = hptr;
	}

	if (!( hptr = occi_os_header( "Content-Type", OcciConfig.requestocci ) ))
		return( liberate_rest_header( root ) );
	else if (!( hptr->previous = foot ))
		root = hptr;
	else	hptr->previous->next = hptr;
	foot = hptr;

	return( root );

}
	

/*	-----------------------------------------------------------	*/
/*	l i b e r a t e _ o c c i _ o s _ c o n f i g u r a t i o n	*/
/*	-----------------------------------------------------------	*/
public	int	liberate_occi_os_configuration(int status)
{
	if ( OcciConfig.host )
		OcciConfig.host = liberate( OcciConfig.host );
	if ( OcciConfig.tls )
		OcciConfig.tls = liberate( OcciConfig.tls );
	if ( OcciConfig.agent )
		OcciConfig.agent = liberate( OcciConfig.agent );
	if ( OcciConfig.user )
		OcciConfig.user = liberate( OcciConfig.user );
	if ( OcciConfig.password )
		OcciConfig.password = liberate( OcciConfig.password );
	if ( OcciConfig.tenantname )
		OcciConfig.tenantname = liberate( OcciConfig.tenantname );
	if ( OcciConfig.tenantid )	
		OcciConfig.tenantid = liberate( OcciConfig.tenantid );
	if ( OcciConfig.authorization )
		OcciConfig.authorization = liberate( OcciConfig.authorization );
	OcciConfig.port = 0;
	return(status);
}

public	int	set_occi_os_configuration( char * host, int port, char * user, char * password, char * tenant, char * agent, char * tls )
{
	liberate_occi_os_configuration(0);

	if ( host )
		if (!( OcciConfig.host = allocate_string( host ) ))
			return( liberate_occi_os_configuration(27) );
	if ( user )
		if (!( OcciConfig.user = allocate_string( user ) ))
			return( liberate_occi_os_configuration(27) );
	if ( password )
		if (!( OcciConfig.password = allocate_string( password ) ))
			return( liberate_occi_os_configuration(27) );
	if ( tenant )
		if (!( OcciConfig.tenantname = allocate_string( tenant ) ))
			return( liberate_occi_os_configuration(27) );
	if ( agent )
		if (!( OcciConfig.agent = allocate_string( agent ) ))
			return( liberate_occi_os_configuration(27) );
	if ( rest_valid_string( tls ) )
	{
		if (!( OcciConfig.tls = allocate_string( tls ) ))
			return( liberate_occi_os_configuration(27) );
	}
	OcciConfig.port = port;
	return( 0 );	
}

/*	--------------------------------------------	*/
/*		o c c i _ o s _ h e a d e r s		*/
/*	--------------------------------------------	*/
private	struct	rest_header * occi_os_headers(char * category, char * content)
{
	struct	rest_header * root=(struct rest_header *) 0;
	struct	rest_header * foot=(struct rest_header *) 0;
	struct	rest_header * hptr=(struct rest_header *) 0;
	struct	rest_header * aptr=(struct rest_header *) 0;

	/* --------------------------------- */
	/* first collect authorization token */
	/* --------------------------------- */
	if (!( aptr = keystone_authorization()))
		return( aptr );

	/* ---------------------------------------------- */
	/* the add in the content type header if provided */
	/* ---------------------------------------------- */
	if ( content )
	{
		if (!( hptr = occi_os_header( "Content-Type", content ) ))
			return( liberate_rest_header( root ) );
		else if (!( hptr->previous = foot ))
			root = hptr;
		else	hptr->previous->next = hptr;
		foot = hptr;

	}

	/* ---------------------------------------------- */
	/* add in the accept header for response messages */
	/* ---------------------------------------------- */
	if ( OcciConfig.acceptocci )
	{
		if (!( hptr = occi_os_header( "Accept", OcciConfig.acceptocci ) ))
			return( liberate_rest_header( root ) );
		else if (!( hptr->previous = foot ))
			root = hptr;
		else	hptr->previous->next = hptr;
		foot = hptr;
	}

	/* ---------------------------------------------- */
	/* then the authorization header if its available */
	/* ---------------------------------------------- */
	if ( OcciConfig.authorization )
	{
		if (!( hptr = aptr ))
			return( liberate_rest_header( root ) );
		else if (!( hptr->previous = foot ))
			root = hptr;
		else	hptr->previous->next = hptr;
		foot = hptr;
	}

	/* ---------------------------------------------- */
	/* then the subscription  header if its available */
	/* ---------------------------------------------- */
	if ( OcciConfig.tenantid )
	{
		if (!( hptr = occi_os_header( "X-Auth-Tenant-Id", OcciConfig.tenantid ) ))
			return( liberate_rest_header( root ) );
		else if (!( hptr->previous = foot ))
			root = hptr;
		else	hptr->previous->next = hptr;
		foot = hptr;
	}

	/* ---------------------------------------------- */
	/* then the user logname  header if its available */
	/* ---------------------------------------------- */
	if ( OcciConfig.tenantname )
	{
		if (!( hptr = occi_os_header( "X-Auth-User", OcciConfig.tenantname ) ))
			return( liberate_rest_header( root ) );
		else if (!( hptr->previous = foot ))
			root = hptr;
		else	hptr->previous->next = hptr;
		foot = hptr;
	}
	/* ---------------------------------------------- */
	/* then the user agent    header if its available */
	/* ---------------------------------------------- */
	if ( OcciConfig.agent )
	{
		if (!( hptr = occi_os_header( "User-Agent", OcciConfig.agent ) ))
			return( liberate_rest_header( root ) );
		else if (!( hptr->previous = foot ))
			root = hptr;
		else	hptr->previous->next = hptr;
		foot = hptr;
	}

	/* ---------------------------------------------- */
	/* then  position the category header if provided */
	/* ---------------------------------------------- */
	if ( category )
	{
		if (!( hptr = occi_os_header( "Category", category ) ))
			return( liberate_rest_header( root ) );
		else if (!( hptr->previous = foot ))
			root = hptr;
		else	hptr->previous->next = hptr;
		foot = hptr;
	}

	return( root );
}

private	struct rest_header * occi_os_compute_headers()
{
	char	buffer[1024];
	sprintf(buffer, "compute; scheme=%chttp://schemas.ogf.org/occi/infrastructure#%c; class=%ckind%c",
			0x0022,0x0022,0x0022,0x0022 );
	return( occi_os_headers( buffer, OcciConfig.requestocci ) );
}

private	struct rest_header * occi_os_storage_headers()
{
	char	buffer[1024];
	sprintf(buffer, "storage; scheme=%chttp://schemas.ogf.org/occi/infrastructure#%c; class=%ckind%c",
			0x0022,0x0022,0x0022,0x0022 );
	return( occi_os_headers( buffer, OcciConfig.requestocci ) );
}

private	struct rest_header * occi_os_instance_action_headers(char * nptr)
{
	char	buffer[1024];
	sprintf(buffer, "%s; scheme=%chttp://schemas.openstack.org/instance/action#%c; class=%caction%c",
			nptr, 0x0022,0x0022,0x0022,0x0022 );
	return( occi_os_headers( buffer, OcciConfig.requestocci ) );
}

private	struct rest_header * occi_os_compute_action_headers(char * nptr)
{
	char	buffer[1024];
	sprintf(buffer, "%s; scheme=%chttp://schemas.ogf.org/occi/infrastructure/compute/action#%c; class=%caction%c",
			nptr, 0x0022,0x0022,0x0022,0x0022 );
	return( occi_os_headers( buffer, OcciConfig.requestocci ) );
}

private	struct rest_header * occi_os_network_headers()
{
	char	buffer[1024];
	sprintf(buffer, "network; scheme=%chttp://schemas.ogf.org/occi/infrastructure#%c; class=%ckind%c",
			0x0022,0x0022,0x0022,0x0022 );
	return( occi_os_headers( buffer, OcciConfig.requestocci ) );
}

private	struct rest_header * occi_os_network_interface_headers()
{
	char	buffer[1024];
	sprintf(buffer, "networkinterface; scheme=%chttp://schemas.ogf.org/occi/infrastructure#%c; class=%ckind%c",
			0x0022,0x0022,0x0022,0x0022 );
	return( occi_os_headers( buffer, OcciConfig.requestocci ) );
}

public	char *	occi_os_category_url( char * term )
{
	char buffer[1024];
	sprintf(buffer,"%s:%u%s",OcciConfig.host,OcciConfig.port,term);
	return( allocate_string( buffer ) );
}

private	char *	occi_os_instance_url( char * term, char * id )
{
	char buffer[1024];
	sprintf(buffer,"%s:%u%s%s",OcciConfig.host,OcciConfig.port,term,id);
	return( allocate_string( buffer ) );
}

private	char *	occi_os_action_url( char * id, char * action )
{
	char buffer[1024];
	sprintf(buffer,"%s?action=%s",id,action);
	return( allocate_string( buffer ) );
}

private	struct	rest_header * occi_os_add_mixin( struct rest_header * root, char * mixin, char * schema )
{
	struct	rest_header * hptr;
	struct	rest_header * foot;
	char buffer[1024];

	if (!( mixin ))
		return( root );
	else if (!( schema ))
		return( root );

	if ((foot = root) != (struct rest_header *) 0)
		while ( foot->next )
			foot = foot->next;

	sprintf(buffer, "%s; scheme=%c%s%c; class=%cmixin%c", mixin, 0x0022,schema,0x0022,0x0022,0x0022 );
	if (!( hptr = occi_os_header( "Category", buffer ) ))
		return( liberate_rest_header( root ) );
	else if (!( hptr->previous = foot ))
		root = hptr;
	else 	hptr->previous->next = hptr;
	foot = hptr;
	return( root );
}

private	struct	rest_header * occi_os_add_kind( struct rest_header * root, char * kind, char * schema )
{
	struct	rest_header * hptr;
	struct	rest_header * foot;
	char buffer[1024];

	if (!( kind ))
		return( root );
	else if (!( schema ))
		return( root );

	if ((foot = root) != (struct rest_header *) 0)
		while ( foot->next )
			foot = foot->next;

	sprintf(buffer, "%s; scheme=%c%s%c; class=%ckind%", kind, 0x0022,schema,0x0022,0x0022,0x0022 );
	if (!( hptr = occi_os_header( "Category", buffer ) ))
		return( liberate_rest_header( root ) );
	else if (!( hptr->previous = foot ))
		root = hptr;
	else 	hptr->previous->next = hptr;
	foot = hptr;
	return( root );
}

private	struct	rest_header * occi_os_add_mixin_rel( struct rest_header * root, char * mixin, char * schema, char * rel )
{
	struct	rest_header * hptr;
	struct	rest_header * foot;
	char buffer[1024];

	if (!( mixin ))
		return( root );
	else if (!( schema ))
		return( root );
	else if (!( rel ))
		return( root );

	if ((foot = root) != (struct rest_header *) 0)
		while ( foot->next )
			foot = foot->next;

	sprintf(buffer, "%s; scheme=%c%s%c; class=%cmixin%c rel=%c%s%c", mixin, 0x0022,schema,0x0022,0x0022,0x0022,0x0022,rel,0x0022 );
	if (!( hptr = occi_os_header( "Category", buffer ) ))
		return( liberate_rest_header( root ) );
	else if (!( hptr->previous = foot ))
		root = hptr;
	else 	hptr->previous->next = hptr;
	foot = hptr;
	return( root );
}

private	struct	rest_header * occi_os_add_attribute( struct rest_header * root, char * name, char * value )
{
	struct	rest_header * hptr;
	struct	rest_header * foot;
	char buffer[1024];

	if ((foot = root) != (struct rest_header *) 0)
		while ( foot->next )
			foot = foot->next;

	sprintf(buffer, "%s=%c%s%c", name, 0x0022,value,0x0022 );
	if (!( hptr = occi_os_header( _OCCI_ATTRIBUTE, buffer ) ))
		return( liberate_rest_header( root ) );
	else if (!( hptr->previous = foot ))
		root = hptr;
	else 	hptr->previous->next = hptr;
	foot = hptr;
	return( root );
}

/* OCCI Capacities
curl -v -X GET localhost:8787/-/
-H 'X-Auth-Token: '$KID 
-H 'X-Auth-Tenant-Id: '$TEN_ID 
-H 'X-Auth-User: '$OS_USER 
*/

public struct	rest_response * occi_os_capacities()
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_headers((char *) 0, (char *) 0)))
		return( occi_os_failure( hptr ));
	else if (!( url = occi_os_category_url( "/-/" ) ))
		return( occi_os_failure( liberate_rest_header( hptr ) ));
	else	return( rest_client_get_request( url, OcciConfig.tls, OcciConfig.agent, hptr ) );
}

/* CREATE COMPUTE instance
curl -v -X POST localhost:8787/compute/ 
-H 'Category: compute; scheme="http://schemas.ogf.org/occi/infrastructure#"; class="kind"' 
-H 'Content-Type: OcciConfig.requestocci' 
-H 'X-Auth-Token: '$KID 
-H 'X-Auth-Tenant-Id: '$TEN_ID 
-H 'X-Auth-User: '$OS_USER 
-H 'Category: itsy; scheme="http://schemas.openstack.org/template/resource#"; class="mixin"' 
-H 'Category: cirros-0.3.0-x86_64-blank; scheme="http://schemas.openstack.org/template/os#"; class="mixin"'
*/

public struct	rest_response * create_occi_os_compute(
		char * machine, char * system, char * machineschema, char * systemschema )
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_compute_headers()))
		return( occi_os_failure( hptr ) );
	else if (!( url = occi_os_category_url( "/compute/" ) ))
		return( occi_os_failure( liberate_rest_header( hptr )) );
	else if (!( hptr = occi_os_add_mixin( hptr, system,
			(systemschema ? systemschema : "http://schemas.openstack.org/template/os") )))
		return( occi_os_failure( hptr ) );
	else if (!( hptr = occi_os_add_mixin( hptr, machine,
			( machineschema ? machineschema : "http://schemas.openstack.org/template/resource") )))
		return( occi_os_failure( hptr ) );
	else	return( rest_client_post_request( url, OcciConfig.tls, OcciConfig.agent, (char *) 0, hptr ) );
}

/* STOP COMPUTE instance
curl -v -X POST "localhost:8787/compute/$VM?action=stop" 
-H 'Content-Type: OcciConfig.requestocci' 
-H 'X-Auth-Token: '$KID 
-H 'X-Auth-Tenant-Id: '$TEN_ID 
-H 'X-Auth-User: '$OS_USER 
-H 'Category: stop; scheme="http://schemas.ogf.org/occi/infrastructure/compute/action#"; class="action"'
*/

public struct	rest_response * stop_occi_os_compute(char * vm)
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_compute_action_headers("stop")))
		return( occi_os_failure( hptr ));
	else if (!( url = occi_os_action_url( vm,"stop" ) ))
		return( occi_os_failure( liberate_rest_header( hptr )) );
	else	return( rest_client_post_request( url, OcciConfig.tls, OcciConfig.agent, (char *) 0, hptr ) );
}

/* START COMPUTE instance
curl -v -X POST localhost:8787/compute/$VM?action=start 
-H 'Content-Type: OcciConfig.requestocci' 
-H 'X-Auth-Token: '$KID 
-H 'X-Auth-Tenant-Id: '$TEN_ID 
-H 'X-Auth-User: '$OS_USER 
-H 'Category: start; scheme="http://schemas.ogf.org/occi/infrastructure/compute/action#"; class="action"'
*/

public struct	rest_response * start_occi_os_compute(char * vm)
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_compute_action_headers("start")))
		return( occi_os_failure( hptr ) );
	else if (!( url = occi_os_action_url( vm,"start" ) ))
		return( occi_os_failure( liberate_rest_header( hptr ) ) );
	else	return( rest_client_post_request( url, OcciConfig.tls, OcciConfig.agent, (char *) 0, hptr ) );
}

/* DELETE COMPUTE instance
curl -v -X DELETE localhost:8787/compute/$VM 
-H 'Content-Type: OcciConfig.requestocci' 
-H 'X-Auth-Token: '$KID 
-H 'X-Auth-Tenant-Id: '$TEN_ID 
-H 'X-Auth-User: '$OS_USER
*/


public struct	rest_response * delete_occi_os_compute(char * id)
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_compute_headers()))
		return( occi_os_failure( hptr ));
	else	return( rest_client_delete_request( id, OcciConfig.tls, OcciConfig.agent, hptr ) );
}

/* DELETE STORAGE instance
curl -v -X DELETE localhost:8787/storage/$VM 
-H 'Content-Type: OcciConfig.requestocci' 
-H 'X-Auth-Token: '$KID 
-H 'X-Auth-Tenant-Id: '$TEN_ID 
-H 'X-Auth-User: '$OS_USER
*/


public struct	rest_response * delete_occi_os_storage(char * id)
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_storage_headers()))
		return( occi_os_failure( hptr ));
	else	return( rest_client_delete_request( id, OcciConfig.tls, OcciConfig.agent, hptr ) );
}

/* CREATE NETWORK
curl -v -X POST localhost:8787/network/ 
-H 'Content-Type: OcciConfig.requestocci' 
-H 'X-Auth-Token: '$KID 
-H 'X-Auth-Tenant-Id: '$TEN_ID 
-H 'X-Auth-User: '$OS_USER 
-H 'Category: network; scheme="http://schemas.ogf.org/occi/infrastructure#"; class="kind"' 
-H 'occi.network.label="www"'
*/

public struct	rest_response * create_occi_os_network(char * label)
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_network_headers()))
		return( occi_os_failure( hptr ));
	else if (!( url = occi_os_category_url( "/network/" ) ))
		return( occi_os_failure( liberate_rest_header( hptr ) ));
	else if (!( hptr = occi_os_add_attribute( hptr, "occi.network.label", label ) ))
		return( occi_os_failure( hptr ));
	else	return( rest_client_post_request( url, OcciConfig.tls, OcciConfig.agent, (char *) 0, hptr ) );
}

/* ATTACH NETWORK to COMPUTE instance
curl -v -X POST localhost:8787/networklink/ 
-H 'Content-Type: OcciConfig.requestocci' 
-H 'X-Auth-Token: '$KID 
-H 'X-Auth-Tenant-Id: '$TEN_ID 
-H 'X-Auth-User: '$OS_USER 
-H 'Category: networkinterface; scheme="http://schemas.ogf.org/occi/infrastructure#"; class="kind"' 
-H 'X-OCCI-Attribute: occi.core.source="http://localhost:8787/compute/'$VM'"' 
-H 'X-OCCI-Attribute: occi.core.target="http://localhost:8787/network/'$NET'"'
*/

public struct	rest_response * attach_occi_os_network(char * vm,char * nw)
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_network_interface_headers()))
		return( occi_os_failure( hptr ));
	else if (!( url = occi_os_category_url( "/networklink/" ) ))
		return( occi_os_failure( liberate_rest_header( hptr ) ));
	else if (!( hptr = occi_os_add_attribute( hptr, "occi.core.source", vm ) ))
		return( occi_os_failure( hptr ));
	else if (!( hptr = occi_os_add_attribute( hptr, "occi.core.target", nw ) ))
		return( occi_os_failure( hptr ) );
	else	return( rest_client_post_request( url, OcciConfig.tls, OcciConfig.agent, (char *) 0, hptr ) );
}

/* GET NETWORK
curl -v -X GET localhost:8787/network/$NET_ID 
-H 'Content-Type: OcciConfig.requestocci' 
-H 'X-Auth-Token: '$KID 
-H 'X-Auth-Tenant-Id: '$TEN_ID 
-H 'X-Auth-User: '$OS_USER
*/

public	struct rest_response * get_occi_os_network( char * id )
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_network_headers() ))
		return( occi_os_failure( hptr ));
	else	return( rest_client_get_request( id, OcciConfig.tls, OcciConfig.agent, hptr ) );
}

/* DELETE NETWORK
curl -v -X DELETE localhost:8787/network/$NET_ID 
-H 'Content-Type: OcciConfig.requestocci' 
-H 'X-Auth-Token: '$KID 
-H 'X-Auth-Tenant-Id: '$TEN_ID 
-H 'X-Auth-User: '$OS_USER
*/

public struct	rest_response * delete_occi_os_network(char * id)
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_network_headers()))
		return( occi_os_failure( hptr ));
 	else	return( rest_client_delete_request( id, OcciConfig.tls, OcciConfig.agent, hptr ) );
}

public int os_occi_initialise_client(char * user,char * password,char * host,char * version,char * agent,char * tls)
{
	struct	url * uptr;
	int	status;
	char * hptr=(char *) 0;
	if (!( uptr = analyse_url( host )))
		return( 0 );
	else if (!( hptr = serialise_url_host_no_port( uptr ) ))
	{
		uptr = liberate_url( uptr );
		return( 0 );
	}
	else
	{
		liberate_occi_os_configuration(0);
		status = set_occi_os_configuration( hptr, uptr->port, user, password, version, agent, tls );
		uptr = liberate_url( uptr );
		hptr = liberate( hptr );
		if ( status )
			return( 0 );
		else	return( 1 );
	}
}

/*	---------------------------------------------------------------------------------	*/
/*				new features being developped					*/
/*	---------------------------------------------------------------------------------	*/

/*	OCCI CREATE SECURITY GROUP
	--------------------------
	curl -v -H 'X-Auth-Token: '$KID 
	-H 'X-Auth-Tenant-Id: '$TEN_ID 
	-H 'X-Auth-User: '$OS_USER 
	-H 'Content-Type: OcciConfig.requestocci' 
	-H 'Category: my_grp; scheme="http://www.mystuff.org/sec#"; class="mixin"; rel="http://schemas.ogf.org/occi/infrastructure/security#group"; location="/mygroups/"' 
	-X POST localhost:8787/-/
 */

public	struct rest_response * create_occi_os_security_group(char * g)
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_headers( (char *) 0, OcciConfig.requestocci ) ))
		return( occi_os_failure( hptr ));
	else if (!( hptr = occi_os_add_mixin_rel( hptr, g, _OCCI_OS_SECURITY, _OCCI_SECURITY_GROUP ) ))
		return( occi_os_failure( hptr ) );
	else if (!( url = occi_os_category_url( "/-/" ) ))
		return( occi_os_failure( liberate_rest_header( hptr )) );
	else	return( rest_client_post_request( url, OcciConfig.tls, OcciConfig.agent, (char *) 0, hptr ) );
}

/*	OCCI CREATE SECURITY RULE
	-------------------------
    	curl -v -X POST localhost:8787/network/security/rule/ 
	-H 'Content-Type: OcciConfig.requestocci' -H 'X-Auth-Token: '$KID 
	-H 'X-Auth-Tenant-Id: '$TEN_ID -H 'X-Auth-User: '$OS_USER 
	-H 'Category: my_grp; scheme="http://www.mystuff.org/sec#"; class="mixin"' 
	-H 'Category: rule; scheme="http://schemas.openstack.org/occi/infrastructure/network/security#"; class="kind"' 
	-H 'X-OCCI-Attribute: occi.network.security.protocol = "TCP"' 
	-H 'X-OCCI-Attribute: occi.network.security.to = 22' 
	-H 'X-OCCI-Attribute: occi.network.security.from = 22' 
	-H 'X-OCCI-Attribute: occi.network.security.range = "0.0.0.0/24"'
*/

public	struct rest_response * create_occi_os_security_rule(char * g, char * protocol,char * from, char * to, char * iprange )
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_headers( (char *) 0, OcciConfig.requestocci ) ))
		return( occi_os_failure( hptr ));
	else if (!( hptr = occi_os_add_mixin( hptr, g, _OCCI_OS_SECURITY )))
		return( occi_os_failure( hptr ) );
	else if (!( hptr = occi_os_add_kind( hptr, "rule", _OCCI_SECURITY_RULE )))
		return( occi_os_failure( hptr ) );
	else if (!( hptr = occi_os_add_attribute(hptr, "occi.network.security.protocol",protocol) ))
		return( occi_os_failure( hptr ));
	else if (!( hptr = occi_os_add_attribute(hptr, "occi.network.security.from",from) ))
		return( occi_os_failure( hptr ));
	else if (!( hptr = occi_os_add_attribute(hptr, "occi.network.security.to",to) ))
		return( occi_os_failure( hptr ));
	else if (!( hptr = occi_os_add_attribute(hptr, "occi.network.security.range",iprange) ))
		return( occi_os_failure( hptr ));
	else if (!( url = occi_os_category_url( "/network/security/rule/" ) ))
		return( occi_os_failure( liberate_rest_header( hptr )) );
	else	return( rest_client_post_request( url, OcciConfig.tls, OcciConfig.agent, (char *) 0, hptr ) );
}

/*	OCCI LIST SECURITY RULES
	------------------------
    	curl -v -X GET localhost:8787/mygroups/ 
	-H 'Content-Type: OcciConfig.requestocci' 
	-H 'X-Auth-Token: '$KID 
	-H 'X-Auth-Tenant-Id: '$TEN_ID 
	-H 'X-Auth-User: '$OS_USER
*/

public	struct rest_response * list_occi_os_security_rules(char * g)
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	char	buffer[1024];
	sprintf(buffer,"/%s/",g);
	if (!( hptr = occi_os_headers( (char *) 0, OcciConfig.requestocci ) ))
		return( occi_os_failure( hptr ));
	else if (!( url = occi_os_category_url( buffer ) ))
		return( occi_os_failure( liberate_rest_header( hptr )) );
	else	return( rest_client_get_request( url, OcciConfig.tls, OcciConfig.agent, hptr ) );
}

/*	OCCI GET SECURITY RULE
	----------------------
	curl -v -H 'X-Auth-Token: '$KID -H 'X-Auth-Tenant-Id: '$TEN_ID -H '
	X-Auth-User: '$OS_USER 
	-X GET http://localhost:8787/network/security/rule/$RULE
*/

public	struct rest_response * get_occi_os_security_rule( char * id )
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_headers( (char *) 0, OcciConfig.requestocci ) ))
		return( occi_os_failure( hptr ));
	else	return( rest_client_get_request( id, OcciConfig.tls, OcciConfig.agent, hptr ) );
}

/* GET COMPUTE
curl -v -X GET localhost:8787/compute/$NET_ID 
-H 'Content-Type: OcciConfig.requestocci' 
-H 'X-Auth-Token: '$KID 
-H 'X-Auth-Tenant-Id: '$TEN_ID 
-H 'X-Auth-User: '$OS_USER
*/

public	struct rest_response * get_occi_os_compute( char * id )
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_compute_headers() ))
		return( occi_os_failure( hptr ));
	else	return( rest_client_get_request( id, OcciConfig.tls, OcciConfig.agent, hptr ) );
}

/* GET STORAGE
curl -v -X GET localhost:8787/storage/$NET_ID 
-H 'Content-Type: OcciConfig.requestocci' 
-H 'X-Auth-Token: '$KID 
-H 'X-Auth-Tenant-Id: '$TEN_ID 
-H 'X-Auth-User: '$OS_USER
*/

public	struct rest_response * get_occi_os_storage( char * id )
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_storage_headers() ))
		return( occi_os_failure( hptr ));
	else	return( rest_client_get_request( id, OcciConfig.tls, OcciConfig.agent, hptr ) );
}

/* 	OCCI DELETE SECURITY RULE
	-------------------------
    	curl -v -H 'X-Auth-Token: '$KID 
	-H 'X-Auth-Tenant-Id: '$TEN_ID 
	-H 'X-Auth-User: '$OS_USER 
	-X DELETE http://localhost:8787/network/security/rule/$RULE
*/

public	struct rest_response * delete_occi_os_security_rule(char * id )
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	char 	buffer[1024];
	if (!( hptr = occi_os_headers( (char *) 0, OcciConfig.requestocci ) ))
		return( occi_os_failure( hptr ));
 	else	return( rest_client_delete_request( id, OcciConfig.tls, OcciConfig.agent, hptr ) );
}

/*	OCCI DELETE SECURITY GROUP
	--------------------------
    	curl -v -H 'X-Auth-Token: '$KID 
	-H 'X-Auth-Tenant-Id: '$TEN_ID 
	-H 'X-Auth-User: '$OS_USER 
	-H 'Content-Type: OcciConfig.requestocci' 
	-H 'Category: my_grp; scheme="http://www.mystuff.org/sec#"; class="mixin"' 
	-X DELETE localhost:8787/-/
*/

public	struct rest_response * delete_occi_os_security_group(char * g)
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	char 	buffer[1024];
	sprintf(buffer,"/security/%s",g );
	if (!( hptr = occi_os_headers( (char *) 0, OcciConfig.requestocci ) ))
		return( occi_os_failure( hptr ));
	else if (!( hptr = occi_os_add_mixin( hptr, g, _OCCI_OS_SECURITY )))
		return( occi_os_failure( hptr ) );
	else if (!( url = occi_os_category_url( "/-/" ) ))
		return( occi_os_failure( liberate_rest_header( hptr )) );
 	else	return( rest_client_delete_request( url, OcciConfig.tls, OcciConfig.agent, hptr ) );
}

/*	OCCI CREATE COMPUTE WITH SECURITY GROUP
	---------------------------------------
    	curl -v -X POST localhost:8787/compute/ 
	-H 'Category: compute; scheme="http://schemas.ogf.org/occi/infrastructure#"; class="kind"' 
	-H 'Content-Type: OcciConfig.requestocci' -H 'X-Auth-Token: '$KID -H 'X-Auth-Tenant-Id: '$TEN_ID 
	-H 'X-Auth-User: '$OS_USER 
	-H 'Category: itsy; scheme="http://schemas.openstack.org/template/resource#"; class="mixin"' 
	-H 'Category: cirros-0.3.0-x86_64-uec; scheme="http://schemas.openstack.org/template/os#"; class="mixin"' 
	-H 'Category: my_grp; scheme="http://www.mystuff.org/sec#"; class="mixin"'
*/

public	struct rest_response * create_occi_os_secure_compute( 
		char * machine, char * system, char * machineschema, char * systemschema, char * g, char * z )
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_compute_headers()))
		return( occi_os_failure( hptr ));
	else if (!( url = occi_os_category_url( "/compute/" ) ))
		return( occi_os_failure( liberate_rest_header( hptr )) );
	else if (!( hptr = occi_os_add_mixin( hptr, system, "http://schemas.openstack.org/template/os" )))
		return( occi_os_failure( hptr ) );
	else if (!( hptr = occi_os_add_mixin( hptr, machine, "http://schemas.openstack.org/template/resource" )))
		return( occi_os_failure( hptr ) );
	else if (!( hptr = occi_os_add_mixin( hptr, g, _OCCI_OS_SECURITY )))
		return( occi_os_failure( hptr ) );
	else	return( rest_client_post_request( url, OcciConfig.tls, OcciConfig.agent, (char *) 0, hptr ) );
}

/*	OCCI ALLOCATE FLOATING IP FOR COMPUTE
	-------------------------------------
    	curl -v -X POST "localhost:8787/compute/$VM?action=alloc_float_ip" 
	-H 'Content-Type: OcciConfig.requestocci' -H 'X-Auth-Token: '$KID -H 'X-Auth-Tenant-Id: '$TEN_ID -H 'X-Auth-User: '$OS_USER 
	-H 'Category: alloc_float_ip; scheme="http://schemas.openstack.org/instance/action#"; class="action"' 
	-H 'X-OCCI-Attribute: org.openstack.network.floating.pool="nova"'
*/

public	struct rest_response * allocate_occi_os_floating_ip( char * id )
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_instance_action_headers("alloc_float_ip")))
		return( occi_os_failure( hptr ));
	else if (!( hptr = occi_os_add_attribute(hptr, "org.openstack.network.floating.pool","nova") ))
		return( occi_os_failure( hptr ));
	else if (!( url = occi_os_action_url( id,"alloc_float_ip" ) ))
		return( occi_os_failure( liberate_rest_header( hptr )) );
	else	return( rest_client_post_request( url, OcciConfig.tls, OcciConfig.agent, (char *) 0, hptr ) );
}

/*	OCCI DE ALLOCATE FLOATING IP OF COMPUTE
	---------------------------------------
    	curl -v -X POST "localhost:8787/compute/$VM?action=dealloc_float_ip" 
	-H 'Content-Type: OcciConfig.requestocci' -H 'X-Auth-Token: '$KID -H 'X-Auth-Tenant-Id: '$TEN_ID 
	-H 'X-Auth-User: '$OS_USER 
	-H 'Category: dealloc_float_ip; scheme="http://schemas.openstack.org/instance/action#"; class="action"'
*/

public	struct rest_response * release_occi_os_floating_ip( char * id )
{
	struct rest_header * hptr=(struct rest_header *) 0;
	char * url;
	if (!( hptr = occi_os_instance_action_headers("dealloc_float_ip")))
		return( occi_os_failure( hptr ));
	else if (!( url = occi_os_action_url( id,"dealloc_float_ip" ) ))
		return( occi_os_failure( liberate_rest_header( hptr )) );
	else	return( rest_client_post_request( url, OcciConfig.tls, OcciConfig.agent, (char *) 0, hptr ) );
}

#endif 	/* _occi_os_client_c */
	/* ----------------- */

