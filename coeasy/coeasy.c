#ifndef	_coeasy_c	
#define	_coeasy_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"

#include "occi_rest_easiclouds_application.h"
#include "occi_rest_easiclouds_connection.h"
#include "occi_rest_easiclouds_description.h"
#include "occi_rest_easiclouds_extra.h"
#include "occi_rest_easiclouds_instance.h"
#include "occi_rest_easiclouds_keypair.h"
#include "occi_rest_easiclouds_link.h"
#include "occi_rest_easiclouds_metadata.h"
#include "occi_rest_easiclouds_model.h"
#include "occi_rest_easiclouds_node.h"
#include "occi_rest_easiclouds_parameters.h"
#include "occi_rest_easiclouds_postconf.h"
#include "occi_rest_easiclouds_server.h"
#include "occi_rest_easiclouds_input.h"
#include "occi_rest_easiclouds_output.h"

/* 	----------------------------------------	*/	
/*	Prototype Contract Negotiation Functions	*/
/* 	----------------------------------------	*/

struct	accords_configuration CoEasy = {
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
	"coeasy.xml",
	"storage",
	"coeasy",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(CoEasy.debug);		}
public	int	check_verbose()		{	return(CoEasy.verbose);		}
public	char *	default_publisher()	{	return(CoEasy.publisher);	}
public	char *	default_operator()	{	return(CoEasy.operator);		}
public	char *	default_tls()		{	return(CoEasy.tls);		}
public	char *	default_zone()		{	return(CoEasy.zone);		}

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

/*	---------------------------------------------------------------	*/  
/*	c o e a s y _ c o n f i g u r a t i o n				*/
/*	---------------------------------------------------------------	*/  
/*	this function loads coeasy configuration			*/
/*	from the xml configuration file.				*/
/*	---------------------------------------------------------------	*/  
private	void	coeasy_configuration()
{
	load_accords_configuration( &CoEasy, "coeasy" );
	return;
}

/*	-----------------------------------------------------	----------	*/  
/*	c o e a s y _ b a n n e r				*/
/*	---------------------------------------------------------------	*/  
/*	this function will be called to display module identification 	*/
/*	when launched from the command line without any parameters.	*/
/*	---------------------------------------------------------------	*/  
private	int	coeasy_banner()
{
	printf("\n   CompatibleOne Easiclouds CoEasy Service : Version 1.0a.0.01");
	printf("\n   Beta Version : 06/03/2013 ");
	printf("\n   Copyright (c) 2013 Iain James Marshall ");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}

/*	------------------------------------------- */
/*	c o e a s y _ i n i t i a l i s e	    */
/*	------------------------------------------- */

private	struct rest_server * coeasy_initialise(  void * v,struct rest_server * sptr )
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
/*	c o e a s y _ a u t h o r i s e 		*/
/*	------------------------------------------- 	*/
private	int	coeasy_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, CoEasy.user ) )
		return(0);
	else if ( strcmp( password, CoEasy.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	-------------------------------------------	*/
/*	c o e a s y _ e x t e n s i o n 		*/
/*	-------------------------------------------	*/
private	struct rest_extension * coeasy_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	--------------------------------------------	*/
/*	c o e a s y _ o p e r a t i o n 		*/
/*	--------------------------------------------	*/
/*	environment and category preparation		*/
/*	--------------------------------------------	*/
private	int	coeasy_operation( char * nptr )
{

	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_coeasy.xml");

	rest_initialise_log( CoEasy.monitor );

	/* ---------------------------------------- */
	/* preparation of application category list */
	/* ---------------------------------------- */

	if (!( optr = occi_easiclouds_application_builder( CoEasy.domain, "easiclouds_application" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_easiclouds_connection_builder( CoEasy.domain, "easiclouds_connection" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_easiclouds_description_builder( CoEasy.domain, "easiclouds_description" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_easiclouds_extra_builder( CoEasy.domain, "easiclouds_extra" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_easiclouds_instance_builder( CoEasy.domain, "easiclouds_instance" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_easiclouds_keypair_builder( CoEasy.domain, "easiclouds_keypair" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_easiclouds_link_builder( CoEasy.domain, "easiclouds_link" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_easiclouds_model_builder( CoEasy.domain, "easiclouds_model" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_easiclouds_node_builder( CoEasy.domain, "easiclouds_node" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_easiclouds_parameters_builder( CoEasy.domain, "easiclouds_parameters" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_easiclouds_postconf_builder( CoEasy.domain, "easiclouds_postconf" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_easiclouds_metadata_builder( CoEasy.domain, "easiclouds_metadata" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_easiclouds_server_builder( CoEasy.domain, "easiclouds_server" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( CoEasy.identity ))
	{
		/* --------------------------	*/
		/* launch simple occi server 	*/
		/* --------------------------	*/
		return( occi_server(  
			nptr, 
			CoEasy.restport, CoEasy.tls, 
			CoEasy.threads, first, (char *) 0 ) );
	}
	else
	{
		/* --------------------------------------------------------------- */
		/* publication and pricing of categories before occi server launch */
		/* --------------------------------------------------------------- */
		initialise_occi_publisher( 
			CoEasy.publisher, (char*) 0, (char *) 0, (char *) 0);

		return( publishing_occi_server(
				CoEasy.user, CoEasy.password,
				CoEasy.identity,  nptr, 
				CoEasy.restport, CoEasy.tls, 
				CoEasy.threads, first ) );
	}
}

/*	------------------------------------------- */
/*	c o e a s y _ o p t i o n s		    */
/*	------------------------------------------- */
/*	Command line option analysis		    */
/*	------------------------------------------- */
private	int	coeasy_options(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	coeasy_configuration();
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
				CoEasy.verbose=1;
				continue;
			case	'd'	:
				CoEasy.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = coeasy_operation(aptr) ))
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
		return( coeasy_banner() );
	else	return( coeasy_options( argc, argv ) );
}

	/* ---------- */
#endif 	/* _coeasy_c */
	/* ---------- */


