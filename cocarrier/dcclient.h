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
#ifndef	_dcclient_h
#define	_dcclient_h


#include "rest.h"

#define	_DC_BUFFERSIZE 1024

public	int	liberate_dc_occi_configuration(int status);
public	int	dc_api_configuration( 
			char * host, 
			char * user, char * password, 
			char * tenant, char * agent, char * tls );

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

// 4.3 Blob Storage
// GET /api/buckets
public struct rest_response * dc_list_buckets();
// GET /api/buckets/:id
public struct rest_response * dc_get_bucket(char * id);
// POST /api/buckets
public char * dc_create_bucket_message(char * name, char * location);
public struct rest_response * dc_create_bucket(char * filename);
// DELETE /api/buckets/:id
public struct rest_response * dc_delete_bucket(char * id);

// GET /api/buckets/:bucket_id/:blob_id
public struct rest_response * dc_get_blob_info(char * bucket, char * blob);
// GET /api/buckets/:bucket_id/:blob_id/content
public struct rest_response * dc_get_blob_content(char * bucket, char * blob);

// PUT /api/buckets/:bucket_id/:blob_id
public struct rest_response * dc_update_blob(char * bucket, char * blob, char * filename);
// POST /api/buckets/:bucket_id
public struct rest_response * dc_create_blob(char * bucket, char *filename);

// DELETE /api/buckets/:bucket_id/:blob_id
public struct rest_response * dc_delete_blob(char * bucket, char * blob);
// POST /api/buckets/:bucket_id/:blob_id
public struct rest_response * dc_update_blob_info(char * bucket, char * blob);

	/* ----------- */
#endif	/* _dcclient_h */
	/* ----------- */

