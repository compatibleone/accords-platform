#ifndef	_conagios_c	
#define	_conagios_c

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

struct	accords_configuration CoNagios = {
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
	"conagios.xml",
	"storage",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(CoNagios.debug);		}
public	int	check_verbose()		{	return(CoNagios.verbose);	}
public	char *	default_publisher()	{	return(CoNagios.publisher);	}
public	char *	default_operator()	{	return(CoNagios.operator);	}
public	char *	default_tls()		{	return(CoNagios.tls);		}
public	char *	default_zone()		{	return(CoNagios.zone);		}

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
/*	c o n a g i o s _ c o n f i g u r a t i o n				*/
/*	---------------------------------------------------------------	*/  
/*	this function loads conagios configuration			*/
/*	from the xml configuration file.				*/
/*	---------------------------------------------------------------	*/  
private	void	conagios_configuration()
{
	load_accords_configuration( &CoNagios, "conagios" );
	return;
}

/*	-----------------------------------------------------	----------	*/  
/*	c o n a g i o s _ b a n n e r				*/
/*	---------------------------------------------------------------	*/  
/*	this function will be called to display module identification 	*/
/*	when launched from the command line without any parameters.	*/
/*	---------------------------------------------------------------	*/  
private	int	conagios_banner()
{
	printf("\n   CompatibleOne Generic CoNagios Service : Version 1.0a.0.01");
	printf("\n   Beta Version : 19/10/2012 ");
	printf("\n   Copyright (c) 2012 Iain James Marshall, Mohamed Mohamed, Sami Yangui");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}

/*	------------------------------------------- */
/*	c o n a g i o s _ i n i t i a l i s e	    */
/*	------------------------------------------- */

private	struct rest_server * conagios_initialise(  void * v,struct rest_server * sptr )
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
/*	c o n a g i o s _ a u t h o r i s e 		*/
/*	------------------------------------------- 	*/
private	int	conagios_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, CoNagios.user ) )
		return(0);
	else if ( strcmp( password, CoNagios.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	-------------------------------------------	*/
/*	c o n a g i o s _ e x t e n s i o n 		*/
/*	-------------------------------------------	*/
private	struct rest_extension * conagios_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

#include "nagioscommand.h"
#include "nagioscontactgroup.h"
#include "nagioscontact.h"
#include "nagioshostdependancy.h"
#include "nagioshostescalation.h"
#include "nagioshostextinfo.h"
#include "nagioshostgroup.h"
#include "nagioshost.h"
#include "nagiosservicedependancy.h"
#include "nagiosserviceescalation.h"
#include "nagiosservicegroup.h"
#include "nagiosservice.h"
#include "nagiosseviceextinfo.h"
#include "nagiostimeperiod.h"

#include "nagioscommand.c"
#include "nagioscontactgroup.c"
#include "nagioscontact.c"
#include "nagioshostdependancy.c"
#include "nagioshostescalation.c"
#include "nagioshostextinfo.c"
#include "nagioshostgroup.c"
#include "nagioshost.c"
#include "nagiosservicedependancy.c"
#include "nagiosserviceescalation.c"
#include "nagiosservicegroup.c"
#include "nagiosservice.c"
#include "nagiosseviceextinfo.c"
#include "nagiostimeperiod.c"

#include "occinagioscommand.c"
#include "occinagioscontactgroup.c"
#include "occinagioscontact.c"
#include "occinagioshostdependancy.c"
#include "occinagioshostescalation.c"
#include "occinagioshostextinfo.c"
#include "occinagioshostgroup.c"
#include "occinagioshost.c"
#include "occinagiosservicedependancy.c"
#include "occinagiosserviceescalation.c"
#include "occinagiosservicegroup.c"
#include "occinagiosservice.c"
#include "occinagiosseviceextinfo.c"
#include "occinagiostimeperiod.c"

/*	--------------------------------------------	*/
/*	c o n a g i o s _ o p e r a t i o n 		*/
/*	--------------------------------------------	*/
/*	environment and category preparation		*/
/*	--------------------------------------------	*/
private	int	conagios_operation( char * nptr )
{

	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_conagios.xml");

	rest_initialise_log( CoNagios.monitor );


	/* ---------------------------------------- */
	/* preparation of application category list */
	/* ---------------------------------------- */
	if (!( optr = occi_nagios_command_builder( CoNagios.domain, "nagios_command" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_nagios_contact_builder( CoNagios.domain, "nagios_contact" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_nagios_contact_group_builder( CoNagios.domain, "nagios_contact_group" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_nagios_host_builder( CoNagios.domain, "nagios_host" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_nagios_host_group_builder( CoNagios.domain, "nagios_host_group" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_nagios_service_builder( CoNagios.domain, "nagios_service" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_nagios_service_group_builder( CoNagios.domain, "nagios_service_group" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_nagios_time_period_builder( CoNagios.domain, "nagios_time_period" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_nagios_host_dependency_builder( CoNagios.domain, "nagios_host_dependency" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;


	if (!( optr = occi_nagios_host_escalation_builder( CoNagios.domain, "nagios_host_escalation" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;


	if (!( optr = occi_nagios_host_ext_info_builder( CoNagios.domain, "nagios_host_ext_info" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_nagios_service_dependency_builder( CoNagios.domain, "nagios_service_dependency" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;


	if (!( optr = occi_nagios_service_escalation_builder( CoNagios.domain, "nagios_service_escalation" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;


	if (!( optr = occi_nagios_service_ext_info_builder( CoNagios.domain, "nagios_service_ext_info" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;


	if (!( CoNagios.identity ))
	{
		/* --------------------------	*/
		/* launch simple occi server 	*/
		/* --------------------------	*/
		return( occi_server(  
			nptr, 
			CoNagios.restport, CoNagios.tls, 
			CoNagios.threads, first, (char *) 0 ) );
	}
	else
	{
		/* --------------------------------------------------------------- */
		/* publication and pricing of categories before occi server launch */
		/* --------------------------------------------------------------- */
		initialise_occi_publisher( 
			CoNagios.publisher, (char*) 0, (char *) 0, (char *) 0);

		return( publishing_occi_server(
				CoNagios.user, CoNagios.password,
				CoNagios.identity,  nptr, 
				CoNagios.restport, CoNagios.tls, 
				CoNagios.threads, first ) );
	}
}

/*	------------------------------------------- */
/*	c o n a g i o s _ o p t i o n s		    */
/*	------------------------------------------- */
/*	Command line option analysis		    */
/*	------------------------------------------- */
private	int	conagios_options(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	conagios_configuration();
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
				CoNagios.verbose=1;
				continue;
			case	'd'	:
				CoNagios.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = conagios_operation(aptr) ))
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
		return( conagios_banner() );
	else	return( conagios_options( argc, argv ) );
}

	/* ---------- */
#endif 	/* _conagios_c */
	/* ---------- */


