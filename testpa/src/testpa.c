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
#ifndef	_testpa_c
#define	_testpa_c

#include "paclient.h"
private	int	debug=0;
private	int	verbose=0;
private	char *	tls=(char *) 0;
private	char *	publisher=(char *) 0;
public	int	check_debug()	{	return(debug);		}
public	int	check_verbose()	{	return(verbose);	}
public	char *	default_publisher()	{	return(publisher);	}
public	char *	default_tls()		{	return(tls);		}

public	int	failure( int e, char * m1, char * m2 )
{
	if ( e )
	{
		printf("\n*** failure %u",e);
		if ( m1 )
			printf(" : %s",m1);
		if ( m2 )
			printf(" : %s",m2);
		printf(" **`\n");
	}
	return( e );
}

private	int	pa_result( struct pa_response * rptr )
{
	char	buffer[2048];
	if ( rptr )
	{
		if ( rptr )
		{
			switch ( rptr->nature )
			{
			case	_TEXT_JSON	:
				json_show( rptr->jsonroot );
				break;
			default			:
			case	_TEXT_XML	:
				if (( rptr->response->status != 204 )
				&&  ( rptr->response->body ))
				{
					if ( rptr->response->type == _FILE_BODY )
					{
						sprintf(buffer,"cat %s",rptr->response->body);
						system( buffer );
					}
					else 	printf("\n%s\n",rptr->response->body);
					printf("\n");
				}
			}
		}
		liberate_pa_response( rptr );
		return(0);
	}
	else	return( failure(99,"no","result") );
}

private	int	pa_operation( char * p1, char * p2, char * p3, char * p4, char * p5, char * p6, char * p7 )
{
	struct	rest_header * hptr = (struct rest_header *) 0;
	char	*	agent = "CO-PACLIENT/1.0";
	char	*	nomfic;
	char	*	personality="";
	char	*	resource=_CORDS_LAUNCH_CFG;

	if (!( p2 ))
		return( failure( 30,"p2", "required") );

	else if (!( p1))
		return( failure( 30,"p1", "required") );

	else if (!( strcasecmp(p1,"LIST" ) ))
	{
		if (!( p2 ))
			return( failure(33, "missing", "parameter" ));
		else if (!( strcasecmp( p2, "HOSTS" ) ))
			pa_result( pa_list_servers() );
		else if (!( strcasecmp( p2, "DEPLOYMENTS" ) ))
			pa_result( pa_list_deployments(p3) );
		else if (!( strcasecmp( p2, "CERTIFICATES" ) ))
			pa_result( pa_list_certificates(p3) );
		else if (!( strcasecmp( p2, "GROUPS" ) ))
			pa_result( pa_list_affinity_groups() );
		else if (!( strcasecmp( p2, "STORAGE" ) ))
			pa_result( pa_list_storage_services() );
		else if (!( strcasecmp( p2, "OPERATIONS" ) ))
			pa_result( pa_list_operations(p3,p4) );
		else if (!( strcasecmp( p2, "PROFILES" ) ))
			pa_result( pa_list_WATM_profiles() );
		else if (!( strcasecmp( p2, "DEFINITIONS" ) ))
			pa_result( pa_list_WATM_definitions(p3) );
		else if (!( strcasecmp( p2, "LOCATIONS" ) ))
			pa_result( pa_list_locations() );
		else	return( failure(33, p1, p2 ) );
		return(0);
	}
	else if (!( strcasecmp(p1,"CREATE" ) ))
	{
		if  (!( strcasecmp( p2, "HOST" ) ))
		{
			//if (!( nomfic = pa_create_server_request( p3, p3, p4, p5, p6 ) ))
			//	return( failure(27,"cannot create","request" ) );
			//else
			//{ 	
				//pa_result( pa_create_server( nomfic ) );
				pa_result( pa_create_server( p3 ) );
				return( 0 );
			//}
		}
		else if (!( strcasecmp( p2, "DEPLOYMENTS" ) ))
		{
			if (!( nomfic = pa_create_deployment_request( p3, p3, p4, p5 ) ))
				return( failure(27,"cannot create","deployment request" ) );
			else
			{ 	
				pa_result( pa_create_deployment( nomfic, p7, p7 ) );
				return( 0 );
			}
		}
		else if  (!( strcasecmp( p2, "GROUP" ) ))
		{
			if (!( nomfic = pa_create_affinity_group_request( p3, p3, p4, p5 ) ))
				return( failure(27,"cannot create","affinity group request" ) );
			else
			{ 	
				pa_result( pa_create_affinity_group( nomfic ) );
				return( 0 );
			}
		}

		else if  (!( strcasecmp( p2, "STORAGE" ) ))
		{
			if (!( nomfic = pa_create_storage_service_request( p3, p3, p4, p5, p6 ) ))
				return( failure(27,"cannot create","storage service request" ) );
			else
			{ 	
				pa_result( pa_create_storage_service( nomfic ) );
				return( 0 );
			}
		}

	}
	else if (!( strcasecmp(p1,"GET" ) ))
	{
		if (!( p2 ))
			return( failure(33, "missing", "parameter" ));
		else if (!( strcasecmp( p2, "SUBSCRIPTION" ) ))
			pa_result( pa_get_subscription() );
		else if (!( strcasecmp( p2, "DEPLOYMENT") ))
			pa_result( pa_get_deployment( p3, p4 ) );
		else if (!( strcasecmp( p2, "HOST" ) ))
			pa_result( pa_get_server( p3 ) );
		else if (!( strcasecmp( p2, "FLAVOR" ) ))
			pa_result( pa_get_flavor( p3 ) );
		else if (!( strcasecmp( p2, "IMAGE" ) ))
			pa_result( pa_get_image( p3 ) );
		else if (!( strcasecmp( p2, "GROUP" ) ))
			pa_result( pa_retrieve_affinity_group( p3 ) );
		else if (!( strcasecmp( p2, "STORAGE") ))
			pa_result( pa_retrieve_storage_service( p3 ) );
		else	return( failure(33, p1, p2 ) );
		return(0);
	}
	else if (!( strcasecmp(p1,"UPDATE" ) ))
	{
		if (!( p2 ))
			return( failure(33, "missing", "parameter" ));
		else if (!( strcasecmp( p2, "HOST" ) ))
		{
			if (!( nomfic = pa_create_server_request( p2, p3, p4, personality, resource ) ))
				pa_result( pa_update_server( p4, nomfic ) );
		}
		else	return( failure(33, p1, p2 ) );
		return( 0 );
	}
	else if (!( strcasecmp(p1,"DELETE" ) ))
	{
		if (!( p2 ))
			return( failure(33, "missing", "parameter" ));
		else if (!( strcasecmp( p2, "DEPLOYMENT") ))
			pa_result( pa_delete_deployment( p3, p4 ) );
		else if (!( strcasecmp( p2, "HOST" ) ))
			pa_result( pa_delete_server( p3 ) );
		else if (!( strcasecmp( p2, "IMAGE" ) ))
			pa_result( pa_delete_image( p3 ) );
		else if (!( strcasecmp( p2, "GROUP" ) ))
			pa_result( pa_delete_affinity_group( p3 ) );
		else if (!( strcasecmp( p2, "STORAGE" ) ))
			pa_result( pa_delete_storage_service( p3 ) );
		else	return( failure(33, p1, p2 ) );
		return(0);
	}
	else	return( failure(32, "incorrect p1", p1 ) );
}

private	int	pa_command(int argc, char * argv[] )
{
	int	status;
	int	argi=1;
	char *	aptr;
	char *	user="ijm";
	char *	pass="4362355f-fd84-47c7-a1ca-582a368ddc50";
	char *	host="https://management.core.windows.net/";
	char *	thost="https://127.0.0.1:8080/";
	char *  other="https://windows.azure.com/";
	char *	agent="CO-PACLIENT/1.0";
	char *	version="2011-10-01";
	char * 	subscription="f346740d-e45e-42e9-80b8-4865f3a855d1";
	char *	namespace="http://schemas.microsoft.com/windowsazure";
	while ( argi < argc )
	{
		if (!( aptr = argv[argi++] ))
			break;
		else if ( *aptr != '-' )
		{
			if ((status = pa_initialise_client( user, pass, host, agent, version, tls,
					namespace, subscription )) != 0)
				return( failure( status, "initialising", "client" ) );
			return( pa_operation( aptr, 
				( argi < argc ? argv[argi] : (char *) 0 ),
				( (argi+1) < argc ? argv[argi+1] : (char *) 0 ),
				( (argi+2) < argc ? argv[argi+2] : (char *) 0 ),
				( (argi+3) < argc ? argv[argi+3] : (char *) 0 ),
				( (argi+4) < argc ? argv[argi+4] : (char *) 0 ),
				( (argi+5) < argc ? argv[argi+5] : (char *) 0 ) ) );

		}
		else if (  *(++aptr) == '-' )
		{
			aptr++;

			if (!( strcasecmp( aptr,"user" ) ))
				user = argv[argi++];
			else if (!( strcasecmp( aptr,"password" ) ))
				pass = argv[argi++];
			else if (!( strcasecmp( aptr,"version" ) ))
				version = argv[argi++];
			else if (!( strcasecmp( aptr,"host" ) ))
				host = argv[argi++];
			else if (!( strcasecmp( aptr,"agent" ) ))
				agent = argv[argi++];
			else if (!( strcasecmp( aptr,"tls" ) ))
				tls = argv[argi++];
			else if (!( strcasecmp( aptr,"verbose" ) ))
				verbose = 1;
			else if (!( strcasecmp( aptr,"debug" ) ))
				debug=1;
			else	return(failure(30,"incorrect","option"));
			continue;
		}
		else
		{
			switch( *(aptr++) )
			{
			case	'v'	:
				verbose=1; continue;
			case	'd'	:
				debug=1; continue;
			default		:
				return(failure(30,"incorrect","option"));
			}
		}
	}
	return(0);
}

private	int	pa_banner()
{
	printf("\n   CO-OS : CompatibleOne ProActive Client Test : Version 1.0a.0.02");
	printf("\n   Beta Version 08/02/2012");
	printf("\n   Copyright (c) 2011,2012 Iain James Marshall, Prologue ");
	printf("\n");
	printf("\n   General Options ");
	printf("\n");
	printf("\n   --verbose, --debug   ");
	printf("\n   --tls      <name>    ");
	printf("\n   --host     <address> ");
	printf("\n   --user     <username> ");
	printf("\n   --password <password> ");
	printf("\n   --agent    <name>     ");
	printf("\n   --version  <value>    ");
	printf("\n");
	printf("\n   CRUD Operations ");
	printf("\n");
	printf("\n   LIST   [ HOSTS   | STORAGE  | OPERATIONS  | LOCATIONS ");
	printf("\n            GROUPS  | PROFILES | DEFINITIONS | CERTIFICATES ] ");
	printf("\n   LIST   [ DEPLOYMENT ] <host> " );
	printf("\n   CREATE [ GROUP   <name> <description> <location>         ] ");
	printf("\n   CREATE [ HOST    <name> <description> <location> <group> ] ");
	printf("\n   CREATE [ STORAGE <name> <description> <location> <group> ] ");
	printf("\n   CREATE [ DEPLOYMENT <name> <image> <configuration> <host> <slot> ] ");
	printf("\n   GET    [ SUBSCRIPTION | HOST | GROUP | CERTIFICATE | STORAGE ] <id> ");
	printf("\n   GET    [ DEPLOYMENT ] <host> <slot> " );
	printf("\n   UPDATE HOST <id> ");
	printf("\n   DELETE [ HOST | GROUP | STORAGE | CERTIFICATE | LOCATION ] <id> ");
	printf("\n   DELETE [ DEPLOYMENT ] <host> <slot> " );
	printf("\n\n");
	return( 0 );
}

public	int	main(int argc, char * argv[] )
{
	if ( argc == 1 )
		return( pa_banner() );
	else	return( pa_command(argc,argv) );
}

	/* --------- */
#endif	/* _testpa_c */
	/* --------- */

