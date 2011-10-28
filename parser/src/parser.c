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
#ifndef	_parser_c
#define	_parser_c

#include "standard.h"
#include "parser.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"

struct	parser_configuration Parser = {
	0,
	0,
	0,0,0,
	(char *) 0,
	(char *) 0, (char *) 0,
	"admin", "admin",
	"http",  80,
	"xmpp",  8000,
	"domain",
	"parser.xml",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(Parser.debug);		}
public	int	check_verbose()		{	return(Parser.verbose);		}
public	char *	default_publisher()	{	return(Parser.publisher);	}
public	char *	default_tls()		{	return(Parser.tls);		}

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
/*			p a r s e r _ l o a d 		*/
/*	---------------------------------------------	*/
/*	this function loads parser    configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	parser_load()
{
	struct	xml_element * document;
	struct	xml_element * eptr;
	struct	xml_element * vptr;
	struct	xml_atribut * aptr;
	set_xml_echo(Parser.debug);
	if (( document = document_parse_file( Parser.config )) != (struct xml_element *) 0)
	{
		if ( Parser.verbose )
			printf("   Parser loading configuration from : %s \n",Parser.config);

		if (( eptr = document_element( document, "parser" )) != (struct xml_element *) 0)
		{
			if ((aptr = document_atribut( eptr, "identity" )) != (struct xml_atribut *) 0)
				Parser.identity = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "publisher")) != (struct xml_atribut *) 0)
				Parser.publisher = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "verbose")) != (struct xml_atribut *) 0)
				Parser.verbose = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "debug")) != (struct xml_atribut *) 0)
				Parser.debug = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "threads")) != (struct xml_atribut *) 0)
				Parser.threads = document_atribut_value( aptr );

			if (( vptr = document_element( eptr, "rest" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "host" )) != (struct xml_atribut *) 0)
					Parser.resthost = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "port" )) != (struct xml_atribut *) 0)
					Parser.restport = document_atribut_value( aptr );
			}
			if (( vptr = document_element( eptr, "security" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "user" )) != (struct xml_atribut *) 0)
					Parser.user = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "password"))  != (struct xml_atribut *) 0)
					Parser.password = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "tls" )) != (struct xml_atribut *) 0)
					if ((Parser.tls = document_atribut_string( aptr )) != 0)
						if (!( strlen(Parser.tls) ))
							Parser.tls = (char *) 0;
				if ((aptr = document_atribut( vptr, "monitor")) != (struct xml_atribut *) 0)
					Parser.monitor = document_atribut_value( aptr );
			}
			if (( vptr = document_element( eptr, "domain" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "name" )) != (struct xml_atribut *) 0)
					Parser.domain = document_atribut_string( aptr );
			}
		}
		document = document_drop( document );
	}
	else if ( Parser.verbose )
		printf("   Parser failed to load configuration from : %s \n",Parser.config);
	set_xml_echo(0);
	return;
}

private	int	banner()
{
	printf("\n   CompatibleOne Request Parser : Version 1.0a.0.02");
	printf("\n   Beta Version : 26/10/2011");
	printf("\n   Copyright (c) 2011 Iain James Marshall, Prologue");
	printf("\n");
	printf("\nOptions: ");
	printf("\n   --debug                    activates debug messages");
	printf("\n   --verbose                  activates info  messages");
	printf("\n   --tls        <name>        specify tls configuration ");
	printf("\n   --threads                  activate thread handlers");
	printf("\n   --resthost   <name>        specify rest server host name or address");
	printf("\n   --restport   <number>      specify rest server port");
	printf("\n   --publisher  <name>        specify url of publisher");
	printf("\n   --identity   <name>        specify url to publish  ");
	printf("\n   --user       <name>        specify user log name   ");
	printf("\n   --password   <value>       specify user password   ");
	printf("\n\n");
	return(0);

}

/*	------------------------------------------------------------------	*/
/*			b r o k e r _ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * parser_initialise(  void * v,struct rest_server * sptr )
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
/*			b r o k e r _ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	parser_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, Parser.user ) )
		return(0);
	else if ( strcmp( password, Parser.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*			b r o k e r _ e x t e n s i o n 			*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * parser_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	------------------------------------------------------------------	*/
/*			b r o k e r _ o p e r a t i o n				*/
/*	------------------------------------------------------------------	*/
private	int	parser_operation( char * nptr )
{
	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_parser.xml");

	if (!( optr = occi_cords_manifest_builder( Parser.domain,"manifest" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	if ( Parser.autopub )
		optr->access   |= _OCCI_AUTO_PUBLISH;

	if (!( optr = occi_cords_tarif_builder( Parser.domain, "tarif" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	if (!( optr = occi_cords_constraint_builder( Parser.domain,"constraint" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	if (!( optr = occi_cords_requirement_builder( Parser.domain,"requirement" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	rest_initialise_log( Parser.monitor );

	if (!( Parser.identity ))
		return( occi_server(  nptr, Parser.restport, Parser.tls, Parser.threads, first, (char *) 0 ) );
	else
	{
		initialise_occi_publisher( Parser.publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			Parser.user, Parser.password,
			Parser.identity,  nptr, 
			Parser.restport, Parser.tls, 
			Parser.threads, first ) );
	}
}

/*	------------------------------------------------------------------	*/
/*				b r o k e r 					*/
/*	------------------------------------------------------------------	*/
private	int	parser(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	parser_load();
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
				Parser.verbose=1;
				continue;
			case	'd'	:
				Parser.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( strcmp( aptr, "auto" ) ))
					Parser.autopub = 1;
				else if (!( strcmp( aptr, "verbose" ) ))
					Parser.verbose = 1;
				else if (!( strcmp( aptr, "debug" ) ))
					Parser.debug = 1;
				else if (!( strcmp( aptr, "config" ) ))
				{
					Parser.config = allocate_string( argv[++argi] );
					parser_load();
				}
				else if (!( strcmp( aptr, "tls" ) ))
					Parser.tls = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "domain" ) ))
					Parser.domain = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "publisher" ) ))
					Parser.publisher = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "identity" ) ))
					Parser.identity = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "user" ) ))
					Parser.user = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "password" ) ))
					Parser.password = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "resthost" ) ))
					Parser.resthost = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "restport" ) ))
					Parser.restport = atoi( argv[++argi] );
				else if (!( strcmp( aptr, "threads" ) ))
					Parser.threads = _REST_THREAD_WORKER;
				else	break;
				continue;
			}
			status = 30;
			break;
		}
		else if (!( status = parser_operation(aptr) ))
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
	else	return( parser( argc, argv ) );
}


	/* --------- */
#endif	/* _parser_c */
	/* --------- */

