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
#ifndef	_dcclient_c
#define	_dcclient_c

// ---------------------------------------
// DELTA CLOUD API CLIENT MODULE
// ---------------------------------------
#include "dcclient.h"
#include "restclient.h"

#define	_CORDS_NULL "(null)"
#define	_CORDS_NONE "none"

struct	dc_api_configuration
{
	char *	authorization;
	char *	tls;
	char *	agent;
	char *	tenant;
	char *	user;
	char *	password;
	char *	host;
};

private struct dc_api_configuration DeltaCloudConfig = 
{
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,
	(char *) 0,
	0
};

/*	------------------------------------------------	*/
/*		d c _ v a l i d _ s t r i n g			*/
/*	------------------------------------------------	*/
private	int	dc_valid_string( char * vptr )
{
	if (!( vptr ))
		return( 0 );
	else if (!( strlen( vptr ) ))
		return( 0 );
	else if (!( strcmp( vptr, _CORDS_NULL ) ))
		return( 0 );
	else if (!( strcmp( vptr, _CORDS_NONE ) ))
		return( 0 );
	else	return( 1 );
}

/*	------------------------------------------------------------	*/
/*	 l i b e r a t e _ d c _ a p i _ c o n f i g u r a t i o n	*/
/*	------------------------------------------------------------	*/
public	int	liberate_dc_api_configuration(int status)
{
	if ( DeltaCloudConfig.host )
		DeltaCloudConfig.host = liberate( DeltaCloudConfig.host );
	if ( DeltaCloudConfig.tls )
		DeltaCloudConfig.tls = liberate( DeltaCloudConfig.tls );
	if ( DeltaCloudConfig.agent )
		DeltaCloudConfig.agent = liberate( DeltaCloudConfig.agent );
	if ( DeltaCloudConfig.user )
		DeltaCloudConfig.user = liberate( DeltaCloudConfig.user );
	if ( DeltaCloudConfig.password )
		DeltaCloudConfig.password = liberate( DeltaCloudConfig.password );
	if ( DeltaCloudConfig.tenant )
		DeltaCloudConfig.tenant = liberate( DeltaCloudConfig.tenant );
	if ( DeltaCloudConfig.authorization )
		DeltaCloudConfig.authorization = liberate( DeltaCloudConfig.authorization );
	return(status);
}

/*	------------------------------------------------------------	*/
/*		   d c _ a p i _ c o n f i g u r a t i o n		*/
/*	------------------------------------------------------------	*/
public	int	dc_api_configuration( char * host, char * user, char * password, char * tenant, char * agent, char * tls )
{
	liberate_dc_api_configuration(0);

	if ( host )
		if (!( DeltaCloudConfig.host = allocate_string( host ) ))
			return( liberate_dc_api_configuration(27) );
	if ( user )
		if (!( DeltaCloudConfig.user = allocate_string( user ) ))
			return( liberate_dc_api_configuration(27) );
	if ( password )
		if (!( DeltaCloudConfig.password = allocate_string( password ) ))
			return( liberate_dc_api_configuration(27) );
	if ( tenant )
		if (!( DeltaCloudConfig.tenant = allocate_string( tenant ) ))
			return( liberate_dc_api_configuration(27) );
	if ( agent )
		if (!( DeltaCloudConfig.agent = allocate_string( agent ) ))
			return( liberate_dc_api_configuration(27) );
	if ( tls )
		if (!( DeltaCloudConfig.tls = allocate_string( tls ) ))
			return( liberate_dc_api_configuration(27) );

	return( 0 );	
}

/*	------------------------------------------------------------	*/
/*			d c _ a p i _ f a i l u r e 			*/
/*	------------------------------------------------------------	*/
private struct rest_response * dc_api_failure( struct rest_header * hptr )
{
	liberate_rest_header( hptr );
	return((struct rest_response *) 0);
}

/*	------------------------------------------------------------	*/
/*			d c _ a u t h e n t i c a t e 			*/
/*	------------------------------------------------------------	*/
public	struct	rest_header   *	dc_authenticate	( )
{
	struct	rest_header 	*	hptr=(struct rest_header * ) 0;

	if (!( DeltaCloudConfig.user ))
		return((struct rest_header *) 0);
	else if (!( DeltaCloudConfig.password ))
		return((struct rest_header *) 0);
	else if (!( DeltaCloudConfig.authorization ))
		if (!( DeltaCloudConfig.authorization = rest_encode_credentials( DeltaCloudConfig.user, DeltaCloudConfig.password ) ))
			return((struct rest_header *) 0);

	if (!( DeltaCloudConfig.authorization ))
		return((struct rest_header *) 0);
	else if (!( hptr = rest_create_header( "Authorization", DeltaCloudConfig.authorization ) ))
		return( hptr );
	else if (!( hptr->next = rest_create_header( _HTTP_ACCEPT, "*/*" ) ))
		return( liberate_rest_header( hptr ) );
	else	return((hptr->next->previous = hptr));
}

/*	------------------------------------------------------------	*/
/*			d c _ c o n t e n t _ t y p e			*/
/*	------------------------------------------------------------	*/
public	struct	rest_header   *	dc_content_type( struct rest_header * hptr, char * filename )
{
	struct	rest_header * wptr;
	struct	rest_header * root;
	if (!( filename ))
		return( hptr );
	else
	{
		root = hptr->next;
		if (!( root->next = rest_create_header( "Content-Type", "application/x-www-form-urlencoded" ) ))
			return( hptr );
		else
		{
		 	root->next->previous = root;
			return( hptr );
		}
	}
}

// ---------------------------------------
// REST API INTERFACE FUNCTION
// ---------------------------------------
// REST GET REQUEST URL
// ---------------------------------------
private struct rest_response * dc_get_request(char * nptr)
{
	char	url[_DC_BUFFERSIZE];
	struct rest_header * hptr=(struct rest_header *) 0;
	sprintf(url,"%s%s?format=xml",DeltaCloudConfig.host,nptr);
	if (!( hptr = dc_authenticate()))
		return( dc_api_failure( hptr ) );
	else	return( rest_client_get_request( url, DeltaCloudConfig.tls, DeltaCloudConfig.agent, hptr ) );
}

// ---------------------------------------
// REST DELETE REQUEST URL
// ---------------------------------------
private struct rest_response * dc_delete_request(char * nptr)
{
	char	url[_DC_BUFFERSIZE];
	struct rest_header * hptr=(struct rest_header *) 0;
	sprintf(url,"%s%s?format=xml",DeltaCloudConfig.host,nptr);
	if (!( hptr = dc_authenticate()))
		return( dc_api_failure( hptr ) );
	else	return( rest_client_delete_request( url, DeltaCloudConfig.tls, DeltaCloudConfig.agent, hptr ) );
}

// ---------------------------------------
// REST POST REQUEST URL
// ---------------------------------------
private struct rest_response * dc_post_request(char * nptr,char * filename)
{
	char	url[_DC_BUFFERSIZE];
	struct rest_header * hptr=(struct rest_header *) 0;
	sprintf(url,"%s%s?format=xml",DeltaCloudConfig.host,nptr);
	if (!( hptr = dc_authenticate()))
		return( dc_api_failure( hptr ) );
	else if (!( hptr = dc_content_type( hptr, filename ) ))
		return( dc_api_failure( hptr ) );
	else	return( rest_client_post_request( url, DeltaCloudConfig.tls, DeltaCloudConfig.agent, filename, hptr ) );
}

// ---------------------------------------
// REST PUT REQUEST URL
// ---------------------------------------
private struct rest_response * dc_put_request(char * nptr,char * filename)
{
	char	url[_DC_BUFFERSIZE];
	struct rest_header * hptr=(struct rest_header *) 0;
	sprintf(url,"%s%s?format=xml",DeltaCloudConfig.host,nptr);
	if (!( hptr = dc_authenticate()))
		return( dc_api_failure( hptr ) );
	else if (!( hptr = dc_content_type( hptr, filename ) ))
		return( dc_api_failure( hptr ) );
	else	return( rest_client_put_request( url, DeltaCloudConfig.tls, DeltaCloudConfig.agent, filename, hptr ) );
}

// ---------------------------------------
// 1. API
// ---------------------------------------
// GET /api
// ---------------------------------------
public struct rest_response * dc_list_features()
{
	return(dc_get_request("/api"));
}

// ---------------------------------------
// 3. Compute Resources
// ---------------------------------------
// 3.1 Realms
// ---------------------------------------
// GET /api/realms
// ---------------------------------------
public struct rest_response * dc_list_realms() 
{ 
	return(dc_get_request("/api/realms"));
}

// ---------------------------------------
// GET /api/realms/:id
// ---------------------------------------
public struct rest_response * dc_get_realm(char * id) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/realms/%s",id);
	return(dc_get_request(buffer));
}

// ---------------------------------------
// 3.2 Hardware Profiles
// ---------------------------------------
// GET /api/hardware_profiles
// ---------------------------------------
public struct rest_response * dc_list_profiles() 
{ 
	return(dc_get_request("/api/hardware_profiles"));
}

// ---------------------------------------
// GET /api/hardware profiles/:id
// ---------------------------------------
public struct rest_response * dc_get_profile(char * id) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/hardware_profiles/%s",id);
	return(dc_get_request(buffer));
}

// ---------------------------------------
// 3.3 Images
// ---------------------------------------
// GET /api/images
// ---------------------------------------
public struct rest_response * dc_list_images() 
{ 
	return(dc_get_request("/api/images"));
}

// ---------------------------------------
// GET /api/images/:id
// ---------------------------------------
public struct rest_response * dc_get_image(char * id) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/images/%s",id);
	return(dc_get_request(buffer));
}

// ---------------------------------------
// CREATE IMAGE MESSAGE
// ---------------------------------------
public char * dc_create_image_message( char * instance, char * name, char * description )
{
	char *	filename;
	FILE *	h;
	if (!( filename = rest_temporary_filename("form")))
		return( filename );
	else if (!( h = fopen( filename, "wa" ) ))
		return( liberate( filename ) );
	else
	{

		fprintf(h,"instance_id=%s",instance);

		if ( dc_valid_string(name) )
			fprintf(h,"&name=%s",name);

		if ( dc_valid_string(description) )
			fprintf(h,"&description=%s",description);

		fclose(h);
		return( filename );
	}
}

// ---------------------------------------
// POST /api/images
// ---------------------------------------
public struct rest_response * dc_create_image(char * filename) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/images");
	return(dc_post_request(buffer,filename));
}

// ---------------------------------------
// DELETE /api/images/:id
// ---------------------------------------
public struct rest_response * dc_delete_image(char * id) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/images/%s",id);
	return(dc_delete_request(buffer));
}

// ---------------------------------------
// 3.4 Instance States
// ---------------------------------------
// GET /api/instance_states
// ---------------------------------------
public struct rest_response * dc_list_states() 
{ 
	return(dc_get_request("/api/instance_states"));
}

// ---------------------------------------
// 3.5 Instances
// ---------------------------------------
// GET /api/instances
// ---------------------------------------
public struct rest_response * dc_list_instances() 
{ 
	return(dc_get_request("/api/instances"));
}

// ---------------------------------------
// GET /api/instances/:id
// ---------------------------------------
public struct rest_response * dc_get_instance(char * id) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/instances/%s",id);
	return(dc_get_request(buffer));
}

// ---------------------------------------
// POST /api/instances/:id/:action
// ---------------------------------------
public struct rest_response * dc_instance_action(char * id, char * action) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/instances/%s/%s",id,action);
	return(dc_post_request(buffer,(char *) 0));
}

// ---------------------------------------
// START /api/instances/:id
// ---------------------------------------
public struct rest_response * dc_start_instance( char * id )
{
	return( dc_instance_action( id, "start" ) );
}

// ---------------------------------------
// STOP /api/instances/:id
// ---------------------------------------
public struct rest_response * dc_stop_instance( char * id )
{
	return( dc_instance_action( id, "stop" ) );
}


// ---------------------------------------
// CREATE INSTANCE PARAMETERS
// ---------------------------------------
public char * dc_create_instance_message( char * name, char * profile, char * image, char * firewall, char * zone )
{
	char *	filename;
	FILE *	h;
	if (!( filename = rest_temporary_filename("form")))
		return( filename );
	else if (!( h = fopen( filename, "wa" ) ))
		return( liberate( filename ) );
	else
	{

		fprintf(h,"image_id=%s",image);

		if ( dc_valid_string(name) )
			fprintf(h,"&name=%s",name);

		if ( dc_valid_string(profile) )
			fprintf(h,"&hwp_id=%s",profile);

		if ( dc_valid_string(firewall) )
			fprintf(h,"&firewalls1=%s",firewall);

		if ( dc_valid_string(zone) )
			fprintf(h,"&realm_id=%s",zone);

		fclose(h);
		return( filename );
	}
}

// ---------------------------------------
// POST /api/instances
// ---------------------------------------
public struct rest_response * dc_create_instance( char * filename ) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/instances");
	return(dc_post_request(buffer,filename));
}

// ---------------------------------------
// DELETE /api/instances/:id
// ---------------------------------------
public struct rest_response * dc_delete_instance( char * id )
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/instances/%s",id);
	return(dc_delete_request(buffer));
}

// ---------------------------------------
// 3.6 Keys
// ---------------------------------------
// GET /api/keys
// ---------------------------------------
public struct rest_response * dc_list_keys() 
{ 
	return(dc_get_request("/api/keys"));
}

// ---------------------------------------
// GET /api/keys/:id
// ---------------------------------------
public struct rest_response * dc_get_key(char * id) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/keys/%s",id);
	return(dc_get_request(buffer));
}

// ---------------------------------------
// CREATE KEY MESSAGE
// ---------------------------------------
public char * dc_create_key_message( char * name )
{
	char *	filename;
	FILE *	h;
	if (!( filename = rest_temporary_filename("form")))
		return( filename );
	else if (!( h = fopen( filename, "wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"name=%s",name);
		fclose(h);
		return( filename );
	}
}

// ---------------------------------------
// POST /api/keys
// ---------------------------------------
public struct rest_response * dc_create_key(char * filename) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/keys");
	return(dc_post_request(buffer,filename));
}

// ---------------------------------------
// DELETE /api/keys/:id
// ---------------------------------------
public struct rest_response * dc_delete_key(char * id) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/keys/%s",id);
	return(dc_delete_request(buffer));
}

// ---------------------------------------
// 3.7 Firewalls
// ---------------------------------------
// GET /api/firewalls
// ---------------------------------------
public struct rest_response * dc_list_firewalls() 
{ 
	return(dc_get_request("/api/firewalls"));
}

// ---------------------------------------
// GET /api/firewalls/:id
// ---------------------------------------
public struct rest_response * dc_get_firewall(char * id) 
{ 	
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/firewalls/%s",id);
	return(dc_get_request(buffer));
}

// ---------------------------------------
// CREATE FIREWALL MESSAGE
// ---------------------------------------
public char * dc_create_firewall_message( char * name, char * description )
{
	char *	filename;
	FILE *	h;
	if (!( filename = rest_temporary_filename("form")))
		return( filename );
	else if (!( h = fopen( filename, "wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"name=%s",name);

		if ( dc_valid_string(description) )
			fprintf(h,"&description=%s",description);

		fclose(h);
		return( filename );
	}
}

// ---------------------------------------
// POST /api/firewalls
// ---------------------------------------
public struct rest_response * dc_create_firewall(char * filename) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/firewalls");
	return(dc_post_request(buffer,filename));
}

// ---------------------------------------
// DELETE /api/firewalls/:id
// ---------------------------------------
public struct rest_response * dc_delete_firewall(char * id) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/firewalls/%s",id);
	return(dc_delete_request(buffer));
}

// ---------------------------------------
// CREATE RULE MESSAGE
// ---------------------------------------
public char * dc_create_rule_message( char * proto, char * from, char * to, char * range )
{
	char *	filename;
	FILE *	h;
	if (!( filename = rest_temporary_filename("form")))
		return( filename );
	else if (!( h = fopen( filename, "wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"protocol=%s",proto);
		fprintf(h,"&port_from=%s",from);
		fprintf(h,"&port_to=%s",to);
		fprintf(h,"&ip_address1=%s",range);

		fclose(h);
		return( filename );
	}
}

// ---------------------------------------
// POST /api/firewalls/:id/rules
// ---------------------------------------
public struct rest_response * dc_create_rule(char * id, char * filename) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/firewalls/%s/rules",id);
	return(dc_post_request(buffer,filename));
}

// ---------------------------------------
// DELETE /api/firewalls/:id/:rule_id
// ---------------------------------------
public struct rest_response * dc_delete_rule(char * fwid, char * rid) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/firewalls/%s/%s",fwid,rid);
	return(dc_delete_request(buffer));
}

// ---------------------------------------
// 3.8 Addresses
// ---------------------------------------
// GET /api/addresses
// ---------------------------------------
public struct rest_response * dc_list_addresses() 
{ 
	return(dc_get_request("/api/addresses"));
}

// ---------------------------------------
// GET /api/addresses/:id
// ---------------------------------------
public struct rest_response * dc_get_address(char * id) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/addresses/%s",id);
	return(dc_get_request(buffer));
}

// ---------------------------------------
// POST /api/addresses
// ---------------------------------------
public struct rest_response * dc_create_address()
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/addresses");
	return(dc_post_request(buffer,(char *) 0));
}

// ---------------------------------------
// DELETE /api/addresses/:id
// ---------------------------------------
public struct rest_response * dc_delete_address(char * id) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/addresses/%s",id);
	return(dc_delete_request(buffer));
}

// ---------------------------------------
// POST /api/addresses/:id/associate
// ---------------------------------------
public struct rest_response * dc_associate_address(char * id, char * server) 
{ 
	FILE *	h;
	char * filename;
	struct	rest_response * rptr=(struct rest_response *) 0;
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/addresses/%s/associate",id);
	if (( filename = rest_temporary_filename("form")) != (char *) 0)
	{
		if (( h = fopen( filename, "wa" )) != (FILE *) 0)
		{
			fprintf(h,"instance_id=%s",server);
			fclose(h);
			rptr = dc_post_request(buffer,filename);
		}
		filename = liberate( filename );
	}
	return( rptr );
}

// POST /api/addresses/:id/disassociate
public struct rest_response * dc_disassociate_address(char * id)
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/addresses/%s/disassociate",id);
	return(dc_post_request(buffer,(char *) 0));
}

// ---------------------------------------
// 3.9 Load Balancers
// ---------------------------------------
// GET /api/load_balancers
// ---------------------------------------
public struct rest_response * dc_list_loadbalancers() 
{ 
	return(dc_get_request("/api/load_balancers"));
}

// ---------------------------------------
// GET /api/load_balancers/:id
// ---------------------------------------
public struct rest_response * dc_get_loadbalancer(char * id) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/load_balancers/%s",id);
	return(dc_get_request(buffer));
}

// ---------------------------------------
// CREATE LOADBALANCER MESSAGE
// ---------------------------------------
public char * dc_create_loadbalancer_message( char * name, char * realm, char * protocol, char * port1, char * port2 )
{
	char *	filename;
	FILE *	h;
	if (!( filename = rest_temporary_filename("form")))
		return( filename );
	else if (!( h = fopen( filename, "wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"name=%s",name);
		fprintf(h,"&realm_id=%s",realm);
		fprintf(h,"&listener_protocol=%s",protocol);
		fprintf(h,"&listener_balancer_port=%s",port1);
		fprintf(h,"&listener_instance_port=%s",port2);

		fclose(h);
		return( filename );
	}
}

// ---------------------------------------
// POST /api/load_balancers
// ---------------------------------------
public struct rest_response * dc_create_loadbalancer(char * filename) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/load_balancers");
	return(dc_post_request(buffer,filename));
}

// ---------------------------------------
// DELETE /api/load_balancers/:id
// ---------------------------------------
public struct rest_response * dc_delete_loadbalancer(char * id) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/load_balancers/%s",id);
	return(dc_delete_request(buffer));
}

// ---------------------------------------
// POST /api/load_balancers/:id/register
// ---------------------------------------
public struct rest_response * dc_register_loadbalancer(char * id,char * server) 
{ 
	FILE *	h;
	char * filename;
	struct	rest_response * rptr=(struct rest_response *) 0;
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/load_balancers/%s/register",id);
	if (( filename = rest_temporary_filename("form")) != (char *) 0)
	{
		if (( h = fopen( filename, "wa" )) != (FILE *) 0)
		{
			fprintf(h,"instance_id=%s",server);
			fclose(h);
			rptr = dc_post_request(buffer,filename);
		}
		filename = liberate( filename );
	}
	return( rptr );
}

// ---------------------------------------
// POST /api/load_balancers/:id/unregister
// ---------------------------------------
public struct rest_response * dc_unregister_loadbalancer(char * id,char * server) 
{ 
	FILE *	h;
	char * filename;
	struct	rest_response * rptr=(struct rest_response *) 0;
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/load_balancers/%s/unregister",id);
	if (( filename = rest_temporary_filename("form")) != (char *) 0)
	{
		if (( h = fopen( filename, "wa" )) != (FILE *) 0)
		{
			fprintf(h,"instance_id=%s",server);
			fclose(h);
			rptr = dc_post_request(buffer,filename);
		}
		filename = liberate( filename );
	}
	return( rptr );
}

// ---------------------------------------
// GET /api/storage_volumes
// ---------------------------------------
public struct rest_response * dc_list_storage()
{ 
	return(dc_get_request("/api/storage_volumes"));
}


// ---------------------------------------
// GET /api/storage_volumes/:id
// ---------------------------------------
public struct rest_response * dc_get_storage(char * id)
{
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/storage_volumes/%s",id);
	return(dc_get_request(buffer));
}

// ---------------------------------------
// CREATE STORAGE MESSAGE
// ---------------------------------------
public char * dc_create_storage_message(char * name, char * size, char * realm)
{
	char *	filename;
	FILE *	h;
	if (!( filename = rest_temporary_filename("form")))
		return( filename );
	else if (!( h = fopen( filename, "wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"name=%s",name);
		fprintf(h,"&capacity=%s",realm);
		fprintf(h,"&realm_id=%s",realm);
		fclose(h);
		return( filename );
	}
}


// ---------------------------------------
// POST /api/storage_volumes
// ---------------------------------------
public struct rest_response * dc_create_storage(char * filename)
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/storage_volumes");
	return(dc_post_request(buffer,filename));
}

// ---------------------------------------
// DELETE /api/storage_volumes/:id
// ---------------------------------------
public struct rest_response * dc_delete_storage(char * id)
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/storage_volumes/%s",id);
	return(dc_delete_request(buffer));
}

// ---------------------------------------
// POST /api/storage_volumes/:id/attach
// ---------------------------------------
public struct rest_response * dc_attach_storage(char * id,char * server)
{ 
	FILE *	h;
	char * filename;
	struct	rest_response * rptr=(struct rest_response *) 0;
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/storage_volumes/%s/attach",id);
	if (( filename = rest_temporary_filename("form")) != (char *) 0)
	{
		if (( h = fopen( filename, "wa" )) != (FILE *) 0)
		{
			fprintf(h,"instance_id=%s",server);
			fclose(h);
			rptr = dc_post_request(buffer,filename);
		}
		filename = liberate( filename );
	}
	return( rptr );
}

// ---------------------------------------
// POST /api/storage_volumes/:id/detach
// ---------------------------------------
public struct rest_response * dc_detach_storage(char * id)
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/storage_volumes/%s/detach",id);
	return(dc_post_request(buffer,(char *) 0));
}

// ---------------------------------------
// 4.3 Blob Storage
// ---------------------------------------
// GET /api/buckets
// ---------------------------------------
public struct rest_response * dc_list_buckets()
{
	return(dc_get_request("/api/buckets"));
}

// ---------------------------------------
// GET /api/buckets/:id
// ---------------------------------------
public struct rest_response * dc_get_bucket(char * id)
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/buckets/%s",id);
	return(dc_delete_request(buffer));
}


// ---------------------------------------
// CREATE BUCKET MESSAGE
// ---------------------------------------
public char * dc_create_bucket_message(char * name, char * location)
{
	char *	filename;
	FILE *	h;
	if (!( filename = rest_temporary_filename("form")))
		return( filename );
	else if (!( h = fopen( filename, "wa" ) ))
		return( liberate( filename ) );
	else
	{
		fprintf(h,"name=%s",name);
		fprintf(h,"&location=%s",location);
		fclose(h);
		return( filename );
	}
}

// ---------------------------------------
// POST /api/buckets
// ---------------------------------------
public struct rest_response * dc_create_bucket(char * filename)
{
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/buckets");
	return(dc_post_request(buffer,filename));
}

// ---------------------------------------
// DELETE /api/buckets/:id
// ---------------------------------------
public struct rest_response * dc_delete_bucket(char * id)
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/buckets/%s",id);
	return(dc_delete_request(buffer));
}


// ---------------------------------------
// GET /api/buckets/:bucket_id/:blob_id
// ---------------------------------------
public struct rest_response * dc_get_blob_info(char * bucket, char * blob)
{
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/buckets/%s/%s",bucket,blob);
	return(dc_get_request(buffer));
}

// ---------------------------------------
// GET /api/buckets/:bucket_id/:blob_id/content
// ---------------------------------------
public struct rest_response * dc_get_blob_content(char * bucket, char * blob)
{
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/buckets/%s/%s/content",bucket,blob);
	return(dc_get_request(buffer));
}

// ---------------------------------------
// PUT /api/buckets/:bucket_id/:blob_id
// ---------------------------------------
public struct rest_response * dc_update_blob(char * bucket, char * blob, char * filename)
{
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/buckets/%s/%s",bucket,blob);
	return(dc_put_request(buffer,filename));
}

// ---------------------------------------
// POST /api/buckets/:bucket_id
// ---------------------------------------
public struct rest_response * dc_create_blob(char * bucket, char *filename)
{
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/buckets/%s",bucket);
	return(dc_post_request(buffer,filename));
}

// ---------------------------------------
// DELETE /api/buckets/:bucket_id/:blob_id
// ---------------------------------------
public struct rest_response * dc_delete_blob(char * bucket, char * blob)
{
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/buckets/%s/%s",bucket,blob);
	return(dc_delete_request(buffer));
}

// ---------------------------------------
// POST /api/buckets/:bucket_id/:blob_id
// ---------------------------------------
public struct rest_response * dc_update_blob_info(char * bucket, char * blob)
{
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/buckets/%s/%s",bucket,blob);
	return(dc_post_request(buffer,(char *) 0));
}

	/* ----------- */
#endif	/* _dcclient_c */
	/* ----------- */


