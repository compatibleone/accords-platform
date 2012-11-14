#ifndef	_copaas_c	
#define	_copaas_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"

/* 	----------------------------------------	*/	
/*	Prototype Contract Negotiation Functions	*/
/* 	----------------------------------------	*/

struct	accords_configuration CoPaas = {
	0,0,
	0,0,0,0,
	(char *) 0,
	(char *) 0, 
	_CORDS_DEFAULT_PUBLISHER,
	_CORDS_DEFAULT_OPERATOR,
	_CORDS_DEFAULT_USER,
	_CORDS_DEFAULT_PASSWORD,
	"http",  80,
	"xmpp",  8000,
	"domain",
	"europe",
	"copaas.xml",
	"storage",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(CoPaas.debug);		}
public	int	check_verbose()		{	return(CoPaas.verbose);		}
public	char *	default_publisher()	{	return(CoPaas.publisher);	}
public	char *	default_operator()	{	return(CoPaas.operator);		}
public	char *	default_tls()		{	return(CoPaas.tls);		}
public	char *	default_zone()		{	return(CoPaas.zone);		}

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

#include "copaas.h"
#include "paas_application.c"
#include "paas_configuration_option.c"
#include "paas_application_configuration_template.c"
#include "paas_application_deployable.c"
#include "paas_application_environment.c"
#include "paas_link.c"
#include "paas_application_manifest.c"
#include "paas_relation.c"
#include "paas_node.c"
#include "paas_application_version.c"
#include "paas_application_version_instance.c"
#include "occipaas_application.c"
#include "occipaas_configuration_option.c"
#include "occipaas_configuration_template.c"
#include "occipaas_deployable.c"
#include "occipaas_environment.c"
#include "occipaas_link.c"
#include "occipaas_manifest.c"
#include "occipaas_relation.c"
#include "occipaas_node.c"
#include "occipaas_version.c"
#include "occipaas_version_instance.c"

/*	---------------------------------------------------------------	*/  
/*	c o p a a s _ c o n f i g u r a t i o n				*/
/*	---------------------------------------------------------------	*/  
/*	this function loads copaas configuration			*/
/*	from the xml configuration file.				*/
/*	---------------------------------------------------------------	*/  
private	void	copaas_configuration()
{
	load_accords_configuration( &CoPaas, "copaas" );
	return;
}

/*	-----------------------------------------------------	----------	*/  
/*	c o p a a s _ b a n n e r				*/
/*	---------------------------------------------------------------	*/  
/*	this function will be called to display module identification 	*/
/*	when launched from the command line without any parameters.	*/
/*	---------------------------------------------------------------	*/  
private	int	copaas_banner()
{
	printf("\n   CompatibleOne Generic CoPaas Service : Version 1.0a.0.01");
	printf("\n   Beta Version : 26/07/2012 ");
	printf("\n   Copyright (c) 2012 Iain James Marshall, Mohamed Mohamed, Sami Yangui");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}

/*	------------------------------------------- */
/*	c o p a a s _ i n i t i a l i s e	    */
/*	------------------------------------------- */

private	struct rest_server * copaas_initialise(  void * v,struct rest_server * sptr )
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

/*	-------------------------------------------	*/
/*	c o p a a s _ a u t h o r i s e 		*/
/*	------------------------------------------- 	*/
private	int	copaas_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, CoPaas.user ) )
		return(0);
	else if ( strcmp( password, CoPaas.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	-------------------------------------------	*/
/*	c o p a a s _ e x t e n s i o n 		*/
/*	-------------------------------------------	*/
private	struct rest_extension * copaas_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	--------------------------------------------	*/
/*	c o p a a s _ o p e r a t i o n 		*/
/*	--------------------------------------------	*/
/*	environment and category preparation		*/
/*	--------------------------------------------	*/
private	int	copaas_operation( char * nptr )
{

	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_copaas.xml");

	rest_initialise_log( CoPaas.monitor );

	/* ---------------------------------------- */
	/* preparation of application category list */
	/* ---------------------------------------- */
	if (!( optr = occi_paas_application_builder( CoPaas.domain, "paas_application" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_paas_deployable_builder( CoPaas.domain, "paas_application_deployable" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_paas_link_builder( CoPaas.domain, "paas_link" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_paas_environment_builder( CoPaas.domain, "paas_application_environment" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_paas_manifest_builder( CoPaas.domain, "paas_application_manifest" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_paas_node_builder( CoPaas.domain, "paas_node" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_paas_version_builder( CoPaas.domain, "paas_application_version" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_paas_relation_builder( CoPaas.domain, "paas_relation" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_paas_version_instance_builder( CoPaas.domain, "paas_application_version_instance" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;


	if (!( optr = occi_paas_configuration_option_builder( CoPaas.domain, "paas_configuration_option" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;


	if (!( optr = occi_paas_configuration_template_builder( CoPaas.domain, "paas_application_configuration_template" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;


	if (!( CoPaas.identity ))
	{
		/* --------------------------	*/
		/* launch simple occi server 	*/
		/* --------------------------	*/
		return( occi_server(  
			nptr, 
			CoPaas.restport, CoPaas.tls, 
			CoPaas.threads, first, (char *) 0 ) );
	}
	else
	{
		/* --------------------------------------------------------------- */
		/* publication and pricing of categories before occi server launch */
		/* --------------------------------------------------------------- */
		initialise_occi_publisher( 
			CoPaas.publisher, (char*) 0, (char *) 0, (char *) 0);

		return( publishing_occi_server(
				CoPaas.user, CoPaas.password,
				CoPaas.identity,  nptr, 
				CoPaas.restport, CoPaas.tls, 
				CoPaas.threads, first ) );
	}
}

/*	------------------------------------------- */
/*	c o p a a s _ o p t i o n s		    */
/*	------------------------------------------- */
/*	Command line option analysis		    */
/*	------------------------------------------- */
private	int	copaas_options(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	copaas_configuration();
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
				CoPaas.verbose=1;
				continue;
			case	'd'	:
				CoPaas.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = copaas_operation(aptr) ))
			continue;
		else	break;
	}
	return(status);
}

/*	--------------------------------------------	*/
/*	m a i n 					*/
/*	--------------------------------------------	*/
/*	Entry point from the command line		*/
/*	--------------------------------------------	*/
public	int	main(int argc, char * argv[] )
{
	if ( argc == 1 )
		return( copaas_banner() );
	else	return( copaas_options( argc, argv ) );
}

	/* ---------- */
#endif 	/* _copaas_c */
	/* ---------- */


