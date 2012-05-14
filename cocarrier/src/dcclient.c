#ifndef	_dcclient_c
#define	_dcclient_c

// ---------------------------------------
// DELTA CLOUD API CLIENT MODULE
// ---------------------------------------
#include "dcclient.h"
#include "restclient.h"

struct	dc_api_configuration
{
	char *	authorization;
	char *	tls;
	char *	agent;
	char *	tenant;
	char *	user;
	char *	password;
	char *	host;
	int	port;
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

/*	------------------------------------------------------------	*/
/*	 l i b e r a t e _ d c _ a p i _ c o n f i g u r a t i o n		*/
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
	DeltaCloudConfig.port = 0;
	return(status);
}

/*	------------------------------------------------------------	*/
/*		s e t _ d c _ a p i _ c o n f i g u r a t i o n			*/
/*	------------------------------------------------------------	*/
public	int	set_dc_api_configuration( char * host, int port, char * user, char * password, char * tenant, char * agent, char * tls )
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

	DeltaCloudConfig.port = port;
	return( 0 );	
}

/*	------------------------------------------------------------	*/
/*			d c _ a p i _ f a i l u r e 					*/
/*	------------------------------------------------------------	*/
private struct rest_response * dc_api_failure( struct rest_header * hptr )
{
	liberate_rest_header( hptr );
	return((struct rest_response *) 0);
}

/*	------------------------------------------------------------	*/
/*			d c _ a u t h e n t i c a t e 				*/
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

// ---------------------------------------
// REST API INTERFACE FUNCTION
// ---------------------------------------
// REST GET REQUEST URL
// ---------------------------------------
private struct rest_response * dc_get_request(char * nptr)
{
	char	url[_DC_BUFFERSIZE];
	struct rest_header * hptr=(struct rest_header *) 0;
	sprintf(url,"%s?format=xml",nptr);
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
	sprintf(url,"%s?format=xml",nptr);
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
	sprintf(url,"%s?format=xml",nptr);
	if (!( hptr = dc_authenticate()))
		return( dc_api_failure( hptr ) );
	else	return( rest_client_post_request( url, DeltaCloudConfig.tls, DeltaCloudConfig.agent, filename, hptr ) );
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
public struct rest_response * dc_create_address(char * filename) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/addresses");
	return(dc_post_request(buffer,filename));
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
public struct rest_response * dc_associate_address(char * id, char * filename) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/addresses/%s/associate",id);
	return(dc_post_request(buffer,filename));
}

// POST /api/addresses/:id/disassociate
public struct rest_response * dc_disassociate_address(char * id, char * filename) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/addresses/%s/disassociate",id);
	return(dc_post_request(buffer,filename));
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
public struct rest_response * dc_register_loadbalancer(char * id,char * filename) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/load_balancers/%s/register",id);
	return(dc_post_request(buffer,filename));
}

// ---------------------------------------
// POST /api/load_balancers/:id/unregister
// ---------------------------------------
public struct rest_response * dc_unregister_loadbalancer(char * id,char * filename) 
{ 
	char buffer[_DC_BUFFERSIZE];
	sprintf(buffer,"/api/load_balancers/%s/unregister",id);
	return(dc_post_request(buffer,filename));
}

	/* ----------- */
#endif	/* _dcclient_c */
	/* ----------- */


