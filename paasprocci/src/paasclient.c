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
	(char *) 0,	/* host		*/
		 0	/* port		*/
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
	return( rest_create_header( "X-PAAS-AUTHORIZE", "None" ) );
}

/*	-----------------------------------------	*/
/*	    p a a s _ g e t _ r e q u e s t		*/
/*	-----------------------------------------	*/
private	struct	rest_response * paas_get_request( char * url )
{
	struct	rest_header * hptr;
	if (!( hptr = paas_authenticate()))
		return((struct rest_response *) 0);
	else 	return( rest_client_get_request( url, Paas.tls, Paas.agent, hptr ) );
}

/*	-----------------------------------------	*/
/*	  p a a s _ d e l e t e _ r e q u e s t		*/
/*	-----------------------------------------	*/
private	struct	rest_response * paas_delete_request( char * url )
{
	struct	rest_header * hptr;
	if (!( hptr = paas_authenticate()))
		return((struct rest_response *) 0);
	else	return( rest_client_get_request( url, Paas.tls, Paas.agent, hptr ) );
}

/*	-----------------------------------------	*/
/*	    p a a s _ p o s t _ r e q u e s t		*/
/*	-----------------------------------------	*/
private	struct	rest_response * paas_post_request( char * url, char * filename )
{
	struct	rest_header * hptr;
	if (!( hptr = paas_authenticate()))
		return((struct rest_response *) 0);
	else	return( rest_client_post_request( url, Paas.tls, Paas.agent, filename, hptr ) );
}

/*	-----------------------------------------	*/
/*	    p a a s _ p u t _ r e q u e s t		*/
/*	-----------------------------------------	*/
private	struct	rest_response * paas_put_request( char * url, char * filename )
{
	struct	rest_header * hptr;
	if (!( hptr = paas_authenticate()))
		return((struct rest_response *) 0);
	else	return( rest_client_put_request( url, Paas.tls, Paas.agent, filename, hptr ) );
}

/*	-----------------------------------------	*/
/*		Application Management			*/
/*	-----------------------------------------	*/
/*	Start an application version instance		*/
/*	-----------------------------------------	*/
public	struct paas_response * start_paas_application(char * application, char * version, char * instance)
{
	/* POST /app/{appId}/version/{versionId}/instance/{instanceId}/action/start */
	char 	uri[2048];
	sprintf(uri,"/app/%s/version/%s/instance/%s/action/start",application, version, instance);
	return( paas_result( paas_post_request( uri, (char *) 0 ) ));
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
public	struct paas_response * stop_paas_application(char * application, char * version, char * instance)
{
	/* POST /app/{appId}/version/{versionId}/instance/{instanceId}/action/stop */
	char 	uri[2048];
	sprintf(uri,"/app/%s/version/%s/instance/%s/action/stop",application, version, instance);
	return( paas_result( paas_post_request( uri, (char *) 0 ) ));
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
public	struct paas_response * create_paas_application(char * artifact, char * description, char * version )
{
	/* POST /app */
	char *	filename=(char *) 0;
	FILE *	h;
	if (!( filename = rest_temporary_filename(".xml")))
		return((struct paas_response *) 0 );
	else if (!( h = fopen( filename, "w" )))
	{
		liberate( filename );
		return((struct paas_response *) 0 );
	}
	else
	{
		fprintf(h,"<application name=%c%s%c description=%c%s%c version=%c%s%c/>\n",
			0x0022,artifact,0x0022,
			0x0022,description,0x0022,
			0x0022,version,0x0022
			);		
		fclose(h);
		return( paas_result( paas_post_request( "/app", filename ) ));
	}
}

/*	-----------------------------------------	*/
/*	Creates a new version with either a file 	*/
/*	in attachment   				*/
/*	(Content-Type: multipart/form-data). 		*/
/*	or a url. 					*/
/*	The supported artefacts are ear, bundle, 	*/
/*	war, ejbjar or a zip (war dir).			*/
/*	A Cloud Application Version Descriptor must 	*/
/*	be provided.					*/
/*	- Artefact					*/
/*	- Description					*/
/*	- VersionLabel					*/
/*	-----------------------------------------	*/
public	struct paas_response * create_paas_application_version(char * application, char * artifact, char * description, char * version )
{
	/* POST /app/{appId}/version */
	char *	filename=(char *) 0;
	char 	uri[2048];
	FILE *	h;
	if (!( filename = rest_temporary_filename(".xml")))
		return((struct paas_response *) 0 );
	else if (!( h = fopen( filename, "w" )))
	{
		liberate( filename );
		return((struct paas_response *) 0 );
	}
	else
	{
		fprintf(h,"<application_version name=%c%s%c description=%c%s%c version=%c%s%c label=%c%s%c/>\n",artifact,description,version,application);		
		fclose(h);
		sprintf(uri,"/app/%s/version",application);	
		return( paas_result( paas_post_request( uri, filename ) ));
	}
}

/*	-----------------------------------------	*/
/*	Creates a new application version instance.	*/
/*	A Cloud Application Version Instance Descriptor	*/
/*	must be provided.				*/
/*	- Deployable topology mapping			*/
/*	- Environment uri				*/
/*	-----------------------------------------	*/
public	struct paas_response * create_paas_application_instance(char * application, char * version, char * topology, char * environment )
{
	/* POST /app/{appId}/version/{versionId}/instance */
	char *	filename=(char *) 0;
	char 	uri[2048];
	FILE *	h;
	if (!( filename = rest_temporary_filename(".xml")))
		return((struct paas_response *) 0 );
	else if (!( h = fopen( filename, "w" )))
	{
		liberate( filename );
		return((struct paas_response *) 0 );
	}
	else
	{
		fprintf(h,"<application_version_instance>\n");		
		fprintf(h,"</application_version_instance>\n");		
		fclose(h);
		sprintf(uri,"/app/%s/version/%s/instance",application,version);	
		return( paas_result( paas_post_request( uri, filename ) ));
	}
}

/*	-----------------------------------------	*/
/*	List applications				*/
/*	-----------------------------------------	*/
public	struct paas_response * list_paas_applications()
{
	/* GET /app/ */
	return( paas_result( paas_get_request( "/app/" ) ));
}

/*	-----------------------------------------	*/
/*	List application versions			*/
/*	-----------------------------------------	*/
public	struct paas_response * list_paas_application_versions(char * application)
{
	/* GET /app/{appId}/version */
	char 	uri[2048];
	sprintf(uri,"/app/%s/version",application);	
	return( paas_result( paas_get_request( uri ) ));
}

/*	-----------------------------------------	*/
/*	List application version instances		*/
/*	-----------------------------------------	*/
public	struct paas_response * list_paas_application_version_instances(char * application,char * version)
{
	/* GET /app/{appId}/version/{versionId} */

	char 	uri[2048];
	sprintf(uri,"/app/%s/version/%s",application,version);	
	return( paas_result( paas_get_request( uri ) ));
}

/*	-----------------------------------------	*/
/*	List application version artefacts		*/
/*	-----------------------------------------	*/
public	struct paas_response * list_paas_application_version_artefacts(char * application,char * version)
{
	/* GET /artefact/{appId}/{versionId}		*/
	char 	uri[2048];
	sprintf(uri,"/artefact/%s/%s",application,version);	
	return( paas_result( paas_get_request( uri ) ));
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
/*	Describe application version			*/
/*	-----------------------------------------	*/
public	struct paas_response * get_paas_application_version(char * application,char * version)
{
	/* GET /app/{appId}/version/{versionid}		*/
	char 	uri[2048];
	sprintf(uri,"/app/%s/version/%s",application,version);
	return( paas_result( paas_get_request( uri ) ));
}

/*	-----------------------------------------	*/
/*	Describe application version instance		*/
/*	-----------------------------------------	*/
public	struct paas_response * get_paas_application_version_instance(char * application,char * version,char * instance)
{
	/* GET /app/{appId}/version/{versionId}/instance/{instanceId}	*/
	char 	uri[2048];
	sprintf(uri,"/app/%s/version/%s/instance/%s",application,version,instance);
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
/*	Delete application version			*/
/*	-----------------------------------------	*/
public	struct paas_response * delete_paas_application_version(char * application,char * version)
{
	/* DELETE /app/{appId}/version/{versionId}	*/
	char 	uri[2048];
	sprintf(uri,"/app/%s/version/%s",application,version);
	return( paas_result( paas_delete_request( uri ) ));
}


/*	-----------------------------------------	*/
/*	 Delete application version instance		*/
/*	-----------------------------------------	*/
public	struct paas_response * delete_paas_application_version_instance(char * application,char * version,char * instance)
{
	/* DELETE /app/{appId}/version/{versionId}/instance/{instanceId} */
	char 	uri[2048];
	sprintf(uri,"/app/%s/version/%s/instance/%s",application,version);
	return( paas_result( paas_delete_request( uri ) ));
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
	sprintf(uri,"/environment/%s/action/start",environment);
	return( paas_result( paas_post_request( uri, (char *) 0 ) ));
}


/*	-----------------------------------------	*/
/*	Stops an environment				*/
/*	-----------------------------------------	*/
public	struct paas_response * stop_paas_environment(char * environment)
{
	/* POST /environment/{envId}/action/stop */
	char 	uri[2048];
	sprintf(uri,"/environment/%s/action/stop",environment);
	return( paas_result( paas_post_request( uri, (char *) 0 ) ));
}

/*	-----------------------------------------	*/
/*	Deploy an application instance on an 		*/
/*	available environment				*/
/*	-----------------------------------------	*/
public	struct paas_response * deploy_paas_application( char * environment, char * application, char * version, char * instance )
{
	/* POST /environment/{envId}/action/deploy/app/{appId}/version/{versionId}/instance/{instanceId} */
	char 	uri[2048];
	sprintf(uri,"/environment/%s/action/deploy/app/%s/version/%s/instance/%s",
		environment,application,version, instance);
	return( paas_result( paas_post_request( uri, (char *) 0 ) ));
}

/*	-----------------------------------------	*/
/*	Undeploy an application instance on an 		*/
/*	available environment				*/
/*	-----------------------------------------	*/
public	struct paas_response * undeploy_paas_application( char * environment, char * application, char * version, char * instance )
{
	/* POST /environment/{envId}/action/undeploy/app/{appId}/version/{versionId}/instance/{instanceId} */
	char 	uri[2048];
	sprintf(uri,"/environment/%s/action/undeploy/app/%s/version/%s/instance/%s",
		environment,application,version, instance);
	return( paas_result( paas_post_request( uri, (char *) 0 ) ));
}

/*	-----------------------------------------	*/
/*	Creates a new environment.			*/
/*	An environment template descriptor must be	*/
/*/	 provided.					*/
/*	-----------------------------------------	*/
public	struct paas_response * create_paas_environment( char * environment )
{
	/* POST /environment	*/
	char *	filename=(char *) 0;
	FILE *	h;
	if (!( filename = rest_temporary_filename(".xml")))
		return((struct paas_response *) 0 );
	else if (!( h = fopen( filename, "w" )))
	{
		liberate( filename );
		return((struct paas_response *) 0 );
	}
	else
	{
		fprintf(h,"<environment name=%c%s%c/>\n",environment);		
		fclose(h);
		return( paas_result( paas_post_request( "/environment", filename ) ));
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
	return( paas_result( paas_get_request( "/environment/" ) ));
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
	sprintf(uri,"/environment/%s/app/",environment);
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
	return( 0 );
}

/*	------------------------------------------------	*/
/*	  i n i t i a l i s e _ p a a s _ c l i e n t		*/
/*	------------------------------------------------	*/
public	int	initialise_paas_client( char * agent, char * tls, char * host, int port )
{
	terminate_paas_client();
	if (!( Paas.agent = allocate_string( agent )))
		return( 27 );
	else if (!( Paas.tls = allocate_string( tls )))
		return( 27 );
	else if (!( Paas.host = allocate_string( host )))
		return( 27 );
	else
	{
		Paas.port = port;
		return( 0 );
	}
}

	/* -------------- */
#endif	/* _paasclient_c */
	/* -------------- */

