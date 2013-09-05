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
#ifndef	_cosched_c
#define	_cosched_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "occibuilder.h"
#include "occiclient.h"
#include "cosacsctrl.h"
#include "cordslang.h"
#include "cp.h"
#include "cb.h"

#define	_COSCHED_MODEL "cosched:model"
#define _COSCHED_ACCOUNT "cosched"

struct	accords_configuration Cosched = {
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
	"cosched.xml",
	"europe",
	"storage",
	"cosched",
	(struct occi_category *) 0,
	(struct occi_category *) 0
	};

public	int	check_debug()		{	return(Cosched.debug);		}
public	int	check_verbose()		{	return(Cosched.verbose);		}
public	char *	default_publisher()	{	return(Cosched.publisher);	}
public	char *	default_operator()	{	return(Cosched.operator);	}
public	char *	default_tls()		{	return(Cosched.tls);		}
public	char *	default_zone()		{	return(Cosched.zone);		}

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
/*	 	   c o s c h e d _ l o a d 		*/
/*	---------------------------------------------	*/
/*	this function loads cosched  configuration	*/
/*	from the xml configuration file.		*/
/*	---------------------------------------------	*/  
private	void	cosched_load()
{
	load_accords_configuration( &Cosched, "cosched" );
	return;
}

private	int	banner()
{
	printf("\n   CompatibleOne Job Scheduling Services : Version 1.0a.0.02");
	printf("\n   Beta Version : 25/05/2012");
	printf("\n   Copyright (c) 2012 Iain James Marshall, Prologue");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}

/*	------------------------------------------------------------------	*/
/*			c o s c h e d _ i n i t i a l i s e			*/
/*	------------------------------------------------------------------	*/
private	struct rest_server * cosched_initialise(  void * v,struct rest_server * sptr )
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
/*			c o s c h e d _ a u t h o r i s e 			*/
/*	------------------------------------------------------------------	*/
private	int	cosched_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, Cosched.user ) )
		return(0);
	else if ( strcmp( password, Cosched.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------------------------------	*/
/*			c o s c h e d _ e x t e n s i o n 				*/
/*	------------------------------------------------------------------	*/
private	struct rest_extension * cosched_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	------------------------------------------------------------------	*/
/*			c o s c h e d _ s y n c h r o n i s e 			*/
/*	------------------------------------------------------------------	*/
private	void	cosched_synchronise()
{
	sleep(1);
}

#include "counter.h"
#include "counter.c"
#include "occicounter.c"

#include "timer.h"
#include "timer.c"
#include "occitimer.c"

#include "cotimer.c"

#include "schedule.h"
#include "schedule.c"
#include "occischedule.c"

/*	-------------------------------------------	*/
/* 	      c r e a t e _ s c h e d u l e   		*/
/*	-------------------------------------------	*/
private	int	create_cords_schedule(struct occi_category * optr, void * vptr,struct rest_request * rptr)
{
	struct	rest_header * hptr;
	struct	occi_response * zptr;
	struct	occi_kind_node * nptr;
	char	buffer[1024];
	struct	cords_schedule * pptr;
	char *	root;
	char *	price;
	char *	work;
	char * wptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else if ( pptr->state )
		return( 0 );
	else if (!( pptr->operation ))
		return( 0 );
	else if (!( wptr = allocate_string( pptr->operation ) ))
		return(0);
	else 
	{
		/* -------------------------------------------- */
		/* detect, collect and keep account information */
		/* -------------------------------------------- */
		if (!( rest_valid_string( pptr->account ) ))
			if ((( hptr = rest_resolve_header( rptr->first, _OCCI_ACCOUNT )) != (struct rest_header *) 0)
			&&   ( rest_valid_string( hptr->value ) != 0 ))
				pptr->account = allocate_string( hptr->value );
	

		/* ----------------------------- */
		/* process the scheduled request */
		/* ----------------------------- */
		pptr->requested = time((long *) 0);
		work = root = wptr;
		while ( *wptr )
		{
			if ( *wptr == '?' )
				break;
			else	wptr++;
		}
		if ( *wptr != '?' )
			return(0);
		else
		{
			*(wptr++) = 0;
			work = wptr;
			while ( *wptr )
			{
				if ( *wptr == '=' )
					break;
				else	wptr++;
			}
			if ( *wptr != '=' )
				return(0);
			else	*(wptr++) = 0;
		}	
		if ( strcmp( work, "action" ) )
			return(0);
		else 
		{
			/* -------------------------------- */
			/* detect and resolve missing price */
			/* -------------------------------- */
			if (!( rest_valid_string( pptr->price ) ))
			{
				sprintf(buffer,"schedule:action:%s",wptr);

				pptr->price = occi_resolve_price( buffer, _CORDS_CONTRACT_AGENT, default_tls());
			}

			pptr->started = time((long *) 0);
			if (!( zptr = ll_cords_invoke_action( root, wptr, _CORDS_CONTRACT_AGENT, default_tls() ) ))
			{
				pptr->completed = time((long *) 0);
				pptr->duration  = pptr->completed - pptr->started;
				pptr->response  = -1;
				pptr->message   = allocate_string("invocation failure");
				root = liberate( root );
				return(0);
			}
			else
			{
				pptr->completed = time((long *) 0);
				pptr->duration  = pptr->completed - pptr->started;
				pptr->state++;
				pptr->response = zptr->response->status;
				pptr->message  = allocate_string( zptr->response->message );
				zptr = occi_remove_response( zptr );
				return(0);
			}
		}
	}
}

private	struct	occi_interface	cords_schedule_interface = {
	create_cords_schedule,
	NULL,
	NULL,
	NULL
	};
	


/*	------------------------------------------------------------------	*/
/*			c o s c h e d _ o p e r a t i o n			*/
/*	------------------------------------------------------------------	*/
private	int	cosched_operation( char * nptr )
{
	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_cosched.xml");

	if (!( optr = occi_cords_schedule_builder( Cosched.domain, "schedule" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback = &cords_schedule_interface;

	initialise_timer_control();

	if (!( optr = occi_cords_counter_builder( Cosched.domain, "counter" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_add_action( optr,"hit","",hit_counter)))
		return(27);

	if (!( optr = occi_cords_timer_builder( Cosched.domain, "timer" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	optr->callback  = &timer_interface;

	if (!( optr = occi_add_action( optr,_CORDS_START,"",start_timer)))
		return(27);
	else if (!( optr = occi_add_action( optr,_CORDS_STOP,"",stop_timer)))
		return(27);

	rest_initialise_log(Cosched.monitor);

	if (!( Cosched.identity ))
		return( occi_server(  nptr, Cosched.restport, Cosched.tls, Cosched.threads, first,(char *) 0 ) );
	else
	{
		initialise_occi_publisher( Cosched.publisher, (char*) 0, (char *) 0, (char *) 0);
		return( publishing_occi_server(
			Cosched.user, Cosched.password,
			Cosched.identity,  nptr, 
			Cosched.restport, Cosched.tls, 
			Cosched.threads, first ) );
	}
}

/*	------------------------------------------------------------------	*/
/*				c o s c h e d 					*/
/*	------------------------------------------------------------------	*/
private	int	cosched(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	cosched_load();
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
				Cosched.verbose=1;
				continue;
			case	'd'	:
				Cosched.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = cosched_operation(aptr) ))
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
	else	return( cosched( argc, argv ) );
}


	/* --------- */
#endif	/* _cosched_c */
	/* --------- */

