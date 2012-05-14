
#ifndef	_dcclient_h
#define	_dcclient_h


#include "rest.h"

#define	_DC_BUFFERSIZE 1024

public	int	liberate_dc_occi_configuration(int status);
public	int	set_dc_occi_configuration( char * host, int port, char * user, char * password, char * tenant, char * agent, char * tls );

// 1. API
// GET /api
public struct rest_response * dc_list_features();

// 3. Compute Resources
// 3.1 Realms
public struct rest_response * dc_list_realms();

// GET /api/realms
// GET /api/realms/:id
public struct rest_response * dc_get_realm(char * id);

// 3.2 Hardware Profiles
// GET /api/hardware_profiles
public struct rest_response * dc_list_profiles();

// GET /api/hardware profiles/:id
public struct rest_response * dc_get_profile(char * id);

// 3.3 Images
// GET /api/images
public struct rest_response * dc_list_images();

// GET /api/images/:id
public struct rest_response * dc_get_image(char * id);

// POST /api/images
public struct rest_response * dc_create_image(char * filename);

// DELETE /api/images/:id
public struct rest_response * dc_delete_image(char * id);

// 3.4 Instance States
// GET /api/instance_states
public struct rest_response * dc_list_states();

// 3.5 Instances
// GET /api/instances
public struct rest_response * dc_list_instances();

// GET /api/instances/:id
public struct rest_response * dc_get_instance(char * id);

// POST /api/instances/:id/:action
public struct rest_response * dc_instance_action(char * id, char * action);

// POST /api/instances
public struct rest_response * dc_create_instance( char * filename );

// DELETE /api/instances/:id
public struct rest_response * dc_delete_instance( char * id );

// 3.6 Keys
// GET /api/keys
public struct rest_response * dc_list_keys();

// GET /api/keys/:id
public struct rest_response * dc_get_key(char * id);

// POST /api/keys
public struct rest_response * dc_create_key(char * filename);

// DELETE /api/keys/:id
public struct rest_response * dc_delete_key(char * id);

// 3.7 Firewalls
// GET /api/firewalls
public struct rest_response * dc_list_firewalls();

// GET /api/firewalls/:id
public struct rest_response * dc_get_firewall(char * id);

// POST /api/firewalls
public struct rest_response * dc_create_firewall(char * filename);

// DELETE /api/firewalls/:id
public struct rest_response * dc_delete_firewall(char * id);

// POST /api/firewalls/:id/rules
public struct rest_response * dc_create_rule(char * id, char * filename);

// DELETE /api/firewalls/:id/:rule_id
public struct rest_response * dc_delete_rule(char * fwid, char * rid);

// 3.8 Addresses
// GET /api/addresses
public struct rest_response * dc_list_addresses();

// GET /api/addresses/:id
public struct rest_response * dc_get_address(char * id);

// POST /api/addresses
public struct rest_response * dc_create_address(char * filename);


// DELETE /api/addresses/:id
public struct rest_response * dc_delete_address(char * id);

// POST /api/addresses/:id/associate
public struct rest_response * dc_associate_address(char * id, char * server);

// POST /api/addresses/:id/disassociate
public struct rest_response * dc_disassociate_address(char * id, char * server);

// 3.9 Load Balancers

// GET /api/load_balancers
public struct rest_response * dc_list_loadbalancers();

// GET /api/load_balancers/:id
public struct rest_response * dc_get_loadbalancer(char * id);

// POST /api/load_balancers
public struct rest_response * dc_create_loadbalancer(char * filename);

// DELETE /api/load_balancers/:id
public struct rest_response * dc_delete_loadbalancer(char * id);

// POST /api/load_balancers/:id/register
public struct rest_response * dc_register_loadbalancer(char * id, char * filename);

// POST /api/load_balancers/:id/unregister
public struct rest_response * dc_unregister_loadbalancer(char * id, char * filename);

	/* ----------- */
#endif	/* _dcclient_h */
	/* ----------- */
