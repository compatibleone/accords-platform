/* ---------------------------------------------------------------------------- */
/* Advanced Capabilities for Compatible One Resources Delivery System - ACCORDS	*/
/* (C) 2011 by Iain James Marshall <ijm667@hotmail.com>				*/
/* ---------------------------------------------------------------------------- */
/*										*/
/* This is free software; you can redistribute it and/or modify it		*/
/* under the terms of the GNU Lesser General Public License as			*/
/* published by the Free Software Foundation; either version 2.1 of		*/
/* the License, or (at your option) any later version.				*/
/*										*/
/* This software is distributed in the hope that it will be useful,		*/
/* but WITHOUT ANY WARRANTY; without even the implied warranty of		*/
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU		*/
/* Lesser General Public License for more details.				*/
/*										*/
/* You should have received a copy of the GNU Lesser General Public		*/
/* License along with this software; if not, write to the Free			*/
/* Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA		*/
/* 02110-1301 USA, or see the FSF site: http://www.fsf.org.			*/
/*										*/
/* ---------------------------------------------------------------------------- */
#ifndef _os_client_h
#define _os_client_h

#include "standard.h"
#include "rest.h"
#include "url.h"
#include "urlpublic.h"
#include "restclient.h"
#include "json.h"
#include "document.h"
#include "osconfig.h"

#define	_TEXT_NONE 	0
#define	_TEXT_XML	1
#define	_TEXT_JSON	2

struct	os_response
{
	int	nature;
	char			*	content;
	struct	xml_element 	*	xmlroot;
	struct	data_element	*	jsonroot;
	struct	rest_response	*	response;
};

#define	_OS_NS_COMPUTE_V11	"http://docs.openstack.org/compute/api/v1.1"
#define	_OS_NS_COMPUTE_V10	"http://docs.rackspacecloud.com/servers/api/v1.0"

public	char * os_create_server_request(
	char * identity, char * flavor, char * image, char * address, char * personality, char * resource, char * group, char * zone );
public	char * os_create_metadata_request( char * values );
public	char * os_create_meta_request( char * key, char * value );
public	char * os_create_image_request(char * identity, char * server );
public	char * os_create_flavor_request(char * identity, char * ram, char * cpus, char * disk );
public	char * os_create_address_request( char * address );
public	char * os_remove_address_request( char * address );
public	char * os_create_security_group_request( char * name );
public	char * os_create_security_rule_request( char * name, char * protocol, char * from, char * to, char * cidr );

public	struct	os_response * 
	os_client_get_request(
		char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	os_response * 
	os_client_delete_request(
		char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	os_response * 
	os_client_head_request(
		char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	os_response * 
	os_client_post_request(
		char * target, char * tls, char * agent, char * filename, struct rest_header * hptr );

public	struct	os_response * 
	os_client_put_request(
		char * target, char * tls, char * agent, char * filename, struct rest_header * hptr );

public	struct	os_response *	os_list_servers	( );
public	struct	os_response *	os_list_flavors( );
public	struct	os_response *	os_list_images  ( );
public	struct	os_response *	os_list_metadata( char * id );
public	struct	os_response *	os_list_image_details();
public	struct	os_response *	os_list_flavor_details();
public	struct	os_response *	os_list_server_details();
public	struct	os_response *	os_list_floating_ips();
public	struct	os_response *	os_list_floating_ip_details();
public	struct	os_response *	os_list_security_groups	( );
public	struct	os_response *	os_list_security_group_details	( );

public	struct	os_response *	os_create_flavor( char * filename );
public	struct	os_response *	os_create_server( char * filename );
public	struct	os_response *	os_create_image( char * filename, char * serverid );
public	struct	os_response *	os_create_address();
public	struct	os_response *	os_server_address( char * filename, char * serverid );
public	struct	os_response *	os_create_security_group(char * filename);
public	struct	os_response *	os_create_security_rule(char * filename);

public	struct	os_response *	os_get_server	( char * id );
public	struct	os_response *	os_get_flavor  ( char * id );
public	struct	os_response *	os_get_image    ( char * id );
public	struct	os_response *	os_get_address( char * id );
public	struct	os_response *	os_get_metadata( char * id, char * name );
public	struct	os_response *	os_get_security_group( char * id );

public	struct	os_response *	os_update_server( char * id, char * filename );
public	struct	os_response *	os_update_metadata( char * id, char * name, char * value );
public	struct	os_response *	os_create_metadata( char * id, char * values );

public	struct	os_response *	os_delete_server( char * id );
public	struct	os_response *	os_delete_image ( char * id );
public	struct	os_response *	os_delete_flavor( char * id );
public	struct	os_response *	os_delete_metadata( char * id, char * name );
public	struct	os_response *	os_delete_address( char * id );
public	struct	os_response *	os_delete_security_group( char * id );
public	struct	os_response *	os_delete_security_rule( char * id );

public	char *	os_build_image_reference( char * nptr );
public	char *	os_build_flavor_reference( char * nptr );
public	struct os_response * liberate_os_response( struct os_response * rptr );	


#endif	/* _os_client_h */
	/* ------------ */


