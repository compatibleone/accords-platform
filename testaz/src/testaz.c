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
#ifndef	_testaz_c
#define	_testaz_c

#include "azclient.h"
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

private	int	az_result( struct az_response * rptr )
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
			case	_TEXT_XML	:
			default			:
				if ( rptr->response->type == _FILE_BODY )
				{
					sprintf(buffer,"cat %s",rptr->response->body);
					system( buffer );
				}
				else 	printf("\n%s\n",rptr->response->body);
			}
			printf("\n");
		}
		liberate_az_response( rptr );
		return(0);
	}
	else	return( failure(99,"no","result") );
}

private	int	az_operation( char * p1, char * p2, char * p3, char * p4 )
{
	struct	rest_header * hptr = (struct rest_header *) 0;
	char	*	agent = "CO-AZCLIENT/1.0";
	char	*	nomfic;
	char	*	personality="";
	char	*	resource=_CORDS_LAUNCH_CFG;
	if (!( p2 ))
		return( failure( 30,"p2", "required") );
	if (!( p1))
		return( failure( 30,"p1", "required") );
	if (!( strcmp(p1,"GET" ) ))
		return( az_result( az_client_get_request( p2, 0, agent, hptr ) ) );
	else if (!( strcmp(p1,"POST" ) ))
		return( az_result( az_client_post_request( p2, 0, agent, p3, hptr ) ) );
	else if (!( strcmp(p1,"DELETE" ) ))
		return( az_result( az_client_delete_request( p2, 0, agent, hptr ) ) );
	else if (!( strcmp(p1,"PUT" ) ))
		return( az_result( az_client_put_request( p2, 0, agent, p3, hptr ) ) );
	else if (!( strcmp(p1,"HEAD" ) ))
		return( az_result( az_client_head_request( p2, 0, agent, hptr ) ) );
	else if (!( strcmp(p1,"LIST" ) ))
	{
		if (!( p2 ))
			return( failure(33, "missing", "parameter" ));
		else if (!( strcmp( p2, "SERVERS" ) ))
			az_result( az_list_servers() );
		else if (!( strcmp( p2, "CERTIFICATES" ) ))
			az_result( az_list_certificates() );
		else if (!( strcmp( p2, "GROUPS" ) ))
			az_result( az_list_affinity_groups() );
		else if (!( strcmp( p2, "LOCATIONS" ) ))
			az_result( az_list_locations() );
		else	return( failure(33, p1, p2 ) );
		return(0);
	}
	else if (!( strcmp(p1,"CREATE" ) ))
	{
		if (!( nomfic = az_create_server_request( p2, p3, p4,personality, resource ) ))
			return( failure(27,"cannot create","request" ) );
		else
		{ 	
			az_result( az_create_server( nomfic ) );
			return( 0 );
		}
	}
	else if (!( strcmp(p1,"RETRIEVE" ) ))
	{
		if (!( p2 ))
			return( failure(33, "missing", "parameter" ));
		else if (!( strcmp( p2, "SERVER" ) ))
			az_result( az_get_server( p3 ) );
		else if (!( strcmp( p2, "FLAVOR" ) ))
			az_result( az_get_flavor( p3 ) );
		else if (!( strcmp( p2, "IMAGE" ) ))
			az_result( az_get_image( p3 ) );
		else	return( failure(33, p1, p2 ) );
		return(0);
	}
	else if (!( strcmp(p1,"UPDATE" ) ))
	{
		if (!( p2 ))
			return( failure(33, "missing", "parameter" ));
		else if (!( strcmp( p2, "SERVER" ) ))
		{
			if (!( nomfic = az_create_server_request( p2, p3, p4, personality, resource ) ))
				az_result( az_update_server( p4, nomfic ) );
		}
		else	return( failure(33, p1, p2 ) );
		return( 0 );
	}
	else if (!( strcmp(p1,"REMOVE" ) ))
	{
		if (!( p2 ))
			return( failure(33, "missing", "parameter" ));
		else if (!( strcmp( p2, "SERVER" ) ))
			az_result( az_delete_server( p3 ) );
		else if (!( strcmp( p2, "IMAGE" ) ))
			az_result( az_delete_image( p3 ) );
		else	return( failure(33, p1, p2 ) );
		return(0);
	}
	else	return( failure(32, "incorrect p1", p1 ) );
}

private	int	az_command(int argc, char * argv[] )
{
	int	status;
	int	argi=1;
	char *	aptr;
	char *	user="ijm";
	char *	pass="4362355f-fd84-47c7-a1ca-582a368ddc50";
	char *	host="https://management.core.windows.net/";
	char *	thost="https://127.0.0.1:8080/";
	char *  other="https://windows.azure.com/";
	char *	agent="CO-AZCLIENT/1.0";
	char *	version="2010-10-28";
	char * 	subscription="f346740d-e45e-42e9-80b8-4865f3a855d1";
	char *	namespace="http://schemas.microsoft.com/windowsazure";
	while ( argi < argc )
	{
		if (!( aptr = argv[argi++] ))
			break;
		else if ( *aptr != '-' )
		{
			if ((status = az_initialise_client( user, pass, host, agent, version, tls,
					namespace, subscription )) != 0)
				return( failure( status, "initialising", "client" ) );
			return( az_operation( aptr, 
				( argi < argc ? argv[argi] : (char *) 0 ),
				( (argi+1) < argc ? argv[argi+1] : (char *) 0 ),
				( (argi+2) < argc ? argv[argi+2] : (char *) 0 ) ) );
		}
		else if (  *(++aptr) == '-' )
		{
			aptr++;

			if (!( strcmp( aptr,"user" ) ))
				user = argv[argi++];
			else if (!( strcmp( aptr,"password" ) ))
				pass = argv[argi++];
			else if (!( strcmp( aptr,"version" ) ))
				version = argv[argi++];
			else if (!( strcmp( aptr,"host" ) ))
				host = argv[argi++];
			else if (!( strcmp( aptr,"agent" ) ))
				agent = argv[argi++];
			else if (!( strcmp( aptr,"tls" ) ))
				tls = argv[argi++];
			else if (!( strcmp( aptr,"verbose" ) ))
				verbose = 1;
			else if (!( strcmp( aptr,"debug" ) ))
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

private	int	az_banner()
{
	printf("\n   CO-OS : CompatibleOne Windows Azure Client Test : Version 1.0a.0.01");
	printf("\n   Beta Version 14/08/2011");
	printf("\n   Copyright (c) 2011 Iain James Marshall, Prologue ");
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
	printf("\n   LIST [ SERVERS | GROUPS | CERTIFICATES | LOCATIONS ] ");
	printf("\n   CREATE <name> <image> <flavor> ");
	printf("\n   RETRIEVE [ SERVER | GROUP | CERTIFICATE ] <id> ");
	printf("\n   UPDATE SERVER <id> ");
	printf("\n   REMOVE [ SERVER | GROUP | CERTIFICATE | LOCATION ] <id> ");
	printf("\n");
	printf("\n   REST Methods");
	printf("\n");
	printf("\n   [ GET | DELETE | HEAD ] <url> ");
	printf("\n   [ PUT | POST ] <url> <filename> ");
	printf("\n\n");
	return( 0 );
}

public	int	main(int argc, char * argv[] )
{
	if ( argc == 1 )
		return( az_banner() );
	else	return( az_command(argc,argv) );
}

	/* --------- */
#endif	/* _testaz_c */
	/* --------- */

