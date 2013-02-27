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
#ifndef	_paasclient_h
#define	_paasclient_h

#include "standard.h"
#include "rest.h"
#include "url.h"
#include "urlpublic.h"
#include "restclient.h"
#include "json.h"
#include "document.h"

#define	_TEXT_NONE 	0
#define	_TEXT_XML	1
#define	_TEXT_JSON	2

#define	_COAPS_BASE	"/CF-api/rest"

struct	paas_client
{
	char *	agent;
	char *	tls;
	char *	service;
	char *	host;
	char *	user;
	char *	pass;
	int	port;
	char *	base;
};

struct	paas_response
{
	int	nature;
	char *	content;
	struct	rest_response * response;	
	struct	xml_element * xmlroot;
	struct	data_element * jsonroot;
};

public	struct paas_response * paas_remove_response( struct paas_response * response );
public	struct paas_response * paas_result( struct rest_response *  response );
public	struct paas_response * start_paas_application(char * application);
public	struct paas_response * get_paas_task( char * task );
public	struct paas_response * stop_paas_application(char * application );
public	struct paas_response * create_paas_application( char * xmlname);
public	struct paas_response * list_paas_applications();
public	struct paas_response * get_paas_application(char * application);
public	struct paas_response * delete_paas_application(char * application);
public	struct paas_response * deploy_paas_application( char * environment, char * application);
public	struct paas_response * undeploy_paas_application(char * environment, char * application);
public	struct paas_response * create_paas_environment( char * xmlname );
public	struct paas_response * delete_paas_environment( char * environment );
public	struct paas_response * list_paas_environment();
public	struct paas_response * get_paas_environment(char * environment);
public	struct paas_response * get_paas_environment_application_instances(char * environment);

public	int	initialise_paas_client( char * agent, char * tls, char * host, int port, char * user, char * pass );		
public	int	terminate_paas_client();

	/* ------------- */
#endif	/* _paasclient_h */
	/* ------------- */

