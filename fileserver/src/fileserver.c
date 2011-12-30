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
#ifndef	_fileserver_c
#define	_fileserver_c

#include "standard.h"
#include "fileserver.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"

struct	fileserver_configuration Fs = {
	0,
	0,0,0,0,
	(char *) 0,
	(char *) 0,
	_CORDS_DEFAULT_PUBLISHER,
	"admin", "admin",
	"http",  80,
	"domain",
	"fileserver.xml",
	};

public	int	check_debug()		{	return(Fs.debug);		}
public	int	check_verbose()		{	return(Fs.verbose);		}
public	char *	default_publisher()	{	return(Fs.publisher);		}
public	char *	default_tls()		{	return(Fs.tls);			}


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

/*	---------------------------------------------	*/  
/*		f i l e s e r v e r _ l o a d 		*/
/*	---------------------------------------------	*/
/*	this function loads fileserver configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	fileserver_load()
{
	struct	xml_element * document;
	struct	xml_element * eptr;
	struct	xml_element * vptr;
	struct	xml_atribut * aptr;

	set_xml_echo(Fs.debug);

	if (( document = document_parse_file( Fs.config )) != (struct xml_element *) 0)
	{
		if ( Fs.verbose )
			printf("   Broker loading configuration from : %s \n",Fs.config);

		if (( eptr = document_element( document, "fileserver" )) != (struct xml_element *) 0)
		{
			if ((aptr = document_atribut( eptr, "identity" )) != (struct xml_atribut *) 0)
				Fs.identity = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "publisher")) != (struct xml_atribut *) 0)
				Fs.publisher = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "verbose")) != (struct xml_atribut *) 0)
				Fs.verbose = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "debug")) != (struct xml_atribut *) 0)
				Fs.debug = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "threads")) != (struct xml_atribut *) 0)
				Fs.threads = document_atribut_value( aptr );

			if (( vptr = document_element( eptr, "rest" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "host" )) != (struct xml_atribut *) 0)
					Fs.resthost = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "port" )) != (struct xml_atribut *) 0)
					Fs.restport = document_atribut_value( aptr );
			}
			if (( vptr = document_element( eptr, "security" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( eptr, "authorise")) != (struct xml_atribut *) 0)
					Fs.authorise = document_atribut_value( aptr );
				if ((aptr = document_atribut( vptr, "user" )) != (struct xml_atribut *) 0)
					Fs.user = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "password"))  != (struct xml_atribut *) 0)
					Fs.password = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "tls" )) != (struct xml_atribut *) 0)
					if ((Fs.tls = document_atribut_string( aptr )) != 0)
						if (!( strlen(Fs.tls) ))
							Fs.tls = (char *) 0;
			}
			if (( vptr = document_element( eptr, "domain" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "name" )) != (struct xml_atribut *) 0)
					Fs.domain = document_atribut_string( aptr );
			}
		}
		document = document_drop( document );
	}
	else if ( Fs.verbose )
		printf("   Broker failed to load configuration from : %s \n",Fs.config);
	set_xml_echo(0);
	return;
}

private	int	banner()
{
	printf("\n   CompatibleOne FileServer : Version 1.0a.0.02");
	printf("\n   Beta Version : 14/10/2011");
	printf("\n   Copyright (c) 2011 Iain James Marshall, Prologue");
	printf("\n");
	printf("\nOptions: ");
	printf("\n   --debug                  activates debug messages");
	printf("\n   --verbose                activates info  messages");
	printf("\n   --threads                activate thread control ");
	printf("\n   --config   <file>        load configuration");
	printf("\n   --authorise <value>      set authorization state");            
	printf("\n   --tls      <name>        specify tls configuration");
	printf("\n   --publisher <name>       specify publisher url ");
	printf("\n   --resthost <name>        specify rest server host name or address");
	printf("\n   --restport <number>      specify rest server port");
	printf("\n   --user     <name>        specify user log name   ");
	printf("\n   --password <value>       specify user password   ");
	printf("\n\n");
	return(0);

}

/*	------------------------------------------------------------------	*/
/*			f i l e s e r v e r_ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * fileserver_initialise(  void * v,struct rest_server * sptr )
{
	struct	rest_extension * xptr;
	if (!( xptr = rest_add_extension( sptr ) ))
		return((struct rest_server *) 0);
	else
	{
		xptr->net = (struct connection *) 0;
		return( sptr );
	}
}

/*	------------------------------------------------------------------	*/
/*			f i l e s e r v e r_ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	fileserver_authorise(  void * v,struct rest_client * cptr, char * username, char * password)
{
	if ( check_verbose )
		printf("   REST Authentication of %s \n",username); 
	if (!( Fs.authorise ))
		return(1);
	else if (!( Fs.user ))
		return(1);
	else if ( strcmp( username, Fs.user ) )
		return(0);
	else if ( strcmp( password, Fs.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*			f i l e s e r v e r_ e x t e n s i o n 			*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * fileserver_extension( void * v,struct rest_server * sptr, 
struct rest_extension * xptr)
{
	return( xptr );
}

/*	------------------------------------------------------------------	*/
/*		f i l e s e r v e r_ g e t _ c a p a c i t i e s			*/
/*	------------------------------------------------------------------	*/
private	struct rest_response * fileserver_get_capacities( 
		struct rest_client * cptr, 
		struct rest_request * rptr,
		struct rest_response * aptr )
{
	return( rest_html_response( aptr, 500, "Server Failure" ) );
}

/*	------------------------------------------------------------------	*/
/*			f i l e s e r v e r_ g e t _ f i l e				*/
/*	------------------------------------------------------------------	*/
private	struct rest_response * fileserver_get_file( 
		struct rest_client * cptr, 
		struct rest_request * rptr,
		struct rest_response * aptr )
{
	struct	stat info;
	char	length[64];
	char	buffer[1024];
	char *	nptr;
	if ( stat( rptr->object,&info ) < 0 )
		return( rest_html_response( aptr, 404, "File Not Found" ) );
	else
	{
		sprintf(length,"%u",info.st_size);
		if (!( rest_response_header(aptr,"Content-Length",length) ))
			return( rest_html_response( aptr, 500, "Server Failure" ) );
		else if (!( rest_response_header(aptr,"Content-Type","text/html") ))
			return( rest_html_response( aptr, 500, "Server Failure" ) );
		else if (!( nptr = allocate_string( rptr->object ) ))
			return( rest_html_response( aptr, 500, "Server Failure" ) );
		else
		{
			rest_response_body(aptr,nptr,0 );
			return( rest_response_status( aptr, 200, "OK" ) );
		}
	}
}

/*	------------------------------------------------------------------	*/
/*				f i l e s e r v e r_ g e t 				*/
/*	------------------------------------------------------------------	*/
private	struct rest_response * fileserver_get( void * v,struct rest_client * cptr, struct rest_request * rptr )
{
	struct	rest_response * aptr;
	printf("   Fs GET Request : %s %s %s \n",rptr->method,rptr->object,rptr->version);
	if (!( aptr = rest_allocate_response(cptr) ))
		return( aptr );
	else if ( rptr->body  )
		return( rest_html_response( aptr, 400, "Unexpected Request Body" ) );
	else	return( fileserver_get_file( cptr, rptr, aptr ) );
}

/*	------------------------------------------------------------------	*/
/*				f i l e s e r v e r_ p o s t 				*/
/*	------------------------------------------------------------------	*/
private	struct rest_response * fileserver_post(  void * v,struct rest_client * cptr, 
		struct rest_request * rptr )
{
	struct	rest_response * aptr;
	struct	rest_header   * hptr;
	struct	xml_element   * document;
	FILE 		      * target;
	
	printf("   Fs POST Request : %s %s %s \n",rptr->method,rptr->object,rptr->version);
	if (!( aptr = rest_allocate_response(cptr) ))
		return( aptr );
	else if (!( rptr->body  ))
		return( rest_html_response( aptr, 404, "Expected Body" ) );
	else 	return( rest_html_response( aptr, 400, "Post Not Available" ) );
}

/*	------------------------------------------------------------------	*/
/*				f i l e s e r v e r_ p u t				*/
/*	------------------------------------------------------------------	*/	
private	struct rest_response * fileserver_put(  void * v,struct rest_client * cptr, 
		struct rest_request * rptr )
{
	struct	rest_response * aptr;
	printf("   Fs PUT Request : %s %s %s \n",rptr->method,rptr->object,rptr->version);
	if (!( aptr = rest_allocate_response(cptr) ))
		return( aptr );
	else if (!( rptr->body ))
		return( rest_html_response( aptr, 400, "Put Requires a Body" ) );
	else 	return( rest_html_response( aptr, 400, "Put Not Available" ) );
}

/*	------------------------------------------------------------------	*/
/*				f i l e s e r v e r_ d e l e t e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_response * fileserver_delete(  void * v,struct rest_client * cptr, 
		struct rest_request * rptr )
{
	struct	rest_response * aptr;
	printf("   Fs DELETE Request : %s %s %s \n",rptr->method,rptr->object,rptr->version);
	if (!( aptr = rest_allocate_response(cptr) ))
		return( aptr );
	else if ( rptr->body  )
		return( rest_html_response( aptr, 400, "Unexpected Request Body" ) );
	else 	return( rest_html_response( aptr, 400, "Delete Not Possible" ) );
}

/*	------------------------------------------------------------------	*/
/*				f i l e s e r v e r_ h e a d				*/
/*	------------------------------------------------------------------	*/
private	struct rest_response * fileserver_head(  void * v,struct rest_client * cptr, struct rest_request * rptr )
{
	struct	rest_response * aptr;
	printf("   Fs HEAD Request : %s %s %s \n",rptr->method,rptr->object,rptr->version);
	if (!( aptr = rest_allocate_response(cptr) ))
		return( aptr );
	else if ( rptr->body  )
		return( rest_html_response( aptr, 400, "Unexpected Request Body" ) );
	else 	return( rest_html_response( aptr, 400, "Head Not Possible" ) );
}

/*	------------------------------------------------------------------	*/
/*			f i l e s e r v e r_ o p e r a t i o n				*/
/*	------------------------------------------------------------------	*/
private	int	fileserver_operation( char * nptr )
{
	int	status=0;
	struct	rest_interface  FsI = 
	{
		(void *) 0,
		fileserver_initialise,
		fileserver_authorise,
		fileserver_get,
		fileserver_post,
		fileserver_put,
		fileserver_delete,
		fileserver_head,
		fileserver_extension,
		(void *) 0,
		(void *) 0,
		(void *) 0,
		(void *) 0

	};

	if (!( Fs.authorise ))
		FsI.authorise = (void *) 0;

	return( rest_server(  nptr, Fs.restport, Fs.tls, Fs.threads, & FsI ) );
}


/*	------------------------------------------------------------------	*/
/*				f i l e s e r v e r					*/
/*	------------------------------------------------------------------	*/
private	int	fileserver(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	fileserver_load();
	while ( argi < argc )
	{
		if (!( aptr = argv[++argi] ))
			break;
		else if ( *aptr == '-' )
		{
			aptr++;
			switch( *(aptr++) )
			{
			case	'v'	:
				Fs.verbose=1;
				continue;
			case	'd'	:
				Fs.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( strcmp( aptr, "verbose" ) ))
					Fs.verbose = 1;
				else if (!( strcmp( aptr, "debug" ) ))
					Fs.debug = 1;
				else if (!( strcmp( aptr, "tls" ) ))
					Fs.tls = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "config" ) ))
				{
					Fs.config = allocate_string( argv[++argi] );
					fileserver_load();
				}
				else if (!( strcmp( aptr, "authorise" ) ))
					Fs.authorise = atoi( argv[++argi] );
				else if (!( strcmp( aptr, "domain" ) ))
					Fs.domain = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "user" ) ))
					Fs.user = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "password" ) ))
					Fs.password = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "resthost" ) ))
					Fs.resthost = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "restport" ) ))
					Fs.restport = atoi( argv[++argi] );
				else if (!( strcmp( aptr, "threads" ) ))
					Fs.threads = _REST_THREAD_WORKER;
				else	break;
				continue;
			}
			status = 30;
			break;
		}
		else if (!( status = fileserver_operation(aptr) ))
			continue;
		else	break;
	}
	return(status);
}

/*	------------------------------------------------------------------	*/
/*					m a i n 				*/
/*	------------------------------------------------------------------	*/
public	int	main(int argc, char * argv[] )
{
	if ( argc == 1 )
		return( banner() );
	else	return( fileserver( argc, argv ) );
}


	/* --------- */
#endif	/* _fileserver_c */
	/* --------- */

