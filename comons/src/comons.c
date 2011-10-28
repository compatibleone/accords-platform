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
#ifndef	_comons_c
#define	_comons_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"

struct	broker_configuration Comons = {
	0,
	0,
	0,0,0,
	(char *) 0,
	(char *) 0, (char *) 0,
	"admin", "admin",
	"http",  80,
	"xmpp",  8000,
	"domain",
	"comons.xml",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(Comons.debug);		}
public	int	check_verbose()		{	return(Comons.verbose);		}
public	char *	default_publisher()	{	return(Comons.publisher);		}
public	char *	default_tls()		{	return(Comons.tls);		}

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
/*		c o m o n s _ l o a d 		*/
/*	---------------------------------------------	*/
/*	this function loads comons  configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	comons_load()
{
	struct	xml_element * document;
	struct	xml_element * eptr;
	struct	xml_element * vptr;
	struct	xml_atribut * aptr;
	set_xml_echo(Comons.debug);
	if (( document = document_parse_file( Comons.config )) != (struct xml_element *) 0)
	{
		if ( Comons.verbose )
			printf("   Comons loading configuration from : %s \n",Comons.config);

		if (( eptr = document_element( document, "comons" )) != (struct xml_element *) 0)
		{
			if ((aptr = document_atribut( eptr, "identity" )) != (struct xml_atribut *) 0)
				Comons.identity = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "publisher")) != (struct xml_atribut *) 0)
				Comons.publisher = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "verbose")) != (struct xml_atribut *) 0)
				Comons.verbose = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "debug")) != (struct xml_atribut *) 0)
				Comons.debug = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "threads")) != (struct xml_atribut *) 0)
				Comons.threads = document_atribut_value( aptr );

			if (( vptr = document_element( eptr, "rest" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "host" )) != (struct xml_atribut *) 0)
					Comons.resthost = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "port" )) != (struct xml_atribut *) 0)
					Comons.restport = document_atribut_value( aptr );
			}
			if (( vptr = document_element( eptr, "security" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "user" )) != (struct xml_atribut *) 0)
					Comons.user = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "password"))  != (struct xml_atribut *) 0)
					Comons.password = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "tls" )) != (struct xml_atribut *) 0)
					if ((Comons.tls = document_atribut_string( aptr )) != 0)
						if (!( strlen(Comons.tls) ))
							Comons.tls = (char *) 0;
				if ((aptr = document_atribut( eptr, "monitor")) != (struct xml_atribut *) 0)
					Comons.monitor = document_atribut_value( aptr );
			}
			if (( vptr = document_element( eptr, "domain" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "name" )) != (struct xml_atribut *) 0)
					Comons.domain = document_atribut_string( aptr );
			}
		}
		document = document_drop( document );
	}
	else if ( Comons.verbose )
		printf("   Comons failed to load configuration from : %s \n",Comons.config);
	set_xml_echo(0);
	return;
}

#include "occibuilder.h"

private	int	banner()
{
	printf("\n   CompatibleOne Monitoring Services : Version 1.0a.0.02");
	printf("\n   Beta Version : 26/10/2011");
	printf("\n   Copyright (c) 2011 Iain James Marshall, Prologue");
	printf("\n");
	printf("\nOptions: ");
	printf("\n   --debug                    activates debug messages");
	printf("\n   --verbose                  activates info  messages");
	printf("\n   --tls                      require transport layer security");
	printf("\n   --threads                  activate thread handlers");
	printf("\n   --resthost   <name>        specify rest server host name or address");
	printf("\n   --restport   <number>      specify rest server port");
	printf("\n   --chathost   <name>        specify chat server host name or address");
	printf("\n   --chatport   <number>      specify chat server port");
	printf("\n   --publisher  <name>        specify url of publisher");
	printf("\n   --identity   <name>        specify url to publish  ");
	printf("\n   --user       <name>        specify user log name   ");
	printf("\n   --password   <value>       specify user password   ");
	printf("\n\n");
	return(0);

}

/*	------------------------------------------------------------------	*/
/*			c o m o n s _ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * comons_initialise(  void * v,struct rest_server * sptr )
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
/*			c o m o n s _ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	comons_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, Comons.user ) )
		return(0);
	else if ( strcmp( password, Comons.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*			c o m o n s _ e x t e n s i o n 			*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * comons_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	------------------------------------------------------------------	*/
/* 	  actions and methods required for the comons instance category		*/
/*	------------------------------------------------------------------	*/
/* none for now */

/*	------------------------------------------------------------------	*/
/*			c o m o n s _ o p e r a t i o n				*/
/*	------------------------------------------------------------------	*/
private	int	comons_operation( char * nptr )
{
	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_comons.xml");


	if (!( optr = occi_cords_monitor_builder( Comons.domain, "monitor" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	if (!( optr = occi_cords_event_builder( Comons.domain, "event" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	if (!( optr = occi_cords_report_builder( Comons.domain, "report" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	rest_initialise_log(Comons.monitor);

	if (!( Comons.identity ))
		return( occi_server(  nptr, Comons.restport, Comons.tls, Comons.threads, first,(char *) 0 ) );
	else
	{
		initialise_occi_publisher( Comons.publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			Comons.user, Comons.password,
			Comons.identity,  nptr, 
			Comons.restport, Comons.tls, 
			Comons.threads, first ) );
	}
}

/*	------------------------------------------------------------------	*/
/*				c o m o n s 					*/
/*	------------------------------------------------------------------	*/
private	int	comons(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	comons_load();
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
				Comons.verbose=1;
				continue;
			case	'd'	:
				Comons.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( strcmp( aptr, "auto" ) ))
					Comons.autopub = 1;
				else if (!( strcmp( aptr, "verbose" ) ))
					Comons.verbose = 1;
				else if (!( strcmp( aptr, "debug" ) ))
					Comons.debug = 1;
				else if (!( strcmp( aptr, "tls" ) ))
					Comons.tls = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "config" ) ))
				{
					Comons.config = allocate_string( argv[++argi] );
					comons_load();
				}
				else if (!( strcmp( aptr, "domain" ) ))
					Comons.domain = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "publisher" ) ))
					Comons.publisher = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "identity" ) ))
					Comons.identity = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "user" ) ))
					Comons.user = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "password" ) ))
					Comons.password = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "resthost" ) ))
					Comons.resthost = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "restport" ) ))
					Comons.restport = atoi( argv[++argi] );
				else if (!( strcmp( aptr, "chathost" ) ))
					Comons.chathost = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "chatport" ) ))
					Comons.chatport = atoi( argv[++argi] );
				else if (!( strcmp( aptr, "threads" ) ))
					Comons.threads = _REST_THREAD_WORKER;
				else	break;
				continue;
			}
			status = 30;
			break;
		}
		else if (!( status = comons_operation(aptr) ))
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
	else	return( comons( argc, argv ) );
}


	/* --------- */
#endif	/* _comons_c */
	/* --------- */

