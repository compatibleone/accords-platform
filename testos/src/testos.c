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
#ifndef	_testos_c
#define	_testos_c

#include "osclient.h"

private char *	group=(char *) 0;
private	int	detail=0;
private	int	debug=0;
private	int	verbose=0;
private	char *	tls=(char *) 0;
private	char *	publisher=(char *) 0;
private	char *	operator="accords";
private	char *	zone="europe";
public	int	check_debug()		{	return(debug);		}
public	int	check_verbose()		{	return(verbose);	}
public	char *	default_publisher()	{	return(publisher);	}
public	char *	default_tls()		{	return(tls);		}
public	char *	default_operator()	{	return(operator);	}
public	char *	default_zone()		{	return(zone);		}

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

private	int	os_result( struct os_response * rptr )
{
	char	buffer[2048];
	if ( rptr )
	{
		switch ( rptr->nature )
		{
		case	_TEXT_JSON	:
			json_show( rptr->jsonroot );
			printf("\n");		
			break;
		case	_TEXT_XML	:
		default			:
			if (( rptr->response->status != 204 )
			&&  ( rptr->response->body ))
			{
				sprintf(buffer,"cat %s",rptr->response->body);
				system( buffer );
				printf("\n");
			}
		}
		liberate_os_response( rptr );
		return(0);
	}
	else	return( failure(99,"no","result") );
}

private	int	os_operation( char * p1, char * p2, char * p3, char * p4, char * p5 )
{
	struct	rest_header * hptr = (struct rest_header *) 0;
	char	*	agent = "OS-CLIENT/1.0";
	char	*	nomfic;
	char	*	personality="";
	char	*	resource=_CORDS_LAUNCH_CFG;

	if (!( p1 ))
		return( failure( 30,"p1", "required") );

	else if (!( strcasecmp(p1,"ADDRESS" ) ))
	{
		os_result( os_create_address() );
		return( 0 );
	}
	else if (!( p2 ))
		return( failure( 30,"p2", "required") );

	else if (!( strcasecmp(p1,"LIST" ) ))
	{
		if (!( p2 ))
			return( failure(33, "missing", "parameter" ));
		else if (!( strcasecmp( p2, "SERVERS" ) ))
			os_result( ( detail ? os_list_server_details() : os_list_servers()) );
		else if (!( strcasecmp( p2, "FLAVORS" ) ))
			os_result( ( detail ? os_list_flavor_details() : os_list_flavors()) );
		else if (!( strcasecmp( p2, "IMAGES" ) ))
			os_result( ( detail ? os_list_image_details() : os_list_images()) );
		else if (!( strcasecmp( p2, "ADDRESSES" ) ))
			os_result( ( detail ? os_list_floating_ip_details() : os_list_floating_ips()) );
		else if (!( strcasecmp( p2, "GROUPS" ) ))
			os_result( ( detail ? os_list_security_group_details() : os_list_security_groups()) );
		else if (!( strcasecmp( p2, "METADATA" ) ))
			os_result( os_list_metadata( p3 ) );
		else	return( failure(33, p1, p2 ) );
		return(0);
	}

	else if (!( strcasecmp(p1,"CREATE" ) ))
	{
		if (!( nomfic = os_create_server_request( p2, p3, p4, p5, personality, resource, group, zone ) ))
			return( failure(27,"cannot create server","request" ) );
		else
		{ 	
			os_result( os_create_server( nomfic ) );
			return( 0 );
		}
	}
	else if (!( strcasecmp(p1,"SNAPSHOT" ) ))
	{
		if (!( nomfic = os_create_image_request( p2, p3 ) ))
			return( failure(27,"cannot create snapshot","request" ) );
		else
		{ 	
			os_result( os_create_image( nomfic, p3 ) );
			return( 0 );
		}
	}
	else if (!( strcasecmp(p1,"ASSOCIATE" ) ))
	{
		if (!( nomfic = os_create_address_request( p2 ) ))
			return( failure(27,"cannot create snapshot","request" ) );
		else
		{ 	
			os_result( os_server_address( nomfic, p3 ) );
			return( 0 );
		}
	}
	else if (!( strcasecmp(p1,"METADATA" ) ))
	{
		if (!( nomfic = os_create_metadata_request( p3 ) ))
			return( failure(27,"cannot create","request" ) );
		else
		{ 	
			os_result( os_create_metadata( p2, nomfic ) );
			return( 0 );
		}
	}
	else if (!( strcasecmp(p1,"GROUP" ) ))
	{
		if (!( nomfic = os_create_security_group_request( p2 ) ))
			return( failure(27,"cannot create","security group request" ) );
		else
		{ 	
			os_result( os_create_security_group( nomfic ) );
			return( 0 );
		}
	}
	else if (!( strcasecmp(p1,"RULE" ) ))
	{
		if (!( nomfic = os_create_security_rule_request( p2, p3, p4, p5, "0.0.0.0/0" ) ))
			return( failure(27,"cannot create","security rule request" ) );
		else
		{ 	
			os_result( os_create_security_rule( nomfic ) );
			return( 0 );
		}
	}
	else if (!( strcasecmp(p1,"FLAVOR" ) ))
	{
		if (!( nomfic = os_create_flavor_request( p2, p3, p4, p5 ) ))
			return( failure(27,"cannot create","flavor request" ) );
		else
		{ 	
			os_result( os_create_flavor( nomfic ) );
			return( 0 );
		}
	}
	else if (!( strcasecmp(p1,"GET" ) ))
	{
		if (!( p2 ))
			return( failure(33, "missing", "parameter" ));
		else if (!( strcasecmp( p2, "SERVER" ) ))
			os_result( os_get_server( p3 ) );
		else if (!( strcasecmp( p2, "FLAVOR" ) ))
			os_result( os_get_flavor( p3 ) );
		else if (!( strcasecmp( p2, "IMAGE" ) ))
			os_result( os_get_image( p3 ) );
		else if (!( strcasecmp( p2, "ADDRESS" ) ))
			os_result( os_get_address( p3 ) );
		else if (!( strcasecmp( p2, "GROUP" ) ))
			os_result( os_get_security_group( p3 ) );
		else if (!( strcasecmp( p2, "METADATA" ) ))
			os_result( os_get_metadata( p3, p4 ) );
		else	return( failure(33, p1, p2 ) );
		return(0);
	}
	else if (!( strcasecmp(p1,"PUT" ) ))
	{
		if (!( p2 ))
			return( failure(33, "missing", "parameter" ));
		else if (!( strcasecmp( p2, "SERVER" ) ))
			os_result( os_update_server( p4, nomfic ) );
		else if (!( strcasecmp( p2, "METADATA" ) ))
		{
			if ((!( p3 )) || (!( p4 )) || (!( p5 )))
				return( failure(33,p1,p2) );
			else	os_result( os_update_metadata( p3, p4, p5 ) );
		}
		else	return( failure(33, p1, p2 ) );
		return( 0 );
	}
	else if (!( strcasecmp(p1,"DELETE" ) ))
	{
		if (!( p2 ))
			return( failure(33, "missing", "parameter" ));
		else if (!( strcasecmp( p2, "SERVER" ) ))
			os_result( os_delete_server( p3 ) );
		else if (!( strcasecmp( p2, "IMAGE" ) ))
			os_result( os_delete_image( p3 ) );
		else if (!( strcasecmp( p2, "ADDRESS" ) ))
			os_result( os_delete_address( p3 ) );
		else if (!( strcasecmp( p2, "FLAVOR" ) ))
			os_result( os_delete_flavor( p3 ) );
		else if (!( strcasecmp( p2, "GROUP" ) ))
			os_result( os_delete_security_group( p3 ) );
		else if (!( strcasecmp( p2, "RULE" ) ))
			os_result( os_delete_security_rule( p3 ) );
		else if (!( strcasecmp( p2, "METADATA" ) ))
			os_result( os_delete_metadata( p3, p4 ) );
		else	return( failure(33, p1, p2 ) );
		return(0);
	}
	else	return( failure(32, "incorrect p1", p1 ) );
}

private	int	os_command(int argc, char * argv[] )
{
	int	status;
	int	argi=1;
	char *	aptr;

	char *	user=(char *) 0;
	char *	pass=(char *) 0;
	char *	host=(char *) 0;
	char *	tenant=(char *) 0;
	char *	agent="CO-OSCLIENT/1.0";
	char *	version="v1.1";

	rest_initialise_log(0);

	while ( argi < argc )
	{
		if (!( aptr = argv[argi++] ))
			break;
		else if ( *aptr != '-' )
		{
			if (!( host ))
				return( failure( status, "missing", "--host parameter" ) );
			else if (!( user ))
				return( failure( status, "missing", "--user parameter" ) );
			else if (!( pass ))
				return( failure( status, "missing", "--password parameter" ) );
			else if ((status = os_initialise_client( user, pass, tenant, host, agent, version, tls )) != 0)
				return( failure( status, "initialising", "client" ) );
			else 	return( os_operation( aptr, 
					( argi < argc ? argv[argi] : (char *) 0 ),
					( (argi+1) < argc ? argv[argi+1] : (char *) 0 ),
					( (argi+2) < argc ? argv[argi+2] : (char *) 0 ),
					( (argi+3) < argc ? argv[argi+3] : (char *) 0 ) ) );
		}
		else if (  *(++aptr) == '-' )
		{
			aptr++;

			if (!( strcasecmp( aptr,"user" ) ))
				user = argv[argi++];
			else if (!( strcasecmp( aptr,"password" ) ))
				pass = argv[argi++];
			else if (!( strcasecmp( aptr,"tenant" ) ))
				tenant = argv[argi++];
			else if (!( strcasecmp( aptr,"group" ) ))
				group = argv[argi++];
			else if (!( strcasecmp( aptr,"zone" ) ))
				zone = argv[argi++];
			else if (!( strcasecmp( aptr,"version" ) ))
				version = argv[argi++];
			else if (!( strcasecmp( aptr,"detail" ) ))
				detail = 1;
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

private	int	os_banner()
{
	printf("\n   CO-OS : CompatibleOne OpenStack Client Test : Version 1.0b.0.01");
	printf("\n   Beta Version 22/04/2012");
	printf("\n   Copyright (c) 2011, 2012 Iain James Marshall, Prologue ");
	printf("\n");
	printf("\n   CRUD Operations ");
	printf("\n");
	printf("\n   LIST [ SERVERS | IMAGES | FLAVORS | ADDRESSES | GROUPS | METADATA <id> ]  ");
	printf("\n   CREATE   <name> <image> <flavor> <ip> ");
	printf("\n   FLAVOR   <name> <ram> <cpus> <disk> ");
	printf("\n   GROUP    <name> ");
	printf("\n   RULE     <group> <protocol> <from> <to> ");
	printf("\n   SNAPSHOT <name> <server> ");
	printf("\n   ADDRESS ");
	printf("\n   METADATA  <id>  <names=values>   ");
	printf("\n   ASSOCIATE <address> <serverid>   ");
	printf("\n   GET    [ SERVER | FLAVOR | IMAGE | GROUP | METADATA ] <id> [ <name> ] ");
	printf("\n   PUT    [ SERVER <id> | METADATA <id> <name> <value> ] ");
	printf("\n   DELETE [ SERVER <id> | IMAGE <id> | ADDRESS <id> | GROUP <id> | RULE <id> | METADATA <id> <name> ] ");
	printf("\n");
	printf("\n   Options");
	printf("\n     --user <username>     set account user name ");
	printf("\n     --password <value>    set account password  ");
	printf("\n     --tenant <value>      set account tenant    ");
	printf("\n     --host <hostname>     set host name         ");
	printf("\n     --version <value>     set host version      ");
	printf("\n     --agent   <name>      set test agent name   ");
	printf("\n     --tls <config>        set tls config        ");
	printf("\n     --detail              display detailed lists ");
	printf("\n     --verbose             activate verbose messages ");
	printf("\n     --debug               activate debug messages ");
	printf("\n\n");
	return( 0 );
}

public	int	main(int argc, char * argv[] )
{
	if ( argc == 1 )
		return( os_banner() );
	else	return( os_command(argc,argv) );
}

	/* --------- */
#endif	/* _testos_c */
	/* --------- */

