/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2013 by Daniel Sprouse (ComputeNext) <daniel@computenext.com>   */
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License");      */
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at                              */
/*                                                                      */
/*  http://www.apache.org/licenses/LICENSE-2.0                          */
/*                                                                      */
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or      */
/* implied.                                                             */
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License.                                       */
/* -------------------------------------------------------------------- */
#ifndef	_testcn_c
#define	_testcn_c

#include "cnclient.h"
#include "cnconfig.h"
#include "xxlink.c"

private char *	group=(char *) 0;
private	int     detail=0;
private	int     debug=0;
private	int     verbose=0;
private	char *	tls=(char *) 0;
public  char *	agent="CO-CNCLIENT/1.0";
public  char *	version="v1.0";
private	char *	publisher=(char *) 0;
private	char *	operator="accords";
private	char *	zone="europe";
private	char *	transactiontype="Paid";
public	int     check_debug()		{	return(debug);		}
public	int     check_verbose()		{	return(verbose);	}
public	char *	default_tls()		{	return(tls);		}
public	char *	default_agent()		{	return(agent);		}
public	char *	default_version()	{	return(version);	}
public	char *	default_publisher()	{	return(publisher);	}
public	char *	default_operator()	{	return(operator);	}
public	char *	default_zone()		{	return(zone);		}

private	struct cn_config * config=(struct cn_config *) 0;

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

#include "cnconfig.c"
#include "cnclient.c"

private	int	cn_result( struct cn_response * rptr )
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
		cn_liberate_response( rptr );
		return(0);
	}
	else	return( failure(99,"no","result") );
}

private	int	cn_operation( char * p1, char * p2, char * p3, char * p4, char * p5, char * p6, char * p7, char * p8 )
{
	char	*	nomfic;

	if (!( p1 ))
		return( failure( 30,"p1", "required") );

	else if (!( p2 ))
		return( failure( 30,"p2", "required") );
	
	else if (!( strcasecmp(p1,"LIST" ) ))
	{
		if (!( p2 ))
			return( failure(33, "missing", "parameter" ));
		else if (!( strcasecmp( p2, "IMAGES" ) ))
			cn_result( cn_list_images(config, "") );
		else if (!( strcasecmp( p2, "INSTANCES" ) ))
			cn_result( cn_list_instancetypes(config, "") );
		else if (!( strcasecmp( p2, "VMS" ) ))
			cn_result( cn_list_virtualmachines(config, "") );
		else if (!( strcasecmp( p2, "VOLUMES" ) ))
			cn_result( cn_list_volumestores(config, "") );
		else if (!( strcasecmp( p2, "SECURITYGROUPS" ) ))
			cn_result( cn_list_securitygroups(config) );
		else if (!( strcasecmp( p2, "KEYPAIRS" ) ))
			cn_result( cn_list_keypairs(config) );
		else if (!( strcasecmp( p2, "WORKLOADS" ) ))
			cn_result( cn_list_workloads(config) );
		else if (!( strcasecmp( p2, "TRANSACTIONS" ) ))
			cn_result( cn_list_transactions(config) );
		else	return( failure(33, p1, p2 ) );
		return(0);
	}
	else if (!( strcasecmp(p1,"GET" ) ))
	{
		if (!( p2 ))
			return( failure(33, "missing", "parameter" ));
		else if (!( strcasecmp( p2, "IMAGE" ) ))
			cn_result( cn_get_image( config, p3 ) );
		else if (!( strcasecmp( p2, "INSTANCE" ) ))
			cn_result( cn_get_instancetype( config, p3 ) );
		else if (!( strcasecmp( p2, "VM" ) ))
			cn_result( cn_get_virtualmachine( config, p3 ) );
		else if (!( strcasecmp( p2, "VOLUME" ) ))
			cn_result( cn_get_volumestore( config, p3 ) );
		else if (!( strcasecmp( p2, "SECURITYGROUP" ) ))
			cn_result( cn_get_securitygroup( config, p3 ) );
		else if (!( strcasecmp( p2, "KEYPAIR" ) ))
			cn_result( cn_get_keypair( config, p3 ) );
		else if (!( strcasecmp( p2, "WORKLOAD" ) ))
			cn_result( cn_get_workload( config, p3 ) );
		else if (!( strcasecmp( p2, "TRANSACTION" ) ))
			cn_result( cn_get_transaction( config, p3 ) );
		else	return( failure(33, p1, p2 ) );
		return(0);
	}
	else if (!( strcasecmp(p1,"DELETE" ) ))
	{
		if (!( p2 ))
			return( failure(33, "missing", "parameter" ));
		else if (!( strcasecmp( p2, "SECURITYGROUP" ) ))
			cn_result( cn_delete_securitygroup( config, p3 ) );
		else if (!( strcasecmp( p2, "KEYPAIR" ) ))
			cn_result( cn_delete_keypair( config, p3 ) );
		else if (!( strcasecmp( p2, "WORKLOAD" ) ))
			cn_result( cn_delete_workload( config, p3 ) );
		else if (!( strcasecmp( p2, "WORKLOADELEMENT" ) ))
			cn_result( cn_delete_workload_element( config, p3, p4 ) );
		else if (!( strcasecmp( p2, "TRANSACTION" ) ))
			cn_result( cn_delete_transaction( config, p3 ) );
		else	return( failure(33, p1, p2 ) );
		return(0);
	}
	else if (!( strcasecmp(p1,"CREATE" ) ))
	{
		if (!( p2 ))
			return( failure(33, "missing", "parameter" ));
		else if (!( strcasecmp( p2, "WORKLOAD" ) ))
		{
			if (!( nomfic = cn_create_workload_request( p3 ) ))
				return( failure(27,"cannot create workload","request" ) );
			else
				cn_result( cn_create_workload(config, nomfic ) );
		}
		else if (!( strcasecmp( p2, "TRANSACTION" ) ))
		{
			if (!( nomfic = cn_create_transaction_request( p3, transactiontype ) ))
				return( failure(27,"cannot create transaction","request" ) );
			else
				cn_result( cn_create_transaction(config, nomfic ) );
		}
		else if (!( strcasecmp( p2, "SECURITYGROUP" ) ))
		{
			if (!( nomfic = cn_create_securitygroup_request( p3, p4, p5, p6 ) ))
				return( failure(27,"cannot create securitygroup","request" ) );
			else
				cn_result( cn_create_securitygroup(config, nomfic ) );
		}
		else if (!( strcasecmp( p2, "KEYPAIR" ) ))
		{
			if (!( nomfic = cn_create_keypair_request( p3, p4, p5 ) ))
				return( failure(27,"cannot create keypair","request" ) );
			else
				cn_result( cn_create_keypair(config, nomfic ) );
		}
		else	return( failure(33, p1, p2 ) );
		return(0);
	}
	else if (!( strcasecmp(p1,"ADD" ) ))
	{
		if (!( p2 ))
			return( failure(33, "missing", "parameter" ));
		else if (!( strcasecmp( p2, "VM" ) ))
		{
			if (!( nomfic = cn_add_vm_request(p4, 1, p5, p6, p7, p8, "") ))
				return( failure(27,"cannot create vm addition","request" ) );
			else
				cn_result( cn_add_vm_to_workload( config, p3, nomfic ) );
		}
		else if (!( strcasecmp( p2, "VS" ) ))
		{
			if (!( nomfic = cn_add_vs_request(p4, p5, p6, p7, atoi(p8) ) ))
				return( failure(27,"cannot create vs addition","request" ) );
			else
				cn_result( cn_add_vs_to_workload( config, p3, nomfic ) );
		}
		else if (!( strcasecmp( p2, "PORT" ) ))
		{
			if (!( nomfic = cn_add_port_request(p4, p5, p6, p7) ))
				return( failure(27,"cannot create port addition","request" ) );
			else
				cn_result( cn_add_port( config, p3, nomfic ) );
		}
		else	return( failure(33, p1, p2 ) );
		return(0);
	}
	else if (!( strcasecmp(p1,"REMOVE" ) ))
	{
		if (!( p2 ))
			return( failure(33, "missing", "parameter" ));
		else if (!( strcasecmp( p2, "PORT" ) ))
		{
			if (!( nomfic = cn_remove_port_request(p4, p5, p6, p7) ))
				return( failure(27,"cannot create port removal","request" ) );
			else
			{
				cn_result( cn_remove_port( config, p3, nomfic ) );
				return( 0 );
			}
		}
		else	return( failure(33, p1, p2 ) );
	}
	else if (!( strcasecmp(p1,"START" ) ))
	{
		if (!( nomfic = cn_start_transaction_request() ))
				return( failure(27,"cannot create start","request" ) );
		else
		{
			cn_result( cn_start_transaction( config, p3, nomfic ) );
			return( 0 );
		}
	}
	else if (!( strcasecmp(p1,"STOP" ) ))
	{
		if (!( nomfic = cn_stop_transaction_request() ))
				return( failure(27,"cannot create stop","request" ) );
		else
		{
			cn_result( cn_stop_transaction( config, p3, nomfic ) );
			return( 0 );
		}
	}

	else	return( failure(32, "incorrect p1", p1 ) );
}

private	int	cn_command(int argc, char * argv[] )
{
  int	status=1;
	int	argi=1;
	char *	aptr;
	char *	apikey=(char *) 0;
	char *	apisec=(char *) 0;
	char *	host=(char *) 0;
	
	rest_initialise_log(0);

	while ( argi < argc )
	{
		if (!( aptr = argv[argi++] ))
			break;
		else if ( *aptr != '-' )
		{
			
			if (!( host ))
				return( failure( status, "missing", "--host parameter" ) );
			else if (!( apikey ))
				return( failure( status, "missing", "--apikey parameter" ) );
			else if (!( apisec ))
				return( failure( status, "missing", "--apisec parameter" ) );
			else if (!(config = cn_initialise_client( apikey, apisec, host, agent, version, tls )))
				return( failure( status, "initialising", "client" ) );
			else 	
			    return( cn_operation( aptr, 
					( argi < argc ? argv[argi] : (char *) 0 ),
					( (argi+1) < argc ? argv[argi+1] : (char *) 0 ),
					( (argi+2) < argc ? argv[argi+2] : (char *) 0 ),
					( (argi+3) < argc ? argv[argi+3] : (char *) 0 ),
					( (argi+4) < argc ? argv[argi+4] : (char *) 0 ),
					( (argi+5) < argc ? argv[argi+5] : (char *) 0 ),
					( (argi+6) < argc ? argv[argi+6] : (char *) 0 ) ) );
		}
		else if (  *(++aptr) == '-' )
		{
			aptr++;

			if (!( strcasecmp( aptr,"apikey" ) ))
				apikey = argv[argi++];
			else if (!( strcasecmp( aptr,"apisec" ) ))
				apisec = argv[argi++];
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
			else if (!( strcasecmp( aptr, "echo" ) ))
				json_set_echo(1);
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

private	int	cn_banner()
{
	printf("\n   CO-CN : CompatibleOne ComputeNext Client Test : Version 1.0b.0.05");
	printf("\n   Beta Version 2/11/2013");
	printf("\n   Copyright (c) 2013 Daniel Sprouse, ComputeNext ");
	printf("\n");
	printf("\n   CRUD Operations ");
	printf("\n");
	printf("\n   LIST        [ IMAGES | INSTANCES | VMS | VOLUMES | SECURITYGROUPS | KEYPAIRS | WORKLOADS | TRANSACTIONS ] ");
	printf("\n   GET         [ IMAGE | INSTANCE | VM | VOLUME | SECURITYGROUP | KEYPAIR | WORKLOAD | TRANSACTION ] <id> ");
	printf("\n   DELETE      [ SECURITYGROUP | KEYPAIR | WORKLOAD | WORKLOADELEMENT <workload> | TRANSACTION ] <id> ");
	printf("\n   CREATE      [ SECURITYGROUP <name> <description> <region> <provider> | KEYPAIR <name> <region> <provider> | ");
	printf("\n                 WORKLOAD <name> | TRANSACTION <workload> ] ");
	printf("\n   ADD         [ VM <workload> <name> <image> <instance> <securitygroup> <keypair> | ");
	printf("\n                 VS <workload> <name> <provider> <region> <zone> <size> | ");
	printf("\n                 PORT <securitygroup> <protocol> <to> <from> <address> ] ");
	printf("\n   REMOVE PORT <securitygroup> <protocol> <to> <from> <address> ");
	printf("\n   START       <transaction> ");
	printf("\n   STOP        <transaction> ");

	printf("\n");
	printf("\n   Options");
	printf("\n     --apikey  <apikey>    set account API key       ");
	printf("\n     --apisec  <apisec>    set account API secret    ");
	printf("\n     --host    <hostname>  set host name             ");
	printf("\n     --version <value>     set host version          ");
	printf("\n     --agent   <name>      set test agent name       ");
	printf("\n     --tls     <config>    set tls config            ");
	printf("\n     --detail              display detailed lists    ");
	printf("\n     --verbose             activate verbose messages ");
	printf("\n     --debug               activate debug messages   ");
	printf("\n\n");
	return( 0 );
}

public	int	main(int argc, char * argv[] )
{
	if ( argc == 1 )
		return( cn_banner() );
	else	return( cn_command(argc,argv) );
}

        /* --------- */
#endif  /* _testos_c */
        /* --------- */

