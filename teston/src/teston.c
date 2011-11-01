/* ------------------------------------------------------------------------------------	*/
/*				 CompatibleOne Cloudware				*/
/* ------------------------------------------------------------------------------------ */
/*											*/
/* Ce fichier fait partie de ce(tte) oeuvre de Iain James Marshall et est mise a 	*/
/* disposition selon les termes de la licence Creative Commons Paternit‚ : 		*/
/*											*/
/*			 	Pas d'Utilisation Commerciale 				*/
/*				Pas de Modification 					*/
/*				3.0 non transcrit.					*/
/*											*/
/* ------------------------------------------------------------------------------------ */
/* 			Copyright (c) 2011 Iain James Marshall for Prologue 		*/
/*				   All rights reserved					*/
/* ------------------------------------------------------------------------------------ */

#ifndef	_teston_c
#define	_teston_c

#include "onclient.h"

private	int	debug=0;
private	int	verbose=1;
private	char *	tls=(char *) 0;
private	char *	publisher=(char *) 0;

public	int	check_debug()		{	return(debug);		}
public	int	check_verbose()		{	return(verbose);	}
public	char *	default_publisher()	{	return(publisher);	}
public	char *	default_tls()		{	return(tls);		}

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
/*					o n _ r e s u l t				*/
/* ------------------------------------------------------------------------------------ */
private	int	on_result( struct on_response * rptr )
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
		liberate_on_response( rptr );
		return(0);
	}
	else	return( failure(99,"no","result") );
}

/* ------------------------------------------------------------------------------------ */
/*			o n _ c l i e n t _ l i s t _ o b j e c t			*/
/* ------------------------------------------------------------------------------------ */
private	struct	on_response * on_list_object( 
		char * keyword, 
		char * tls, 
		char * agent, 
		struct rest_header * hptr )
{
	if (!( keyword ))
		return((struct on_response *) 0);
	else if (!( strcasecmp( keyword, "server" ) ))
	{
		return( on_get_request( "/compute" ) );
	}
	else if (!( strcasecmp( keyword, "compute" ) ))
	{
		return( on_get_request( "/compute" ) );
	}
	else if (!( strcasecmp( keyword, "storage" ) ))
	{
		return( on_get_request( "/storage" ) );
	}
	else if (!( strcasecmp( keyword, "network" ) ))
	{
		return( on_get_request( "/network" ) );
	}
	else	return((struct on_response *) 0);
}

/* ------------------------------------------------------------------------------------ */
/*			o n _ c l i e n t _ p o s t _ o b j e c t			*/
/* ------------------------------------------------------------------------------------ */
private	struct	on_response * on_post_object( 
		char * keyword, 
		char * tls, 
		char * agent,
		struct rest_header * hptr,
		char * p3,
		char * p4,
		char * p5 )
{
	char *	filename;
	if (!( keyword ))
		return((struct on_response *) 0);
	else if (!( strcasecmp( keyword, "server" ) ))
	{
		if (!( filename = on_create_server_request( p3, p4, p5, "", "" )))
			return((struct on_response *) 0);
		else 	return( on_post_request( "/compute", filename ) );
	}
	else if (!( strcasecmp( keyword, "compute" ) ))
	{
		if (!( filename = on_create_compute_request( p3, p4, p5, "", "" )))
			return((struct on_response *) 0);
		else	return( on_post_request( "/compute", filename ) );
	}
	else if (!( strcasecmp( keyword, "storage" ) ))
	{
		if (!( filename = on_create_storage_request( p3, p4, "OS", p5 ) ))
			return((struct on_response *) 0);
		else	return( on_post_request( "/storage", filename ) );
	}
	else if (!( strcasecmp( keyword, "network" ) ))
	{
		if (!( filename = on_create_network_request( p3, p4, p5 )))
			return((struct on_response *) 0);
		else	return( on_post_request( "/network", filename ) );
	}
	else	return((struct on_response *) 0);
}

/* ------------------------------------------------------------------------------------ */
/*			o n _ c l i e n t _ g e t _ o b j e c t				*/
/* ------------------------------------------------------------------------------------ */
private	struct	on_response * on_get_object( 
		char * keyword, 
		char * tls, 
		char * agent, 
		char * parameter, 
		struct rest_header * hptr )
{
	char buffer[2048];
	if (!( keyword ))
		return((struct on_response *) 0);
	else if (!( strcasecmp( keyword, "server" ) ))
	{
		sprintf(buffer,"%s/%s","/compute",parameter );
		return( on_get_request( buffer ) );
	}
	else if (!( strcasecmp( keyword, "compute" ) ))
	{
		sprintf(buffer,"%s/%s","/compute",parameter );
		return( on_get_request( buffer ) );
	}
	else if (!( strcasecmp( keyword, "storage" ) ))
	{
		sprintf(buffer,"%s/%s","/storage",parameter );
		return( on_get_request( buffer ) );
	}
	else if (!( strcasecmp( keyword, "network" ) ))
	{
		sprintf(buffer,"%s/%s","/network",parameter );
		return( on_get_request( buffer ) );
	}
	else	return((struct on_response *) 0);
}

/* ------------------------------------------------------------------------------------ */
/*			o n _ c l i e n t _ p u t _ o b j e c t				*/
/* ------------------------------------------------------------------------------------ */
private	struct	on_response * on_put_object( 
		char * keyword, 
		char * tls, 
		char * agent, 
		char * parameter, 
		char * filename,
		struct rest_header * hptr )
{
	char buffer[2048];
	if (!( keyword ))
		return((struct on_response *) 0);
	else if (!( strcasecmp( keyword, "server" ) ))
	{
		sprintf(buffer,"%s/%s","/compute",parameter );
		return( on_put_request( buffer, filename ) );
	}
	else if (!( strcasecmp( keyword, "compute" ) ))
	{
		sprintf(buffer,"%s/%s","/compute",parameter );
		return( on_put_request( buffer, filename ) );
	}
	else if (!( strcasecmp( keyword, "storage" ) ))
	{
		sprintf(buffer,"%s/%s","/storage",parameter );
		return( on_put_request( buffer, filename ) );
	}
	else if (!( strcasecmp( keyword, "network" ) ))
	{
		sprintf(buffer,"%s/%s","/network",parameter );
		return( on_put_request( buffer, filename ) );
	}
	else	return((struct on_response *) 0);
}

/* ------------------------------------------------------------------------------------ */
/*			o n _ c l i e n t _ d e l e t e _ o b j e c t			*/
/* ------------------------------------------------------------------------------------ */
private	struct	on_response * on_delete_object( 
		char * keyword, 
		char * tls, 
		char * agent, 
		char * parameter, 
		struct rest_header * hptr )
{
	char buffer[2048];
	if (!( keyword ))
		return((struct on_response *) 0);
	else if (!( strcasecmp( keyword, "server" ) ))
	{
		sprintf(buffer,"%s/%s","/compute",parameter );
		return( on_delete_request( buffer ) );
	}
	else if (!( strcasecmp( keyword, "compute" ) ))
	{
		sprintf(buffer,"%s/%s","/compute",parameter );
		return( on_delete_request( buffer ) );
	}
	else if (!( strcasecmp( keyword, "storage" ) ))
	{
		sprintf(buffer,"%s/%s","/storage",parameter );
		return( on_delete_request( buffer ) );
	}
	else if (!( strcasecmp( keyword, "network" ) ))
	{
		sprintf(buffer,"%s/%s","/network",parameter );
		return( on_delete_request( buffer ) );
	}
	else	return((struct on_response *) 0);
}


/* ------------------------------------------------------------------------------------ */
/*				o n _ o p e r a t i o n					*/
/* ------------------------------------------------------------------------------------ */
private	int	on_operation( char * p1, char * p2, char * p3, char * p4, char * p5 )
{
	struct	rest_header * hptr = (struct rest_header *) 0;
	char	*	agent = "ON-CLIENT/1.0";
	char	*	nomfic;
	if (!( p1))
		return( failure( 30,"p1", "required") );
	else if (!( p2 ))
		return( failure( 30,"p2", "required") );
	else if (!( strcasecmp(p1,"LIST" ) ))
		return( on_result( on_list_object( p2, default_tls(), agent, hptr ) ) );
	else if (!( p3 ))
		return( failure( 30,"p3", "required") );
	else if (!( strcasecmp(p1,"GET" ) ))
		return( on_result( on_get_object( p2, default_tls(), agent, p3, hptr ) ) );
	else if (!( strcasecmp(p1,"DELETE" ) ))
		return( on_result( on_delete_object( p2, default_tls(), agent, p3, hptr ) ) );
	else if (!( p4 ))
		return( failure( 30,"p4", "required") );
	else if (!( strcasecmp(p1,"PUT" ) ))
		return( on_result( on_put_object( p2, default_tls(), agent, p3, p4, hptr ) ) );
	else if (!( p5 ))
		return( failure( 30,"p5", "required") );
	else if (!( strcasecmp(p1,"POST" ) ))
		return( on_result( on_post_object( p2, default_tls(), agent, hptr, p3, p4, p5 ) ) );
	else	return( failure(32, "incorrect value for p1: ", p1 ) );
}

/* ------------------------------------------------------------------------------------ */
/*				o n _ c o m m a n d 					*/
/* ------------------------------------------------------------------------------------ */
private	int	on_command(int argc, char * argv[] )
{
	int	status;
	int	argi=1;
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
			else if ((status = on_initialise_client( user, password, host, agent, version, tls )) != 0)
				return( failure( status, "initialising", "client" ) );
			else	return( on_operation( aptr, argv[argi], argv[argi+1], argv[argi+2], argv[argi+ 3] ) );
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
/*				o n _ b a n n e r 					*/
/* ------------------------------------------------------------------------------------ */
private	int	on_banner()
{
	printf("\n   CO-OS : CompatibleOne OpenNebula Client Test : Version 1.0a.0.01");
	printf("\n   Beta Version 01/11/2011");
	printf("\n   Copyright (c) 2011 Iain James Marshall, Prologue" );
	printf("\n");
	printf("\n   CRUD Operations ");
	printf("\n");
	printf("\n   LIST [ compute | storage | network | server ] ");
	printf("\n");
	printf("\n   REST Methods");
	printf("\n");
	printf("\n   [ GET    [ compute | storage | network | server ] {id} ");
	printf("\n   [ DELETE [ compute | storage | network | server ] {id} ");
	printf("\n   [ POST   [ compute | storage | network | server ] {filename} ");
	printf("\n   [ PUT    [ compute | storage | network | server ] {id} {filename} ");
	printf("\n\n");
	return( 0 );
}

/* ------------------------------------------------------------------------------------ */
/*					m a i n 					*/
/* ------------------------------------------------------------------------------------ */
public	int	main(int argc, char * argv[] )
{
	if ( argc == 1 )
		return( on_banner() );
	else	return( on_command(argc,argv) );
}

	/* --------- */
#endif	/* _teston_c */
	/* --------- */


