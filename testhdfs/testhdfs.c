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
#ifndef	_testhdfs_c
#define	_testhdfs_c

#include "hdfsclient.h"
#include "xxlink.c"

private	int	debug=0;
private	int	verbose=1;
private	char *	tls=(char *) 0;
private	char *	zone="europe";
private	char *	operator="europe";
private	char *	publisher=(char *) 0;
private	struct 	hdfs_subscription * subscription = (struct hdfs_subscription *) 0;
public	int	check_debug()		{	return(debug);		}
public	int	check_verbose()		{	return(verbose);	}
public	char *	default_publisher()	{	return(publisher);	}
public	char *	default_tls()		{	return(tls);		}
public	char *	default_zone()		{	return(zone);		}
public	char *	default_operator()	{	return(operator);	}

/* ------------------------------------------------------------------------------------ */
/*					f a i l u r e					*/
/* ------------------------------------------------------------------------------------ */
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

/* ------------------------------------------------------------------------------------ */
/*					h d f s _ r e s u l t				*/
/* ------------------------------------------------------------------------------------ */
private	int	show_hdfs_result( struct hdfs_response * rptr )
{
	char	buffer[2048];
	if ( rptr )
	{
		switch ( rptr->nature )
		{
		case	_TEXT_JSON	:
			if ( rptr->jsonroot )
			{
				json_show( rptr->jsonroot );
				printf("\n");		
			}
			break;
		case	_TEXT_XML	:
			if ( rptr->xmlroot )
			{
				document_show_element( rptr->xmlroot, 0 );
				printf("\n");
			}
			break;
		default			:
			if (!( rptr->response ))
				break;
			else if (!( rptr->response->body ))
				break;
			else if ( rptr->response->status != 204 )
			{
				sprintf(buffer,"cat %s",rptr->response->body);
				system( buffer );
				printf("\n");
			}
		}
		hdfs_remove_response( rptr );
		return(0);
	}
	else	return( failure(99,"no","result") );
}

private	struct hdfs_response * hdfs_write_path( char * path, char * tls, char * agent, char * filename )
{
	struct	rest_header * hptr;
	struct	hdfs_subscription * sptr;
	struct	hdfs_response * rptr;
	struct	hdfs_response * rrptr;

	/* ------------------------------------ */
	/* resolve the subscription information */
	/* ------------------------------------ */
	if (!( sptr = subscription ))
		return((struct hdfs_response *) 0);
	/* --------------------------------- */
	/* create a new file name node entry */
	/* --------------------------------- */
	else if (!( rptr = hdfs_create_file( sptr, path, 0644 ) ))
		return( rptr );
	/* ------------------------------------- */
	/* resolve the data node location header */
	/* ------------------------------------- */
	else if (!( hptr = rest_resolve_header( rptr->response->first,_HTTP_LOCATION ) ))
		return( hdfs_remove_response( rptr ) );
	/* --------------------------------------- */
	/* write the file contents to the datanode */
	/* --------------------------------------- */
	else if (!( rrptr = hdfs_write_file( sptr, hptr->value, filename ) ))
		return( hdfs_remove_response( rptr ) );
	else
	{
		/* ---------------------------------- */
		/* return the result of the operation */
		/* ---------------------------------- */
		rptr = hdfs_remove_response( rptr );
		return( rrptr );
	}
}

private	struct hdfs_response * hdfs_read_path( char * path, char * tls, char * agent )
{
	struct	rest_header * hptr;
	struct	hdfs_subscription * sptr;
	struct	hdfs_response * rptr;
	struct	hdfs_response * rrptr;

	/* ------------------------------------ */
	/* resolve the subscription information */
	/* ------------------------------------ */
	if (!( sptr = subscription ))
		return((struct hdfs_response *) 0);
	/* --------------------------------- */
	/* request access to the datanode id */
	/* --------------------------------- */
	else if (!( rptr = hdfs_open_file( sptr, path, 0, -1 ) ))
		return( rptr );
	/* ------------------------------------- */
	/* resolve the data node location header */
	/* ------------------------------------- */
	else if (!( hptr = rest_resolve_header( rptr->response->first,_HTTP_LOCATION ) ))
		return( hdfs_remove_response( rptr ) );
	/* -------------------------------------- */
	/* read the data from the data node entry */
	/* -------------------------------------- */
	else if (!( rrptr = hdfs_read_file( sptr, hptr->value ) ))
		return( hdfs_remove_response( rptr ) );
	else
	{
		/* ---------------------------------- */
		/* return the result of the operation */
		/* ---------------------------------- */
		rptr = hdfs_remove_response( rptr );
		return( rrptr );
	}
}

private	struct hdfs_response * hdfs_delete_path( char * path, char * tls, char * agent )
{
	struct	hdfs_subscription * sptr;
	struct	hdfs_response * rptr;

	/* ------------------------------------ */
	/* resolve the subscription information */
	/* ------------------------------------ */
	if (!( sptr = subscription ))
		return((struct hdfs_response *) 0);
	/* --------------------- */
	/* perform the operation */
	/* --------------------- */
	else if (!( rptr = hdfs_delete_file( sptr, path, 1 ) ))
		return( rptr );
	else	return( rptr );
}

private	struct hdfs_response * hdfs_rename_path( char * path, char * tls, char * agent, char * newpath )
{
	struct	hdfs_subscription * sptr;
	struct	hdfs_response * rptr;

	/* ------------------------------------ */
	/* resolve the subscription information */
	/* ------------------------------------ */
	if (!( sptr = subscription ))
		return((struct hdfs_response *) 0);
	/* --------------------- */
	/* perform the operation */
	/* --------------------- */
	else if (!( rptr = hdfs_rename_file( sptr, path, newpath ) ))
		return( rptr );
	else	return( rptr );
}

private	struct hdfs_response * hdfs_mkdir_path( char * path, char * tls, char * agent )
{
	struct	hdfs_subscription * sptr;
	struct	hdfs_response * rptr;

	/* ------------------------------------ */
	/* resolve the subscription information */
	/* ------------------------------------ */
	if (!( sptr = subscription ))
		return((struct hdfs_response *) 0);
	/* --------------------- */
	/* perform the operation */
	/* --------------------- */
	else if (!( rptr = hdfs_mkdir( sptr, path, 0644 ) ))
		return( rptr );
	else	return( rptr );
}

private	struct hdfs_response * hdfs_status_path( char * path, char * tls, char * agent )
{
	struct	hdfs_subscription * sptr;
	struct	hdfs_response * rptr;

	/* ------------------------------------ */
	/* resolve the subscription information */
	/* ------------------------------------ */
	if (!( sptr = subscription ))
		return((struct hdfs_response *) 0);
	/* --------------------- */
	/* perform the operation */
	/* --------------------- */
	else if (!( rptr = hdfs_stat( sptr, path ) ))
		return( rptr );
	else	return( rptr );
}

private	struct hdfs_response * hdfs_list_path( char * path, char * tls, char * agent )
{
	struct	hdfs_subscription * sptr;
	struct	hdfs_response * rptr;

	/* ------------------------------------ */
	/* resolve the subscription information */
	/* ------------------------------------ */
	if (!( sptr = subscription ))
		return((struct hdfs_response *) 0);
	/* --------------------- */
	/* perform the operation */
	/* --------------------- */
	else if (!( rptr = hdfs_lsdir( sptr, path ) ))
		return( rptr );
	else	return( rptr );
}

/* ------------------------------------------------------------------------------------ */
/*				h d f s _ o p e r a t i o n					*/
/* ------------------------------------------------------------------------------------ */
private	int	hdfs_operation( char * p1, char * p2, char * p3, char * p4, char * p5, char * p6, char * p7 )
{
	struct	rest_header * hptr = (struct rest_header *) 0;
	char	*	agent = "ON-CLIENT/1.0";
	char	*	nomfic;
	if (!( p1))
		return( failure( 30,"p1", "required") );
	else if (!( p2 ))
		return( failure( 30,"p2", "required") );
	else if (!( p3 ))
		return( failure( 30,"p3", "required") );
	else if (!( strcasecmp(p1,"WRITE" ) ))
		return( show_hdfs_result( hdfs_write_path( p2, default_tls(), agent, p3 ) ) );
	else if (!( strcasecmp(p1,"READ" ) ))
		return( show_hdfs_result( hdfs_read_path( p2, default_tls(), agent ) ) );
	else if (!( strcasecmp(p1,"DELETE" ) ))
		return( show_hdfs_result( hdfs_delete_path( p2, default_tls(), agent ) ) );
	else if (!( strcasecmp(p1,"RENAME" ) ))
		return( show_hdfs_result( hdfs_rename_path( p2, default_tls(), agent, p3 ) ) );
	else if (!( strcasecmp(p1,"MKDIR" ) ))
		return( show_hdfs_result( hdfs_mkdir_path( p2, default_tls(), agent ) ) );
	else if (!( strcasecmp(p1,"STATUS" ) ))
		return( show_hdfs_result( hdfs_status_path( p2, default_tls(), agent ) ) );
	else if (!( strcasecmp(p1,"LIST" ) ))
		return( show_hdfs_result( hdfs_list_path( p2, default_tls(), agent ) ) );
	else	return( failure(32, "incorrect value for p1: ", p1 ) );
}

/* ------------------------------------------------------------------------------------ */
/*				h d f s _ c o m m a n d 					*/
/* ------------------------------------------------------------------------------------ */
private	int	hdfs_command(int argc, char * argv[] )
{
	int	status;
	int	argi=1;
	int	port=0;
	char *	aptr;
	char *	user=(char *) 0;
	char *	password=(char *) 0;
	char *	host=(char *) 0;
	char *	agent="CO-ONCLIENT/1.0";
	char *	version="v1.0";
	while ( argi < argc )
	{
		if (!( aptr = argv[argi++] ))
			break;
		else if ( *aptr != '-' )
		{
			if (!( host ))
				return( failure( status, "requires value for", "--host" ) );
			else if (!( user ))
				return( failure( status, "requires value for", "--user" ) );
			else if (!( password ))
				return( failure( status, "requires value for", "--password" ) );
			else if ((subscription = hdfs_initialise_client( user, password, host, agent, version, tls, port )) != 0)
				return( failure( status, "initialising", "client" ) );
			else	return( hdfs_operation( aptr, argv[argi], argv[argi+1], argv[argi+2], argv[argi+ 3], argv[argi+ 4], argv[argi+ 5] ) );
		}
		else if (  *(++aptr) == '-' )
		{
			aptr++;

			if (!( strcasecmp( aptr,"user" ) ))
				user = argv[argi++];
			else if (!( strcasecmp( aptr,"password" ) ))
				password = argv[argi++];
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

/* ------------------------------------------------------------------------------------ */
/*				h d f s _ b a n n e r 					*/
/* ------------------------------------------------------------------------------------ */
private	int	hdfs_banner()
{
	printf("\n   CO-OS : CompatibleOne Hadoop HDFC Test Client : Version 1.0a.0.01");
	printf("\n   Beta Version 20/01/2013");
	printf("\n   Copyright (c) 2011,2013 Iain James Marshall, Prologue" );
	printf("\n");
	printf("\n   REST Methods");
	printf("\n");
	printf("\n   [ WRITE    {path} {filename} ");
	printf("\n   [ READ     {path} {filename} ");
	printf("\n   [ DELETE   {path} ");
	printf("\n   [ RENAME   {path} {newpath}  ");
	printf("\n   [ MKDIR    {path} ");
	printf("\n   [ STATUS   {path} ");
	printf("\n   [ LIST     {path} ");
	printf("\n ");
	printf("\n   Options");
	printf("\n     --user <username>     set account user name ");
	printf("\n     --password <value>    set account password  ");
	printf("\n     --host <hostname>     set host name         ");
	printf("\n     --version <value>     set host version      ");
	printf("\n     --agent   <name>      set test agent name   ");
	printf("\n     --tls <config>        set tls config        ");
	printf("\n     --verbose             activate verbose messages ");
	printf("\n     --debug               activate debug messages ");

	printf("\n\n");
	return( 0 );
}

/* ------------------------------------------------------------------------------------ */
/*					m a i n 					*/
/* ------------------------------------------------------------------------------------ */
public	int	main(int argc, char * argv[] )
{
	if ( argc == 1 )
		return( hdfs_banner() );
	else	return( hdfs_command(argc,argv) );
}

	/* ----------- */
#endif	/* _testhdfs_c */
	/* ----------- */


