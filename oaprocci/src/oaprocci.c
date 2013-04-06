/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2013 by Iain James Marshall (Prologue) <ijm667@hotmail.com>	*/
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License");      */
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at                              */
/*                                                                      */
/*  http://www.apache.org/licenses/LICENSE-2.0                          */
/*                                                                      */
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or      */
/* implied.                                                             */
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License.                                       */
/* -------------------------------------------------------------------- */
#ifndef	_oaprocci_c	
#define	_oaprocci_c

#include "standard.h"
#include "broker.h"
#include "rest.h"
#include "occi.h"
#include "document.h"
#include "cordspublic.h"
#include "occipublisher.h"
#include "cordslang.h"
#include "cosacsctrl.h"
#include "stdnode.h"

struct	accords_configuration Configuration = {
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
	"accords.xml",
	"europe",
	"storage",
	"onprocci",
	(struct occi_category *) 0,
	(struct occi_category *) 0
};


public	int	check_debug()		{	return(Configuration.debug);		}
public	int	check_verbose()		{	return(Configuration.verbose);		}
public	char *	default_publisher()	{	return(Configuration.publisher);	}
public	char *	default_operator()	{	return(Configuration.operator);		}
public	char *	default_tls()		{	return(Configuration.tls);		}
public	char *	default_zone()		{	return(Configuration.zone);		}

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

#define	_CORDS_ONAPP "onapp"

/* 	----------------------------------------	*/	
/*	Prototype Contract Negotiation Functions	*/
/* 	----------------------------------------	*/
#include "oaconfig.h"
#include "oaconfig.c"
#include "occioaconfig.c"
#include "onapp.h"
#include "onapp.c"
#include "occionapp.c"
#include "oaclient.h"
#include "oaclient.c"
#include "oacontract.h"
#include "oacontract.c"

/*	---------------------------------------------------------------	*/  
/*	o n a p p _ c o n f i g u r a t i o n               */
/*	---------------------------------------------------------------	*/  
/*	this function loads oaprocci configuration                      */
/*	from the xml configuration file.                                */
/*	---------------------------------------------------------------	*/  
private	void	oaprocci_configuration()
{
	load_accords_configuration( &Configuration, "oaprocci" );
	return;
}

/*	---------------------------------------------------------------	*/  
/*		o n a p p _ b a n n e r                                 */
/*	---------------------------------------------------------------	*/  
/*	this function will be called to display module identification 	*/
/*	when launched from the command line without any parameters.     */
/*	---------------------------------------------------------------	*/  
private	int	oaprocci_banner()
{
	printf("\n   CompatibleOne OnApp PROCCI : Version 1.0a.0.01");
	printf("\n   Beta Version : 17/03/2013 ");
	printf("\n   Copyright (c) 2013 Iain James Marshall(Prologue)");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}

/*	------------------------------------------- */
/*		o n a p p _ i n i t i a l i s e     */
/*	------------------------------------------- */

private	struct rest_server * oaprocci_initialise(  void * v,struct rest_server * sptr )
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
/*	o n a p p _ a u t h o r i s e           */
/*	------------------------------------------- */
private	int	oaprocci_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
{
	if ( strcmp( username, Configuration.user ) )
		return(0);
	else if ( strcmp( password, Configuration.password ) )
		return(0);
	else if (!( cptr->user = allocate_string( username ) ))
		return(0);
	else if (!( cptr->pass = allocate_string( password ) ))
		return(0);
	else	return(1);
}

/*	------------------------------------------- */
/*	o n a p p _ e x t e n s i o n   */
/*	-------------------------------------------	*/
private	struct rest_extension * oaprocci_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	-------------------------------------------	*/
/*	oaprocci C R U D Method  Extension Functions*/
/*	-------------------------------------------	*/
/* 	   c r e a t e _ o n a p p      */
/*	-------------------------------------------	*/
private	int	create_onapp(struct occi_category * optr, void * vptr, struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	onapp * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(create_onapp_contract(optr, pptr,rptr));
}

/*	-------------------------------------------	*/
/* 	 r e t r i e v e _ o n a p p    */
/*	-------------------------------------------	*/
private	int	retrieve_onapp(struct occi_category * optr, void * vptr, struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	onapp * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	   u p d a t e _ o n a p p		*/
/*	-------------------------------------------	*/
private	int	update_onapp(struct occi_category * optr, void * vptr, struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	onapp * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(0);
}

/*	-------------------------------------------	*/
/* 	   d e l e t e  _ o n a p p     */
/*	-------------------------------------------	*/
private	int	delete_onapp(struct occi_category * optr, void * vptr, struct rest_request * rptr)
{
	struct	occi_kind_node * nptr;
	struct	onapp * pptr;
	if (!( nptr = vptr ))
		return(0);
	else if (!( pptr = nptr->contents ))
		return(0);
	else	return(delete_onapp_contract(optr, pptr, rptr));
}

/*	--------------------------------------------	*/
/*	CRUD Methods Relay Table                        */
/*	--------------------------------------------	*/
private	struct	occi_interface	oaprocci_interface = {
	create_onapp,
	retrieve_onapp,
	update_onapp,
	delete_onapp
	};	

/*	--------------------------------------------	*/
/*	o n a p p _ o p e r a t i o n 		*/
/*	--------------------------------------------	*/
/*	environment and category preparation		    */
/*	--------------------------------------------	*/
private	int	oaprocci_operation( char * nptr )
{

	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_oaprocci.xml");

	rest_initialise_log( Configuration.monitor );

	/* ---------------------------------------- */
	/* preparation of application category list */
	/* ---------------------------------------- */
	if (!( optr = occi_oa_config_builder( Configuration.domain, "onapp_configuration" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_onapp_builder( Configuration.domain, "onapp" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = &oaprocci_interface;
	/* -------------------------------------------------------------- */
	/* this option indicates that the category is a provider category */
	/* and con be used as a legal value for the provider attribute of */
	/* a node description of a manifest.                              */
	/* -------------------------------------------------------------- */
	optr->access    |= _OCCI_PROVIDER;

	/* ---------------------------------------------*/
	/* Addition of the start, stop and save actions */
	/* these are necessary for a PROCCI to be able  */
	/* coordinate provisioning with the calls from  */
	/* the service manager for the management of    */
	/* the service life cycle.                      */
	/* ---------------------------------------------*/
	if (!( optr = occi_add_action( optr, _CORDS_START, "", start_onapp)))
		return( 27 );
	else if (!( optr = occi_add_action( optr, _CORDS_STOP," ", stop_onapp)))
		return( 27 );
	else if (!( optr = occi_add_action( optr, _CORDS_SAVE, "", save_onapp)))
		return( 27 );

	if (!( Configuration.identity ))
	{
		/* --------------------------	*/
		/* launch simple occi server 	*/
		/* --------------------------	*/
		return( occi_server(  
			nptr, 
			Configuration.restport, Configuration.tls, 
			Configuration.threads, first, (char *) 0 ) );
	}
	else
	{
		/* --------------------------------------------------------------- */
		/* publication and pricing of categories before occi server launch */
		/* --------------------------------------------------------------- */
		initialise_occi_publisher( 
			Configuration.publisher, (char*) 0, (char *) 0, (char *) 0);

		return( publishing_occi_server(
				Configuration.user, Configuration.password,
				Configuration.identity,  nptr, 
				Configuration.restport, Configuration.tls, 
				Configuration.threads, first ) );
	}
}

/*	------------------------------------------- */
/*	o n a p p _ o p t i o n s       */
/*	------------------------------------------- */
/*	Command line option analysis                */
/*	------------------------------------------- */
private	int	oaprocci_options(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	oaprocci_configuration();
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
				Configuration.verbose=1;
				continue;
			case	'd'	:
				Configuration.debug = 0xFFFF;
				continue;
			case	'-'	:
				if (!( argi = accords_configuration_option( aptr, argi, argv )))
					break;
				else	continue;
			}
			status = 30;
			break;
		}
		else if (!( status = oaprocci_operation(aptr) ))
			continue;
		else	break;
	}
	return(status);
}

/*	-------------------------------------------- */
/*	m a i n                                      */
/*	-------------------------------------------- */
/*	Entry point from the command line            */
/*	-------------------------------------------- */
public	int	main(int argc, char * argv[] )
{
	if ( argc == 1 )
		return( oaprocci_banner() );
	else	return( oaprocci_options( argc, argv ) );
}

        /* ----------  */
#endif 	/* _oaprocci_c */
        /* ----------  */


