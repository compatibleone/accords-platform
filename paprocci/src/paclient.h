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
#ifndef _pa_client_h
#define _pa_client_h

#include "standard.h"
#include "rest.h"
#include "url.h"
#include "urlpublic.h"
#include "restclient.h"
#include "json.h"
#include "document.h"
#include "paconfig.h"
#include "proactive.h"

#define	_TEXT_NONE 	0
#define	_TEXT_XML	1
#define	_TEXT_JSON	2

struct	pa_response
{
	int	nature;
	char			*	content;
	struct	xml_element 	*	xmlroot;
	struct	data_element	*	jsonroot;
	struct	rest_response	*	response;
};

#define	_PA_NS_COMPUTE	"http://schemas.microsoft.com/windowsazure"

/* ----------------------- */
/* proactive request functions */
/* ----------------------- */

public	struct	pa_response * 
	pa_client_get_request(
		char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	pa_response * 
	pa_client_delete_request(
		char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	pa_response * 
	pa_client_head_request(
		char * target, char * tls, char * agent, struct rest_header * hptr );

public	struct	pa_response * 
	pa_client_post_request(
		char * target, char * tls, char * agent, char * filename, struct rest_header * hptr );

public	struct	pa_response * 
	pa_client_put_request(
		char * target, char * tls, char * agent, char * filename, struct rest_header * hptr );

/* --------------- */
/* hosted services */
/* --------------- */
public	struct	pa_response *	pa_create_hosted_service( char * filename );
public	struct	pa_response *	pa_retrieve_hosted_service( char * filename );
public	struct	pa_response *	pa_update_hosted_service( char * filename );
public	struct	pa_response *	pa_delete_hosted_service( char * filename );
public	struct	pa_response *	pa_list_hosted_services();

/* --------------- */
/* affinity groups */
/* --------------- */
//public	struct	pa_response *	pa_create_affinity_group( char * filename );
//public	struct	pa_response *	pa_retrieve_affinity_group( char * filename );
//public	struct	pa_response *	pa_update_affinity_group( char * filename );
//public	struct	pa_response *	pa_delete_affinity_group( char * filename );
//public	struct	pa_response *	pa_list_affinity_groups();

//public	char * pa_create_affinity_group_request(
//	char * name,
//	char * label, 
//	char * description,
//	char * location );

/* ------------ */
/* certificates */
/* ------------ */
public	struct	pa_response *	pa_create_certificate( char * filename );
public	struct	pa_response *	pa_retrieve_certificate( char * filename );
public	struct	pa_response *	pa_delete_certificate( char * filename );
public	struct	pa_response *	pa_list_certificates(char * server);

/* --------- */
/* locations */
/* --------- */
public	struct	pa_response *	pa_list_locations();

/* ------- */
/* servers */
/* ------- */
public	struct	pa_response *	pa_list_servers	();
public	struct	pa_response *	pa_create_server(struct proactive * constr);
public	struct	pa_response *	pa_delete_server(struct proactive * constr);
public	struct	pa_response *	pa_get_server	( char * id );
public	struct	pa_response *	pa_update_server( char * id, char * filename );


/* ------- */
/* flavors */
/* ------- */
//public	struct	pa_response *	pa_list_flavors();
//public	struct	pa_response *	pa_get_flavor  ( char * id );

/* ------ */
/* images */
/* ------ */
//public	struct	pa_response *	pa_list_images  ();
//public	struct	pa_response *	pa_create_image( char * filename );
//public	struct	pa_response *	pa_get_image    ( char * id );
//public	struct	pa_response *	pa_delete_image ( char * id );
	
//public	char * pa_create_image_request(
//	char * identity, char * server );

/* ---------- */
/* operations */
/* ---------- */
public	struct	pa_response *	pa_list_operations();

/* ----------- */
/* deployments */
/* ----------- */
public	struct	pa_response *	pa_list_deployments(char * server);
public	struct	pa_response *	pa_create_deployment( char * filename, char * server, char * slot );
public	struct	pa_response *	pa_get_deployment( char * server, char * slot);
public	struct	pa_response *	pa_delete_deployment( char * server, char * slot );

/* ------------- */
/* subscriptions */
/* ------------- */
public	struct	pa_response *	pa_get_subscription();

public	struct 	pa_response * 	liberate_pa_response( struct pa_response * rptr );	

/* ----------------------- */
/* trafic manager profiles */
/* ----------------------- */
//public	struct	pa_response *	pa_list_WATM_profiles();

/* ---------------------------------- */
/* trafic manager profile definitions */
/* ---------------------------------- */
//public	struct	pa_response *	pa_list_WATM_definitions(char * profile );

/* ---------------- */
/* storage services */
/* ---------------- */
//public	struct	pa_response *	pa_create_storage_service( char * filename );
//public	struct	pa_response *	pa_retrieve_storage_service( char * filename );
//public	struct	pa_response *	pa_update_storage_service( char * filename );
//public	struct	pa_response *	pa_delete_storage_service( char * filename );
//public	struct	pa_response *	pa_list_storage_services();

//public	char * pa_create_storage_service_request(
//	char * name,
//	char * label, 
//	char * description,
//	char * group,
//	char * location );

#endif	/* _pa_client_h */
	/* ------------ */




