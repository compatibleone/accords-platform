#include "command.h"
#include "cordslang.h"
#include "occiresolver.h"
#include "occilogin.h"
#include "cp.h"

private	int	verbose=0;
private	int	debug=0;

private	char *	agent="CO-COMMAND/1.0";
private	char *	publisher=(char *) 0;
private	char *	tls=(char *) 0;

public	char *	default_publisher()	{	return( publisher );	}
public	char *	default_tls()		{	return( tls );		}
public	int	check_verbose()		{	return( verbose );	}
public	int	check_debug()		{	return( debug );	}

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

/*	-----------------------------------------------------	*/
/*		c o r d s _ s e r v i c e _ a c t i o n		*/
/*	-----------------------------------------------------	*/
private	int	ll_cords_service_action( char * id, char * action )
{
	struct	occi_response * zptr;
	char 	buffer[2048];
	char *	result;
	char *	sptr;

	initialise_occi_resolver( publisher, (char *) 0, (char *) 0, (char *) 0 );

	if (!( sptr = occi_resolve_category_provider( _CORDS_SERVICE, agent, tls ) ))
		return( 500 );
	else	sprintf(buffer,"%s/%s/%s",sptr,_CORDS_SERVICE,id );

	if (!( zptr =  cords_invoke_action( buffer, action, agent, tls ) ))
		return(501);
	else
	{
		zptr = occi_remove_response( zptr );
		return( 0 );
	}
}

/*	-----------------------------------------------------	*/
/*	t e s t _ c o r d s _ b r o k e r _ o p e r a t i o n	*/
/*	-----------------------------------------------------	*/
private	int	cords_service_action( char * id, char * action )
{
	int	status;
	char *	auth;

	initialise_occi_resolver( _DEFAULT_PUBLISHER, (char *) 0, (char *) 0, (char *) 0 );

	if (!( auth = login_occi_user( "test-broker","co-system",agent, tls ) ))
		return(403);
	else 	(void) occi_client_authentication( auth );

	status = ll_cords_service_action( id, action );

	(void) logout_occi_user( "test-broker","co-system",agent, auth, tls );	

	return( status );
}

private	int	service_operation( char * command, char * service )
{
	char *	id;
	FILE *	h;
	if (!( command ))
		return( 31 );
	else if (!( service ))
		return( 32 );
	else if (!( h = fopen( service, "r" ) ))
		return( 40 );
	else if (!( publisher ))
		return( failure( 0, "publisher", "undefined" ) );
	{
		fclose(h);
		for (	id=service;
			*service != 0;
			service++ )
		{
			if ( *service == '/' )
				id = (service+1);
		}

		if ( verbose )
			printf("\n Service %s { %s } \n",command,id);

		if (!( strcasecmp( command, "START" ) ))
			return( cords_service_action( id, "start" ) );
		else if (!( strcasecmp( command, "STOP" ) ))
			return( cords_service_action( id, "stop" ) );
		else if (!( strcasecmp( command, "SAVE" ) ))
			return( cords_service_action( id, "save" ) );
		else	return( failure( 30,"incorrect command", command ) );
	}	
}


private	int	operation( int argc, char * argv[] )
{
	int	argi=1;
	char *	aptr;
	int	status;
	char *	command=(char *) 0;
	while ( argi < argc )
	{
		if (!( aptr = argv[argi++] ))
			break;
		else if ( *aptr != '-' )
		{
			if (!( command ))
			{
				command = aptr;
				continue;
			}
			else if (!(status = service_operation( command, aptr ) ))
				continue;
			else	return( failure( status, command, aptr ) );
		}
		else
		{
			aptr++;
			if ( *aptr == '-' )
			{
				aptr++;
				if (!( strcmp( aptr, "verbose" ) ))
					verbose = 1;
				else if (!( strcmp( aptr, "debug" ) ))
					debug = 1;
				else if (!( strcmp( aptr, "tls" ) ))
					tls = argv[argi++];
				else if (!( strcmp( aptr, "publisher" ) ))
					publisher = argv[argi++];
				else if (!( strcmp( aptr, "agent" ) ))
					agent = argv[argi++];
				else	
				{
					return( failure( 30, "incorrect command option", aptr ) );
				}
			}
		}	
	}
	return( 0 );
}

private	int	banner()
{
	printf("\n   CompatibleOne Command Line Tool : Version 1.0a.0.01");
	printf("\n   Copyright (c) Iain James Marshall ");
	printf("\n   Usage : ");
	printf("\n         command <options> START <service_file> ");
	printf("\n         command <options> STOP  <service_file> ");
	printf("\n         command <options> SAVE  <service_file> ");
	printf("\n   Options: ");
	printf("\n         --publisher <publisher>      specify publisher identity ");
	printf("\n         --agent     <agent>          specify agent identity ");
	printf("\n         --tls       <security>       specify security configuration ");
	printf("\n         --verbose                    activate verbose messages");
	printf("\n         --debug                      activate debug messages \n");
	return( 0 );
}


public	int	main( int argc, char * argv[] )
{
	if ( argc == 1 )
		return( banner() );
	else	return( operation( argc, argv ) );
}

