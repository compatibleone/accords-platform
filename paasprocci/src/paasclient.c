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

#ifndef	_paasclient_c
#define	_paasclient_c

/*	-----------------------------------------	*/
/*		P A A  S   C L I E N T 			*/
/*	This file contains the client code allowing	*/
/*	access by the JPAAS PROCCI to the platform	*/
/*	JPAAS through the REST interface.		*/
/*	-----------------------------------------	*/
#include "paasclient.h"
#include "restclient.h"
#include "restpublic.h"

/*	--------------------------	*/
/*	Paas Client Configuration	*/
/*	--------------------------	*/
private	struct	paas_client Paas = 
{
	(char *) 0,	/* agent 	*/
	(char *) 0,	/* tls		*/
	(char *) 0,	/* service 	*/
	(char *) 0,	/* host		*/
	(char *) 0,	/* user		*/
	(char *) 0,	/* pass		*/
		 0,	/* port		*/
	(char *) 0	/* base		*/

};

/*	-----------------------------------------	*/
/*	 p a a s _ r e m o v e _ r e s p o n s e 	*/
/*	-----------------------------------------	*/
public	struct	paas_response * paas_remove_response( struct paas_response *  rptr )
{
	if ( rptr )
	{
		if ( rptr->jsonroot )
			rptr->jsonroot = drop_data_element( rptr->jsonroot );
		if ( rptr->xmlroot )
			rptr->xmlroot = document_drop( rptr->xmlroot );
		if ( rptr->content )
			liberate( rptr->content );
		if ( rptr->response )
			rptr->response = liberate_rest_response( rptr->response );
		rptr = liberate( rptr );
	}
	return((struct paas_response *) 0);
}

/*	-----------------------------------------	*/
/*		p a a s _ r e s u l t 			*/
/*	-----------------------------------------	*/
public	struct	paas_response * paas_result( struct rest_response *  aptr )
{
	struct	paas_response * rptr;
	struct	rest_header * hptr=(struct rest_header *) 0;;
	if ( aptr )
	{
		if ( check_verbose() )
		{
			printf("   PAAS Client Response : %s %u %s\n",aptr->version,aptr->status,aptr->message);

			if ( check_debug() )
			{
				for (	hptr=aptr->first;
					hptr != (struct rest_header *) 0;
					hptr = hptr->next )
				{
					if (!( hptr->name )) 
						continue;
					else	printf("   %s: %s\n",hptr->name,hptr->value);
				}
				if ( aptr->body )
				{
					printf("   [ %s ] \n",aptr->body );
				}
			}
		}
	}
	if (!( rptr = allocate( sizeof( struct paas_response ) ) ))
		return( rptr );
	else
	{
		rptr->nature = _TEXT_NONE;
		rptr->content= (char *) 0;
		rptr->xmlroot = (struct xml_element *) 0;
		rptr->jsonroot = (struct data_element *) 0;
		rptr->response = aptr;
		if (!( aptr->body ))
			return(rptr);
		if (!( hptr = rest_resolve_header( aptr->first, "Content-Type" ) ))
			return(rptr);
		else if (!( rptr->content = allocate_string( hptr->value ) ))
			return( rptr );
		else if ((!( strcmp( rptr->content, "text/json" 	)))
		     ||  (!( strcmp( rptr->content, "application/json" 	)))
		     ||  (!( strcmp( rptr->content, "x-application/json"))))
		{
			rptr->nature = _TEXT_JSON;
			rptr->jsonroot = json_parse_file( aptr->body );
			return( rptr );
		}
		else if ((!( strcmp( rptr->content, "text/xml" 		)))
		     ||  (!( strcmp( rptr->content, "application/xml" 	)))
		     ||  (!( strcmp( rptr->content, "x-application/xml"))))
		{
			rptr->nature = _TEXT_XML;
			rptr->xmlroot = document_parse_file( aptr->body );
			return( rptr );
		}
		else	return( rptr );
		
	}
}	

/*	-----------------------------------------	*/
/*	   p a a s _ a u t h e n t i c a t e		*/
/*	-----------------------------------------	*/
private	struct	rest_header * paas_authenticate()
{
	struct	rest_header * hptr;
	struct	rest_header * root=(struct rest_header *) 0;
	struct	rest_header * foot=(struct rest_header *) 0;
	char *	sptr;
	if (!( hptr = rest_create_header( _HTTP_ACCEPT, "application/xml" ) ))
		return( hptr );
	else	foot = root = hptr;
	if ((!( Paas.user )) || (!( Paas.pass )))
		return( root );
	else if (!( sptr = rest_encode_credentials( Paas.user, Paas.pass ) ))
		return( root );
	else if (!( hptr = rest_create_header( _HTTP_AUTHORIZATION, sptr ) ))
		return( root );
	else	
	{
		hptr->previous = root;
		root->next = hptr;
		return( root );
	}
}

/*	-----------------------------------------	*/
/*	    p a a s _ g e t _ r e q u e s t		*/
/*	-----------------------------------------	*/
private	struct	rest_response * paas_get_request( char * url )
{
	struct	rest_header * hptr;
	char 	burl[2048];
	sprintf(burl,"%s://%s:%u%s%s", Paas.service, Paas.host, Paas.port, Paas.base, url );
	if (!( hptr = paas_authenticate()))
		return((struct rest_response *) 0);
	else 	return( rest_client_get_request( burl, Paas.tls, Paas.agent, hptr ) );
}

/*	-----------------------------------------	*/
/*	  p a a s _ d e l e t e _ r e q u e s t		*/
/*	-----------------------------------------	*/
private	struct	rest_response * paas_delete_request( char * url )
{
	struct	rest_header * hptr;
	char 	burl[2048];
	sprintf(burl,"%s://%s:%u%s%s", Paas.service, Paas.host, Paas.port, Paas.base, url );
	if (!( hptr = paas_authenticate()))
		return((struct rest_response *) 0);
	else	return( rest_client_get_request( burl, Paas.tls, Paas.agent, hptr ) );
}

/*	-----------------------------------------	*/
/*	    p a a s _ p o s t _ r e q u e s t		*/
/*	-----------------------------------------	*/
private	struct	rest_response * paas_post_request( char * url, char * filename, struct rest_header * ctptr )
{
	struct	rest_header * hptr;
	char 	burl[2048];
	sprintf(burl,"%s://%s:%u%s%s", Paas.service, Paas.host, Paas.port, Paas.base,url );
	if (!( hptr = paas_authenticate()))
		return((struct rest_response *) 0);
	else
	{
	 	if (!( ctptr ))
			ctptr = hptr;
		else 
		{
			ctptr->next = hptr;
			hptr->previous = ctptr;
		}
		return( rest_client_post_request( burl, Paas.tls, Paas.agent, filename, ctptr ) );
	}
}

/*	-----------------------------------------	*/
/*	    p a a s _ p u t _ r e q u e s t		*/
/*	-----------------------------------------	*/
private	struct	rest_response * paas_put_request( char * url, char * filename )
{
	struct	rest_header * hptr;
	char 	burl[2048];
	sprintf(burl,"%s://%s:%u%s%s", Paas.service, Paas.host, Paas.port, Paas.base,url );
	if (!( hptr = paas_authenticate()))
		return((struct rest_response *) 0);
	else	return( rest_client_put_request( burl, Paas.tls, Paas.agent, filename, hptr ) );
}

/*	-----------------------------------------	*/
/*		Application Management			*/
/*	-----------------------------------------	*/
/*	Start an application version instance		*/
/*	-----------------------------------------	*/
public	struct paas_response * start_paas_application(char * application )
{
	/* POST /app/{appId}/version/{versionId}/instance/{instanceId}/action/start */
	char 	uri[2048];
	sprintf(uri,"/app/%s/start",application);
	return( paas_result( paas_post_request( uri, (char *) 0,(struct rest_header *) 0 ) ));
}

/*	-----------------------------------------	*/
/*	retrieve the result of the operation		*/
/*	-----------------------------------------	*/
public	struct	paas_response * get_paas_task( char * task )
{
	/* GET /task/${taskid}	*/
	char 	uri[2048];
	sprintf(uri,"/task/%s",task);
	return( paas_result( paas_get_request( uri ) ));
}

/*	-----------------------------------------	*/
/*	Stop an application version instance		*/
/*	-----------------------------------------	*/
public	struct paas_response * stop_paas_application(char * application)
{
	/* POST /app/{appId}/stop */
	char 	uri[2048];
	sprintf(uri,"/app/%s/stop",application);
	return( paas_result( paas_post_request( uri, (char *) 0,(struct rest_header *) 0 ) ));
}
/*	-----------------------------------------	*/
/*	Creates a new application.			*/
/*	-----------------------------------------	*/
/*	If the application is multitenant, 		*/
/*	it will be accessible for all tenant		*/
/*	A Cloud Application Descriptor must be provided.*/
/*	- Application name				*/
/*	- Description					*/
/*	-Multi-tenant (yes/no).				*/
/*	-----------------------------------------	*/
public	struct paas_response * create_paas_application( char* filename )
{
	/* POST /app */
	struct	rest_header * hptr;
	if (!( hptr = rest_create_header( _HTTP_CONTENT_TYPE, "application/xml" ) ))
		return( (struct paas_response *) 0);
	else	return( paas_result( paas_post_request( "/app", filename, hptr ) ));
}

/*	-----------------------------------------	*/
/*	Updates an exiting application.			*/
/*	-----------------------------------------	*/
public	struct paas_response * update_paas_application( char * application, char* filename )
{
	/* POST /app */
	char 	uri[2048];
	struct	rest_header * hptr;
	if (!( hptr = rest_create_header( _HTTP_CONTENT_TYPE, "application/xml" ) ))
		return( (struct paas_response *) 0);

	else
	{
		sprintf(uri,"/app/%s/update",application);
		return( paas_result( paas_post_request( uri, filename, hptr ) ));
	}
}

/*	-----------------------------------------	*/
/*	List applications				*/
/*	-----------------------------------------	*/
public	struct paas_response * list_paas_applications()
{
	/* GET /app/ */
	return( paas_result( paas_get_request( "/app" ) ));
}

/*	-----------------------------------------	*/
/*	Describe application.				*/
/*	-----------------------------------------	*/
public	struct paas_response * get_paas_application(char * application)
{
	/* GET /app/{appId}		*/
	char 	uri[2048];
	sprintf(uri,"/app/%s",application);
	return( paas_result( paas_get_request( uri ) ));
}

/*	-----------------------------------------	*/
/*	Delete application.				*/
/*	Removes all existing versions			*/
/*	-----------------------------------------	*/
public	struct paas_response * delete_paas_application(char * application)
{
	/* DELETE /app/{appId} */
	char 	uri[2048];
	sprintf(uri,"/app/%s",application);
	return( paas_result( paas_delete_request( uri ) ));
}

/*	-----------------------------------------	*/
/*	Delete all applications.			*/
/*	-----------------------------------------	*/
public	struct paas_response * delete_paas_applications()
{
	/* DELETE /app/delete */
	return( paas_result( paas_delete_request( "/app/delete" ) ));
}

/*	-----------------------------------------	*/
/*		Environment Management			*/
/*	-----------------------------------------	*/

/*	-----------------------------------------	*/
/*	Starts an environment				*/
/*	-----------------------------------------	*/
public	struct paas_response * start_paas_environment(char * environment)
{
	/* POST /environment/{envId}/action/start */
	char 	uri[2048];
	sprintf(uri,"/environment/%s/start",environment);
	return( paas_result( paas_post_request( uri, (char *) 0,(struct rest_header *) 0 ) ));
}


/*	-----------------------------------------	*/
/*	Stops an environment				*/
/*	-----------------------------------------	*/
public	struct paas_response * stop_paas_environment(char * environment)
{
	/* POST /environment/{envId}/stop */
	char 	uri[2048];
	sprintf(uri,"/environment/%s/stop",environment);
	return( paas_result( paas_post_request( uri, (char *) 0,(struct rest_header *) 0 ) ));
}

/*	-----------------------------------------	*/
/*	Re Starts an environment 			*/
/*	-----------------------------------------	*/
public	struct paas_response * restart_paas_environment(char * environment)
{
	/* POST /environment/{envId}/action/start */
	char 	uri[2048];
	sprintf(uri,"/environment/%s/restart",environment);
	return( paas_result( paas_post_request( uri, (char *) 0,(struct rest_header *) 0 ) ));
}


/*	-----------------------------------------	*/
/*	Deploy an application instance on an 		*/
/*	available environment				*/
/*	-----------------------------------------	*/
public	struct paas_response * deploy_paas_application( char * environment, char * application, char * war, char * boundary )
{
	/* POST /environment/{envId}/action/deploy/app/{appId} */
	struct	rest_header * hptr;
	char 	uri[2048];
	char 	buffer[1024];
	sprintf(buffer,"multipart/form-data; boundary=%s",boundary);
	sprintf(uri,"/app/%s/action/deploy/env/%s",application,environment);
	if (!( hptr = rest_create_header( _HTTP_CONTENT_TYPE, buffer ) ))
		return((struct paas_response *) 0);
	else	return( paas_result( paas_post_request( uri, war, hptr  ) ));
}

/*	-----------------------------------------	*/
/*	Undeploy an application instance on an 		*/
/*	available environment				*/
/*	-----------------------------------------	*/
public	struct paas_response * undeploy_paas_application( char * environment, char * application)
{
	/* POST /environment/{envId}/action/undeploy/app/{appId} */
	char 	uri[2048];
	sprintf(uri,"/app/%s/action/undeploy/env/%s",
		application,environment);
	return( paas_result( paas_post_request( uri, (char *) 0,(struct rest_header *) 0 ) ));
}

/*	-----------------------------------------	*/
/*	Creates a new environment.			*/
/*	An environment template descriptor must be	*/
/*/	 provided.					*/
/*	-----------------------------------------	*/
public	struct paas_response * create_paas_environment( char * filename )
{
	/* POST /environment	*/
	struct	rest_header * hptr;
	if (!( hptr = rest_create_header( _HTTP_CONTENT_TYPE, "application/xml" ) ))
		return( (struct paas_response *) 0);
	else 	return( paas_result( paas_post_request( "/environment", filename,hptr ) ));
}

public	struct paas_response * update_paas_environment( char * id, char * filename )
{
	/* POST /environment/is/update	*/
	char 	buffer[2048];
	struct	rest_header * hptr;
	if (!( hptr = rest_create_header( _HTTP_CONTENT_TYPE, "application/xml" ) ))
		return( (struct paas_response *) 0);
	else
	{
		sprintf(buffer,"/environment/%s/update",id);
		return( paas_result( paas_post_request( buffer, filename,hptr ) ));
	}
}

/*	-----------------------------------------	*/
/*	Deletes an environment				*/
/*	-----------------------------------------	*/
public	struct paas_response * delete_paas_environment( char * environment )
{
	/* DELETE /environment/{envId} */
	char 	uri[2048];
	sprintf(uri,"/environment/%s",environment);
	return( paas_result( paas_delete_request( uri ) ));
}

/*	-----------------------------------------	*/
/*	List the available environments			*/
/*	-----------------------------------------	*/
public	struct paas_response * list_paas_environment()
{
	/* GET /environment */
	return( paas_result( paas_get_request( "/environment" ) ));
}

/*	-----------------------------------------	*/
/*	Get the description of an environment		*/
/*	-----------------------------------------	*/
public	struct paas_response * get_paas_environment(char * environment)
{
	/* GET /environment/{envid} */
	char 	uri[2048];
	sprintf(uri,"/environment/%s",environment);
	return( paas_result( paas_get_request( uri ) ));
}

/*	-----------------------------------------	*/
/*	List the deployed application instances 	*/
/*	in an environment				*/
/*	-----------------------------------------	*/
public	struct paas_response * get_paas_environment_application_instances(char * environment)
{
	/* GET /environment/{envid}/app/ */
	char 	uri[2048];
	sprintf(uri,"/environment/%s/app",environment);
	return( paas_result( paas_get_request( uri ) ));
}

/*	------------------------------------------------	*/
/*	  t e r m i n a t e _ p a a s _ c l i e n t		*/
/*	------------------------------------------------	*/
public	int	terminate_paas_client()
{
	if ( Paas.agent )
		Paas.agent = liberate( Paas.agent );
	if ( Paas.tls )
		Paas.tls = liberate( Paas.tls );
	if ( Paas.host )
		Paas.host = liberate( Paas.host );
	if ( Paas.base )
		Paas.base = liberate( Paas.base );
	return( 0 );
}

/*	------------------------------------------------	*/
/*	  i n i t i a l i s e _ p a a s _ c l i e n t		*/
/*	------------------------------------------------	*/
public	int	initialise_paas_client( char * agent, char * tls, char * host, int port, char * user, char * password )
{
	struct	url * uptr;

	terminate_paas_client();

	/* ---------------------------- */
	/* use the mandatory parameters */
	/* ---------------------------- */
	if (!( Paas.agent = allocate_string( agent )))
		return( 27 );
	else if (!( Paas.user = allocate_string( user )))
		return( 27 );
	else if (!( Paas.pass = allocate_string( password )))
		return( 27 );
	else if (!( Paas.base = allocate_string( _COAPS_BASE )))
		return( 27 );
	else if (!( uptr = analyse_url( host ) ))
		return( 30 );
	else
	{
		/* --------------------------------- */
		/* analyse and use the port and host */
		/* --------------------------------- */
		if (!( port ))
			port = uptr->port;

		if (!( Paas.host = allocate_string( uptr->host ) ))
		{
			uptr = liberate_url( uptr );
			return( 27 );
		}
		else	uptr = liberate_url( uptr );

		/* -------------------------------------------- */
		/* handle the optional transport layer security */
		/* -------------------------------------------- */ 
		if (!( rest_valid_string( tls ) ))
			Paas.tls = (char *) 0;
		else if (!( Paas.tls = allocate_string( tls )))
			return( 27 );

		/* ------------------------------- */
		/* set the service prefix and port */
		/* ------------------------------- */
		if (!( rest_valid_string( Paas.tls ) ))
		{
			Paas.service = "http";
			if (!( Paas.port = port ))
				Paas.port = 80;
		}
		else
		{
			Paas.service = "https";
			if (!( Paas.port = port ))
				Paas.port = 443;
		}
		return( 0 );
	}
}

	/* -------------- */
#endif	/* _paasclient_c */
	/* -------------- */


