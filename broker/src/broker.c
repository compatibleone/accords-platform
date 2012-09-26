/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>     */
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License"); 	*/
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at 				*/
/*  									*/
/*  http://www.apache.org/licenses/LICENSE-2.0 				*/
/*  									*/
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 	*/
/* implied. 								*/
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License. 					*/
/* -------------------------------------------------------------------- */
#ifndef	_broker_c
#define	_broker_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"

struct	accords_configuration Broker = {
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
	"broker.xml",
	"europe",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(Broker.debug);		}
public	int	check_verbose()		{	return(Broker.verbose);		}
public	char *	default_publisher()	{	return(Broker.publisher);	}
public	char *	default_operator()	{	return(Broker.operator);	}
public	char *	default_tls()		{	return(Broker.tls);		}
public	char *	default_identity()	{	return(Broker.identity);	}
public	char *	default_zone()		{	return(Broker.zone);		}

public	int	failure( int e, char * m1, char * m2 )
{
	if ( e )
	{
		printf("\n***(%u) failure %u",getpid(),e);
		if ( m1 )
			printf(" : %s",m1);
		if ( m2 )
			printf(" : %s",m2);
		printf(" **`\n");
	}
	return( e );
}

/*	---------------------------------------------	*/  
/*			b r o k e r _ l o a d 		*/
/*	---------------------------------------------	*/
/*	this function loads broker    configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	broker_load()
{
	load_accords_configuration( &Broker, "broker" );
	return;
}

#include "occibuilder.h"

private	int	banner()
{
	printf("\n   CompatibleOne Request Broker : Version 1.0a.0.06");
	printf("\n   Beta Version : 25/05/2012");
	printf("\n   Copyright (c) 2011, 2012 Iain James Marshall, Prologue");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}

/*	------------------------------------------------------------------	*/
/*			b r o k e r _ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * broker_initialise(  void * v,struct rest_server * sptr )
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
private	int	broker_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, Broker.user ) )
		return(0);
	else if ( strcmp( password, Broker.password ) )
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
private	struct rest_extension * broker_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	------------------------------------------------------------------	*/
/* 	  actions and methods required for the broker service category		*/
/*	------------------------------------------------------------------	*/
#include "brokerprovider.c"		
#include "brokerservice.c"		
#include "brokerplan.c"		

/*	------------------------------------------------------------------	*/
/*			b r o k e r _ o p e r a t i o n				*/
/*	------------------------------------------------------------------	*/
private	int	broker_operation( char * nptr )
{
	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_broker.xml");

	if (!( optr = occi_cords_action_builder( Broker.domain, "action" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	if (!( optr = broker_plan_builder( Broker.domain,"plan" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	if (!( optr = broker_provider_builder( Broker.domain,"provider" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_cords_profile_builder( Broker.domain,"profile" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	if (!( optr = broker_service_builder( Broker.domain,"service" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_cords_instance_builder( Broker.domain,"instance" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;

	if ( Broker.autopub ) optr->access   |= _OCCI_AUTO_PUBLISH;

	rest_initialise_log(Broker.monitor);

	if (!( Broker.identity ))
		return( occi_server(  nptr, Broker.restport, Broker.tls, Broker.threads, first, (char *) 0 ) );
	else
	{
		initialise_occi_publisher( Broker.publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			Broker.user, Broker.password,
			Broker.identity,  nptr, 
			Broker.restport, Broker.tls, 
			Broker.threads, first ) );
	}
}

/*	------------------------------------------------------------------	*/
/*				b r o k e r 					*/
/*	------------------------------------------------------------------	*/
private	int	broker(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	broker_load();
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
				Broker.verbose=1;
				continue;
			case	'd'	:
				Broker.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = broker_operation(aptr) ))
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
	else	return( broker( argc, argv ) );
}


	/* --------- */
#endif	/* _broker_c */
	/* --------- */

