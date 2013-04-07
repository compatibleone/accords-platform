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
#include "occiresolver.h"
#include "cordslang.h"

struct	accords_configuration Comons = {
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
	"comons.xml",
	"europe",
	"storage",
	"comons",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(Comons.debug);		}
public	int	check_verbose()		{	return(Comons.verbose);		}
public	char *	default_publisher()	{	return(Comons.publisher);	}
public	char *	default_operator()	{	return(Comons.operator);	}
public	char *	default_tls()		{	return(Comons.tls);		}
public	char *	default_zone()		{	return(Comons.zone);		}

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
/*	 	   c o m o n s _ l o a d 		*/
/*	---------------------------------------------	*/
/*	this function loads comons  configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	comons_load()
{
	load_accords_configuration( &Comons, "comons" );
	return;
}

#include "occibuilder.h"

private	int	banner()
{
	printf("\n   CompatibleOne Monitoring Services : Version 1.0a.0.06");
	printf("\n   Beta Version : 23/08/2012");
	printf("\n   Copyright (c) 2011, 2012 Iain James Marshall, Prologue");
	printf("\n");
	accords_configuration_options();
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

#include "comonssession.c"
#include "comonsevent.c"
#include "comonsmonitor.c"

/*	------------------------------------------------------------------	*/
/*			c o m o n s _ o p e r a t i o n				*/
/*	------------------------------------------------------------------	*/
private	int	comons_operation( char * nptr )
{
	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_comons.xml");

	/* -------------------------------------- */
	/* monitoring monitor category management */
	/* -------------------------------------- */
	if (!( optr = occi_cords_monitor_builder( Comons.domain, _CORDS_MONITOR ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback = &monitor_interface;
	optr->access  |= _OCCI_NO_PRICING;

	if (!( optr = occi_add_action( optr,_CORDS_START,"",start_monitor)))
		return( 27 );
	else if (!( optr = occi_add_action( optr,_CORDS_STOP,"",stop_monitor)))
		return( 27 );

	/* ------------------------------------ */
	/* monitoring event category management */
	/* ------------------------------------ */
	if (!( optr = comons_event_builder( Comons.domain ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	/* --------------------------------------- */
	/* monitoring consumer category management */
	/* --------------------------------------- */
	if (!( optr = occi_cords_consumer_builder( Comons.domain, _CORDS_CONSUMER ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;
	optr->access |= _OCCI_NO_PRICING;

	/* -------------------------------------- */
	/* monitoring session category management */
	/* -------------------------------------- */
	if (!( optr = comons_session_builder( Comons.domain ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	/* ------------------------------------- */
	/* monitoring metric category management */
	/* ------------------------------------- */
	if (!( optr = occi_cords_metric_builder( Comons.domain, _CORDS_METRIC ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;
	optr->access |= _OCCI_NO_PRICING;

	if (!( optr = occi_cords_alert_builder( Comons.domain, _CORDS_ALERT ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;
	optr->access |= _OCCI_NO_PRICING;

	if (!( optr = occi_cords_report_builder( Comons.domain, _CORDS_REPORT ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = (void *) 0;
	optr->access |= _OCCI_NO_PRICING;

	Comons.monitor &= ~2;

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
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
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

