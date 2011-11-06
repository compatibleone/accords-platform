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
#ifndef	_procci_c
#define	_procci_c

#include "standard.h"
#include "procci.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"

struct	procci_configuration Procci = {
	0,
	0,0,0,
	(char *) 0,
	(char *) 0, (char *) 0,
	"admin", "admin",

	"http",  80,
	"xmpp",  8000,

	"domain",
	"procci.xml",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(Procci.debug);		}
public	int	check_verbose()		{	return(Procci.verbose);		}
public	char *	default_publisher()	{	return(Procci.publisher);	}
public	char *	default_tls()		{	return(Procci.tls);		}

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
/*			p r o c c i _ l o a d 		*/
/*	---------------------------------------------	*/
/*	this function loads procci    configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	procci_load()
{
	struct	xml_element * document;
	struct	xml_element * eptr;
	struct	xml_element * vptr;
	struct	xml_atribut * aptr;
	set_xml_echo(Procci.debug);
	if (( document = document_parse_file( Procci.config )) != (struct xml_element *) 0)
	{
		if ( Procci.verbose )
			printf("   Procci loading configuration from : %s \n",Procci.config);

		if (( eptr = document_element( document, "procci" )) != (struct xml_element *) 0)
		{
			if ((aptr = document_atribut( eptr, "identity" )) != (struct xml_atribut *) 0)
				Procci.identity = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "publisher")) != (struct xml_atribut *) 0)
				Procci.publisher = document_atribut_string( aptr );
			if ((aptr = document_atribut( eptr, "verbose")) != (struct xml_atribut *) 0)
				Procci.verbose = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "debug")) != (struct xml_atribut *) 0)
				Procci.debug = document_atribut_value( aptr );
			if ((aptr = document_atribut( eptr, "threads")) != (struct xml_atribut *) 0)
				Procci.threads = document_atribut_value( aptr );

			if (( vptr = document_element( eptr, "rest" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "host" )) != (struct xml_atribut *) 0)
					Procci.resthost = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "port" )) != (struct xml_atribut *) 0)
					Procci.restport = document_atribut_value( aptr );
			}
			if (( vptr = document_element( eptr, "security" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "user" )) != (struct xml_atribut *) 0)
					Procci.user = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "password"))  != (struct xml_atribut *) 0)
					Procci.password = document_atribut_string( aptr );
				if ((aptr = document_atribut( vptr, "tls" )) != (struct xml_atribut *) 0)
					if ((Procci.tls = document_atribut_string( aptr )) != 0)
						if (!( strlen(Procci.tls) ))
							Procci.tls = (char *) 0;
				if ((aptr = document_atribut( vptr, "monitor")) != (struct xml_atribut *) 0)
					Procci.monitor = document_atribut_value( aptr );
			}
			if (( vptr = document_element( eptr, "domain" )) != (struct xml_element *) 0)
			{
				if ((aptr = document_atribut( vptr, "name" )) != (struct xml_atribut *) 0)
					Procci.domain = document_atribut_string( aptr );
			}
		}
		document = document_drop( document );
	}
	else if ( Procci.verbose )
		printf("   Procci failed to load configuration from : %s \n",Procci.config);
	set_xml_echo(0);
	return;
}

private	int	banner()
{
	printf("\n   CompatibleOne Request Procci : Version 1.0a.0.02");
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
/*			p r o c c i _ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * procci_initialise(  void * v,struct rest_server * sptr )
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
/*			p r o c c i _ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	procci_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, Procci.user ) )
		return(0);
	else if ( strcmp( password, Procci.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*			p r o c c i _ e x t e n s i o n 			*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * procci_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	------------------------------------------------------------------	*/
/*	inclusion of the actions and methods required for contract handling	*/
/*	------------------------------------------------------------------	*/
#include "proccicontract.c"

/*	------------------------------------------------------------------	*/
/*			p r o c c i _ o p e r a t i o n				*/
/*	------------------------------------------------------------------	*/
private	int	procci_operation( char * nptr )
{

	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_procci.xml");

	if (!( optr = occi_cords_node_builder( Procci.domain,"node" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = procci_contract_builder( Procci.domain,"contract" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_cords_parameter_builder( Procci.domain,"parameter" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_cords_instruction_builder( Procci.domain,"instruction" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_cords_value_builder( Procci.domain,"value" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_cords_infrastructure_builder( Procci.domain,"infrastructure" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_cords_compute_builder( Procci.domain,"compute" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_cords_storage_builder( Procci.domain,"storage" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_cords_network_builder( Procci.domain,"network" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_cords_image_builder( Procci.domain,"image" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_cords_system_builder( Procci.domain,"system" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	if (!( optr = occi_cords_package_builder( Procci.domain,"package" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	rest_initialise_log( Procci.monitor );

	if (!( Procci.identity ))
		return( occi_server(  nptr, Procci.restport, Procci.tls, Procci.threads, first, (char *) 0 ) );
	else
	{
		initialise_occi_publisher( Procci.publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			Procci.user, Procci.password,
			Procci.identity,  nptr, 
			Procci.restport, Procci.tls, 
			Procci.threads, first ) );
	}
}


/*	------------------------------------------------------------------	*/
/*				p r o c c i 					*/
/*	------------------------------------------------------------------	*/
private	int	procci(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	procci_load();
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
				Procci.verbose=1;
				continue;
			case	'd'	:
				Procci.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( strcmp( aptr, "verbose" ) ))
					Procci.verbose = 1;
				else if (!( strcmp( aptr, "debug" ) ))
					Procci.debug = 1;
				else if (!( strcmp( aptr, "config" ) ))
				{
					Procci.config = allocate_string( argv[++argi] );
					procci_load();
				}
				else if (!( strcmp( aptr, "tls" ) ))
					Procci.tls = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "domain" ) ))
					Procci.domain = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "publisher" ) ))
					Procci.publisher = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "identity" ) ))
					Procci.identity = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "user" ) ))
					Procci.user = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "password" ) ))
					Procci.password = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "resthost" ) ))
					Procci.resthost = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "restport" ) ))
					Procci.restport = atoi( argv[++argi] );
				else if (!( strcmp( aptr, "chathost" ) ))
					Procci.chathost = allocate_string( argv[++argi] );
				else if (!( strcmp( aptr, "chatport" ) ))
					Procci.chatport = atoi( argv[++argi] );
				else if (!( strcmp( aptr, "threads" ) ))
					Procci.threads = _REST_THREAD_WORKER;
				else	break;
				continue;
			}
			status = 30;
			break;
		}
		else if (!( status = procci_operation(aptr) ))
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
	else	return( procci( argc, argv ) );
}


	/* --------- */
#endif	/* _procci_c */
	/* --------- */

