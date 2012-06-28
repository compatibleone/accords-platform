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

struct	keystone_config
{
	char *	requestauth;
	char *	acceptauth;
	char *	tenantname;
	char *	tenantid;
	char *	host;
	char *	glance;
	char *	volume;
};

struct	os_subscription
{


	struct	os_config 	Os;
	struct	keystone_config KeyStone;
};

public	struct	os_subscription * os_allocate_subscription();
public	struct	os_subscription * os_liberate_subscription(struct os_subscription * sptr);

#define	_OS_NS_COMPUTE_V11	"http://docs.openstack.org/compute/api/v1.1"
#define	_OS_NS_COMPUTE_V10	"http://docs.rackspacecloud.com/servers/api/v1.0"

public	struct os_subscription * os_initialise_client( 
		char * user, char * password, char * tenant, 
		char * host, char * agent, char * version, char * tls );
public	char * os_create_server_request(
	struct os_subscription *  sptr, 
	char * identity, char * flavor, char * image, char * address, char * personality, char * resource, char * group, char * zone );
public	char * os_create_metadata_request(struct os_subscription *  sptr, char * values );
public	char * os_create_meta_request(struct os_subscription *  sptr, char * key, char * value );
public	char * os_create_image_request(struct os_subscription *  sptr,char * identity, char * server );
public	char * os_create_flavor_request(struct os_subscription *  sptr,char * identity, char * ram, char * cpus, char * disk );
public	char * os_create_address_request(struct os_subscription *  sptr, char * address );
public	char * os_remove_address_request(struct os_subscription *  sptr, char * address );
public	char * os_create_security_group_request(struct os_subscription *  sptr, char * name );
public	char * os_create_security_rule_request(struct os_subscription *  sptr, char * name, char * protocol, char * from, char * to, char * cidr );

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

public	struct	os_response *	os_list_servers	(struct os_subscription *  sptr );
public	struct	os_response *	os_list_flavors(struct os_subscription *  sptr );
public	struct	os_response *	os_list_images  (struct os_subscription *  sptr );
public	struct	os_response *	os_list_server_metadata(struct os_subscription *  sptr, char * id );
public	struct	os_response *	os_list_image_metadata(struct os_subscription *  sptr, char * id );
public	struct	os_response *	os_list_image_details(struct os_subscription *  sptr);
public	struct	os_response *	os_list_flavor_details(struct os_subscription *  sptr);
public	struct	os_response *	os_list_server_details(struct os_subscription *  sptr);
public	struct	os_response *	os_list_floating_ips(struct os_subscription *  sptr);
public	struct	os_response *	os_list_floating_ip_details(struct os_subscription *  sptr);
public	struct	os_response *	os_list_security_groups	(struct os_subscription *  sptr );
public	struct	os_response *	os_list_security_group_details	(struct os_subscription *  sptr );

public	struct	os_response *	os_create_flavor(struct os_subscription *  sptr, char * filename );
public	struct	os_response *	os_create_server(struct os_subscription *  sptr, char * filename );
public	struct	os_response *	os_create_image(struct os_subscription *  sptr, char * filename, char * serverid, int ispublic );
public	struct	os_response *	os_create_address(struct os_subscription *  sptr);
public	struct	os_response *	os_server_address(struct os_subscription *  sptr, char * filename, char * serverid );
public	struct	os_response *	os_create_security_group(struct os_subscription *  sptr,char * filename);
public	struct	os_response *	os_create_security_rule(struct os_subscription *  sptr,char * filename);

public	struct	os_response *	os_get_server	(struct os_subscription *  sptr, char * id );
public	struct	os_response *	os_get_flavor  (struct os_subscription *  sptr, char * id );
public	struct	os_response *	os_get_image    (struct os_subscription *  sptr, char * id );

public	struct	os_response *	os_get_glance	(struct os_subscription *  sptr,  char * id );
public	struct	os_response *	os_glance_access(struct os_subscription *  sptr,  char * id, int ispublic );
public	struct	os_response *	os_head_glance	(struct os_subscription *  sptr,  char * id );
public	struct	os_response *	os_get_address(struct os_subscription *  sptr, char * id );
public	struct	os_response *	os_get_server_metadata(struct os_subscription *  sptr, char * id, char * name );
public	struct	os_response *	os_get_image_metadata(struct os_subscription *  sptr, char * id, char * name );
public	struct	os_response *	os_get_security_group(struct os_subscription *  sptr, char * id );

public	struct	os_response *	os_update_server(struct os_subscription *  sptr, char * id, char * filename );
public	struct	os_response *	os_update_server_metadata(struct os_subscription *  sptr, char * id, char * name, char * value );
public	struct	os_response *	os_update_image_metadata(struct os_subscription *  sptr, char * id, char * name, char * value );
public	struct	os_response *	os_create_server_metadata(struct os_subscription *  sptr, char * id, char * values );
public	struct	os_response *	os_create_image_metadata(struct os_subscription *  sptr, char * id, char * values );

public	struct	os_response *	os_delete_server(struct os_subscription *  sptr, char * id );
public	struct	os_response *	os_delete_image (struct os_subscription *  sptr, char * id );
public	struct	os_response *	os_delete_flavor(struct os_subscription *  sptr, char * id );
public	struct	os_response *	os_delete_server_metadata(struct os_subscription *  sptr, char * id, char * name );
public	struct	os_response *	os_delete_image_metadata(struct os_subscription *  sptr, char * id, char * name );
public	struct	os_response *	os_delete_address(struct os_subscription *  sptr, char * id );
public	struct	os_response *	os_delete_security_group(struct os_subscription *  sptr, char * id );
public	struct	os_response *	os_delete_security_rule(struct os_subscription *  sptr, char * id );

public	char *	os_build_image_reference(struct os_subscription *  sptr, char * nptr );
public	char *	os_build_flavor_reference(struct os_subscription *  sptr, char * nptr );
public	struct os_response * liberate_os_response(struct os_response * rptr );	


#endif	/* _os_client_h */
	/* ------------ */


