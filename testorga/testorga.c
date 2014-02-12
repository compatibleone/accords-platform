#ifndef	_testorga_c
#define	_testorga_c


#include "testorga.h"
#include "orgaclient.h"

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
public	char *	resolve_provider_account()	{ return( "provider"); }
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

private	int	orga_banner()
{
	printf("\n   TEST ORGA : CompatibleOne ORGA Client Version 1.0a.0.02 ");
	printf("\n   Beta Version 31/10/2013");
	printf("\n   Copyright (c) 2011, 2013 Iain James Marshall, Prologue ");
	printf("\n");
	printf("\n   CRUD Operations");
	printf("\n     POST    USER name ");
	printf("\n     POST    ACCOUNT username ");
	printf("\n     POST    TRANSACTION account value type label ");
	printf("\n     LIST    [  USER | ACCOUNT | TRANSACTION ] id ");
	printf("\n     GET     [  USER | ACCOUNT | TRANSACTION ] id ");
	printf("\n     DELETE  [  USER | ACCOUNT | TRANSACTION ] is ");
	printf("\n     PUT    USER id name ");
	printf("\n     PUT    ACCOUNT id username ");
	printf("\n     PUT    TRANSACTION id account value type label ");
	printf("\n   Options");
	printf("\n     --user <username>     set account user name ");
	printf("\n     --password <value>    set account password  ");
	printf("\n     --tenant <value>      set account tenant    ");
	printf("\n     --host <hostname>     set host name         ");
	printf("\n     --version <value>     set host version      ");
	printf("\n     --agent   <name>      set test agent name   ");
	printf("\n     --tls <config>        set tls config        ");
	printf("\n     --verbose             activate verbose messages ");
	printf("\n     --debug               activate debug messages ");
	printf("\n\n");
	return( 0 );
}

private	struct	cords_account		Customer;

private	int	new_orga_customer_id()
{
	if ( Customer.id )
		Customer.id = liberate( Customer.id );
	if (!( Customer.id = allocate_string( rest_allocate_uuid() ) ))
		return( 0 );
	else	return( 1 );
}

private	int	orga_customer_id( char * p )
{
	if (!( p ))
		return(0);
	else
	{	
		if ( Customer.id )
			Customer.id = liberate( Customer.id );
		if (!( Customer.id = allocate_string( p ) ))
			return( 0 );
		else	return( 1 );
	}
}

private	int	orga_customer_name( char * p )
{
	if (!( p ))
		return(0);
	else
	{	
		if ( Customer.name )
			Customer.name = liberate( Customer.name );
		if (!( Customer.name = allocate_string( p ) ))
			return( 0 );
		else	return( 1 );
	}
}

private	struct	orga_account		Account;
private	struct	orga_transaction	Transaction;

private	int	orga_transaction_account( char * p )
{
	if (!( p ))
		return(0);
	else
	{	
		if ( Transaction.account )
			Transaction.account = liberate( Transaction.account );
		if (!( Transaction.account = allocate_string( p ) ))
			return( 0 );
		else	return( 1 );
	}
}

private	int	orga_transaction_label( char * p )
{
	if (!( p ))
		return(0);
	else
	{	
		if ( Transaction.label )
			Transaction.label = liberate( Transaction.label );
		if (!( Transaction.label = allocate_string( p ) ))
			return( 0 );
		else	return( 1 );
	}
}

private	int	orga_transaction_value( char * p )
{
	if (!( p ))
		return(0);
	else
	{	
		if ( Transaction.value )
			Transaction.value = liberate( Transaction.value );
		if (!( Transaction.value = allocate_string( p ) ))
			return( 0 );
		else	return( 1 );
	}
}

private	int	orga_transaction_currency( char * p )
{
	if (!( p ))
		return(0);
	else
	{	
		if ( Transaction.currency )
			Transaction.currency = liberate( Transaction.currency );
		if (!( Transaction.currency = allocate_string( p ) ))
			return( 0 );
		else	return( 1 );
	}
}


private	int	orga_check( struct rest_response * rptr )
{
	int	status;
	if ( rptr )
	{
		if ( verbose )
			printf("TEST ORGA Response: %u %s\n",rptr->status, rptr->message);
		status = rptr->status;
		rptr = rest_liberate_response( rptr );
	}
	else	
	{
		if ( verbose )
			printf("TEST ORGA Response: %u %s\n",666,"No Response Message");
		status = 666;
	}
	return( status );
}

private	int	orga_list_operation( struct orga_subscription * sptr, 
		char * p1, char * p2, char * p3,
		char * p4, char * p5, char * p6, char * p7 )
{
	if (!( p1 ))
		return( 0 );
	else if (!( strcasecmp( p1, "USER" ) ))
		return( orga_check( orga_list_user( sptr, &Customer )) );
	else if (!( strcasecmp( p1, "ACCOUNT" ) ))
		return( orga_check( orga_list_account( sptr,&Account )) );
	else if (!( strcasecmp( p1, "TRANSACTION" ) ))
		return( orga_check( orga_list_transaction( sptr, &Transaction  )) );
	else 	return( 0 );
}

private	int	orga_create_operation( struct orga_subscription * sptr, 
		char * p1, char * p2, char * p3,
		char * p4, char * p5, char * p6, char * p7 )
{
	if (!( p1 ))
		return( 0 );
	else if (!( strcasecmp( p1, "USER" ) ))
	{
		if (!( new_orga_customer_id() ))
			return( 0 );
		else if (!( orga_customer_name( p2 ) ))
			return( 0 );
		return( orga_check( orga_create_user( sptr, &Customer, "Customer" ) ));
	}
	else if (!( strcasecmp( p1, "ACCOUNT" ) ))
		return( orga_check( orga_create_account( sptr,&Account ) ));
	else if (!( strcasecmp( p1, "TRANSACTION" ) ))
	{
		if (!( orga_transaction_account( p2 ) ))
			return( 0 );
		else if (!( orga_transaction_label( p3 ) ))
			return( 0 );
		else if (!( orga_transaction_value( p4 ) ))
			return( 0 );
		else if (!( orga_transaction_currency( p5 ) ))
			return( 0 );
		return( orga_check( orga_create_transaction( sptr, &Transaction  ) ));
	}
	else 	return( 0 );
}

private	int	orga_retrieve_operation( struct orga_subscription * sptr, 
		char * p1, char * p2, char * p3,
		char * p4, char * p5, char * p6, char * p7 )
{
	if (!( p1 ))
		return( 0 );
	else if (!( strcasecmp( p1, "USER" ) ))
	{
		if (!( orga_customer_id( p2 ) ))
			return( 0 );
		else	return( orga_check( orga_retrieve_user( sptr, &Customer, "Customer")) );
	}
	else if (!( strcasecmp( p1, "ACCOUNT" ) ))
		return( orga_check( orga_retrieve_account( sptr,&Account )) );
	else if (!( strcasecmp( p1, "TRANSACTION" ) ))
		return( orga_check( orga_retrieve_transaction( sptr, &Transaction  )) );
	else 	return( 0 );
}

private	int	orga_delete_operation( struct orga_subscription * sptr, 
		char * p1, char * p2, char * p3,
		char * p4, char * p5, char * p6, char * p7 )
{
	if (!( p1 ))
		return( 0 );
	else if (!( strcasecmp( p1, "USER" ) ))
	{
		if (!( orga_customer_id( p2 ) ))
			return( 0 );
		else	return( orga_check( orga_delete_user( sptr, &Customer, "Customer" )) );
	}
	else if (!( strcasecmp( p1, "ACCOUNT" ) ))
		return( orga_check( orga_delete_account( sptr,&Account )) );
	else if (!( strcasecmp( p1, "TRANSACTION" ) ))
	{
		if (!( orga_transaction_account( p2 ) ))
			return( 0 );
		else if (!( orga_transaction_label( p3 ) ))
			return( 0 );
		else if (!( orga_transaction_value( p4 ) ))
			return( 0 );
		else if (!( orga_transaction_currency( p5 ) ))
			return( 0 );
		return( orga_check( orga_delete_transaction( sptr, &Transaction  )) );
	}
	else 	return( 0 );
}

private	int	orga_update_operation( struct orga_subscription * sptr, 
		char * p1, char * p2, char * p3,
		char * p4, char * p5, char * p6, char * p7 )
{
	if (!( p1 ))
		return( 0 );
	else if (!( strcasecmp( p1, "USER" ) ))
	{
		if (!( orga_customer_id( p2 ) ))
			return( 0 );
		else if (!( orga_customer_name( p3 ) ))
			return( 0 );
		else 	return( orga_check( orga_update_user( sptr, &Customer, "Customer" )) );
	}
	else if (!( strcasecmp( p1, "ACCOUNT" ) ))
		return( orga_check( orga_update_account( sptr,&Account )) );
	else if (!( strcasecmp( p1, "TRANSACTION" ) ))
	{
		if (!( orga_transaction_account( p2 ) ))
			return( 0 );
		else if (!( orga_transaction_label( p3 ) ))
			return( 0 );
		else if (!( orga_transaction_value( p4 ) ))
			return( 0 );
		else if (!( orga_transaction_currency( p5 ) ))
			return( 0 );
		return( orga_check( orga_update_transaction( sptr, &Transaction )) );
	}
	else 	return( 0 );
}


private	int	orga_operation( struct orga_subscription * sptr, char * aptr, 
		char * p1, char * p2, char * p3,
		char * p4, char * p5, char * p6, char * p7 )
{
	if (!( aptr ))
		return( 0 );
	else if (!( strcasecmp( aptr, "LIST") ))
		return( orga_list_operation( sptr, p1, p2, p3, p4, p5, p6, p7 ) );
	else if (!( strcasecmp( aptr, "POST") ))
		return( orga_create_operation( sptr, p1, p2, p3, p4, p5, p6, p7 ) );
	else if (!( strcasecmp( aptr, "GET") ))
		return( orga_retrieve_operation( sptr, p1, p2, p3, p4, p5, p6, p7 ) );
	else if (!( strcasecmp( aptr, "PUT") ))
		return( orga_update_operation( sptr, p1, p2, p3, p4, p5, p6, p7 ) );
	else if (!( strcasecmp( aptr, "DELETE") ))
		return( orga_delete_operation( sptr, p1, p2, p3, p4, p5, p6, p7 ) );
	else	return( 0 );
}

public	int	orga_command(int argc, char * argv[] )
{
	char *	config=_ORGA_CLIENT_CONFIG;
	int	status;
	int	argi=1;
	char *	aptr;
	char *	agent="C-TESTORGA/1.0";
	struct	orga_subscription * subscription;
	rest_initialise_log(0);
	memset(&Customer,0,sizeof(struct cords_account));
	memset(&Account,0,sizeof(struct orga_account));
	memset(&Transaction,0,sizeof(struct orga_transaction));
	while ( argi < argc )
	{
		if (!( aptr = argv[argi++] ))
			break;
		else if ( *aptr != '-' )
		{
			if (!( agent ))
				return( failure( status, "missing", "--agent parameter" ) );
			else	set_default_agent( agent );
			if (!( config ))
				return( failure( status, "missing", "--config parameter" ) );
			if (!(subscription = check_orga_subscription( config, "Missing parameter ??" ) ))
				return( failure( status, "initialising", "client" ) );
			else 	return( orga_operation( subscription, aptr, 
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

			if (!( strcasecmp( aptr,"agent" ) ))
				agent = argv[argi++];
			else if (!( strcasecmp( aptr,"config" ) ))
				config = argv[argi++];
			else if (!( strcasecmp( aptr,"verbose" ) ))
				verbose=1;
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

public	int	main(int argc, char * argv[] )
{
	if ( argc == 1 )
		return( orga_banner() );
	else	return( orga_command(argc,argv) );
}

	/* ----------- */
#endif	/* _testorga_c */
	/* ----------- */

