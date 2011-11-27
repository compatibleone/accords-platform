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
#ifndef	_coobas_c
#define	_coobas_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"

struct	broker_configuration CooBas = {
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
	"coobas.xml",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(CooBas.debug);		}
public	int	check_verbose()		{	return(CooBas.verbose);		}
public	char *	default_publisher()	{	return(CooBas.publisher);		}
public	char *	default_tls()		{	return(CooBas.tls);			}

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
/*		c o o b a s _ l o a d 		*/
/*	---------------------------------------------	*/
/*	this function loads coobas configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	coobas_load()
{
	struct	xml_element * document;
	struct	xml_element * eptr;
	struct	xml_element * vptr;
	struct	xml_atribut * aptr;
	set_xml_echo(CooBas.debug);
	if (( document = document_parse_file( CooBas.config )) != (struct xml_element *) 0)
	{
		if ( CooBas.verbose )
			printf("   CooBas loading configuration from : %s \n",CooBas.config);

		if (( eptr = document_element( document, "coobas" )) != (struct xml_element *) 0)
		{
			if ((aptr = document_atribut( eptr, "identity" )) != (struct xml_atribut *) 0)
				CooBas.identity = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "publisher")) != (struct xml_atribut *) 0)
				CooBas.publisher = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "verbose")) != (struct xml_atribut *) 0)
				CooBas.verbose = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "debug")) != (struct xml_atribut *) 0)
				CooBas.debug = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "threads")) != (struct xml_atribut *) 0)
				CooBas.threads = document_atribut_value( aptr );

			if (( vptr = document_element( eptr, "rest" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "host" )) != (struct xml_atribut *) 0)
					CooBas.resthost = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "port" )) != (struct xml_atribut *) 0)
					CooBas.restport = document_atribut_value( aptr );
			}
			if (( vptr = document_element( eptr, "security" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "user" )) != (struct xml_atribut *) 0)
					CooBas.user = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "password"))  != (struct xml_atribut *) 0)
					CooBas.password = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "tls" )) != (struct xml_atribut *) 0)
					if ((CooBas.tls = document_atribut_string( aptr )) != 0)
						if (!( strlen(CooBas.tls) ))
							CooBas.tls = (char *) 0;
				if ((aptr = document_atribut( vptr, "monitor")) != (struct xml_atribut *) 0)
					CooBas.monitor = document_atribut_value( aptr );
			}
			if (( vptr = document_element( eptr, "domain" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "name" )) != (struct xml_atribut *) 0)
					CooBas.domain = document_atribut_string( aptr );
			}
		}
		document = document_drop( document );
	}
	else if ( CooBas.verbose )
		printf("   CooBas failed to load configuration from : %s \n",CooBas.config);
	set_xml_echo(0);
	return;
}

#include "occibuilder.h"

private	int	banner()
{
	printf("\n   CompatibleOne Ordering, Billing and Accounting COOBAS : Version 1.0a.0.02");
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
/*			c o o b a s _ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * coobas_initialise(  void * v,struct rest_server * sptr )
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
/*			c o o b a s _ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	coobas_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, CooBas.user ) )
		return(0);
	else if ( strcmp( password, CooBas.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*			c o o b a s _ e x t e n s i o n 			*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * coobas_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	------------------------------------------------------------------	*/
/* 	  actions and methods required for the coobas instance category		*/
/*	------------------------------------------------------------------	*/

/*	------------------------------------------------------------------	*/
/*			c o o b a s _ o p e r a t i o n				*/
/*	------------------------------------------------------------------	*/
private	int	coobas_operation( char * nptr )
{
	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_coobas.xml");

	if (!( optr = occi_cords_account_builder( CooBas.domain, "account" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	if (!( optr = occi_cords_transaction_builder( CooBas.domain, "transaction" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	if (!( optr = occi_cords_price_builder( CooBas.domain, "price" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	if (!( optr = occi_cords_invoice_builder( CooBas.domain, "invoice" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	rest_initialise_log(CooBas.monitor );

	if (!( CooBas.identity ))
		return( occi_server(nptr,CooBas.restport,CooBas.tls,CooBas.threads,first, (char *) 0 ) );
	else
	{
		initialise_occi_publisher( CooBas.publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			CooBas.user, CooBas.password,
			CooBas.identity,  nptr, 
			CooBas.restport, CooBas.tls, 
			CooBas.threads, first ) );
	}
}

/*	------------------------------------------------------------------	*/
/*				c o o b a s 					*/
/*	------------------------------------------------------------------	*/
private	int	coobas(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	coobas_load();
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
				CooBas.verbose=1;
				continue;
			case	'd'	:
				CooBas.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( strcmp( aptr, "auto" ) ))
					CooBas.autopub = 1;
				else if (!( strcmp( aptr, "verbose" ) ))
					CooBas.verbose = 1;
				else if (!( strcmp( aptr, "debug" ) ))
					CooBas.debug = 1;
				else if (!( strcmp( aptr, "config" ) ))
				{
					CooBas.config = allocate_string( argv[++argi] );
					coobas_load();
				}
				else if (!( strcmp( aptr, "tls" ) ))
					CooBas.tls = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "domain" ) ))
					CooBas.domain = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "publisher" ) ))
					CooBas.publisher = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "identity" ) ))
					CooBas.identity = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "user" ) ))
					CooBas.user = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "password" ) ))
					CooBas.password = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "resthost" ) ))
					CooBas.resthost = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "restport" ) ))
					CooBas.restport = atoi( argv[++argi] );
				else if (!( strcmp( aptr, "chathost" ) ))
					CooBas.chathost = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "chatport" ) ))
					CooBas.chatport = atoi( argv[++argi] );
				else if (!( strcmp( aptr, "threads" ) ))
					CooBas.threads = _REST_THREAD_WORKER;
				else	break;
				continue;
			}
			status = 30;
			break;
		}
		else if (!( status = coobas_operation(aptr) ))
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
	else	return( coobas( argc, argv ) );
}


	/* --------- */
#endif	/* _coobas_c */
	/* --------- */

