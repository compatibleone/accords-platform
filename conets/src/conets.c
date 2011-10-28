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
#ifndef	_conets_c
#define	_conets_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"

struct	broker_configuration Conets = {
	0,
	0,
	0,0,0,
	(char *) 0,
	(char *) 0, (char *) 0,
	"admin", "admin",
	"http",  80,
	"xmpp",  8000,
	"domain",
	"conets.xml",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(Conets.debug);		}
public	int	check_verbose()		{	return(Conets.verbose);		}
public	char *	default_publisher()	{	return(Conets.publisher);		}
public	char *	default_tls()		{	return(Conets.tls);		}

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
/*		c o n e t s _ l o a d 		*/
/*	---------------------------------------------	*/
/*	this function loads conets  configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	conets_load()
{
	struct	xml_element * document;
	struct	xml_element * eptr;
	struct	xml_element * vptr;
	struct	xml_atribut * aptr;
	set_xml_echo(Conets.debug);
	if (( document = document_parse_file( Conets.config )) != (struct xml_element *) 0)
	{
		if ( Conets.verbose )
			printf("   Conets loading configuration from : %s \n",Conets.config);

		if (( eptr = document_element( document, "conets" )) != (struct xml_element *) 0)
		{
			if ((aptr = document_atribut( eptr, "identity" )) != (struct xml_atribut *) 0)
				Conets.identity = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "publisher")) != (struct xml_atribut *) 0)
				Conets.publisher = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "verbose")) != (struct xml_atribut *) 0)
				Conets.verbose = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "debug")) != (struct xml_atribut *) 0)
				Conets.debug = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "threads")) != (struct xml_atribut *) 0)
				Conets.threads = document_atribut_value( aptr );

			if (( vptr = document_element( eptr, "rest" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "host" )) != (struct xml_atribut *) 0)
					Conets.resthost = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "port" )) != (struct xml_atribut *) 0)
					Conets.restport = document_atribut_value( aptr );
			}
			if (( vptr = document_element( eptr, "security" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "user" )) != (struct xml_atribut *) 0)
					Conets.user = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "password"))  != (struct xml_atribut *) 0)
					Conets.password = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "tls" )) != (struct xml_atribut *) 0)
					if ((Conets.tls = document_atribut_string( aptr )) != 0)
						if (!( strlen(Conets.tls) ))
							Conets.tls = (char *) 0;
				if ((aptr = document_atribut( vptr, "monitor")) != (struct xml_atribut *) 0)
					Conets.monitor = document_atribut_value( aptr );
			}
			if (( vptr = document_element( eptr, "domain" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "name" )) != (struct xml_atribut *) 0)
					Conets.domain = document_atribut_string( aptr );
			}
		}
		document = document_drop( document );
	}
	else if ( Conets.verbose )
		printf("   Conets failed to load configuration from : %s \n",Conets.config);
	set_xml_echo(0);
	return;
}

#include "occibuilder.h"

private	int	banner()
{
	printf("\n   CompatibleOne Networking Services : Version 1.0a.0.02");
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
/*			c o n e t s _ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * conets_initialise(  void * v,struct rest_server * sptr )
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
/*			c o n e t s _ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	conets_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, Conets.user ) )
		return(0);
	else if ( strcmp( password, Conets.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*			c o n e t s _ e x t e n s i o n 			*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * conets_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	------------------------------------------------------------------	*/
/* 	  actions and methods required for the conets instance category		*/
/*	------------------------------------------------------------------	*/
/* none for now */

/*	------------------------------------------------------------------	*/
/*			c o n e t s _ o p e r a t i o n				*/
/*	------------------------------------------------------------------	*/
private	int	conets_operation( char * nptr )
{
	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_conets.xml");


	if (!( optr = occi_cords_ipaddress_builder( Conets.domain, "ipaddress" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	if (!( optr = occi_cords_domain_builder( Conets.domain, "domain" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	optr->callback  = (void *) 0;

	rest_initialise_log( Conets.monitor );

	if (!( Conets.identity ))
		return( occi_server(  nptr, Conets.restport, Conets.tls, Conets.threads, first,(char *) 0 ) );
	else
	{
		initialise_occi_publisher( Conets.publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			Conets.user, Conets.password,
			Conets.identity,  nptr, 
			Conets.restport, Conets.tls, 
			Conets.threads, first ) );
	}
}

/*	------------------------------------------------------------------	*/
/*				c o n e t s 					*/
/*	------------------------------------------------------------------	*/
private	int	conets(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	conets_load();
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
				Conets.verbose=1;
				continue;
			case	'd'	:
				Conets.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( strcmp( aptr, "auto" ) ))
					Conets.autopub = 1;
				else if (!( strcmp( aptr, "verbose" ) ))
					Conets.verbose = 1;
				else if (!( strcmp( aptr, "debug" ) ))
					Conets.debug = 1;
				else if (!( strcmp( aptr, "tls" ) ))
					Conets.tls = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "config" ) ))
				{
					Conets.config = allocate_string( argv[++argi] );
					conets_load();
				}
				else if (!( strcmp( aptr, "domain" ) ))
					Conets.domain = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "publisher" ) ))
					Conets.publisher = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "identity" ) ))
					Conets.identity = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "user" ) ))
					Conets.user = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "password" ) ))
					Conets.password = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "resthost" ) ))
					Conets.resthost = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "restport" ) ))
					Conets.restport = atoi( argv[++argi] );
				else if (!( strcmp( aptr, "chathost" ) ))
					Conets.chathost = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "chatport" ) ))
					Conets.chatport = atoi( argv[++argi] );
				else if (!( strcmp( aptr, "threads" ) ))
					Conets.threads = _REST_THREAD_WORKER;
				else	break;
				continue;
			}
			status = 30;
			break;
		}
		else if (!( status = conets_operation(aptr) ))
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
	else	return( conets( argc, argv ) );
}


	/* --------- */
#endif	/* _conets_c */
	/* --------- */

