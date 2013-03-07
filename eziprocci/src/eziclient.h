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
#ifndef	_eziclient_h	
#define	_eziclient_h

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

struct	ezi_response
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

struct	ezi_subscription
{
	struct	ezi_config 	Ezi;
	struct	keystone_config KeyStone;
};


#define	_OS_NS_COMPUTE_V11	"http://docs.openstack.org/compute/api/v1.1"
#define	_OS_NS_COMPUTE_V10	"http://docs.rackspacecloud.com/servers/api/v1.0"

public	struct	ezi_subscription * ezi_allocate_subscription();
public	struct	ezi_subscription * ezi_liberate_subscription(struct ezi_subscription * sptr);

public	struct ezi_subscription * ezi_initialise_client( 
		char * user, char * password, char * tenant, 
		char * host, char * agent, char * version, char * tls );

public	struct	ezi_response * 
	ezi_client_get_request(
		char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	ezi_response * 
	ezi_client_delete_request(
		char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	ezi_response * 
	ezi_client_head_request(
		char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	ezi_response * 
	ezi_client_post_request(
		char * target, char * tls, char * agent, char * filename, struct rest_header * hptr );

public	struct	ezi_response * 
	ezi_client_put_request(
		char * target, char * tls, char * agent, char * filename, struct rest_header * hptr );

public	struct	ezi_response *	ezi_create_server(struct ezi_subscription *  sptr, char * filename );
public	struct	ezi_response *	ezi_get_server	 (struct ezi_subscription *  sptr, char * id );
public	struct	ezi_response *	ezi_delete_server(struct ezi_subscription *  sptr, char * id );
public	struct	ezi_response *	liberate_ezi_response(struct ezi_response * rptr );	


	/* ------------ */
#endif	/* _eziclient_h */
	/* ------------ */
