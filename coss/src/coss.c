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
#ifndef	_coss_c
#define	_coss_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"

struct	broker_configuration Coss = {
	0,
	0,
	0,0,0,
	(char *) 0,
	(char *) 0, 
	_CORDS_DEFAULT_PUBLISHER,
	"admin", "admin",
	"http",  80,
	"xmpp",  8000,
	"domain",
	"coss.xml",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(Coss.debug);		}
public	int	check_verbose()		{	return(Coss.verbose);	}
public	char *	default_publisher()	{	return(Coss.publisher);	}
public	char *	default_tls()		{	return(Coss.tls);		}

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
/*		c o s s _ l o a d 		*/
/*	---------------------------------------------	*/
/*	this function loads coss  configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	coss_load()
{
	struct	xml_element * document;
	struct	xml_element * eptr;
	struct	xml_element * vptr;
	struct	xml_atribut * aptr;
	set_xml_echo(Coss.debug);
	if (( document = document_parse_file( Coss.config )) != (struct xml_element *) 0)
	{
		if ( Coss.verbose )
			printf("   Coss loading configuration from : %s \n",Coss.config);

		if (( eptr = document_element( document, "coss" )) != (struct xml_element *) 0)
		{
			if ((aptr = document_atribut( eptr, "identity" )) != (struct xml_atribut *) 0)
				Coss.identity = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "publisher")) != (struct xml_atribut *) 0)
				Coss.publisher = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "verbose")) != (struct xml_atribut *) 0)
				Coss.verbose = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "debug")) != (struct xml_atribut *) 0)
				Coss.debug = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "threads")) != (struct xml_atribut *) 0)
				Coss.threads = document_atribut_value( aptr );

			if (( vptr = document_element( eptr, "rest" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "host" )) != (struct xml_atribut *) 0)
					Coss.resthost = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "port" )) != (struct xml_atribut *) 0)
					Coss.restport = document_atribut_value( aptr );
			}
			if (( vptr = document_element( eptr, "security" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "user" )) != (struct xml_atribut *) 0)
					Coss.user = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "password"))  != (struct xml_atribut *) 0)
					Coss.password = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "tls" )) != (struct xml_atribut *) 0)
					if ((Coss.tls = document_atribut_string( aptr )) != 0)
						if (!( strlen(Coss.tls) ))
							Coss.tls = (char *) 0;
				if ((aptr = document_atribut( vptr, "monitor")) != (struct xml_atribut *) 0)
					Coss.monitor = document_atribut_value( aptr );
			}
			if (( vptr = document_element( eptr, "domain" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "name" )) != (struct xml_atribut *) 0)
					Coss.domain = document_atribut_string( aptr );
			}
		}
		document = document_drop( document );
	}
	else if ( Coss.verbose )
		printf("   Coss failed to load configuration from : %s \n",Coss.config);
	set_xml_echo(0);
	return;
}

#include "occibuilder.h"

private	int	banner()
{
	printf("\n   CompatibleOne Security Services COSS : Version 1.0a.0.02");
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
/*			c o s s _ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * coss_initialise(  void * v,struct rest_server * sptr )
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
/*			c o s s _ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	coss_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, Coss.user ) )
		return(0);
	else if ( strcmp( password, Coss.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*			c o s s _ e x t e n s i o n 			*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * coss_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	------------------------------------------------------------------	*/
/* 	  actions and methods required for the coss instance category		*/
/*	------------------------------------------------------------------	*/
#include "accountuser.c"

/*	------------------------------------------------------------------	*/
/*			c o s s _ o p e r a t i o n				*/
/*	------------------------------------------------------------------	*/
private	int	coss_operation( char * nptr )
{
	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_coss.xml");

	if (!( optr = occi_cords_security_builder( Coss.domain, "security" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	if (!( optr = account_user_builder( Coss.domain, "user" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	if (!( optr = occi_cords_authorisation_builder( Coss.domain, "authorisation" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	rest_initialise_log(Coss.monitor);

	if (!( Coss.identity ))
		return( occi_server(  nptr, Coss.restport, Coss.tls, Coss.threads, first, (char *) 0 ) );
	else
	{
		initialise_occi_publisher( Coss.publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			Coss.user, Coss.password,
			Coss.identity,  nptr, 
			Coss.restport, Coss.tls, 
			Coss.threads, first ) );
	}
}

/*	------------------------------------------------------------------	*/
/*				c o s s 					*/
/*	------------------------------------------------------------------	*/
private	int	coss(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	coss_load();
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
				Coss.verbose=1;
				continue;
			case	'd'	:
				Coss.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( strcmp( aptr, "auto" ) ))
					Coss.autopub = 1;
				else if (!( strcmp( aptr, "verbose" ) ))
					Coss.verbose = 1;
				else if (!( strcmp( aptr, "debug" ) ))
					Coss.debug = 1;
				else if (!( strcmp( aptr, "tls" ) ))
					Coss.tls = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "config" ) ))
				{
					Coss.config = allocate_string( argv[++argi] );
					coss_load();
				}
				else if (!( strcmp( aptr, "domain" ) ))
					Coss.domain = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "publisher" ) ))
					Coss.publisher = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "identity" ) ))
					Coss.identity = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "user" ) ))
					Coss.user = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "password" ) ))
					Coss.password = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "resthost" ) ))
					Coss.resthost = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "restport" ) ))
					Coss.restport = atoi( argv[++argi] );
				else if (!( strcmp( aptr, "threads" ) ))
					Coss.threads = _REST_THREAD_WORKER;
				else	break;
				continue;
			}
			status = 30;
			break;
		}
		else if (!( status = coss_operation(aptr) ))
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
	else	return( coss( argc, argv ) );
}


	/* --------- */
#endif	/* _coss_c */
	/* --------- */

