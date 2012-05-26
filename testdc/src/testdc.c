#ifndef	_testdc_c
#define	_testdc_c

#include "standard.h"
#include "dcclient.h"

private	int	verbose=0;
private	int	debug=0;
private	char *	publisher=(char *) 0;
private	char *	operator="accords";
private	char *	zone="europe";
private	char *	tls=(char *) 0;

public	int	check_debug()		{	return( debug);		}
public	int	check_verbose()		{	return( verbose);	}
public	char *	default_zone()		{	return( zone );		}
public	char *	default_operator()	{	return( operator );	}
public	char *	default_tls()		{	return( tls );		}
public	int	failure(int status, char * message)
{
	if ( status )
		printf("**failure(%u,%s)**\n",status,message);
	return( status );
}

public	char *	default_publisher()	
{
	return( publisher );
}

private	int	dc_result( struct rest_response* rptr )
{
	rptr = liberate_rest_response( rptr );
	return( 0 );
}

private	int	dc_operation( char * host, char * user, char * password, char * command, char * subject, char * option, char * option2 )
{
	if (!( host ))
		return( 30 );
	else if (!( user ))
		return( 31 );
	else if (!( password ))
		return( 32 );
	else if (!( command ))
		return( 33 );
	else if (!( subject ))
		return( 34 );
	else if (!( strcmp( command, "list" ) ))
	{
		if (!( strcmp( subject, "features" ) ))
			return( dc_result( dc_list_features() ) );
		else if (!( strcmp( subject, "profiles" ) ))
			return( dc_result( dc_list_profiles() ) );
		else if (!( strcmp( subject, "images" ) ))
			return( dc_result( dc_list_images() ) );
		else if (!( strcmp( subject, "storage" ) ))
			return( dc_result( dc_list_storage() ) );
		else if (!( strcmp( subject, "servers" ) ))
			return( dc_result( dc_list_instances() ) );
		else if (!( strcmp( subject, "keys" ) ))
			return( dc_result( dc_list_keys() ) );
		else if (!( strcmp( subject, "addresses" ) ))
			return( dc_result( dc_list_addresses() ) );
		else if (!( strcmp( subject, "firewalls" ) ))
			return( dc_result( dc_list_firewalls() ) );
		else if (!( strcmp( subject, "buckets" ) ))
			return( dc_result( dc_list_buckets() ) );
		else if (!( strcmp( subject, "loadbalancers" ) ))
			return( dc_result( dc_list_loadbalancers() ) );
		else	return( failure( 35, subject ) );
	}

	else if (!( strcmp( command, "get" ) ))
	{
		if (!( option ))
			return( failure( 36, "option") );
		else if (!( strcmp( subject, "profile" ) ))
			return( dc_result( dc_get_profile(option) ) );
		else if (!( strcmp( subject, "image" ) ))
			return( dc_result( dc_get_image(option) ) );
		else if (!( strcmp( subject, "storage" ) ))
			return( dc_result( dc_get_storage(option) ) );
		else if (!( strcmp( subject, "server" ) ))
			return( dc_result( dc_get_instance(option) ) );
		else if (!( strcmp( subject, "key" ) ))
			return( dc_result( dc_get_key(option) ) );
		else if (!( strcmp( subject, "addresse" ) ))
			return( dc_result( dc_get_address(option) ) );
		else if (!( strcmp( subject, "firewall" ) ))
			return( dc_result( dc_get_firewall(option) ) );
		else if (!( strcmp( subject, "bucket" ) ))
			return( dc_result( dc_get_bucket(option) ) );
		else if (!( strcmp( subject, "blob" ) ))
			return( dc_result( dc_get_blob_info(option,option2) ) );
		else if (!( strcmp( subject, "loadbalancer" ) ))
			return( dc_result( dc_get_loadbalancer(option) ) );
		else	return( failure( 35, subject ) );
	}

	else if (!( strcmp( command, "create" ) ))
	{
		if (!( option ))
			return( failure( 36, "option") );
		else if (!( strcmp( subject, "image" ) ))
			return( dc_result( dc_create_image(option) ) );
		else if (!( strcmp( subject, "key" ) ))
			return( dc_result( dc_create_key(option) ) );
		else if (!( strcmp( subject, "server" ) ))
			return( dc_result( dc_create_instance(option) ) );
		else if (!( strcmp( subject, "addresse" ) ))
			return( dc_result( dc_create_address(option) ) );
		else if (!( strcmp( subject, "firewall" ) ))
			return( dc_result( dc_create_firewall(option) ) );
		else if (!( strcmp( subject, "loadbalancer" ) ))
			return( dc_result( dc_create_loadbalancer(option) ) );
		else	return( failure( 35, subject ) );
	}

	else if (!( strcmp( command, "delete" ) ))
	{
		if (!( option ))
			return( failure( 36, "option") );
		else if (!( strcmp( subject, "image" ) ))
			return( dc_result( dc_delete_image(option) ) );
		else if (!( strcmp( subject, "storage" ) ))
			return( dc_result( dc_delete_storage(option) ) );
		else if (!( strcmp( subject, "server" ) ))
			return( dc_result( dc_delete_instance(option) ) );
		else if (!( strcmp( subject, "key" ) ))
			return( dc_result( dc_delete_key(option) ) );
		else if (!( strcmp( subject, "addresse" ) ))
			return( dc_result( dc_delete_address(option) ) );
		else if (!( strcmp( subject, "firewall" ) ))
			return( dc_result( dc_delete_firewall(option) ) );
		else if (!( strcmp( subject, "rule" ) ))
			return( dc_result( dc_delete_rule(option,option2) ) );
		else if (!( strcmp( subject, "bucket" ) ))
			return( dc_result( dc_delete_bucket(option) ) );
		else if (!( strcmp( subject, "blob" ) ))
			return( dc_result( dc_delete_blob(option,option2) ) );
		else if (!( strcmp( subject, "loadbalancer" ) ))
			return( dc_result( dc_delete_loadbalancer(option) ) );
		else	return( failure( 35, subject ) );
	}

	else if (!( strcmp( command, "start" ) ))
	{
		if (!( strcmp( subject, "server" ) ))
			return( dc_result( dc_start_instance(option) ) );
	}

	else if (!( strcmp( command, "stop" ) ))
	{
		if (!( strcmp( subject, "server" ) ))
			return( dc_result( dc_stop_instance(option) ) );
	}

	else if (!( strcmp( command, "attach" ) ))
	{
		if (!( strcmp( subject, "storage" ) ))
			return( dc_result( dc_attach_storage(option,option2) ) );
	}

	else if (!( strcmp( command, "detach" ) ))
	{
		if (!( strcmp( subject, "storage" ) ))
			return( dc_result( dc_detach_storage(option) ) );
	}

	else if (!( strcmp( command, "register" ) ))
	{
		if (!( strcmp( subject, "loadbalancer" ) ))
			return( dc_result( dc_register_loadbalancer(option,option2) ) );
	}

	else if (!( strcmp( command, "unregister" ) ))
	{
		if (!( strcmp( subject, "loadbalancer" ) ))
			return( dc_result( dc_unregister_loadbalancer(option,option2) ) );
	}

	else if (!( strcmp( command, "associate" ) ))
	{
		if (!( strcmp( subject, "address" ) ))
			return( dc_result( dc_associate_address(option,option2) ) );
	}

	else if (!( strcmp( command, "disassociate" ) ))
	{
		if (!( strcmp( subject, "address" ) ))
			return( dc_result( dc_disassociate_address(option) ) );
	}

	else	return( failure( 37, command ) );

	return(0);
}

private int	dc_create_command( char * subject, int argi, int argc, char * argv[] )
{
	char *	filename;
	int	argn=0;
	char *	args[10];
	for ( argn=0; argn < 10; argn++ ) args[argn] = (char *) 0;
	argn=0;
	while ( argi < argc )
		args[argn++] = argv[argi++];

	if (!( strcmp( subject, "server" ) ))
	{
		if (!( filename = dc_create_instance_message( args[0], args[1], args[2], args[3], args[4] ) ))
			return( 46 );
		else 	return( dc_result( dc_create_instance( filename ) ) );
	}
	if (!( strcmp( subject, "rule" ) ))
	{
		if (!( filename = dc_create_rule_message(  args[1], args[2], args[3], args[4] ) ))
			return( 46 );
		else 	return( dc_result( dc_create_rule( args[0], filename ) ) );
	}
	if (!( strcmp( subject, "loadbalancer" ) ))
	{
		if (!( filename = dc_create_loadbalancer_message( args[0], args[1], args[2], args[3], args[4] ) ))
			return( 46 );
		else 	return( dc_result( dc_create_loadbalancer( filename ) ) );
	}
	else if (!( strcmp( subject, "key" ) ))
	{
		if (!( filename = dc_create_key_message( args[0] ) ))
			return( 46 );
		else 	return( dc_result( dc_create_key( filename ) ) );
	}
	else if (!( strcmp( subject, "image" ) ))
	{
		if (!( filename = dc_create_image_message( args[0], args[1], args[2] ) ))
			return( 46 );
		else 	return( dc_result( dc_create_image( filename ) ) );
	}
	else if (!( strcmp( subject, "storage" ) ))
	{
		if (!( filename = dc_create_storage_message( args[0], args[1], args[2] ) ))
			return( 46 );
		else 	return( dc_result( dc_create_storage( filename ) ) );
	}
	else if (!( strcmp( subject, "firewall" ) ))
	{
		if (!( filename = dc_create_firewall_message( args[0], args[1] ) ))
			return( 46 );
		else 	return( dc_result( dc_create_firewall( filename ) ) );
	}
	else if (!( strcmp( subject, "bucket" ) ))
	{
		if (!( filename = dc_create_bucket_message( args[0], args[1] ) ))
			return( 46 );
		else 	return( dc_result( dc_create_bucket( filename ) ) );
	}
	else if (!( strcmp( subject, "blob" ) ))
	{
		return( dc_result( dc_update_blob( args[0], args[1], args[2] ) ) );
	}
	else if (!( strcmp( subject, "address" ) ))
	{
		return( dc_result( dc_create_address() ) );
	}

}

private int	dc_test_main( int argc, char * argv[] )
{
	char *	host=(char *) 0;
	char *	user=(char *) 0;
	char * 	tenant=(char *) 0;
	char *	agent="deltacloud-client/1.0";
	char *	password=(char *) 0;
	char *	command=(char *) 0;
	char *	subject=(char *) 0;
	int	argi=1;
	char * aptr;
	while ( argi < argc )
	{
		if (!( aptr = argv[argi++] ))
			break;
		else if ( *aptr != '-' )
		{
			dc_api_configuration( host, user, password, tenant, agent,tls );
			if (!( command ))
				command = aptr;
			else if (!( subject ))
			{
				subject = aptr;
				if (!( strcmp( command, "list" ) ))
					return( dc_operation( host, user, password, command, subject, (char *) 0,(char *) 0 ) );
				else if (!( strcmp( command, "create" ) ))
					return( dc_create_command( subject, argi, argc, argv ) );
			}
			else	return( dc_operation( host, user, password, command, subject, aptr, argv[argi]) );
		}
		else if ( *(aptr+1) = '-' )
		{
			aptr+=2;
			if (!( strcmp(aptr,"host" )))
				host = argv[argi++];
			else if (!( strcmp(aptr,"user" )))
				user = argv[argi++];
			else if (!( strcmp(aptr,"password" )))
				password = argv[argi++];
			else if (!( strcmp(aptr,"tenant" )))
				tenant = argv[argi++];
			else if (!( strcmp(aptr,"tls" )))
				tls = argv[argi++];
			else if (!( strcmp( aptr, "verbose" ) ))
				verbose =1;
			else if (!( strcmp( aptr, "debug" ) ))
				debug =1;
			else
			{
				printf("\nincorrect option: %s\n",aptr);
				return(30);
			}
		}
		else
		{
			printf("\nincorrect option: %s\n",aptr);
			return(30);
		}
	}
	return( 0 );
}

private int dc_test_banner()
{
	printf("\n   CompatibleOne Delta Cloud Test Client : Version 1.0a.0.01 ");
	printf("\n   Beta Version 14/05/2012 ");
	printf("\n   Copyright (c) 2012 Iain James Marshall\n");
	printf("\n   Options:\n");
	printf("\n   --host     {hostname}       specify delta cloud host ");
	printf("\n   --user     {username}       specify delta cloud user ");
	printf("\n   --password {password}       specify user password    ");
	printf("\n   --tls      {filename}       specify traansport layer security ");
	printf("\n   --verbose                   activate information messages ");
	printf("\n   --debug                     activate debug messages \n");
	printf("\n   Methods:\n");
	printf("\n   list   [ features | profiles | images | servers | keys | addresses | firewalls | loadbalancers ]");
	printf("\n   list   [ storage | buckets ] ");
	printf("\n   get    [ profile | storage | image | server | key | addresse | firewall | loadbalancer ] {id} ");
	printf("\n   delete [ storage | image | server | key | addresse | firewall | rule | loadbalancer ] {id} ");
	printf("\n   create server name image profile firewall realm ");
	printf("\n   create loadbalancer name realm protocol port1 port2 ");
	printf("\n   create image instance name description ");
	printf("\n   create firewall name description ");
	printf("\n   create storage name capacity realm ");
	printf("\n   create rule firewall proto from to range ");
	printf("\n   create key name ");
	printf("\n   create address ");
	printf("\n   get    bucket id ");
	printf("\n   get    blob   bucketid blobid ");
	printf("\n   create bucket name location ");
	printf("\n   create blob   bucketid blobid filename ");
	printf("\n   delete bucket id "); 
	printf("\n   delete blob   bucketid blobid \n");
	printf("\n   Actions:\n");
	printf("\n   start        server id ");
	printf("\n   stop         server id ");
	printf("\n   attach       storage id server ");
	printf("\n   detach       storage id ");
	printf("\n   register     loadbalancer id server ");
	printf("\n   unregister   loadbalancer id server ");
	printf("\n   associate    address id server ");
	printf("\n   disassociate address id ");
	printf("\n\n");
	return( 0 );
}

public int	main( int argc, char * argv[] )
{
	int	status;
	if ( argc == 1 )
		return( dc_test_banner() );
	else if (!( status = dc_test_main(argc, argv) ))
		return(status);
	else
	{
		printf("**failure(%u,%s)**\n",status,argv[0]);
		return(0);
	}
}

#endif 	/* _testdc_c */
	/* -------- */

