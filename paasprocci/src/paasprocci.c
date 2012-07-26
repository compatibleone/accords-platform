#ifndef	_paasprocci_c	
#define	_paasprocci_c

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

struct	accords_configuration PaasProcci = {
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
	"paasprocci.xml",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(PaasProcci.debug);		}
public	int	check_verbose()		{	return(PaasProcci.verbose);		}
public	char *	default_publisher()	{	return(PaasProcci.publisher);	}
public	char *	default_operator()	{	return(PaasProcci.operator);		}
public	char *	default_tls()		{	return(PaasProcci.tls);		}
public	char *	default_zone()		{	return(PaasProcci.zone);		}

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

#include "paasprocci.h"
#include "paas_application.c"
#include "paas_configuration_option.c"
#include "paas_configuration_template.c"
#include "paas_deployable.c"
#include "paas_environment.c"
#include "paas_link.c"
#include "paas_manifest.c"
#include "paas_relation.c"
#include "paas_node.c"
#include "paas_version.c"
#include "paas_version_instance.c"
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
/*	p a a s p r o c c i _ c o n f i g u r a t i o n			*/
/*	---------------------------------------------------------------	*/  
/*	this function loads paasprocci configuration			*/
/*	from the xml configuration file.				*/
/*	---------------------------------------------------------------	*/  
private	void	paasprocci_configuration()
{
	load_accords_configuration( &PaasProcci, "paasprocci" );
	return;
}

/*	---------------------------------------------------------------	*/  
/*	p a a s p r o c c i _ b a n n e r				*/
/*	---------------------------------------------------------------	*/  
/*	this function will be called to display module identification 	*/
/*	when launched from the command line without any parameters.	*/
/*	---------------------------------------------------------------	*/  
private	int	paasprocci_banner()
{
	printf("\n   CompatibleOne Generic PaasProcci Service : Version 1.0a.0.01");
	printf("\n   Beta Version : 26/07/2012 ");
	printf("\n   Copyright (c) 2012 Iain James Marshall, Mohamed Mohamed, Sami Yangui");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}

/*	------------------------------------------- */
/*	p a a s p r o c c i _ i n i t i a l i s e		*/
/*	------------------------------------------- */

private	struct rest_server * paasprocci_initialise(  void * v,struct rest_server * sptr )
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
/*	p a a s p r o c c i _ a u t h o r i s e 		*/
/*	------------------------------------------- 	*/
private	int	paasprocci_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, PaasProcci.user ) )
		return(0);
	else if ( strcmp( password, PaasProcci.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------- */
/*	p a a s p r o c c i _ e x t e n s i o n 		*/
/*	-------------------------------------------	*/
private	struct rest_extension * paasprocci_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	--------------------------------------------	*/
/*	p a a s p r o c c i _ o p e r a t i o n 		*/
/*	--------------------------------------------	*/
/*	environment and category preparation		*/
/*	--------------------------------------------	*/
private	int	paasprocci_operation( char * nptr )
{

	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_paasprocci.xml");

	rest_initialise_log( PaasProcci.monitor );

	/* ---------------------------------------- */
	/* preparation of application category list */
	/* ---------------------------------------- */
	if (!( optr = occi_paas_application_builder( PaasProcci.domain, "paas_application" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_paas_deployable_builder( PaasProcci.domain, "paas_deployable" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_paas_link_builder( PaasProcci.domain, "paas_link" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_paas_environment_builder( PaasProcci.domain, "paas_environment" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_paas_manifest_builder( PaasProcci.domain, "paas_manifest" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_paas_node_builder( PaasProcci.domain, "paas_node" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_paas_version_builder( PaasProcci.domain, "paas_version" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_paas_relation_builder( PaasProcci.domain, "paas_relation" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_paas_version_instance_builder( PaasProcci.domain, "paas_version_instance" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;


	if (!( optr = occi_paas_configuration_option_builder( PaasProcci.domain, "paas_configuration_option" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;


	if (!( optr = occi_paas_configuration_template_builder( PaasProcci.domain, "paas_configuration_template" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;


	if (!( PaasProcci.identity ))
	{
		/* --------------------------	*/
		/* launch simple occi server 	*/
		/* --------------------------	*/
		return( occi_server(  
			nptr, 
			PaasProcci.restport, PaasProcci.tls, 
			PaasProcci.threads, first, (char *) 0 ) );
	}
	else
	{
		/* --------------------------------------------------------------- */
		/* publication and pricing of categories before occi server launch */
		/* --------------------------------------------------------------- */
		initialise_occi_publisher( 
			PaasProcci.publisher, (char*) 0, (char *) 0, (char *) 0);

		return( publishing_occi_server(
				PaasProcci.user, PaasProcci.password,
				PaasProcci.identity,  nptr, 
				PaasProcci.restport, PaasProcci.tls, 
				PaasProcci.threads, first ) );
	}
}

/*	------------------------------------------- */
/*	p a a s p r o c c i _ o p t i o n s			*/
/*	------------------------------------------- */
/*	Command line option analysis			*/
/*	------------------------------------------- */
private	int	paasprocci_options(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	paasprocci_configuration();
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
				PaasProcci.verbose=1;
				continue;
			case	'd'	:
				PaasProcci.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = paasprocci_operation(aptr) ))
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
		return( paasprocci_banner() );
	else	return( paasprocci_options( argc, argv ) );
}

	/* ---------- */
#endif 	/* _paasprocci_c */
	/* ---------- */


