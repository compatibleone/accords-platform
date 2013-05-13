#ifndef	_testorga_c
#define	_testorga_c


#include "testorga.h"
#include "orgaclient.h"
#include "orgaclient.c"

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

private	int	orga_banner()
{
	printf("\n   TEST ORGA : CompatibleOne ORGA Client Version 1.0a ");
	printf("\n   Beta Version 25/04/2013");
	printf("\n   Copyright (c) 2011, 2013 Iain James Marshall, Prologue ");
	printf("\n");
	printf("\n   CRUD Operations");
	printf("\n     POST USER ");
	printf("\n     GET USER ");
	printf("\n     LIST USER ");
	printf("\n     DELETE USER ");
	printf("\n     UPDATE USER ");
	printf("\n     POST ACCOUNT ");
	printf("\n     GET ACCOUNT ");
	printf("\n     LIST ACCOUNT ");
	printf("\n     DELETE ACCOUNT ");
	printf("\n     UPDATE ACCOUNT ");
	printf("\n     POST TRANSACTION ");
	printf("\n     GET TRANSACTION ");
	printf("\n     LIST TRANSACTION ");
	printf("\n     DELETE TRANSACTION ");
	printf("\n     UPDATE TRANSACTION ");
	printf("\n   Options");
	printf("\n     --user <username>     set account user name ");
	printf("\n     --password <value>    set account password  ");
	printf("\n     --tenant <value>      set account tenant    ");
	printf("\n     --keyname <value>     set server keyname    ");
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

public	int	orga_command(int argc, char * argv[] )
{
	return( 0 );
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

