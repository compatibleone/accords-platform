/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2013 by Daniel Sprouse (ComputeNext) <daniel@computenext.com>   */
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
#ifndef	_cnprocci_c	
#define	_cnprocci_c

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
#include "occi_rest_cnconfig.h"
#include "occi_rest_computenext.h"

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

#define	_CORDS_COMPUTENEXT "computenext"

/* 	----------------------------------------	*/	
/*	Prototype Contract Negotiation Functions	*/
/* 	----------------------------------------	*/
#include "cnconfig.h"
#include "computenext.h"
#include "cnclient.h"
#include "cnclient.c"
#include "cncontract.h"
#include "cncontract.c"

/*	---------------------------------------------------------------	*/  
/*	c o m p u t e n e x t _ c o n f i g u r a t i o n               */
/*	---------------------------------------------------------------	*/  
/*	this function loads cnprocci configuration                      */
/*	from the xml configuration file.                                */
/*	---------------------------------------------------------------	*/  
private	void	cnprocci_configuration()
{
	load_accords_configuration( &Configuration, "cnprocci" );
	return;
}

/*	---------------------------------------------------------------	*/  
/*	c o m p u t e n e x t _ b a n n e r                                     */
/*	---------------------------------------------------------------	*/  
/*	this function will be called to display module identification 	*/
/*	when launched from the command line without any parameters.     */
/*	---------------------------------------------------------------	*/  
private	int	cnprocci_banner()
{
	printf("\n   CompatibleOne ComputeNext PROCCI : Version 1.0a.0.01");
	printf("\n   Beta Version : 17/03/2013 ");
	printf("\n   Copyright (c) 2013 Daniel Sprouse(ComputeNext) and Iain James Marshall(Prologue)");
	printf("\n");
	accords_configuration_options();
	printf("\n\n");
	return(0);

}

/*	------------------------------------------- */
/*	c o m p u t e n e x t _ i n i t i a l i s e         */
/*	------------------------------------------- */

private	struct rest_server * cnprocci_initialise(  void * v,struct rest_server * sptr )
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
/*	c o m p u t e n e x t _ a u t h o r i s e           */
/*	------------------------------------------- */
private	int	cnprocci_authorise(  void * v,struct rest_client * cptr, char * username, char * password )
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
/*	c o m p u t e n e x t _ e x t e n s i o n   */
/*	-------------------------------------------	*/
private	struct rest_extension * cnprocci_extension( void * v,struct rest_server * sptr, struct rest_extension * xptr)
{
	return( xptr );
}

/*	-------------------------------------------	*/
/*	cnprocci C R U D Method  Extension Functions*/
/*	-------------------------------------------	*/
/* 	   c r e a t e _ c o m p u t e n e x t      */
/*	-------------------------------------------	*/
private	int	create_computenext(struct occi_category * optr, void * vptr, struct rest_request * rptr)
{
	struct	computenext * pptr;
	pptr = vptr;
    return(create_computenext_contract(optr, pptr,rptr));
}

/*	-------------------------------------------	*/
/* 	   d e l e t e  _ c o m p u t e n e x t     */
/*	-------------------------------------------	*/
private	int	delete_computenext(struct occi_category * optr, void * vptr, struct rest_request * rptr)
{
	struct	computenext * pptr;
	pptr = vptr;
    return(delete_computenext_contract(optr, pptr, rptr));
}

/*	--------------------------------------------	*/
/*	CRUD Methods Relay Table                        */
/*	--------------------------------------------	*/
private	struct	occi_interface	cnprocci_interface = {
	create_computenext,
	NULL,
	NULL,
	delete_computenext
	};	

/*	--------------------------------------------	*/
/*	c o m p u t e n e x t _ o p e r a t i o n 		*/
/*	--------------------------------------------	*/
/*	environment and category preparation		    */
/*	--------------------------------------------	*/
private	int	cnprocci_operation( char * nptr )
{

	struct	occi_category * first=(struct occi_category *) 0;
	struct	occi_category * last=(struct occi_category *) 0;
	struct	occi_category * optr=(struct occi_category *) 0;

	set_autosave_cords_xlink_name("links_cnprocci.xml");

	rest_initialise_log( Configuration.monitor );

	/* ---------------------------------------- */
	/* preparation of application category list */
	/* ---------------------------------------- */
	if (!( optr = occi_cn_config_builder( Configuration.domain, "computenext_configuration" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;

	if (!( optr = occi_computenext_builder( Configuration.domain, "computenext" ) ))
		return( 27 );
	else if (!( optr->previous = last ))
		first = optr;
	else	optr->previous->next = optr;
	last = optr;
	optr->callback  = &cnprocci_interface;
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
	if (!( optr = occi_add_action( optr, _CORDS_START, "", start_computenext)))
		return( 27 );
	else if (!( optr = occi_add_action( optr, _CORDS_STOP," ", stop_computenext)))
		return( 27 );
	else if (!( optr = occi_add_action( optr, _CORDS_SAVE, "", save_computenext)))
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
/*	c o m p u t e n e x t _ o p t i o n s       */
/*	------------------------------------------- */
/*	Command line option analysis                */
/*	------------------------------------------- */
private	int	cnprocci_options(int argc, char * argv[] )
{
	int	status=0;
	int	argi=0;
	char *	aptr;
	cnprocci_configuration();
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
		else if (!( status = cnprocci_operation(aptr) ))
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
		return( cnprocci_banner() );
	else	return( cnprocci_options( argc, argv ) );
}

        /* ----------  */
#endif 	/* _cnprocci_c */
        /* ----------  */


