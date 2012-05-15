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
public char * dc_create_image_message( char * instance, char * name, char * description );
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
public char * dc_create_instance_message( char * name, char * profile, char * image, char * firewall, char * zone );
public struct rest_response * dc_create_instance( char * filename );

// DELETE /api/instances/:id
public struct rest_response * dc_delete_instance( char * id );

// START /api/instances/:id
public struct rest_response * dc_start_instance( char * id );

// STOP /api/instances/:id
public struct rest_response * dc_stop_instance( char * id );

// 3.6 Keys
// GET /api/keys
public struct rest_response * dc_list_keys();

// GET /api/keys/:id
public struct rest_response * dc_get_key(char * id);

// POST /api/keys
public char * dc_create_key_message( char * name );
public struct rest_response * dc_create_key(char * filename);

// DELETE /api/keys/:id
public struct rest_response * dc_delete_key(char * id);

// 3.7 Firewalls
// GET /api/firewalls
public struct rest_response * dc_list_firewalls();

// GET /api/firewalls/:id
public struct rest_response * dc_get_firewall(char * id);

// POST /api/firewalls
public char * dc_create_firewall_message( char * name, char * description );
public struct rest_response * dc_create_firewall(char * filename);

// DELETE /api/firewalls/:id
public struct rest_response * dc_delete_firewall(char * id);

// POST /api/firewalls/:id/rules
public char * dc_create_rule_message( char * proto, char * from, char * to, char * range );
public struct rest_response * dc_create_rule(char * id, char * filename);

// DELETE /api/firewalls/:id/:rule_id
public struct rest_response * dc_delete_rule(char * fwid, char * rid);

// 3.8 Addresses
// GET /api/addresses
public struct rest_response * dc_list_addresses();

// GET /api/addresses/:id
public struct rest_response * dc_get_address(char * id);

// POST /api/addresses
public struct rest_response * dc_create_address();


// DELETE /api/addresses/:id
public struct rest_response * dc_delete_address(char * id);

// POST /api/addresses/:id/associate
public struct rest_response * dc_associate_address(char * id, char * server);

// POST /api/addresses/:id/disassociate
public struct rest_response * dc_disassociate_address(char * id);

// 3.9 Load Balancers

// GET /api/load_balancers
public struct rest_response * dc_list_loadbalancers();

// GET /api/load_balancers/:id
public struct rest_response * dc_get_loadbalancer(char * id);

// POST /api/load_balancers
public char * dc_create_loadbalancer_message( char * name, char * profile, char * image, char * firewall, char * zone );
public struct rest_response * dc_create_loadbalancer(char * filename);

// DELETE /api/load_balancers/:id
public struct rest_response * dc_delete_loadbalancer(char * id);

// POST /api/load_balancers/:id/register
public struct rest_response * dc_register_loadbalancer(char * id, char * server);

// POST /api/load_balancers/:id/unregister
public struct rest_response * dc_unregister_loadbalancer(char * id, char * server);

// 4.1 Storage Volumes

// GET /api/storage_volumes
public struct rest_response * dc_list_storage();

// GET /api/storage_volumes/:id
public struct rest_response * dc_get_storage(char * id);

// POST /api/storage_volumes
public char * dc_create_storage_message(char * name, char * size, char * realm);
public struct rest_response * dc_create_storage(char * filename);

// DELETE /api/storage_volumes/:id
public struct rest_response * dc_delete_storage(char * id);
// POST /api/storage_volumes/:id/attach
public struct rest_response * dc_attach_storage(char * id,char * server);
// POST /api/storage_volumes/:id/detach
public struct rest_response * dc_detach_storage(char * id);

	/* ----------- */
#endif	/* _dcclient_h */
	/* ----------- */
