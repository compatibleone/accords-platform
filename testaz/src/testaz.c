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
private	char * 	operator="accords";
private	char *	zone="europe";
private	char *	hostingservice="compatibleone";
public	char *	default_operator()	{	return(operator);	}
public	char *	default_zone()		{	return(zone);		}
public	int	check_debug()		{	return(debug);		}
public	int	check_verbose()		{	return(verbose);	}
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
				if ( rptr->jsonroot )
					json_show( rptr->jsonroot );
				break;
			case	_TEXT_XML	:
				if ( rptr->xmlroot )
					document_show_element( rptr->xmlroot, 0 );
				break;
			default			:
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
		liberate_az_response( rptr );
		return(0);
	}
	else	return( failure(99,"no","result") );
}

private	int	az_operation( char * p1, char * p2, char * p3, char * p4, char * p5, char * p6, char * p7, char * p8, char * p9, char * p10 )
{
	struct	rest_header * hptr = (struct rest_header *) 0;
	char	*	agent = "CO-AZCLIENT/1.0";
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
		else if (!( strcasecmp( p2, "OS" ) ))
			az_result( az_list_os_images() );
		else if (!( strcasecmp( p2, "DISKS" ) ))
			az_result( az_list_os_disks() );
		else if (!( strcasecmp( p2, "HOSTS" ) ))
			az_result( az_list_hosted_services() );
		else if (!( strcasecmp( p2, "NETWORK" ) ))
			az_result( az_list_network() );
		else if (!( strcasecmp( p2, "CONTAINERS" ) ))
			az_result( az_list_containers( p3 ) );
		else if (!( strcasecmp( p2, "VM" ) ))
			az_result( az_list_vm( p3 ) );
		else if (!( strcasecmp( p2, "DEPLOYMENTS" ) ))
			az_result( az_list_deployments(p3) );
		else if (!( strcasecmp( p2, "CERTIFICATES" ) ))
			az_result( az_list_certificates(p3) );
		else if (!( strcasecmp( p2, "GROUPS" ) ))
			az_result( az_list_affinity_groups() );
		else if (!( strcasecmp( p2, "STORAGE" ) ))
			az_result( az_list_storage_services() );
		else if (!( strcasecmp( p2, "OPERATIONS" ) ))
			az_result( az_list_operations(p3,p4) );
		else if (!( strcasecmp( p2, "PROFILES" ) ))
			az_result( az_list_WATM_profiles() );
		else if (!( strcasecmp( p2, "DEFINITIONS" ) ))
			az_result( az_list_WATM_definitions(p3) );
		else if (!( strcasecmp( p2, "LOCATIONS" ) ))
			az_result( az_list_locations() );
		else	return( failure(33, p1, p2 ) );
		return(0);
	}
	else if (!( strcasecmp(p1,"CAPTURE" ) ))
	{
		if  (!( strcasecmp( p2, "VM" ) ))
		{

			if (!( nomfic = az_capture_vm_request( p5, p6, p7, atoi(p8)  ) ))
				return( failure(27,"cannot create","vm deployment request" ) );
			else
			{ 	
				az_result( az_operation_vm( nomfic, p3, p4 ) );
				return( 0 );
			}
		}
	}
	else if (!( strcasecmp(p1,"START" ) ))
	{
		if  (!( strcasecmp( p2, "VM" ) ))
		{

			if (!( nomfic = az_start_vm_request( ) ))
				return( failure(27,"cannot create","vm deployment request" ) );
			else
			{ 	
				az_result( az_operation_vm( nomfic, p3, p4 ) );
				return( 0 );
			}
		}
	}
	else if (!( strcasecmp(p1,"SHUTDOWN" ) ))
	{
		if  (!( strcasecmp( p2, "VM" ) ))
		{

			if (!( nomfic = az_shutdown_vm_request( ) ))
				return( failure(27,"cannot create","vm deployment request" ) );
			else
			{ 	
				az_result( az_operation_vm( nomfic, p3, p4 ) );
				return( 0 );
			}
		}
	}
	else if (!( strcasecmp(p1,"RESTART" ) ))
	{
		if  (!( strcasecmp( p2, "VM" ) ))
		{

			if (!( nomfic = az_restart_vm_request( ) ))
				return( failure(27,"cannot create","vm deployment request" ) );
			else
			{ 	
				az_result( az_operation_vm( nomfic, p3, p4 ) );
				return( 0 );
			}
		}
	}
	else if (!( strcasecmp(p1,"CREATE" ) ))
	{
		if  (!( strcasecmp( p2, "HOST" ) ))
		{
			if (!( nomfic = az_create_hosted_service_request( p3, p3, p4, p5, p6 ) ))
				return( failure(27,"cannot create","request" ) );
			else
			{ 	
				az_result( az_create_hosted_service( nomfic ) );
				return( 0 );
			}
		}
		else if  (!( strcasecmp( p2, "OS" ) ))
		{
			if (!( nomfic = az_create_os_request( p3, p4, p5, p6 ) ))
				return( failure(27,"cannot create","OS request" ) );
			else
			{ 	
				az_result( az_create_os_image( nomfic ) );
				return( 0 );
			}
		}
		else if  (!( strcasecmp( p2, "DISK" ) ))
		{
			if (!( nomfic = az_add_os_disk_request( p3, p4, p5, atoi(p6) ) ))
				return( failure(27,"cannot create","DISK request" ) );
			else
			{ 	
				az_result( az_add_os_disk( nomfic ) );
				return( 0 );
			}
		}
		else if (!( strcasecmp( p2, "DEPLOYMENTS" ) ))
		{
			if (!( nomfic = az_create_deployment_request( p3, p3, p4, p5 ) ))
				return( failure(27,"cannot create","deployment request" ) );
			else
			{ 	
				az_result( az_create_deployment( nomfic, p7, p7 ) );
				return( 0 );
			}
		}
		else if  (!( strcasecmp( p2, "VM" ) ))
		{

			if (!( nomfic = az_create_vm_request( p4, p3, p5, p6, p7, p8, p9, atoi(p10), (char *) 0 ) ))
				return( failure(27,"cannot create","vm deployment request" ) );
			else
			{ 	
				az_result( az_create_vm( nomfic ) );
				return( 0 );
			}
		}
		else if  (!( strcasecmp( p2, "GROUP" ) ))
		{
			if (!( nomfic = az_create_affinity_group_request( p3, p3, p4, p5 ) ))
				return( failure(27,"cannot create","affinity group request" ) );
			else
			{ 	
				az_result( az_create_affinity_group( nomfic ) );
				return( 0 );
			}
		}

		else if  (!( strcasecmp( p2, "STORAGE" ) ))
		{
			if (!( nomfic = az_create_storage_service_request( p3, p3, p4, p5, p6 ) ))
				return( failure(27,"cannot create","storage service request" ) );
			else
			{ 	
				az_result( az_create_storage_service( nomfic ) );
				return( 0 );
			}
		}
		else if (!( strcasecmp( p2, "NETWORK" ) ))
		{
			if (!( nomfic = az_create_network_config_request( p3, p4, p5, p6 ) ))
				return( failure(27,"cannot create","network config" ) );
			else
			{
				az_result( az_update_network_config(  nomfic ) );
				return(0);
			}
		}
		else	return( failure(33, p1, p2 ) );

	}
	else if (!( strcasecmp(p1,"GET" ) ))
	{
		if (!( p2 ))
			return( failure(33, "missing", "parameter" ));
		else if (!( strcasecmp( p2, "SUBSCRIPTION" ) ))
			az_result( az_get_subscription() );
		else if (!( strcasecmp( p2, "DEPLOYMENT") ))
			az_result( az_get_deployment( p3, p4 ) );
		else if (!( strcasecmp( p2, "HOST" ) ))
			az_result( az_get_hosted_service( p3 ) );
		else if (!( strcasecmp( p2, "VM" ) ))
			az_result( az_get_vm( p3, p4 ) );
		else if (!( strcasecmp( p2, "NETWORK" ) ))
			az_result( az_get_network_config() );
		else if (!( strcasecmp( p2, "FLAVOR" ) ))
			az_result( az_get_flavor( p3 ) );
		else if (!( strcasecmp( p2, "IMAGE" ) ))
			az_result( az_get_image( p3 ) );
		else if (!( strcasecmp( p2, "OS" ) ))
			az_result( az_get_os_image( p3 ) );
		else if (!( strcasecmp( p2, "DISK" ) ))
			az_result( az_get_os_disk( p3 ) );
		else if (!( strcasecmp( p2, "GROUP" ) ))
			az_result( az_retrieve_affinity_group( p3 ) );
		else if (!( strcasecmp( p2, "STORAGE") ))
			az_result( az_retrieve_storage_service( p3 ) );
		else	return( failure(33, p1, p2 ) );
		return(0);
	}
	else if (!( strcasecmp(p1,"UPDATE" ) ))
	{
		if (!( p2 ))
			return( failure(33, "missing", "parameter" ));
		else if (!( strcasecmp( p2, "HOST" ) ))
		{
			if (!( nomfic = az_create_hosted_service_request( p3, p3, p4, p5, p6 ) ))
				return( failure(27,"cannot create","affinity group request" ) );
			else
			{
				az_result( az_update_hosted_service( p4, nomfic ) );
				return(0);
			}
		}
		else	return( failure(33, p1, p2 ) );
		return( 0 );
	}
	else if (!( strcasecmp(p1,"DELETE" ) ))
	{
		if (!( p2 ))
			return( failure(33, "missing", "parameter" ));
		else if (!( strcasecmp( p2, "DEPLOYMENT") ))
			az_result( az_delete_deployment( p3, p4 ) );
		else if (!( strcasecmp( p2, "HOST" ) ))
			az_result( az_delete_hosted_service( p3 ) );
		else if (!( strcasecmp( p2, "VM" ) ))
			az_result( az_delete_vm( p3, p4 ) );
		else if (!( strcasecmp( p2, "OS" ) ))
			az_result( az_delete_os_image( p3 ) );
		else if (!( strcasecmp( p2, "DISK" ) ))
			az_result( az_delete_os_disk( p3 ) );
		else if (!( strcasecmp( p2, "IMAGE" ) ))
			az_result( az_delete_image( p3 ) );
		else if (!( strcasecmp( p2, "GROUP" ) ))
			az_result( az_delete_affinity_group( p3 ) );
		else if (!( strcasecmp( p2, "STORAGE" ) ))
			az_result( az_delete_storage_service( p3 ) );
		else if (!( strcasecmp( p2, "NETWORK" ) ))
		{
			if (!( nomfic = az_delete_network_config_request( p3, p4, p5, p6 ) ))
				return( failure(27,"cannot delete","network config" ) );
			else
			{
				az_result( az_delete_network_config( nomfic ) );
				return(0);
			}
		}
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
	char *	user="test";
	char *	pass="C1o2_i3p4s";
	char *	host="https://management.core.windows.net/";
	char *	thost="https://127.0.0.1:8080/";
	char *  other="https://windows.azure.com/";
	char *	agent="CO-AZCLIENT/1.0";
	char *	version="2012-03-01";
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
			else if (( status = az_initialise_service( hostingservice )) != 0)
				return( failure( status, "initialising", "service" ) );
			
			else return( az_operation( aptr, 
				( argi < argc ? argv[argi] : (char *) 0 ),
				( (argi+1) < argc ? argv[argi+1] : (char *) 0 ),
				( (argi+2) < argc ? argv[argi+2] : (char *) 0 ),
				( (argi+3) < argc ? argv[argi+3] : (char *) 0 ),
				( (argi+4) < argc ? argv[argi+4] : (char *) 0 ),
				( (argi+5) < argc ? argv[argi+5] : (char *) 0 ),
				( (argi+6) < argc ? argv[argi+6] : (char *) 0 ),
				( (argi+7) < argc ? argv[argi+7] : (char *) 0 ),
				( (argi+8) < argc ? argv[argi+8] : (char *) 0 ) ) );

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
			else if (!( strcasecmp( aptr,"service" ) ))
				hostingservice = argv[argi++];
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

private	int	az_banner()
{
	printf("\n   CO-OS : CompatibleOne Windows Azure Client Test : Version 1.0a.0.04");
	printf("\n   Beta Version 03/08/2012");
	printf("\n   Copyright (c) 2011,2012 Iain James Marshall, Prologue ");
	printf("\n");
	printf("\n   General Options ");
	printf("\n   --verbose, --debug   ");
	printf("\n   --tls            <name>    ");
	printf("\n   --host           <address> ");
	printf("\n   --user           <username> ");
	printf("\n   --password       <password> ");
	printf("\n   --agent          <name>     ");
	printf("\n   --hostingservice <name>     ");
	printf("\n   --version        <value>    ");
	printf("\n   AZURE Operations ");
	printf("\n   Hosted Service Operations: HOST");
	printf("\n          CREATE HOST <name> <description> <location> <group> ");
	printf("\n          LIST   HOSTS");
	printf("\n          GET HOST <name> ");
	printf("\n          UPDATE HOST <name> ");
	printf("\n          DELETE HOST <name> ");
	printf("\n   Network Configuration Operations: NETWORK");
	printf("\n          CREATE NETWORK <name> <group> <address> <label>");
	printf("\n          LIST   NETWORK ");
	printf("\n          GET    NETWORK ");
	printf("\n          DELETE NETWORK ");
	printf("\n   Deployment and Role Operations: VM");
	printf("\n          CREATE   VM <deployment> <role> <image> <media> <flavor> <network> <zone> <access_value> ");
	printf("\n          START    VM <deployment> <role> ");
	printf("\n          CAPTURE  VM <deployment> <role> <host> <label> <image> <option> ");
	printf("\n          SHUTDOWN VM <deployment> <role> ");
	printf("\n          RESTART  VM <deployment> <role> ");
	printf("\n          LIST     VM <deployment>");
	printf("\n          GET      VM <deployment> <role>");
	printf("\n          DELETE VM <deployment> <role>");
	printf("\n   Affinity Group Operations: GROUP");
	printf("\n          CREATE GROUP   <name> <description> <location>  ");
	printf("\n          LIST   GROUP ");
	printf("\n          GET    GROUP <id> ");
	printf("\n          DELETE GROUP <id> ");
	printf("\n   Storage Operations: STORAGE, OS and DISK");
	printf("\n          CREATE [ STORAGE <name> <description> <location> <group> ] ");
	printf("\n          LIST   STORAGE ");
	printf("\n          GET    STORAGE <id> ");
	printf("\n          DELETE STORAGE <id> ");
	printf("\n          LIST   OS ");
	printf("\n          CREATE OS <name> <label> <media> [Linux|Windows]");
	printf("\n          GET    OS <name> ");
	printf("\n          DELETE OS <name> ");
	printf("\n          LIST   DISKS ");
	printf("\n          GET    DISK <name>");
	printf("\n          DELETE DISK <name>");
	printf("\n          CREATE DISK <name> <label> <media> <option>");
	printf("\n          LIST   CONTAINERS ");
	printf("\n   Service Deployment Operations: DEPLOYMENT ");
	printf("\n          CREATE DEPLOYMENT <name> <image> <configuration> <host> <slot> ");
	printf("\n          LIST   DEPLOYMENT <host> " );
	printf("\n          GET    DEPLOYMENT <host> <slot> " );
	printf("\n          DELETE DEPLOYMENT <host> <slot> " );
	printf("\n   LIST   [ OPERATIONS | LOCATIONS | PROFILES | DEFINITIONS | CERTIFICATES ] ");
	printf("\n   GET    [ SUBSCRIPTION | CERTIFICATE ] <id> ");
	printf("\n   DELETE [ CERTIFICATE  | LOCATION    ] <id> ");
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

